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
	t_Tree *prev;
}t_Tree;

/* Alokace noveho stromu a pridani do seznamu stromu */
void Tree_Create();

bool Tree_Empty();

void Tree_AddOp(int token);

void Tree_AddNode(t_Value *value);

void Tree_AddAssignment();

void Tree_AddID(S_String *attr);

S_String *Tree_PopActStr();

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

#endif