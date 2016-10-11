#include "FIO.h"
#include "ERROR.h"

#include <stdio.h>

static FILE *f;

/* true pokud se uspesne otevre */
bool FIO_Open(char *filename)
{
	f = fopen(filename, "r");
	if(f == NULL){
		ERROR_exit(ENV_ERR);
	}
	FIO_Opened = true;
	return true;
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
bool FIO_Close()
{
	if(!fclose(f)){
		FIO_Opened = false;
		return true;
	}
	ERROR_exit(ENV_ERR);
	return false;
}