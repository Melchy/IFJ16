#include "Node.h"
#include "Tokens.h"

#include <stdlib.h>
#include <stdbool.h>

/* Vraci true, pokud je uzel operator */
bool Node_IsOp(t_Node *n)
{
	switch(n->token){
		case tkn_MUL:
		case tkn_DIV:
		case tkn_PLUS:
		case tkn_MINUS:
		case tkn_EXCL:
		case tkn_HIGHER:
		case tkn_LOWER:
		case tkn_EHIGHER:
		case tkn_ELOWER:
		case tkn_EQUAL:
		case tkn_NEQUAL:
		case tkn_AND:
		case tkn_OR:
			return true;
		default:
			return false;
	}
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

int Node_GetTkn(t_Node *n){
	if(n != NULL)
		return n->token;
	return -1;
}

int Node_GetInt(t_Node *n){
	return atoi(n->attr->str);
}

t_Node *Node_GetLChild(t_Node *n){
	return n->l_child;
}

t_Node *Node_GetRChild(t_Node *n){
	return n->r_child;
}