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

void HASHVAR_PrintL()
{
	printf("**LOCAL HASH TAB**\n");
	S_Var *v;
	for (int i = 0; i < SIZETAB; i++)
	{
		v = local->tab[i];
		while(v != NULL)
		{
			printf("[ID]: %s [val]: ", v->ID->str); VT_PrintOne(v->value);
			v = v->next;
		}
	}
	putchar('\n');
}

void HASHVAR_RemoveTable()
{
	htabL *tmp = local->prev;
	printf("%s\n", "vypis toho co by se melo smazat (HASHVAR.c rade 53)");
	HASHVAR_PrintL();
	if(local != NULL){
		S_Var *act; S_Var *prev = NULL;
		for (int i = 0; i < SIZETAB; i++)
		{
			act = local->tab[i];
			while(act != NULL){
				prev = act;
				MEM_free(act->ID);
				if(act->value->type != tkn_TRUE && act->value->type != tkn_FALSE && act->value->VT_index != -1)
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

void HASHVAR_PrintG()
{
	printf("**GLOBAL HASH TAB**\n");
	S_Var *v;
	for (int i = 0; i < SIZETAB; i++)
	{
		v = global[i];
		while(v != NULL)
		{
			printf("[ID]: %s [val]: ", v->ID->str); VT_PrintOne(v->value);
			v = v->next;
		}
	}
	putchar('\n');
}

