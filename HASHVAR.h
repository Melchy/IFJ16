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
typedef struct htabL{
	S_Var** tab;
	struct htabL *prev;
}htabL;

void HASHVAR_InitGlobal();
void HASHVAR_PrintL();
bool HASHVAR_AddG(S_Var *data);
S_Var *HASHVAR_FindG(S_String *ID);
bool HASHVAR_RemoveG(S_String *ID);
void HASHVAR_PrintG();

void HASHVAR_AddTable();
void HASHVAR_RemoveTable();
bool HASHVAR_AddL(S_Var *data);
S_Var *HASHVAR_FindL(S_String *ID);
bool HASHVAR_RemoveL(S_String *ID);



#endif