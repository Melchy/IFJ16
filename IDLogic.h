#ifndef IDLOGIC_H
#define IDLOGIC_H

#include "STR.h"
#include "VARTAB.h"
#include "HASHFCE.h"

#include <stdbool.h>

void IL_SetClass(S_String *newClass);

S_String *IL_GetClass();

void IL_AllocVar(S_String *ID, int token, bool global);

void IL_SetVal(S_String *ID, t_Value *value);

t_Value *IL_GetVal(S_String *ID);

void IL_InitFce(S_String *ID, S_String *type,size_t offset);

S_Fce *IL_GetFce(S_String *ID);

bool IL_AllocParam(S_Fce *fce, t_Value *val, int argNumber);

#endif