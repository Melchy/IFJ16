/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

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