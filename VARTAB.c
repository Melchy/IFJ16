#include "VARTAB.h"
#include "STR.h"
#include "MEM.h"
#include "Tokens.h"

#include <stdbool.h>

static ArrInt_t 	int_arr;
static ArrDouble_t 	double_arr;
static ArrStr_t		str_arr;

static t_Value *Operators;
static t_Value *Bools;
static t_Value *LParen;
static t_Value *Zero;

void VT_InitTabs()
{
	int_arr.arr = MEM_malloc(INIT_SIZE*sizeof(int));
	double_arr.arr = MEM_malloc(INIT_SIZE*sizeof(double));
	str_arr.arr = MEM_malloc(INIT_SIZE*sizeof(S_String *));

	int_arr.size = 
	double_arr.size = 
	str_arr.size = INIT_SIZE;

	int_arr.act_index = 
	double_arr.act_index = 
	str_arr.act_index = 0;

	VT_AddInt(0);
	Zero = MEM_malloc(sizeof(t_Value));
	Zero->type = tkn_NUM;
	Zero->VT_index = 0;

	Operators = MEM_malloc(OPS_COUNT*sizeof(t_Value));
	for (int i = 0; i < 14; i++)
	{
		Operators[i].type = OPS_OFFSET+i; 		// pluskem zacinaji operatory v Tokens.h, to ma hodnotu 12 == OPS_OFFSET
		Operators[i].VT_index = -1;
	}

	Bools = MEM_malloc(2*sizeof(t_Value));
	Bools[0].type = tkn_FALSE; 	Bools[0].VT_index = -1;
	Bools[1].type = tkn_TRUE; 	Bools[1].VT_index = -1;

	LParen = MEM_malloc(sizeof(t_Value));
	LParen->type = tkn_LPAREN; LParen->VT_index = -1;
}

t_Value *Val_Create(int type, int index)
{
	t_Value *value = MEM_malloc(sizeof(t_Value));
	value->type = type;
	value->VT_index = index;
	return value;
}

t_Value *VT_AddInt(int val)
{
	if(int_arr.act_index == int_arr.size){
		int_arr.arr = MEM_realloc(int_arr.arr, 2*int_arr.size*sizeof(int));
		int_arr.size *= 2;
	}
	int_arr.arr[int_arr.act_index] = val;

	return Val_Create(tkn_NUM, int_arr.act_index++);
}
int VT_GetInt(int index)
{
	if(index < int_arr.act_index)
		return int_arr.arr[index];
	return -1;
}

t_Value *VT_AddDouble(double val)
{
	if(double_arr.act_index == double_arr.size){
		double_arr.arr = MEM_realloc(double_arr.arr, 2*double_arr.size*sizeof(double));
		double_arr.size *= 2;
	}
	double_arr.arr[double_arr.act_index] = val;	

	return Val_Create(tkn_REAL, double_arr.act_index++);
}
double VT_GetDouble(int index)
{
	if(index < double_arr.act_index)
		return double_arr.arr[index];
	return 0.0;
}


t_Value *VT_AddStr(S_String *val)
{
	if(str_arr.act_index == str_arr.size){
		str_arr.arr = MEM_realloc(str_arr.arr, 2*str_arr.size*sizeof(S_String *));
		str_arr.size *= 2;
	}
	str_arr.arr[str_arr.act_index] = val;	

	return Val_Create(tkn_LIT, str_arr.act_index++);
}
S_String *VT_GetStr(int index)
{
	if(index < str_arr.act_index)
		return str_arr.arr[index];
	return NULL;
}

t_Value *VT_AddBool(int tkn_bool)
{
	return tkn_bool == tkn_FALSE ? Bools : Bools+1;
}

bool VT_GetBool(t_Value *val)
{
	return val->type == tkn_TRUE;
}

t_Value *VT_GetOp(int tkn_op)
{
	return Operators+(tkn_op-OPS_OFFSET);
}

t_Value *VT_GetLParen()
{
	return LParen;
}

t_Value *VT_GetZeroInt()
{
	return Zero;
}

void VT_PrintOne(t_Value *v)
{
	switch(v->type){
		case tkn_NUM:
			printf("INT: %d\n", VT_GetInt(v->VT_index));
			break;
		case tkn_LIT:
			printf("STR: %s\n", VT_GetStr(v->VT_index)->str);
			break;
		case tkn_REAL:
			printf("DOUBLE: %g\n", VT_GetDouble(v->VT_index));
			break;
		case tkn_TRUE:
			printf("BOOL: true\n");
			break;
		case tkn_FALSE:
			printf("BOOL: false\n");
			break;
		case tkn_PLUS:
			printf("+\n");
			break;
		case tkn_MINUS:
			printf("-\n");
			break;
		case tkn_MUL:
			printf("*\n");
			break;
		case tkn_DIV:
			printf("/\n");
			break;
		case tkn_HIGHER:
			printf(">\n");
			break;
		case tkn_LOWER:
			printf("<\n");
			break;
		case tkn_EHIGHER:
			printf(">=\n");
			break;
		case tkn_ELOWER:
			printf("<=\n");
			break;
		case tkn_EQUAL:
			printf("==\n");
			break;
		case tkn_NEQUAL:
			printf("!=\n");
			break;
		case tkn_EXCL:
			printf("!\n");
			break;
		case tkn_AND:
			printf("&&\n");
			break;
		case tkn_OR:
			printf("||\n");
			break;
		case tkn_ASSIGN:
			printf("=\n");
			break;
	}
}

void VT_PrintAll(){
	printf("**INTS**\n");
	for (int i = 0; i < int_arr.act_index; i++)
		printf("[%d]: %d\n", i, int_arr.arr[i]);	

	printf("**DOUBLES**\n");
	for (int i = 0; i < double_arr.act_index; i++)
		printf("[%d]: %g\n", i, double_arr.arr[i]);

	printf("**STRINGS**\n");
	for (int i = 0; i < str_arr.act_index; i++)
		printf("[%d]: %s\n", i, str_arr.arr[i]->str);
}