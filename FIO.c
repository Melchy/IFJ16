#include "FIO.h"
#include "ERROR.h"

#include <stdio.h>

static FILE *f;
static bool FIO_Opened;

/* true pokud se uspesne otevre */
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

/* true pokud se uspesne zavre */
void FIO_Close()
{
	if(FIO_Opened)
		fclose(f);
}