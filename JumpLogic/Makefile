CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic

all: test

test: JumpLogicTest.o JumpLogic.o
	$(CC) $(CFLAGS) JumpLogicTest.o JumpLogic.o -o JumpLogicTest

JumpLogic.o: JumpLogic.c JumpLogic.h str.h
	$(CC) $(CFLAGS) -c JumpLogic.c -o JumpLogic.o

JumpLogicTest.o: JumpLogicTest.c JumpLogic.h
	$(CC) $(CFLAGS) -c JumpLogicTest.c -o JumpLogicTest.o

clean: 
	rm *.o JumpLogicTest