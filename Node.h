#ifndef NODE_H
#define NODE_H

#include "STR.h"

#include <stdbool.h>

/* Uzel stromu - jedna jedna rodicovska vetev, dva potomci */
typedef struct t_Node t_Node;
struct t_Node{
	int token;
	S_String *attr;
	t_Node *parent;
	t_Node *l_child;
	t_Node *r_child;
};

bool Node_IsOp(t_Node *n);

bool Node_IsBottom(t_Node *n);

bool Node_IsUnary(t_Node *n);

int Node_GetTkn(t_Node *n);

int Node_GetInt(t_Node *n);

t_Node *Node_GetLChild(t_Node *n);

t_Node *Node_GetRChild(t_Node *n);

#endif