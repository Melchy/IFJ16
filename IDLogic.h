/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#ifndef IDLOGIC_H
#define IDLOGIC_H

#include "STR.h"
#include "VARTAB.h"
#include "HASHFCE.h"
#include "HASHLOCAL.h"

#include <stdbool.h>

void IL_NestDown();

void IL_NestUp();

void IL_RemoveNest();

int IL_GetNesting();

void IL_SetReachable(int n);

int IL_GetReachable();

void IL_SetClass(S_String *newClass);

S_String *IL_GetClass();

void IL_AllocVar(S_String *ID, int token, bool global);

void IL_SetVal(S_String *ID, t_Value *value);

t_Value *IL_GetVal(S_String *ID);

void IL_InitFce(S_String *ID, int returnType, long offset, S_Param *firstParam);

S_Fce *IL_GetFce(S_String *ID);

bool IL_AllocParam(S_Fce *fce, t_Value *val, int argNumber);

#endif