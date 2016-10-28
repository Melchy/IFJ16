#ifndef TREE_H
#define TREE_H

#include "STR.h"
#include "Node.h"
#include "VARTAB.h"

/* Struktura stromu, ukazatele na top uzel stromu, aktivni uzel a na predchazejci strom */
typedef struct t_Tree t_Tree;
typedef struct t_Tree{
	t_Node *Top;
	t_Node *Act;
	t_Tree *prev;
}t_Tree;

/* Alokace noveho stromu a pridani do seznamu stromu */
void Tree_Create();

/* Pridani operatoru nebo operandu do stromu */
void Tree_Add(int token);

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