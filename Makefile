.PHONY: all

all: edonr-tests

#CC	= gcc
CC	= clang

CFLAGS	= -Os -Wall

OBJS	= edonr-tests.o edonr.o edonr_new.o

edonr-tests: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o edonr-tests

clean:
	rm -f $(OBJS) edonr-tests
