CC=gcc
CFLAGS=-Wall -Werror -g -O2


all: avl.o

avl.o: avl.c avl.h Makefile
	$(CC) -c $(CFLAGS) -o $@ $<


clean:
	rm avl.o


.PHONY: all clean
