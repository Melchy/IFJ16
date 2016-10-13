#include "TokenList.h"

void TL_Init(L_Token* L)
{
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void TL_Dispose(L_Token* L)
{
	S_Token temp;
	S_Token disp = L->First;

	while(disp != NULL)
	{
		temp = disp->next;
		free(disp);
		disp = temp;
	}

  	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void TL_GoToFirst(L_Token* L)
{
	L->Act = L->First;
}

void TL_GoToLast(L_Token* L)
{
	L->Act = L->Last;
}

void TL_Predecessor(L_Token* L)
{
	if(L->Act != NULL)
	{
		L->Act = L->Act->prev;
	}
}

void TL_Successor(L_Token* L)
{
	if(L->Act != NULL)
	{
		L->Act = L->Act->next;
	}
}

void TL_DeleteAct(L_Token* L)
{
	if(L->Act != NULL)
	{
		S_Token temp = L->Act;

		if(L->Act->prev == NULL && L->Act->next != NULL)
		{
			L->Act = temp->next;
			L->Act->prev = NULL;
			L->First = L->Act;
		}
		else if(L->Act->next == NULL && L->Act->prev != NULL)
		{
			L->Act = temp->prev;
			L->Act->next = NULL;
			L->Last = L->Act;
		}
		else if(L->Act->prev == NULL && L->Act->next == NULL)
		{
			L->First = NULL;
			L->Act = NULL;
			L->Last = NULL;
		}
		else
		{
			L->Act = temp->prev;
			L->Act->next = temp->next;
			temp->next->prev = L->Act;
		}

		free(temp);
	}
}

void TL_DeletePrev(L_Token* L)
{
	if(L->Act != NULL && L->Act->prev != NULL)
	{
		S_Token temp = L->Act->prev;
		L->Act->prev = temp->prev;

		if(temp->prev != NULL)
		{
			L->Act->prev->next = L->Act;
		}
		else
		{
			L->First = L->Act;
		}

		free(temp);
	}
}

void TL_DeleteNext(L_Token* L)
{
	if(L->Act != NULL && L->Act->next != NULL)
	{
		S_Token temp = L->Act->next;
		L->Act->next = temp->next;

		if(temp->next != NULL)
		{
			L->Act->next->prev = L->Act;
		}
		else
		{
			L->Last = L->Act;
		}

		free(temp);
	}
}

void TL_InsertFirst(L_Token* L, int token_number, char* str)
{
	S_Token temp = L->First;
	L->First = malloc(sizeof(struct S_Token));

	if(L->First == NULL)
	{
		printf("Error! \n");
	}
	else
	{
		L->First->type = token_number;
		L->First->attribute = str;
		L->First->prev = NULL;
		L->First->next = temp;
		
		if(temp == NULL)
		{
			L->Last = L->First;
		}
		else
		{
			temp->prev = L->First;
		}
	}
}

void TL_InsertLast(L_Token* L, int token_number, char* str)
{
	S_Token temp = L->Last;
	L->Last = malloc(sizeof(struct S_Token));

	if(L->Last == NULL)
	{
		printf("Error! \n");
	}
	else
	{
		L->Last->type = token_number;
		L->Last->attribute = str;
		L->Last->prev = temp;
		L->Last->next = NULL;

		if(temp == NULL)
		{
			L->First = L->Last;
		}
		else
		{
			temp->next = L->Last;
		}
	}	
}

void TL_InsertPrev(L_Token* L, int token_number, char *str)
{
	if(L->Act != NULL)
	{
		S_Token temp = L->Act->prev;
		L->Act->prev = malloc(sizeof(struct S_Token));

		if(L->Act->prev == NULL)
		{
			printf("Error! \n");
		}
		else
		{
			L->Act->prev->type = token_number;
			L->Act->prev->attribute = str;
			L->Act->prev->prev = temp;
			L->Act->prev->next = L->Act;
			
			if(temp != NULL)
			{
				temp->next = L->Act->prev;
			}
			else
			{
				L->First = L->Act->prev;
			}
		}
	}	
}

void TL_InsertNext(L_Token* L, int token_number, char *str)
{
	if(L->Act != NULL)
	{
		S_Token temp = L->Act->next;
		L->Act->next = malloc(sizeof(struct S_Token));

		if(L->Act->next == NULL)
		{
			printf("Error! \n");
		}
		else
		{
			L->Act->next->type = token_number;
			L->Act->next->attribute = str;
			L->Act->next->next = temp;
			L->Act->next->prev = L->Act;

			if(temp != NULL)
			{
				temp->prev = L->Act->next;
			}
			else
			{
				L->Last = L->Act->next;
			}
		}
	}	
}

S_Token TL_GetFirst(L_Token* L)
{
	return L->First;
}

S_Token TL_GetLast(L_Token *L)
{
	return L->Last;
}

S_Token TL_GetAct(L_Token* L)
{
	return L->Act;
}

S_Token TL_GetPrev(L_Token* L)
{
	return L->Act->prev;
}

S_Token TL_GetNext(L_Token* L)
{
	return L->Act->next;
}