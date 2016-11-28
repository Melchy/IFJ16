/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#include "fio.h"
#include "error.h"

#include <stdio.h>

static FILE *f;
static bool FIO_Opened;

void FIO_Open(char *filename)
{
	f = fopen(filename, "r");
	if(f == NULL){
		ERROR_exit(ENV_ERR);
	}
	FIO_Opened = true;
}

int FIO_GetChar()
{
	return fgetc(f);
}

int FIO_UngetChar(int c)
{
	return ungetc(c, f);
}

bool FIO_MoveToPosition(long newPos)
{
	if(!fseek(f, newPos, SEEK_SET))
		return true;
	return false;
}

long FIO_GetPosition()
{
	if (f != NULL)
		return ftell(f);
	return 0;
}

void FIO_Close()
{
	if(FIO_Opened) // pokud je vubec otevreny
		fclose(f);
}