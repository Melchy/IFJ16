CC=gcc
CFLAGS=-O3 -Wall -Wextra -std=c11 -pedantic

all: ifj16_rtg

ifj16_rtg: main.o scan.o fio.o error.o mem.o tree.o node.o expr.o vartab.o str.o hashvar.o hash.o idlogic.o hashfce.o jl.o pars.o ial.o hashlocal.o
	$(CC) $(CFLAGS) main.o scan.o fio.o error.o mem.o tree.o node.o expr.o vartab.o str.o hashvar.o hash.o idlogic.o hashfce.o jl.o pars.o ial.o hashlocal.o -o ifj16_rtg

hashlocal.o: hashlocal.c hashlocal.h hash.h str.h vartab.h
	$(CC) $(CFLAGS) -c hashlocal.c -o hashlocal.o

idlogic.o: idlogic.c idlogic.h str.h vartab.h hashvar.h hashfce.h
	$(CC) $(CFLAGS) -c idlogic.c -o idlogic.o

hash.o: hash.c hash.h str.h
	$(CC) $(CFLAGS) -c hash.c -o hash.o

hashfce.o: hashfce.c hashfce.h hash.h str.h mem.h
	$(CC) $(CFLAGS) -c hashfce.c -o hashfce.o

hashvar.o: hashvar.c hashvar.h hash.h str.h vartab.h mem.h
	$(CC) $(CFLAGS) -c hashvar.c -o hashvar.o

str.o: str.c str.h mem.h error.h fio.h
	$(CC) $(CFLAGS) -c str.c -o str.o

vartab.o: vartab.c vartab.h mem.h str.h
	$(CC) $(CFLAGS) -c vartab.c -o vartab.o

expr.o: expr.c expr.h node.h tree.h tokens.h error.h vartab.h
	$(CC) $(CFLAGS) -c expr.c -o expr.o

node.o: node.c node.h tokens.h
	$(CC) $(CFLAGS) -c node.c -o node.o

tree.o: tree.c tree.h str.h mem.h tokens.h node.h
	$(CC) $(CFLAGS) -c tree.c -o tree.o

mem.o: mem.c mem.h error.h
	$(CC) $(CFLAGS) -c mem.c -o mem.o

error.o: error.c error.h mem.h fio.h
	$(CC) $(CFLAGS) -c error.c -o error.o

fio.o: fio.c fio.h error.h
	$(CC) $(CFLAGS) -c fio.c -o fio.o

scan.o: scan.c scan.h fio.h str.h mem.h error.h tokens.h
	$(CC) $(CFLAGS) -c scan.c -o scan.o

jl.o: jl.c jl.h mem.h
	$(CC) $(CFLAGS) -c jl.c -o jl.o

ial.o: mem.h mem.c str.h str.c
	$(CC) $(CFLAGS) -c ial.c -o ial.o

pars.o: pars.c pars.h scan.h fio.h str.h mem.h tree.h tokens.h expr.h vartab.h hashvar.h idlogic.h hashfce.h jl.h ial.h
	$(CC) $(CFLAGS) -c pars.c -o pars.o


main.o: main.c scan.h fio.h str.h mem.h tree.h tokens.h expr.h vartab.h hashvar.h idlogic.h hashfce.h jl.h pars.h ial.h
	$(CC) $(CFLAGS) -c main.c -o main.o



clean: 
	rm *.o ifj16_rtg