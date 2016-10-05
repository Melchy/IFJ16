#include "FIO.h"

#include <stdio.h>

static FILE *f;

/* true pokud se uspesne otevre */
bool FIO_Open(char *filename)
{
	f = fopen(filename, "r");
	if(f == NULL){
		fprintf(stderr, "Unable to close the source file '%s'.\n", filename);
		return false;
	}
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
	if(!fclose(f))
		return true;
	fprintf(stderr, "Unable to close the source file.");
	return false;
}