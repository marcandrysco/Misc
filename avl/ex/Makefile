CC = gcc
LD = gcc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: ex1 ex2

## example 1

ex1: ex1.o ../avl.o
	$(LD) $^ -o $@ $(LDFLAGS)

ex1.o: ex1.c Makefile
	$(CC) -c $< -o $@ $(CFLAGS)

## example 2

ex2: ex2.o ../avl.o
	$(LD) $^ -o $@ $(LDFLAGS)

ex2.o: ex2.c Makefile
	$(CC) -c $< -o $@ $(CFLAGS)

## avl library

../avl.o:
	$(MAKE) ../
