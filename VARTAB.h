#ifndef VARTAB_H
#define VARTAB_H

#include "STR.h"

#include <stdbool.h>

#define INIT_SIZE 10

typedef struct ArrInt_t{
	int *arr;
	int size;
	int act_index;
}ArrInt_t;

typedef struct ArrDouble_t{
	double *arr;
	int size;
	int act_index;
}ArrDouble_t;

typedef struct ArrStr_t{
	S_String **arr;
	int size;
	int act_index;
}ArrStr_t;

typedef struct ArrBool_t{
	bool *arr;
	int size;
	int act_index;
}ArrBool_t;

void VT_InitTabs();

int VT_AddInt(int val);

int VT_AddDouble(double val);

int VT_AddStr(S_String *val);

int VT_AddBool(bool val);

// pouze pro ucely testovani
void VT_PrintAll();

#endif