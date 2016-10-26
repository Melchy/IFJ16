CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic

all: ifj16_rtg

ifj16_rtg: main.o SCAN.o FIO.o ERROR.o MEM.o Tree.o Node.o EXPR.o VARTAB.o STR.o
	$(CC) $(CFLAGS) main.o SCAN.o FIO.o ERROR.o MEM.o Tree.o Node.o EXPR.o VARTAB.o STR.o -o ifj16_rtg

STR.O: STR.c STR.h MEM.h ERROR.h FIO.h
	$(CC) $(CFLAGS) -c STR.c -o STR.o

VARTAB.o: VARTAB.c VARTAB.h MEM.h STR.h
	$(CC) $(CFLAGS) -c VARTAB.c -o VARTAB.o

EXPR.o: EXPR.c EXPR.h Node.h Tree.h Tokens.h
	$(CC) $(CFLAGS) -c EXPR.c -o EXPR.o

Node.o: Node.c Node.h Tokens.h
	$(CC) $(CFLAGS) -c Node.c -o Node.o

Tree.o: Tree.c Tree.h STR.h MEM.h Tokens.h Node.h
	$(CC) $(CFLAGS) -c Tree.c -o Tree.o

MEM.o: MEM.c MEM.h ERROR.h
	$(CC) $(CFLAGS) -c MEM.c -o MEM.o

ERROR.o: ERROR.c ERROR.h MEM.h FIO.h
	$(CC) $(CFLAGS) -c ERROR.c -o ERROR.o

FIO.o: FIO.c FIO.h ERROR.h
	$(CC) $(CFLAGS) -c FIO.c -o FIO.o

SCAN.o: SCAN.c SCAN.h FIO.h STR.h MEM.h ERROR.h Tokens.h
	$(CC) $(CFLAGS) -c SCAN.c -o SCAN.o

main.o: main.c SCAN.h FIO.h STR.h MEM.h Tree.h Tokens.h EXPR.h VARTAB.h
	$(CC) $(CFLAGS) -c main.c -o main.o

clean: 
	rm *.o ifj16_rtg