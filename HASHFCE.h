#ifndef HASHFCE_H
#define HASHFCE_H

#include "STR.h"

#include <stdbool.h>

typedef struct S_Param{
	S_String *ID;
	int type;
	struct S_Param *next;
}S_Param;

typedef struct S_Fce{
	S_String *ID;
	S_String *type;
	long offset;
	S_Param *firstParam;
	struct S_Fce *next;
}S_Fce;

typedef S_Fce** htabF;

void HASHFCE_InitFceTab();

bool HASHFCE_Add(S_Fce *data);

bool HASHFCE_Remove(S_String *ID);

S_Fce *HASHFCE_Find(S_String *ID);

void HASHFCE_Print();

#endif