CC=gcc
CFLAGS=-Wall -Werror -g -O2


all: mdbg.o

mdbg.o: mdbg.c mdbg.h Makefile
	$(CC) -c $(CFLAGS) -o $@ $<


clean:
	rm avl.o


.PHONY: all clean
