#include <stdio.h>
#include <stdlib.h>

/* ---Deklarace struktur--- */

typedef struct S_Token{
	int type;
	char *attribute;
	struct S_Token *prev;
	struct S_Token *next;
} *S_Token;

typedef struct L_Token{
    S_Token First;
    S_Token Act;
    S_Token Last;
} L_Token;

/* ---Deklarace funkci--- */

void TL_Init(L_Token *);
void TL_Dispose(L_Token *);
void TL_GoToFirst(L_Token *);
void TL_GoToLast(L_Token *);
void TL_Predecessor(L_Token *);
void TL_Successor(L_Token *);
void TL_DeleteAct(L_Token *);
void TL_DeletePrev(L_Token *);
void TL_DeleteNext(L_Token *);
void TL_InsertFirst(L_Token *, int , char *);
void TL_InsertLast(L_Token *, int , char *);
void TL_InsertPrev(L_Token *, int, char *);
void TL_InsertNext(L_Token *, int, char *);

S_Token TL_GetFirst(L_Token *);
S_Token TL_GetLast(L_Token *);
S_Token TL_GetAct(L_Token *);
S_Token TL_GetPrev(L_Token *);
S_Token TL_GetNext(L_Token *);
