.PHONY: all

all: edonr-tests edonr-new edonr-old

#CC	= gcc
CC	= clang

CFLAGS	= -Os -Wall

OBJS	= edonr.o edonr_new.o

edonr-tests: $(OBJS) edonr-tests.o
	$(CC) $(CFLAGS) $(OBJS) edonr-tests.o -o edonr-tests

edonr-new: $(OBJS) edonr-new.o
	$(CC) $(CFLAGS) $(OBJS) edonr-new.o -o edonr-new

edonr-old: $(OBJS) edonr-old.o
	$(CC) $(CFLAGS) $(OBJS) edonr-old.o -o edonr-old

clean:
	rm -f *.o edonr-tests edonr-new edonr-old
