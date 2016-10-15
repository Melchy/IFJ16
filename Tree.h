#ifndef TREE_H
#define TREE_H

#include "STR.h"

typedef struct t_Branch t_Branch;
struct t_Branch{
	int token;
	S_String *attr;
	t_Branch *parent;
	t_Branch *l_child;
	t_Branch *r_child;
};

typedef struct t_Tree t_Tree;
typedef struct t_Tree{
	t_Branch *Top;
	t_Branch *Act;
	t_Tree *prev;
}t_Tree;

void Tree_Create();

void Tree_AddOp(int token);

void Tree_AddVal(int token, S_String *attr);

void Tree_NestIn();

void Tree_NestOut();

void Tree_RemoveParen();

void Tree_GoTop();

void Tree_Dispose();

void Tree_Print();

#endif