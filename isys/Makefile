CC=gcc
CFLAGS=-Wall -Werror -g -O2


all: isys-posix.o

isys-posix.o: posix.c Makefile
	$(CC) -c $(CFLAGS) -o $@ $<


clean:
	rm posix.o


.PHONY: all clean
