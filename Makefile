CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic

all: ifj16_rtg

ifj16_rtg: main.o SCAN.o FIO.o
	$(CC) $(CFLAGS) main.o SCAN.o FIO.o -o ifj16_rtg

FIO.o: FIO.c FIO.h
	$(CC) $(CFLAGS) -c FIO.c -o FIO.o

SCAN.o: SCAN.c SCAN.h FIO.h
	$(CC) $(CFLAGS) -c SCAN.c -o SCAN.o

main.o: main.c SCAN.h FIO.h
	$(CC) $(CFLAGS) -c main.c -o main.o