#ifndef HASHLOCAL_H
#define HASHLOCAL_H

#include "HASH.h"
#include "STR.h"
#include "VARTAB.h"

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