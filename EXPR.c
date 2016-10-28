#include "EXPR.h"
#include "Tree.h"
#include "Node.h"
#include "Tokens.h"
#include "ERROR.h"
#include "VARTAB.h"
#include "SEM.h"

#include <stdio.h>
#include <stdlib.h>

static t_Value *solve_BOOL_BOOL(t_Value *l, t_Value *r, int op)
{
	switch(op){
		case tkn_EQUAL:
			return VT_GetBool(l) == VT_GetBool(r) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
		case tkn_NEQUAL:
			return VT_GetBool(l) != VT_GetBool(r) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
		case tkn_AND:
			return VT_GetBool(l) && VT_GetBool(r) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
		case tkn_OR:
			return VT_GetBool(l) || VT_GetBool(r) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	}

	return NULL;
}


static t_Value *solve_REAL_REAL(t_Value *l, t_Value *r, int op)
{
	switch(op){
	case tkn_PLUS:
		return VT_AddDouble(VT_GetDouble(l->VT_index) + VT_GetDouble(r->VT_index));
	case tkn_MINUS:
		return VT_AddDouble(VT_GetDouble(l->VT_index) - VT_GetDouble(r->VT_index));
	case tkn_MUL:
		return VT_AddDouble(VT_GetDouble(l->VT_index) * VT_GetDouble(r->VT_index));
	case tkn_DIV:
		return VT_AddDouble(VT_GetDouble(l->VT_index) / VT_GetDouble(r->VT_index));
	case tkn_HIGHER:
		return VT_GetDouble(l->VT_index) > VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_LOWER:
		return VT_GetDouble(l->VT_index) < VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_EHIGHER:
		return VT_GetDouble(l->VT_index) >= VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_ELOWER:
		return VT_GetDouble(l->VT_index) <= VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_EQUAL:
		return VT_GetDouble(l->VT_index) == VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_NEQUAL:
		return VT_GetDouble(l->VT_index) != VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	}

	return NULL;
}

static t_Value *solve_REAL_NUM(t_Value *l, t_Value *r, int op)
{
	switch(op){
	case tkn_PLUS:
		return VT_AddDouble(VT_GetDouble(l->VT_index) + VT_GetInt(r->VT_index));
	case tkn_MINUS:
		return VT_AddDouble(VT_GetDouble(l->VT_index) - VT_GetInt(r->VT_index));
	case tkn_MUL:
		return VT_AddDouble(VT_GetDouble(l->VT_index) * VT_GetInt(r->VT_index));
	case tkn_DIV:
		return VT_AddDouble(VT_GetDouble(l->VT_index) / VT_GetInt(r->VT_index));
	case tkn_HIGHER:
		return VT_GetDouble(l->VT_index) > VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_LOWER:
		return VT_GetDouble(l->VT_index) < VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_EHIGHER:
		return VT_GetDouble(l->VT_index) >= VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_ELOWER:
		return VT_GetDouble(l->VT_index) <= VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_EQUAL:
		return VT_GetDouble(l->VT_index) == VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_NEQUAL:
		return VT_GetDouble(l->VT_index) != VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	}

	return NULL;
}

static t_Value *solve_NUM_REAL(t_Value *l, t_Value *r, int op)
{
	switch(op){
	case tkn_PLUS:
		return VT_AddDouble(VT_GetInt(l->VT_index) + VT_GetDouble(r->VT_index));
	case tkn_MINUS:
		return VT_AddDouble(VT_GetInt(l->VT_index) - VT_GetDouble(r->VT_index));
	case tkn_MUL:
		return VT_AddDouble(VT_GetInt(l->VT_index) * VT_GetDouble(r->VT_index));
	case tkn_DIV:
		return VT_AddDouble(VT_GetInt(l->VT_index) / VT_GetDouble(r->VT_index));
	case tkn_HIGHER:
		return VT_GetInt(l->VT_index) > VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_LOWER:
		return VT_GetInt(l->VT_index) < VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_EHIGHER:
		return VT_GetInt(l->VT_index) >= VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_ELOWER:
		return VT_GetInt(l->VT_index) <= VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_EQUAL:
		return VT_GetInt(l->VT_index) == VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_NEQUAL:
		return VT_GetInt(l->VT_index) != VT_GetDouble(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	}

	return NULL;
}

static t_Value *solve_NUM_NUM(t_Value *l, t_Value *r, int op)
{
	switch(op){
	case tkn_PLUS:
		return VT_AddInt(VT_GetInt(l->VT_index) + VT_GetInt(r->VT_index));
	case tkn_MINUS:
		return VT_AddInt(VT_GetInt(l->VT_index) - VT_GetInt(r->VT_index));
	case tkn_MUL:
		return VT_AddInt(VT_GetInt(l->VT_index) * VT_GetInt(r->VT_index));
	case tkn_DIV:
		return VT_AddInt(VT_GetInt(l->VT_index) / VT_GetInt(r->VT_index));
	case tkn_HIGHER:
		return VT_GetInt(l->VT_index) > VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_LOWER:
		return VT_GetInt(l->VT_index) < VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_EHIGHER:
		return VT_GetInt(l->VT_index) >= VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_ELOWER:
		return VT_GetInt(l->VT_index) <= VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_EQUAL:
		return VT_GetInt(l->VT_index) == VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	case tkn_NEQUAL:
		return VT_GetInt(l->VT_index) != VT_GetInt(r->VT_index) ? VT_AddBool(tkn_TRUE) : VT_AddBool(tkn_FALSE);
	}

	return NULL;
}


static t_Value *EasySolve(t_Value *l, t_Value *r, int op)
{		
	if (!SEM_IsAllowed(l, r, op))
		ERROR_exit(SEM_ERR_TYPE);

	if(l == NULL && op == tkn_EXCL)
		return VT_GetBool(r) ? VT_AddBool(tkn_FALSE) : VT_AddBool(tkn_TRUE);

	if(l->type == tkn_NUM && r->type == tkn_NUM)
		return solve_NUM_NUM(l, r, op);
	if(l->type == tkn_NUM && r->type == tkn_REAL)
		return solve_NUM_REAL(l, r, op);
	if(l->type == tkn_REAL && r->type == tkn_NUM)
		return solve_REAL_NUM(l, r, op);
	if(l->type == tkn_REAL && r->type == tkn_REAL)
		return solve_REAL_REAL(l, r, op);
	if(l->type == tkn_LIT && r->type == tkn_LIT && op == tkn_PLUS){
		S_String *s = STR_Create("");
		STR_ConCat(s, VT_GetStr(l->VT_index));
		STR_ConCat(s, VT_GetStr(r->VT_index));
		return VT_AddStr(s);
	}
	if((l->type == tkn_TRUE || l->type == tkn_FALSE ) && (r->type == tkn_TRUE || r->type == tkn_FALSE))
		return solve_BOOL_BOOL(l, r, op);

	return NULL;
}

static t_Value *RecSolve(t_Node *n)
{
	t_Value *l; t_Value *r; int op = Node_GetType(n);
	if(Node_GetLChild(n) == NULL){
		if(op == tkn_MINUS || op == tkn_PLUS)
			l = VT_GetZeroInt();
		else
			l = NULL;
	}
	else if(!Node_IsBottom(Node_GetLChild(n)))
		l = RecSolve(Node_GetLChild(n));
	else{
		l = Node_GetValue(Node_GetLChild(n));
	}
	if(!Node_IsBottom(Node_GetRChild(n)))
		r = RecSolve(Node_GetRChild(n));
	else
		r = Node_GetValue(Node_GetRChild(n));

	// printf("l:%d r:%d tkn:%d\n", l, r, tkn);
	return EasySolve(l, r, op);

	return NULL;
}

t_Value *EXPR_Solve()
{
	t_Value *res;
	t_Node *n = Tree_GetTopNode();
	if(!Node_IsOp(n)){
		res = Node_GetValue(n);
		return res;
	}
	res = RecSolve(n);
	return res;
}