#ifndef FIO_H 
#define FIO_H

#include <stdio.h>
#include <stdbool.h>

bool FIO_MoveToPosition(long newPos);

void FIO_Open(char *filename);

int FIO_GetChar();

int FIO_UngetChar(int c);

long FIO_GetPosition();

void FIO_Close();

#endif