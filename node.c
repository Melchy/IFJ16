/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#include "node.h"
#include "tokens.h"
#include "hashvar.h"
#include "vartab.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Vraci true, pokud je uzel operator */
bool Node_IsOp(t_Node *n)
{
	if(n != NULL && n->value != NULL)
		return n->value->type >= tkn_PLUS && n->value->type <= tkn_OR;
	return false;
}

bool Node_IsBottom(t_Node *n){
	if (n != NULL)
		return (n->l_child == NULL) && (n->r_child == NULL);
	return false;
}

bool Node_IsUnary(t_Node *n){
	if(n != NULL)
		return ((n->l_child == NULL) && (n->r_child != NULL)) || ((n->l_child != NULL) && (n->r_child == NULL));
	return false;
}

bool Node_IsLParen(t_Node *n){
	if(n != NULL && n->value != NULL)
		return n->value->type == tkn_LPAREN;
	return false;
}

t_Value *Node_GetValue(t_Node *n){
	if(n != NULL)
		return n->value;
	return NULL;
}

int Node_GetType(t_Node *n){
	if(n != NULL && n->value != NULL)
		return n->value->type;
	return -1;
}

int Node_GetIndex(t_Node *n){
	if(n != NULL && n->value != NULL)
		return n->value->VT_index;
	return -1;
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

t_Node *Node_GetParent(t_Node *n){
	return n->parent;
}