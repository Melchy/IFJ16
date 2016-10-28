#include "ERROR.h"
#include "MEM.h"
#include "FIO.h"

#include <stdlib.h>

void ERROR_exit(int errCode)
{
	MEM_clearAll();
	FIO_Close();
	exit(errCode);
}

void ERROR_exit2(int errCode, void *ptrtofree)
{
	free(ptrtofree);
	ERROR_exit(errCode);
}