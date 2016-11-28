/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#ifndef HASHFCE_H
#define HASHFCE_H

#include "str.h"

#include <stdbool.h>

typedef struct S_Param{
	S_String *ID;
	int type;
	struct S_Param *next;
}S_Param;

typedef struct S_Fce{
	S_String *ID;
	int returnType;
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