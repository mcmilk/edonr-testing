
/*
 * Copyright (c) 2022 Tino Reichardt <milky-zfs@mcmilk.de>
 */

#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "edonr.h"

const char *progname = "edonr-testing";
const char *VERSION = "0.1";

int opt_benchmark = 1;
int opt_verbose = 0;
int opt_iterations = 1;

typedef void (*edonr_init_f)(EdonRState *ctx, size_t hashbitlen);
typedef void (*edonr_update_f)(EdonRState *ctx, const uint8_t *data, size_t len);
typedef void (*edonr_final_f)(EdonRState *ctx, uint8_t *md);

typedef struct edonr_impl_ops {
	edonr_init_f init;
	edonr_update_f update;
	edonr_final_f final;

	const char *name;
} edonr_impl_ops_t;

const edonr_impl_ops_t edonr_old_impl = {
	.init = EdonRInit,
	.update = EdonRUpdate,
	.final = EdonRFinal,
	.name = "edonr-old"
};

const edonr_impl_ops_t edonr_new_impl = {
	.init = EdonRInit_new,
	.update = EdonRUpdate_new,
	.final = EdonRFinal_new,
	.name = "edonr-new"
};

static void version(void)
{
	printf("%s version %s\n"
	       "\nCopyright (c) 2022 Tino Reichardt" "\n"
	       "\n", progname, VERSION);
	exit(0);
}

static void usage(void)
{
	printf("\n Usage: %s [OPTION]"
	       "\n"
	       "\n Options:"
	       "\n  -b    Benchmarking mode."
	       "\n  -h    Display a help and quit."
	       "\n  -v    Be more verbose."
	       "\n  -V    Show version information and quit."
	       "\n"
	       "\n Additional Options:"
	       "\n  -i N  Set number of iterations for testing (default: 1)."
	       "\n"
	       "\n Report bugs to: https://github.com/mcmilk/edonr-testing/issues"
	       "\n", progname);

	exit(0);
}

#define	SEC				1
#define	MILLISEC			1000
#define	NANOSEC				1000000000

#define	MSEC2NSEC(m)	((time_t)(m) * (NANOSEC / MILLISEC))
#define	NSEC2MSEC(n)	((n) / (NANOSEC / MILLISEC))

#define MSEC_PER_SEC	1000L
#define USEC_PER_SEC	1000000L
#define NSEC_PER_SEC	1000000000L

#if defined(__sun__)
static uint64_t my_gethrtime(void)
{
	return (uint64_t)gethrtime();
}
#else
static uint64_t my_gethrtime(void)
{
	struct timespec ts;
#ifdef CLOCK_MONOTONIC_RAW
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
#else
	clock_gettime(CLOCK_MONOTONIC, &ts);
#endif
	return (uint64_t) ((ts.tv_sec * NSEC_PER_SEC) + ts.tv_nsec);
}
#endif

typedef struct {
	uint64_t bs1k;
	uint64_t bs4k;
	uint64_t bs16k;
	uint64_t bs64k;
	uint64_t bs256k;
	uint64_t bs1m;
	uint64_t bs4m;
	const edonr_impl_ops_t *edonr;
} chksum_stat_t;

void check_result(const char *ref, const char *res, int len)
{
	if (strncmp(ref, res, len) != 0) {
		printf("Wrong Result! %s != %s\n", ref, res);
		exit(1);
	}
}

static void chksum_run(chksum_stat_t * cs, int round, uint64_t * result)
{
	uint64_t start;
	uint64_t run_bw, run_time_ns, run_count, size = 0;
	uint32_t l, loops = 0;
	EdonRState ctx;
	void *buf;
	unsigned char md[64 * 2 + 1];

	switch (round) {
	case 1:		/* 1k */
		size = 1 << 10;
		loops = 128;
		break;
	case 2:		/* 2k */
		size = 1 << 12;
		loops = 64;
		break;
	case 3:		/* 4k */
		size = 1 << 14;
		loops = 32;
		break;
	case 4:		/* 16k */
		size = 1 << 16;
		loops = 16;
		break;
	case 5:		/* 256k */
		size = 1 << 18;
		loops = 8;
		break;
	case 6:		/* 1m */
		size = 1 << 20;
		loops = 4;
		break;
	case 7:		/* 4m */
		size = 1 << 22;
		loops = 1;
		break;
	}

	buf = malloc(size);
	if (!buf)
		exit(111);

	/* warmup */
	start = my_gethrtime();
	cs->edonr->init(&ctx, 512);

	run_count = 0;
	do {
		for (l = 0; l < loops; l++, run_count++)
			cs->edonr->update(&ctx, buf, size*8);

		run_time_ns = my_gethrtime() - start;
	} while (run_time_ns < MSEC2NSEC(1));
	cs->edonr->final(&ctx, md);

	/* benchmark */
	start = my_gethrtime();
	cs->edonr->init(&ctx, 512);
	run_count = 0;
	do {
		for (l = 0; l < loops; l++, run_count++)
			cs->edonr->update(&ctx, buf, size*8);

		run_time_ns = my_gethrtime() - start;
	} while (run_time_ns < MSEC2NSEC(2));
	cs->edonr->final(&ctx, md);
	run_time_ns = my_gethrtime() - start;

	free(buf);
	run_bw = size * run_count * NANOSEC;
	run_bw /= run_time_ns;	/* B/s */
	*result = run_bw / 1024 / 1024;	/* MiB/s */
}

static void chksum_benchit(chksum_stat_t * cs)
{
	chksum_run(cs, 1, &cs->bs1k);
	chksum_run(cs, 2, &cs->bs4k);
	chksum_run(cs, 3, &cs->bs16k);
	chksum_run(cs, 4, &cs->bs64k);
	chksum_run(cs, 5, &cs->bs256k);
	chksum_run(cs, 6, &cs->bs1m);
	chksum_run(cs, 7, &cs->bs4m);

	printf("%-23s", cs->edonr->name);
	printf("%8llu", (unsigned long long)cs->bs1k);
	printf("%8llu", (unsigned long long)cs->bs4k);
	printf("%8llu", (unsigned long long)cs->bs16k);
	printf("%8llu", (unsigned long long)cs->bs64k);
	printf("%8llu", (unsigned long long)cs->bs256k);
	printf("%8llu", (unsigned long long)cs->bs1m);
	printf("%8llu\n", (unsigned long long)cs->bs4m);
}

static const edonr_impl_ops_t *edonr_impls[] = {
	&edonr_old_impl,
	&edonr_new_impl,
	NULL
};

int main(int argc, char *argv[])
{
	chksum_stat_t cs;
	int i, opt;

	/* same order as in help option -h */
	while ((opt = getopt(argc, argv, "bfvVi:h?")) != -1) {
		switch (opt) {
		case 'b':	/* benchmark */
			opt_benchmark = 1;
			break;

		case 'v':	/* be more verbose */
			opt_verbose++;
			break;

		case 'V':	/* version */
			version();
			break;

		case 'i':	/* iterations */
			opt_iterations = atoi(optarg);
			break;

		case 'h':
		case '?':
		default:
			usage();
			/* not reached */
		}
	}

	if (opt_benchmark) {
		/* header */
		printf("%-23s", "implementation");
		printf("%8s", "1k");
		printf("%8s", "4k");
		printf("%8s", "16k");
		printf("%8s", "64k");
		printf("%8s", "256k");
		printf("%8s", "1m");
		printf("%8s\n", "4m");
		int j;

		for (j = 0; j < opt_iterations; j++) {
			for (i = 0; edonr_impls[i]; i++) {
				cs.edonr = edonr_impls[i];
				chksum_benchit(&cs);
			}
		}
	}

	return 0;
}
