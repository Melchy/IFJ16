/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#include "hash.h"
#include "hashvar.h"
#include "str.h"
#include "vartab.h"
#include "mem.h"

#include <stdbool.h>

static htabG global;

void HASHVAR_InitGlobal()
{
	global = MEM_malloc(SIZETAB*(sizeof(S_Var *)));
	for (int i = 0; i < SIZETAB; i++)
		global[i] = NULL;
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

