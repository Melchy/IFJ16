/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#ifndef HASHVAR_H
#define HASHVAR_H

#include "STR.h"
#include "VARTAB.h"

typedef struct S_Var{
	S_String *ID;
	t_Value *value;
	struct S_Var *next;
}S_Var;

typedef S_Var** htabG;

void HASHVAR_InitGlobal();
bool HASHVAR_AddG(S_Var *data);
S_Var *HASHVAR_FindG(S_String *ID);
bool HASHVAR_RemoveG(S_String *ID);
void HASHVAR_PrintG();

#endif