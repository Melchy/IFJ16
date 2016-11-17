#include "IDLogic.h"
#include "STR.h"
#include "VARTAB.h"
#include "HASHVAR.h"
#include "HASHFCE.h"
#include "MEM.h"
#include "ERROR.h"

static S_String *ActClass;

void IL_SetClass(S_String *newClass)
{
	ActClass = STR_Create(newClass->str);
}

S_String *IL_GetClass()
{
	return ActClass;
}

void IL_AllocVar(S_String *ID, int token, bool global)
{
	S_Var *new = MEM_malloc(sizeof(S_Var));
	if(global){
		S_String *fullID = STR_Create(ActClass->str);
		STR_AddChar(fullID, '.');
		STR_ConCat(fullID, ID);
		new->ID = fullID;
	} else {
		new->ID = STR_Create(ID->str);
	}
	new->value = VT_GetEmpty(token);
	new->next = NULL;
	if(global){
		if(HASHVAR_FindG(new->ID) != NULL || HASHFCE_Find(new->ID) != NULL){
			ERROR_exit(SEM_ERR_DEF);
		}
		HASHVAR_AddG(new);
	}
	else{
		if(HASHVAR_FindL(new->ID) != NULL){
			ERROR_exit(SEM_ERR_DEF);
		}
		HASHVAR_AddL(new);
	}
}

void IL_SetVal(S_String *ID, t_Value *value)
{
	S_Var *var;
	if(STR_FindChar(ID, '.') != -1)
		var = HASHVAR_FindG(ID);
	else{
		var = HASHVAR_FindL(ID);
		if(var == NULL)
		{
			S_String *fullID = STR_Create(ActClass->str);
			STR_AddChar(fullID, '.');
			STR_ConCat(fullID, ID);
			var = HASHVAR_FindG(fullID);
		}
	}
	if(var == NULL)
		ERROR_exit(SEM_ERR_DEF);
	
	var->value = value;
}

t_Value *IL_GetVal(S_String *ID)
{
	S_Var *var;
	if(STR_FindChar(ID, '.') != -1)
		var = HASHVAR_FindG(ID);
	else{
		var = HASHVAR_FindL(ID);
		if(var == NULL)
		{
			S_String *fullID = STR_Create(ActClass->str);
			STR_AddChar(fullID, '.');
			STR_ConCat(fullID, ID);
			var = HASHVAR_FindG(fullID);
		}
	}
	if(var == NULL){
		ERROR_exit(SEM_ERR_DEF);
	}
	return var->value;
}

void IL_InitFce(S_String *ID, int returnType, long offset, S_Param *firstParam)
{
	S_Fce *new = MEM_malloc(sizeof(S_Fce));
	S_String *fullID = STR_Create(ActClass->str);
	STR_AddChar(fullID, '.');
	STR_ConCat(fullID, ID);
	new->ID = fullID;
	new->returnType = returnType;
	new->offset = offset;
	new->firstParam = firstParam;
	new->next = NULL;
	if(HASHVAR_FindG(new->ID) != NULL || HASHFCE_Find(new->ID) != NULL){
		ERROR_exit(SEM_ERR_DEF);
	}
	HASHFCE_Add(new);
}

S_Fce *IL_GetFce(S_String *ID)
{
	S_String *fullID;
	if(STR_FindChar(ID, '.') == -1){
		fullID = STR_Create(ActClass->str);
		STR_AddChar(fullID, '.');
		STR_ConCat(fullID, ID);
	}else{
		fullID = STR_Create(ID->str);
	}

	S_Fce *f = HASHFCE_Find(fullID);
	if(f == NULL) ERROR_exit(SEM_ERR_DEF);
	MEM_free(fullID);
	return f;
}

bool IL_AllocParam(S_Fce *fce, t_Value *val, int argNumber)
{
	S_Param *par = fce->firstParam;
	for (int i = 0; i < argNumber; i++)
	{
		if(par == NULL)
			return false;
		par = par->next;
	}
	if(par == NULL)
		return false;

	IL_AllocVar(par->ID, val->type, false);
	IL_SetVal(par->ID, val);
	if(par->next == NULL)
		return true;
	return false;
}