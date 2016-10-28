#include "Node.h"
#include "Tokens.h"
#include "HASHVAR.h"
#include "VARTAB.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Vraci true, pokud je uzel operator */
bool Node_IsOp(t_Node *n)
{
	if(n->value->type >= tkn_PLUS && n->value->type <= tkn_OR)
		return true;
	return false;
}

bool Node_IsBottom(t_Node *n){
	if (n != NULL)
		return (n->l_child == NULL) && (n->r_child == NULL);
	return false;
}

bool Node_IsUnary(t_Node *n){
	return (n->l_child == NULL) && (n->r_child != NULL);
}

bool Node_IsLParen(t_Node *n){
	//if(n->value != NULL)
	//printf("TYPE:%d VT_index:%d\n", n->value->type, n->value->VT_index);
	return n->value->type == tkn_LPAREN;
}

t_Value *Node_GetValue(t_Node *n){
	return n->value;
}

int Node_GetType(t_Node *n){
	return n->value->type;
}

int Node_GetIndex(t_Node *n){
	return n->value->VT_index;
}

int Node_GetInt(t_Node *n){
	return VT_GetInt(n->value->VT_index);
}

double Node_GetDouble(t_Node *n){
	return VT_GetDouble(n->value->VT_index);
}

S_String *Node_GetStr(t_Node *n){
	return VT_GetStr(n->value->VT_index);
}

t_Node *Node_GetLChild(t_Node *n){
	return n->l_child;
}

t_Node *Node_GetRChild(t_Node *n){
	return n->r_child;
}