#ifndef EXPR_H
#define EXPR_H

#include "VARTAB.h"
#include "STR.h"

void EXPR_Create();

void EXPR_AddSym(int token);

void EXPR_AddVal(int token, S_String *attr);

t_Value *EXPR_Solve();

#endif