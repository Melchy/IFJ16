#include "VARTAB.h"
#include "STR.h"
#include "MEM.h"

#include <stdbool.h>

static ArrInt_t 	int_arr;
static ArrDouble_t 	double_arr;
static ArrStr_t		str_arr;
static ArrBool_t	bool_arr;

void VT_InitTabs()
{
	int_arr.arr = MEM_malloc(INIT_SIZE*sizeof(int));
	double_arr.arr = MEM_malloc(INIT_SIZE*sizeof(double));
	str_arr.arr = MEM_malloc(INIT_SIZE*sizeof(S_String *));
	bool_arr.arr = MEM_malloc(INIT_SIZE*sizeof(bool));

	int_arr.size = 
	double_arr.size = 
	str_arr.size = 
	bool_arr.size = INIT_SIZE;

	int_arr.act_index = 
	double_arr.act_index = 
	str_arr.act_index = 
	bool_arr.act_index = 0;
}

int VT_AddInt(int val)
{
	if(int_arr.act_index == int_arr.size){
		printf("index: %d, size: %d\n", int_arr.act_index, int_arr.size);
		int_arr.arr = MEM_realloc(int_arr.arr, 2*int_arr.size*sizeof(int));
		int_arr.size *= 2;
	}
	int_arr.arr[int_arr.act_index] = val;

	return int_arr.act_index++;
}

int VT_AddDouble(double val)
{
	if(double_arr.act_index == double_arr.size){
		double_arr.arr = MEM_realloc(double_arr.arr, 2*double_arr.size*sizeof(double));
		double_arr.size *= 2;
	}
	double_arr.arr[double_arr.act_index] = val;	

	return double_arr.act_index++;
}

int VT_AddStr(S_String *val)
{
	if(str_arr.act_index == str_arr.size){
		str_arr.arr = MEM_realloc(str_arr.arr, 2*str_arr.size*sizeof(S_String *));
		str_arr.size *= 2;
	}
	str_arr.arr[str_arr.act_index] = val;	

	return str_arr.act_index++;
}

int VT_AddBool(bool val)
{
	if(bool_arr.act_index == bool_arr.size){
		bool_arr.arr = MEM_realloc(bool_arr.arr, 2*bool_arr.size*sizeof(bool));
		bool_arr.size *= 2;
	}
	bool_arr.arr[bool_arr.act_index] = val;	

	return bool_arr.act_index++;
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

	printf("**BOOLS**\n");
	for (int i = 0; i < bool_arr.act_index; i++)
		printf("[%d]: %s\n", i, bool_arr.arr[i] ? "true" : "false");
}