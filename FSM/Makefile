CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic

all: rtg

rtg: main.c rtg_FSM.c rtg_FSM.h
	$(CC) $(CFLAGS) main.c rtg_FSM.c -o rtg