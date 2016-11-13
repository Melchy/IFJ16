CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic

all: ifj16_rtg

ifj16_rtg: main.o SCAN.o FIO.o ERROR.o MEM.o Tree.o Node.o EXPR.o VARTAB.o STR.o SEM.o HASHVAR.o HASH.o IDLogic.o HASHFCE.o JL.o Pars.o
	$(CC) $(CFLAGS) main.o SCAN.o FIO.o ERROR.o MEM.o Tree.o Node.o EXPR.o VARTAB.o STR.o SEM.o HASHVAR.o HASH.o IDLogic.o HASHFCE.o JL.o Pars.o -o ifj16_rtg

IDLogic.o: IDLogic.c IDLogic.h STR.h VARTAB.h HASHVAR.h HASHFCE.h
	$(CC) $(CFLAGS) -c IDLogic.c -o IDLogic.o

HASH.o: HASH.c HASH.h STR.h
	$(CC) $(CFLAGS) -c HASH.c -o HASH.o

HASHFCE.o: HASHFCE.c HASHFCE.h HASH.h STR.h MEM.h
	$(CC) $(CFLAGS) -c HASHFCE.c -o HASHFCE.o

HASHVAR.o: HASHVAR.c HASHVAR.h HASH.h STR.h VARTAB.h MEM.h
	$(CC) $(CFLAGS) -c HASHVAR.c -o HASHVAR.o

SEM.o: SEM.c SEM.h VARTAB.h Tokens.h IDLogic.h HASHVAR.h ERROR.h
	$(CC) $(CFLAGS) -c SEM.c -o SEM.o

STR.o: STR.c STR.h MEM.h ERROR.h FIO.h
	$(CC) $(CFLAGS) -c STR.c -o STR.o

VARTAB.o: VARTAB.c VARTAB.h MEM.h STR.h
	$(CC) $(CFLAGS) -c VARTAB.c -o VARTAB.o

EXPR.o: EXPR.c EXPR.h Node.h Tree.h Tokens.h ERROR.h VARTAB.h SEM.h
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

JL.o: JL.c JL.h MEM.h
	$(CC) $(CFLAGS) -c JL.c -o JL.o

Pars.o: Pars.c Pars.h SCAN.h FIO.h STR.h MEM.h Tree.h Tokens.h EXPR.h VARTAB.h HASHVAR.h IDLogic.h HASHFCE.h JL.h
	$(CC) $(CFLAGS) -c Pars.c -o Pars.o


main.o: main.c SCAN.h FIO.h STR.h MEM.h Tree.h Tokens.h EXPR.h VARTAB.h HASHVAR.h IDLogic.h HASHFCE.h JL.h Pars.h
	$(CC) $(CFLAGS) -c main.c -o main.o



clean: 
	rm *.o ifj16_rtg