#include "HASHFCE.h"
#include "STR.h"
#include "HASH.h"
#include "MEM.h"

#include <stdbool.h>

static htabF global;

void HASHFCE_InitFce()
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
	MEM_free(tmp->type);
	MEM_free(tmp);
	return true;
}