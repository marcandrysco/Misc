CC = gcc
LD = gcc
CFLAGS = -g -O2 -Wall
LDFLAGS = -lm

all: ex1

## example 1

ex1: ex1.o  ../membuf.o ../../mdbg/mdbg.o
	$(LD) $^ -o $@ $(LDFLAGS)

ex1.o: ex1.c Makefile
	$(CC) -c $< -o $@ $(CFLAGS)

## membuf library

../membuf.o:
	$(MAKE) -C ../

## dependencies

../../mdbg/mdbg.o:
	$(MAKE) -C ../../mdbg
