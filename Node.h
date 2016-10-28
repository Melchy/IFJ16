#ifndef NODE_H
#define NODE_H

#include "STR.h"
#include "HASHVAR.h"
#include "VARTAB.h"

#include <stdbool.h>

/* Uzel stromu - jedna rodicovska vetev, dva potomci */
typedef struct t_Node t_Node;
struct t_Node{
	t_Value *value;
	t_Node *parent;
	t_Node *l_child;
	t_Node *r_child;
};

bool Node_IsOp(t_Node *n);

bool Node_IsBottom(t_Node *n);

bool Node_IsUnary(t_Node *n);

bool Node_IsLParen(t_Node *n);

t_Value *Node_GetValue(t_Node *n);

int Node_GetType(t_Node *n);

int Node_GetIndex(t_Node *n);

int Node_GetInt(t_Node *n);

double Node_GetDouble(t_Node *n);

S_String *Node_GetStr(t_Node *n);

t_Node *Node_GetLChild(t_Node *n);

t_Node *Node_GetRChild(t_Node *n);

#endif