
/*
 * code borrwed from fefe's embutils
 * License: GPLv2
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "edonr.h"

#define DIGESTSZ 64

static void do_edonr(int fd, char hexl[DIGESTSZ/2+1], char hexd[DIGESTSZ+2])
{
	EdonRState context;
	uint8_t digest[64+2];
	ssize_t len;

	unsigned long long filelen = 0;
	char *t;
	int i;
	int c;
	static uint8_t buf[1024];
	const uint8_t *data = buf;

	edonrInit(&context, 512);

	while ((len = read(fd, buf, sizeof(buf))) > 0) {
		filelen += len;
		if (len)
			edonrUpdate(&context, data, len*8);
	}

	t = hexl + 16;
	*t = ' ';
	do {
		c = ((unsigned int)filelen & 0xf) + '0';
		*--t = c > '9' ? c + 'a' - 10 - '0' : c;
		filelen >>= 4;
	} while (t > hexl);

	edonrFinal(&context, digest);
	t = hexd;
	for (i = 0; i < 32; i++) {
		c = (digest[i] >> 4) + '0';
		*t++ = c > '9' ? c + 'a' - 10 - '0' : c;
		c = (digest[i] & 0xf) + '0';
		*t++ = c > '9' ? c + 'a' - 10 - '0' : c;
	}
	t[0] = t[1] = ' ';
}

int main(int argc, char **argv)
{
	int len = 0;
	char hexl[DIGESTSZ/2+1];
	char hexd[DIGESTSZ+2];
	int fd;
	char *name;

	if (argc <= 1)
		goto only_stdin;

	while ((name = *++argv) != NULL) {

		if (name[0] == '-' && name[1] == 'n' && name[2] == '\0') {
			len = 1;
			continue;
		}

		if (name[0] == '-' && name[1] == '\0') {
 only_stdin:		name = "-";
			fd = 0;
		} else {
			if ((fd = open(name, O_RDONLY)) < 0)
				continue;
		}

		do_edonr(fd, hexl, hexd);
		if (len)
			write(1, hexl, DIGESTSZ/2+1);
		write(1, hexd, DIGESTSZ+2);
		write(1, name, strlen(name));
		write(1, "\n", 1);
		if (fd > 0)
			close(fd);
	}

	return 0;
}
