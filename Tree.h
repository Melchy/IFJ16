#ifndef TREE_H
#define TREE_H

#include "STR.h"

typedef struct t_Branch t_Branch;
struct t_Branch{
	int token;
	S_String* attr;
	t_Branch* parent;
	t_Branch* l_child;
	t_Branch* r_child;
};

typedef struct t_Tree{
	t_Branch* Top;
	t_Branch* Act;
}t_Tree;

t_Tree *Tree_Init();

void Tree_AddOp(t_Tree *T, int token);

void Tree_AddVal(t_Tree *T, int token, S_String *attr);

void Tree_NestIn(t_Tree *T);

void Tree_NestOut(t_Tree *T);

void Tree_GoTop(t_Tree *T);

void Tree_GoBot(t_Tree *T);

void Tree_Dispose(t_Tree *T);

void Tree_RemoveParen(t_Tree *T);

void Tree_Print(t_Tree *T);

#endif