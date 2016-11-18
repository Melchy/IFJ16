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
	if(!STR_Compare(ID, VT_GetStr(VT_GetEmptyString()->VT_index))){
		Tree_AddNode(VT_GetEmpty(tkn_NUM));
		return;
	}

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
	if(token >= tkn_PLUS && token <= tkn_OR){
		Tree_AddOp(token); return;
	}
	else if(token == tkn_LPAREN){
		Tree_NestIn(); return;
	}
	else if(token == tkn_RPAREN){
		Tree_NestOut(); return;
	}

	// pridame do stromu konkretni t_Value podle prichoziho tokenu
	switch(token){
		case tkn_NUM:
			if(attr == NULL)
				Tree_AddNode(VT_GetEmpty(tkn_NUM));
			else
				{ STR_StringToInt(attr, &x); Tree_AddNode(VT_AddInt(x)); }
			return;
		case tkn_REAL:
			if(attr == NULL)
				Tree_AddNode(VT_GetEmpty(tkn_REAL));
			else
				{ STR_StringToDouble(attr, &y); Tree_AddNode(VT_AddDouble(y)); }
			return;
		case tkn_LIT:
			if(attr == NULL)
				Tree_AddNode(VT_GetEmpty(tkn_LIT));
			else
				Tree_AddNode(VT_AddStr(STR_Create(attr->str)));
			return;
		case tkn_TRUE:
		case tkn_FALSE:
			Tree_AddNode(VT_AddBool(token));
			return;
		case tkn_ID:
			Tree_AddID(attr);
			return;
		default: 
			ERROR_exit(SYN_ERR);
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
	if(v == NULL)
		return NULL;
	
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

static void safeAssignment(S_String *ID, t_Value *value)
{
	t_Value *id = IL_GetVal(ID);
	if(id == NULL){
		ERROR_exit(SEM_ERR_DEF);
	}
	if(id->type == tkn_NUM){
		if(value->type == tkn_NUM)
			IL_SetVal(ID, value);
			//IL_SetVal(ID, VT_AddInt(VT_GetInt(value->VT_index)));
		else if(value->type == tkn_REAL)
			IL_SetVal(ID, VT_AddInt((int)VT_GetDouble(value->VT_index)));
		else
			ERROR_exit(SEM_ERR_TYPE);
	}
	if(id->type == tkn_REAL){
		if(value->type == tkn_REAL)
			IL_SetVal(ID, value);
			//IL_SetVal(ID, VT_AddDouble(VT_GetDouble(value->VT_index)));
		else if(value->type == tkn_NUM)
			IL_SetVal(ID, VT_AddDouble((double)VT_GetInt(value->VT_index)));
		else
			ERROR_exit(SEM_ERR_TYPE);
	}
	if(id->type == tkn_LIT){
		if(value->type == tkn_LIT)
			IL_SetVal(ID, value);
			//IL_SetVal(ID, VT_AddStr(VT_GetStr(value->VT_index)));
		else
			ERROR_exit(SEM_ERR_TYPE);
	}
	if(id->type == tkn_TRUE || id->type == tkn_FALSE || id->type == tkn_BOOL){
		if(value->type == tkn_TRUE || value->type == tkn_FALSE)
			IL_SetVal(ID, VT_AddBool(VT_GetBool(value) ? tkn_TRUE : tkn_FALSE));
		else
			ERROR_exit(SEM_ERR_TYPE);
	}
}

static void makeAssigns(t_Value *value)
{
	S_String *ID;
	while((ID = Tree_PopAssign()) != NULL) // popujeme z assign listu a zpetne prirazujeme vysledek
		safeAssignment(ID, value);
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
	Tree_Create();
	return res;
}

static void RecSyntax(t_Node *n)
{
	if(n == NULL)
		return;

	RecSyntax(Node_GetLChild(n));
	RecSyntax(Node_GetRChild(n));

	t_Value *val = Node_GetValue(n);
	t_Node *par = Node_GetParent(n);

	if(Node_IsOp(n)){
		if(Node_IsBottom(n))
			ERROR_exit(SYN_ERR);
	}
	if(val->type == tkn_PLUS){
		if(par != NULL){
			if(par->value->type == tkn_PLUS && (Node_IsUnary(par) || Node_IsUnary(n)))
				ERROR_exit(SYN_ERR);
		}
	}
	if(val->type == tkn_MINUS){
		if(par != NULL){
			if(par->value->type == tkn_MINUS && (Node_IsUnary(par) || Node_IsUnary(n)))
				ERROR_exit(SYN_ERR);
		}
	}
	if(val->type == tkn_MUL ||
		val->type == tkn_DIV ||
		val->type == tkn_OR ||
		val->type == tkn_AND ||
		val->type == tkn_HIGHER ||
		val->type == tkn_LOWER ||
		val->type == tkn_EHIGHER ||
		val->type == tkn_ELOWER ||
		val->type == tkn_EQUAL ||
		val->type == tkn_NEQUAL ) 
	{
		if(Node_IsUnary(n))
			ERROR_exit(SYN_ERR);
	}
	if(val->type == tkn_EXCL && !Node_IsUnary(n))
		ERROR_exit(SYN_ERR);
	if(val->type == tkn_NUM ||
		val->type == tkn_REAL ||
		val->type == tkn_LIT ||
		val->type == tkn_TRUE ||
		val->type == tkn_FALSE ||
		val->type == tkn_BOOL )
	{
		if(par != NULL){
			if(par->value->type == tkn_NUM ||
				par->value->type == tkn_REAL ||
				par->value->type == tkn_LIT ||
				par->value->type == tkn_TRUE ||
				par->value->type == tkn_FALSE ||
				par->value->type == tkn_BOOL )
				ERROR_exit(SYN_ERR);
		}
	}
	if(val->type == tkn_LPAREN && !Node_IsUnary(n))
		ERROR_exit(SYN_ERR);
}

void EXPR_CheckSyntax(bool emptyAllowed)
{
	if(Tree_Empty() && !emptyAllowed)
		ERROR_exit(SYN_ERR);
	if(Tree_GetNest() != 0)
		ERROR_exit(SYN_ERR);

	t_Node *n = Tree_GetTopNode();
	RecSyntax(n);
}

void EXPR_Dispose(){
	Tree_Dispose();
}