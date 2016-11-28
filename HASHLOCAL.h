/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#ifndef HASHLOCAL_H
#define HASHLOCAL_H

#include "hash.h"
#include "str.h"
#include "vartab.h"

#include <stdlib.h>

typedef struct S_VarL{
	S_String *ID;
	t_Value *value;
	int nest;
	struct S_VarL *next;
}S_VarL;

typedef S_VarL** tabLocal;

void HASHLOCAL_InitLocal();
void HASHLOCAL_Print();

void HASHLOCAL_Add(S_VarL *data);
S_VarL *HASHLOCAL_Find(S_String *ID);
void HASHLOCAL_RemoveNest(int nest);

#endif