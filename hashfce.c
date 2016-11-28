/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#include "hashfce.h"
#include "str.h"
#include "hash.h"
#include "mem.h"

#include <stdbool.h>

static htabF global;

void HASHFCE_InitFceTab()
{
	global = MEM_malloc(SIZETAB*(sizeof(S_Fce *)));
	for (int i = 0; i < SIZETAB; i++)
	{
		global[i] = NULL;
	}
}

bool HASHFCE_Add(S_Fce *data)
{
	S_Fce *tmp = global[HASH_Spread(data->ID)];
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

S_Fce *HASHFCE_Find(S_String *ID)
{
	S_Fce *tmp = global[HASH_Spread(ID)];
	while(tmp != NULL && STR_Compare(ID, tmp->ID)){
		tmp = tmp->next;
	}
	return tmp;
}

bool HASHFCE_Remove(S_String *ID)
{
	S_Fce *prev = NULL;
	S_Fce *tmp = global[HASH_Spread(ID)];
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
	S_Param *q;
	S_Param *p = tmp->firstParam;
	while(p != NULL)
	{
		q = p;
		MEM_free(q);
		p = p->next;
	}
	MEM_free(tmp);
	return true;
}

void HASHFCE_Print()
{
	printf("**FCE GLOBAL HASH TAB**\n");
	S_Fce *f;
	for (int i = 0; i < SIZETAB; i++)
	{
		f = global[i];
		while(f != NULL)
		{
			printf("[ID]: %s [returnType]: %d [offset]: %ld", f->ID->str, f->returnType, f->offset);
			f = f->next;
			putchar('\n');
		}
	}
	putchar('\n');
}