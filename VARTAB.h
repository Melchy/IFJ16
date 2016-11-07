#ifndef VARTAB_H
#define VARTAB_H

#include "STR.h"

#include <stdbool.h>

#define INIT_SIZE 10
#define OPS_COUNT 14
#define OPS_OFFSET 12

typedef struct t_Value{
	int type;
	int VT_index;
}t_Value;

typedef struct ArrInt_t{
	int *arr;
	int size;
	int act_index;
}ArrInt_t;

typedef struct ArrDouble_t{
	double *arr;
	int size;
	int act_index;
}ArrDouble_t;

typedef struct ArrStr_t{
	S_String **arr;
	int size;
	int act_index;
}ArrStr_t;

void VT_InitTabs();

t_Value *VT_AddInt(int val);
int VT_GetInt(int index);

t_Value *VT_AddDouble(double val);
double VT_GetDouble(int index);

t_Value *VT_AddStr(S_String *val);
S_String *VT_GetStr(int index);

t_Value *VT_AddBool(int tkn_bool);
bool VT_GetBool(t_Value *val);
bool VT_GetBoolSafe(t_Value *val);

t_Value *VT_GetOp(int tkn_op);
t_Value *VT_GetLParen();
t_Value *VT_GetZeroInt();
t_Value *VT_GetEmpty(int token);

// pouze pro ucely testovani
void VT_PrintOne(t_Value *v);
void VT_PrintAll();

#endif