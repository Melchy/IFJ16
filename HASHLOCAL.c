#include "HASHLOCAL.h"

static tabLocal local;

void HASHLOCAL_InitLocal(){
	local = MEM_malloc(SIZETAB*(sizeof(S_VarL *)));
	for (int i = 0; i < SIZETAB; i++)
		local[i] = NULL;
}

void HASHLOCAL_Print(){
	printf("**LOCAL HASH TAB**\n");
	S_VarL *v;
	for (int i = 0; i < SIZETAB; i++)
	{
		v = local[i];
		while(v != NULL)
		{
			printf("[nest]: %d [ID]: %s [val]: ", v->nest, v->ID->str); VT_PrintOne(v->value);
			v = v->next;
		}
	}
	putchar('\n');
}

void HASHLOCAL_Add(S_VarL *data){
	S_VarL *tmp = local[HASH_Spread(data->ID)];
	data->next = tmp;
	local[HASH_Spread(data->ID)] = data;
}

S_VarL *HASHLOCAL_Find(S_String *ID){
	S_VarL *tmp = local[HASH_Spread(ID)];
	while(tmp != NULL && STR_Compare(ID, tmp->ID)){
		tmp = tmp->next;
	}
	return tmp;
}

void HASHLOCAL_RemoveNest(int nest){
	S_VarL *act; S_VarL *prev = NULL;

	for (int i = 0; i < SIZETAB; i++)
	{
		
		act = local[i];
		while(act != NULL && act->nest == nest){

			prev = act;
			MEM_free(act->ID);
			if(act->value->type != tkn_TRUE && act->value->type != tkn_FALSE && act->value->VT_index != -1)
				MEM_free(act->value);
			MEM_free(act);

			act = prev->next;
		}
		local[i] = act;
	}
}