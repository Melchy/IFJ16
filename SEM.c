#include "VARTAB.h"
#include "Tokens.h"

#include <stdio.h>
#include <stdbool.h>

static bool arithmetic(t_Value *l, t_Value *r)
{
	if(l->type == tkn_NUM){
		if(r->type == tkn_NUM)
			return true;
		if(r->type == tkn_REAL)
			return true;
	}

	if(l->type == tkn_REAL){
		if(r->type == tkn_NUM || r->type == tkn_REAL)
			return true;
	}

	return false;
}

static bool logic_cmp(t_Value *l, t_Value *r)
{
	if ((l->type == tkn_NUM || l->type == tkn_REAL) && (r->type == tkn_NUM || r->type == tkn_REAL))
		return true;
	return false;
}

static bool logic_cmp2(t_Value *l, t_Value *r)
{
	if ((l->type == tkn_NUM || l->type == tkn_REAL) && (r->type == tkn_NUM || r->type == tkn_REAL))
		return true;
	if ((l->type == tkn_TRUE || l->type == tkn_FALSE) && (r->type == tkn_TRUE || r->type == tkn_FALSE))
		return true;
	return false;
}

static bool logic_bool(t_Value *l, t_Value *r)
{
	if ((l->type == tkn_FALSE || l->type == tkn_TRUE) && (r->type == tkn_FALSE || r->type == tkn_TRUE))
		return true;
	return false;
}

static bool bool_unary(t_Value *r)
{
	if(r->type == tkn_FALSE || r->type == tkn_TRUE)
		return true;
	return false;
}

bool SEM_IsAllowed(t_Value *l, t_Value *r, int op)
{
	if(op == tkn_EXCL)
		return bool_unary(r);

	if(l->type == tkn_LIT && r->type == tkn_LIT && op == tkn_PLUS)
		return true;

	switch(op){
		case tkn_PLUS:
		case tkn_MINUS:
		case tkn_MUL:
		case tkn_DIV:
			return arithmetic(l, r);
		case tkn_HIGHER:
		case tkn_LOWER:
		case tkn_EHIGHER:
		case tkn_ELOWER:
			return logic_cmp(l, r);
		case tkn_EQUAL:
		case tkn_NEQUAL:
			return logic_cmp2(l, r);
		case tkn_AND:
		case tkn_OR:
			return logic_bool(l, r);			
	}
	return false;
}