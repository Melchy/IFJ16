#ifndef FIO_H 
#define FIO_H

#include <stdbool.h>

//! bool FIO_MoveToPosition(int newPos);

//! int FIO_GetPosition();

bool FIO_Open(char *filename);

int FIO_GetChar();

int FIO_UngetChar(int c);

bool FIO_Close();

#endif