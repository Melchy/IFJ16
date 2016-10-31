#ifndef HASHFCE_H
#define HASHFCE_H

#include "STR.h"

#include <stdbool.h>

typedef struct S_Fce{
	S_String *ID;
	S_String *type;
	long offset;
	struct S_Fce *next;
}S_Fce;

typedef S_Fce** htabF;

void HASHFCE_InitFce();

bool HASHFCE_Add(S_Fce *data);

bool HASHFCE_Remove(S_String *ID);

S_Fce *HASHFCE_Find(S_String *ID);

void HASHFCE_Print();

#endif