#include "EXPR.h"
#include "Tree.h"
#include "Node.h"
#include "Tokens.h"

#include <stdlib.h>

static int RecursiveSolveI(t_Node *n)
{
	int l = 0; int r = 0; int tkn = Node_GetTkn(n);
	if(Node_GetLChild(n) == NULL)
		l = 0;
	else if(!Node_IsBottom(Node_GetLChild(n)))
		l = RecursiveSolveI(Node_GetLChild(n));
	else
		l = Node_IsUnary(n) ? 0 : Node_GetInt(Node_GetLChild(n));
	if(!Node_IsBottom(Node_GetRChild(n)))
		r = RecursiveSolveI(Node_GetRChild(n));
	else
		r = Node_GetInt(Node_GetRChild(n));

	// printf("l:%d r:%d tkn:%d\n", l, r, tkn);
	switch(tkn){
		case tkn_PLUS:
			return l+r;
		case tkn_MINUS:
			return l-r;
		case tkn_MUL:
			return l*r;
		case tkn_DIV:
			return l/r;
		default: break;
	}
	return 0;
}

int EXPR_IntSolve()
{
	int res = 0;
	t_Node *n = Tree_GetTopNode();
	if(!Node_IsOp(n)){
		res = Node_GetInt(n);
		return res;
	}
	res = RecursiveSolveI(n);
	return res;
}

// double EXPR_DoubleSolve();
// bool EXPR_BoolSolve();