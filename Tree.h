#ifndef TREE_H
#define TREE_H

#include "STR.h"
#include "Node.h"
#include "VARTAB.h"

#include <stdbool.h>

typedef struct t_Assign{
	S_String *ID;
	struct t_Assign *prev;
}t_Assign;

/* Struktura stromu, ukazatele na top uzel stromu, aktivni uzel a na predchazejci strom */
typedef struct t_Tree t_Tree;
typedef struct t_Tree{
	t_Node *Top;
	t_Node *Act;
	t_Assign *LastAssign;
	S_String *ActStr;
	bool assignable;
	bool isconcat;
	int nest;
	t_Tree *prev;
}t_Tree;

/* Alokace noveho stromu a pridani do seznamu stromu */
void Tree_Create();

/* Vraci true v pripade, ze je strom prazdny */
bool Tree_Empty();

/* Prida operator do stromu */
void Tree_AddOp(int token);

/* Prida node do stromu (hodnotu) */
void Tree_AddNode(t_Value *value);

/* Vytvori prirazeni */
void Tree_AddAssignment();

/* Prida ID do stromu (i pro prirazeni i pro operand) */
void Tree_AddID(S_String *attr);

/* Provadi pop identifikatoru, ktery byl poslan do stromu v predchozim kroku */
S_String *Tree_PopActStr();

/* Provadi pop posledniho identifikatoru z assign listu */
S_String *Tree_PopAssign();

/* Pridani leve zavorky do stromu - pomocna logika */
void Tree_NestIn();

/* Vynoreni k nejblizsi leve zavorce stromu */
void Tree_NestOut();

/* Zjednoduseni stromu smazanim levych zavorek (vola se az po precteni celeho vyrazu) */
void Tree_RemoveParen();

/* Nastaveni aktivity na top uzel stromu */
void Tree_GoTop();

/* Popnuti stromu ze seznamu stromu a jeho uvolneni */
void Tree_Dispose();

/* Tisk stromu na konzoli */
void Tree_Print();

t_Node *Tree_GetTopNode();

int Tree_GetNest();

bool Tree_GetConcat();

void Tree_SetConcat();

#endif