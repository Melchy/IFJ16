#include "HASH.h"
#include "HASHVAR.h"
#include "STR.h"
#include "VARTAB.h"
#include "MEM.h"

#include <stdbool.h>

static htabG global;
static htabL *local;

void HASHVAR_InitGlobal()
{
	global = MEM_malloc(SIZETAB*(sizeof(S_Var *)));
	for (int i = 0; i < SIZETAB; i++)
	{
		global[i] = NULL;
	}
}

void HASHVAR_AddTable()
{
	htabL *prev = local;
	local = MEM_malloc(sizeof(htabL));
	local->prev = prev;
	local->tab = MEM_malloc(SIZETAB*(sizeof(S_Var *)));
	for (int i = 0; i < SIZETAB; i++)
	{
		local->tab[i] = NULL;
	}

}

void HASHVAR_RemoveTable()
{
	htabL *tmp = local->prev;
	if(local != NULL){
		S_Var *act; S_Var *prev = NULL;
		for (int i = 0; i < SIZETAB; i++)
		{
			act = local->tab[i];
			while(act != NULL){
				prev = act;
				MEM_free(act->ID);
				MEM_free(act->value);
				MEM_free(act);
				act = prev->next;
			}
		}
	}
	local = tmp;
}


bool HASHVAR_AddG(S_Var *data)
{
	S_Var *tmp = global[HASH_Spread(data->ID)];
	if (tmp == NULL){
		global[HASH_Spread(data->ID)] = data;
		return true;
	}

	while(tmp->next != NULL){
		tmp = tmp->next;
	}

	tmp->next = data;
	return true;
}

S_Var *HASHVAR_FindG(S_String *ID)
{
	S_Var *tmp = global[HASH_Spread(ID)];
	while(tmp != NULL && STR_Compare(ID, tmp->ID)){
		tmp = tmp->next;
	}
	return tmp;
}

bool HASHVAR_RemoveG(S_String *ID)
{
	S_Var *prev = NULL;
	S_Var *tmp = global[HASH_Spread(ID)];
	while(tmp != NULL && STR_Compare(ID, tmp->ID)){
		prev = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL) return false;

	if(prev == NULL){
		global[HASH_Spread(ID)] = tmp->next;
	} else {
		prev->next = tmp->next;
	}
	MEM_free(tmp->ID);
	MEM_free(tmp->value);
	MEM_free(tmp);
	return true;
}

bool HASHVAR_AddL(S_Var *data)
{
	S_Var *tmp = local->tab[HASH_Spread(data->ID)];
	if (tmp == NULL){
		local->tab[HASH_Spread(data->ID)] = data;
		return true;
	}

	while(tmp->next != NULL){
		tmp = tmp->next;
	}

	tmp->next = data;
	return true;
}

S_Var *HASHVAR_FindL(S_String *ID)
{
	if(local == NULL)
		return NULL;

	S_Var *tmp = local->tab[HASH_Spread(ID)];
	while(tmp != NULL && STR_Compare(ID, tmp->ID)){
		tmp = tmp->next;
	}
	return tmp;
}

bool HASHVAR_RemoveL(S_String *ID)
{
	S_Var *prev = NULL;
	S_Var *tmp = local->tab[HASH_Spread(ID)];
	while(tmp != NULL && STR_Compare(ID, tmp->ID)){
		prev = tmp;
		tmp = tmp->next;
	}
	if(tmp == NULL) return false;

	if(prev == NULL){
		local->tab[HASH_Spread(ID)] = tmp->next;
	} else {
		prev->next = tmp->next;
	}
	MEM_free(tmp->ID);
	MEM_free(tmp->value);
	MEM_free(tmp);
	return true;	
}
