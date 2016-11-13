#include "EXPR.h"
#include "Tree.h"
#include "Node.h"
#include "Tokens.h"
#include "ERROR.h"
#include "VARTAB.h"
#include "SEM.h"
#include "IDLogic.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool is_concat;

void EXPR_Create()
{
	// pro kazdy vyraz vytvorime unikatni strom
	Tree_Create();
	if(is_concat)
		is_concat = false;
}

/* Funkci volame pred kazdym pridanim do stromu */
static void Add_IDVal()
{
	S_String *ID = Tree_PopActStr();
	if(ID == NULL) // pokud zadny identifikator neceka na pridani do stromu, vratime se
		return;

	t_Value *val = IL_GetVal(ID);
	if(val == NULL)
		ERROR_exit(INIT_ERR);
	Tree_AddNode(val);
}

void EXPR_AddVal(int token, S_String *attr)
{
	if(token == tkn_ASSIGN){
		Tree_AddAssignment();
		return;
	}

	Add_IDVal();
	int x; double y;

	// pokud se jedna o operator, pridame ho, v pripade zavorek se zanorime/vynorime
	if(token >= tkn_PLUS && token <= tkn_OR)
		Tree_AddOp(token);
	else if(token == tkn_LPAREN)
		Tree_NestIn();
	else if(token == tkn_RPAREN)
		Tree_NestOut();

	// pridame do stromu konkretni t_Value podle prichoziho tokenu
	switch(token){
		case tkn_NUM:
			STR_StringToInt(attr, &x);
			Tree_AddNode(VT_AddInt(x));
		break;
		case tkn_REAL:
			STR_StringToDouble(attr, &y);
			Tree_AddNode(VT_AddDouble(y));
		break;
		case tkn_LIT:
			Tree_AddNode(VT_AddStr(STR_Create(attr->str)));
		break;
		case tkn_TRUE:
		case tkn_FALSE:
			Tree_AddNode(VT_AddBool(token));
		break;
		case tkn_ID:
			Tree_AddID(attr);
		break;
		default: break;
	}
}

void EXPR_AddVal2(t_Value *value)
{	
	Add_IDVal();

	Tree_AddNode(value);
}


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
	if(l == NULL && op == tkn_EXCL) // pokud se jedna o negaci boolu
		return VT_GetBool(r) ? VT_AddBool(tkn_FALSE) : VT_AddBool(tkn_TRUE);
	if(l == NULL || r == NULL) // pokud neni co resit
		return NULL;
	if(l->type == tkn_NUM && r->type == tkn_NUM) // pokud se jedna o dva integery
		return solve_NUM_NUM(l, r, op);
	if(l->type == tkn_NUM && r->type == tkn_REAL) // pokud se jedna o integer a double
		return solve_NUM_REAL(l, r, op);
	if(l->type == tkn_REAL && r->type == tkn_NUM) // ...
		return solve_REAL_NUM(l, r, op);
	if(l->type == tkn_REAL && r->type == tkn_REAL)
		return solve_REAL_REAL(l, r, op);
	if(l->type == tkn_LIT && r->type == tkn_LIT && op == tkn_PLUS){
		S_String *s = STR_Create("");
		STR_ConCat(s, VT_GetStr(l->VT_index));
		STR_ConCat(s, VT_GetStr(r->VT_index));
		return VT_AddStr(s);
	}
	if(l->type == tkn_LIT && r->type == tkn_NUM && op == tkn_PLUS){
		S_String *s = STR_Create("");
		STR_ConCat(s, VT_GetStr(l->VT_index));
		STR_ConCat(s, STR_IntToString(VT_GetInt(r->VT_index)));
		return VT_AddStr(s);
	}
	if(l->type == tkn_NUM && r->type == tkn_LIT && op == tkn_PLUS){
		S_String *s = STR_Create("");
		STR_ConCat(s, STR_IntToString(VT_GetInt(l->VT_index)));
		STR_ConCat(s, VT_GetStr(r->VT_index));
		return VT_AddStr(s);
	}
	if(l->type == tkn_LIT && r->type == tkn_REAL && op == tkn_PLUS){
		S_String *s = STR_Create("");
		STR_ConCat(s, VT_GetStr(l->VT_index));
		STR_ConCat(s, STR_DoubleToString(VT_GetDouble(r->VT_index)));
		return VT_AddStr(s);
	}
	if(l->type == tkn_REAL && r->type == tkn_LIT && op == tkn_PLUS){
		S_String *s = STR_Create("");
		STR_ConCat(s, STR_DoubleToString(VT_GetDouble(l->VT_index)));
		STR_ConCat(s, VT_GetStr(r->VT_index));
		return VT_AddStr(s);
	}
	if((l->type == tkn_TRUE || l->type == tkn_FALSE ) && (r->type == tkn_TRUE || r->type == tkn_FALSE))
		return solve_BOOL_BOOL(l, r, op);

	// pokud jsme nenasli vhodnou kombinaci operandu a operatoru, vratime se = chyba
	return NULL;
}

static t_Value *toString(t_Value *v)
{
	switch(v->type){
		case tkn_LIT:
			return v;
		case tkn_NUM:
			return VT_AddStr(STR_IntToString(VT_GetInt(v->VT_index)));
		case tkn_REAL:
			return VT_AddStr(STR_DoubleToString(VT_GetDouble(v->VT_index)));
	}
	return v;
}

static t_Value *RecSolve(t_Node *n)
{	
	// IMPORTANT - pokud je jeden jediny operand literal, jedna se o konkatenaci 
	// a je potreba volat easysolve se stringovymi operandy (tostring(value))

	if(!is_concat && (Node_GetType(Node_GetLChild(n)) == tkn_LIT || Node_GetType(Node_GetRChild(n)) == tkn_LIT))
		is_concat = true;

	t_Value *l; t_Value *r; int op = Node_GetType(n);
	if(Node_GetLChild(n) == NULL) // osetreni unarniho minus (jako levy operand dosadime nulu)
	{
		if(op == tkn_MINUS || op == tkn_PLUS)
			l = VT_GetZeroInt();
		else
			l = NULL;
	}
	else if(!Node_IsBottom(Node_GetLChild(n))) // pokud levy potomek neni koncovy, volame rekurzi
		l = RecSolve(Node_GetLChild(n));
	else{
		l = Node_GetValue(Node_GetLChild(n));
	}

	if(!Node_IsBottom(Node_GetRChild(n))) // pokud pravy potomek neni koncovy, volame rekurzi
		r = RecSolve(Node_GetRChild(n));
	else
		r = Node_GetValue(Node_GetRChild(n));

	if(is_concat)
		return EasySolve(toString(l), toString(r), op);

	return EasySolve(l, r, op);
}

static void makeAssigns(t_Value *value)
{
	S_String *ID;
	while((ID = Tree_PopAssign()) != NULL) // popujeme z assign listu a zpetne prirazujeme vysledek
		SEM_SafeAssignment(ID, value);
}

t_Value *EXPR_Solve()
{
	Add_IDVal();
	//Tree_Print();

	Tree_RemoveParen(); // pred solve pro zjednoduseni odstranime zavorky
	if(Tree_Empty())
		return NULL;
	
	t_Value *res;
	t_Node *n = Tree_GetTopNode();
	if(!Node_IsOp(n)) // pokud je na topu hodnota, rekurze nema smysl (napr. x=5;)
		res = Node_GetValue(n);
	else
		res = RecSolve(n);
	if(res == NULL)
		ERROR_exit(SEM_ERR_TYPE);
	makeAssigns(res);
	Tree_Dispose();
	return res;
}