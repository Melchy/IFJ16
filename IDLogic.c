#include "IDLogic.h"
#include "STR.h"
#include "VARTAB.h"
#include "HASHVAR.h"
#include "HASHLOCAL.h"
#include "HASHFCE.h"
#include "MEM.h"
#include "ERROR.h"

static S_String *ActClass;
static int nesting;
static int reachableNesting;

void IL_NestDown()
{ nesting--; }

void IL_NestUp()
{ nesting++; }

void IL_RemoveNest()
{ HASHLOCAL_RemoveNest(nesting); }

int IL_GetNesting()
{ return nesting; }

void IL_SetReachable(int n)
{ reachableNesting = n; }

int IL_GetReachable()
{ return reachableNesting; }

void IL_SetClass(S_String *newClass)
{
	ActClass = STR_Create(newClass->str);
}

S_String *IL_GetClass()
{
	return ActClass;
}

static void allocLocal(S_String *ID, int token){
	S_VarL *new = MEM_malloc(sizeof(S_VarL));
	new->value = VT_GetEmpty(token);
	new->ID = STR_Create(ID->str);
	new->nest = nesting;
	S_VarL *found = HASHLOCAL_Find(new->ID);
	if(found != NULL && found->nest >= reachableNesting){
		printf("nastalo\n");
		ERROR_exit(SEM_ERR_DEF);
	}
	HASHLOCAL_Add(new);
}

void IL_AllocVar(S_String *ID, int token, bool global)
{
	if(!global){
		allocLocal(ID,token);
		return;
	}

	S_Var *new = MEM_malloc(sizeof(S_Var));
	new->value = VT_GetEmpty(token);
	new->next = NULL;
	if(global) {
		S_String *fullID = STR_Create(ActClass->str);
		STR_AddChar(fullID, '.');
		STR_ConCat(fullID, ID);
		new->ID = fullID;
		if(HASHVAR_FindG(new->ID) != NULL || HASHFCE_Find(new->ID) != NULL){
			ERROR_exit(SEM_ERR_DEF);
		}
		HASHVAR_AddG(new);
	}
	/* 
	else {
		new->ID = STR_Create(ID->str);
		if(HASHVAR_FindL(new->ID) != NULL){
			ERROR_exit(SEM_ERR_DEF);
		}
		HASHVAR_AddL(new);
	}*/
}

void IL_SetVal(S_String *ID, t_Value *value)
{
	S_Var *var;
	S_VarL *varl;
	if(STR_FindChar(ID, '.') != -1)
		var = HASHVAR_FindG(ID);
	else{
		varl = HASHLOCAL_Find(ID);
		if(varl == NULL || varl->nest < reachableNesting)
		{
			S_String *fullID = STR_Create(ActClass->str);
			STR_AddChar(fullID, '.');
			STR_ConCat(fullID, ID);
			var = HASHVAR_FindG(fullID);
		}
		else{
			varl->value = value;
			return;
		}
	}
	if(var == NULL){
		ERROR_exit(SEM_ERR_DEF);
	}
	
	var->value = value;
}

t_Value *IL_GetVal(S_String *ID)
{
	S_Var *var;
	S_VarL *varl;
	if(STR_FindChar(ID, '.') != -1)
		var = HASHVAR_FindG(ID);
	else{
		varl = HASHLOCAL_Find(ID);
		if(varl == NULL || varl->nest < reachableNesting)		
		{
			S_String *fullID = STR_Create(ActClass->str);
			STR_AddChar(fullID, '.');
			STR_ConCat(fullID, ID);
			var = HASHVAR_FindG(fullID);
		}
		else{
			return varl->value;
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
	if(par->type == tkn_ALLTYPES)
		goto skipSwitch;

	switch(val->type){
		case tkn_NUM:
		case tkn_INT:
			if(par->type == tkn_INT)
				break;
			if(par->type == tkn_DOUBLE)
				val = VT_AddDouble(VT_GetInt(val->VT_index));
			else
				ERROR_exit(SEM_ERR_TYPE);
		break;
		case tkn_REAL:
		case tkn_DOUBLE:
			if(par->type == tkn_DOUBLE)
				break;
			if(par->type == tkn_INT)
				val = VT_AddInt(VT_GetDouble(val->VT_index));
			else
				ERROR_exit(SEM_ERR_TYPE);
		break;
		case tkn_STRING:
		case tkn_LIT:
			if(par->type != tkn_STRING)
				ERROR_exit(SEM_ERR_TYPE);
		break;
		case tkn_TRUE:
		case tkn_FALSE:
			if(par->type != tkn_BOOL)
				ERROR_exit(SEM_ERR_TYPE);
		break;
		default:
		break;
	}

	skipSwitch:
	
	IL_AllocVar(par->ID, val->type, false);
	IL_SetVal(par->ID, val);
	if(par->next == NULL)
		return true;
	return false;
}