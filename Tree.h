#ifndef TREE_H
#define TREE_H

#include "STR.h"

/* Uzel stromu - jedna jedna rodicovska vetev, dva potomci */
typedef struct t_Node t_Node;
struct t_Node{
	int token;
	S_String *attr;
	t_Node *parent;
	t_Node *l_child;
	t_Node *r_child;
};

/* Struktura stromu, ukazatele na top uzel stromu, aktivni uzel a na predchazejci strom */
typedef struct t_Tree t_Tree;
typedef struct t_Tree{
	t_Node *Top;
	t_Node *Act;
	t_Tree *prev;
}t_Tree;

/* Alokace noveho stromu a pridani do seznamu stromu */
void Tree_Create();

/* Pridani operace do stromu na zaklade jejich priorit */
void Tree_AddOp(int token);

/* Pridani hodnoty (literal nebo promenna) do stromu */
void Tree_AddVal(int token, S_String *attr);

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

#endif