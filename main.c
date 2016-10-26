#include "FIO.h"
#include "SCAN.h"
#include "STR.h"
#include "MEM.h"
#include "Tree.h"
#include "EXPR.h"
#include "Tokens.h"
#include "VARTAB.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int test_MEM()
{
 	char * a = MEM_malloc(sizeof(char));
 	printf("a alloc\n");
 	MEM_printAll();
 	a = MEM_realloc(a,sizeof(char)*2);
 	printf("a realloc\n");
 	MEM_printAll();
 	MEM_free(a);
 	MEM_printAll();
 	printf("free a\n");
 	a = MEM_realloc(a,sizeof(char)*2);
 	printf("free realloc a\n");
 	MEM_printAll();
 	MEM_clearAll();
	return 0;
}

int test_SCAN_GetToken() // vypisuje hodnoty tokenu a atributu na stdout
{
	FIO_Open("java.code");
	int token;
	SCAN_attr.str = MEM_malloc(ATTR_SIZE*(sizeof(char)));
	while((token = SCAN_GetToken()) != EOF)
	{
		printf("tkn:%d", token);
		if(SCAN_attr.str[0] != '\0')
			printf("\t\tattr:%s", SCAN_attr.str);
		putchar('\n');
	}
	MEM_clearAll();
	FIO_Close();
	return 0;
}

int test_SCAN_FindToken()
{
	FIO_Open("java.code");
	SCAN_attr.str = MEM_malloc(ATTR_SIZE*(sizeof(char)));
	if(SCAN_FindToken(tkn_LBLOCK)){
		printf("Ve slozene zavorce\n");
		if (SCAN_FindToken(tkn_RBLOCK))
			printf("Venku\n");
	}
	MEM_clearAll();
	FIO_Close();
	return 0;
}

int test_Tree()
{
	FIO_Open("java.code");
	int token;
	SCAN_attr.str = MEM_malloc(ATTR_SIZE*(sizeof(char)));
	Tree_Create();
	while((token = SCAN_GetToken()) != EOF)
	{
		if((token >= tkn_PLUS && token <= tkn_NEQUAL) || (token >= tkn_EXCL && token <= tkn_OR))
			Tree_AddOp(token);
		else if(token == tkn_LPAREN)
			Tree_NestIn();
		else if(token == tkn_RPAREN)
			Tree_NestOut();
		else{
			S_String *tmp = MEM_malloc(sizeof(S_String));
			tmp->str = MEM_malloc(ATTR_SIZE*(sizeof(char)));
			strcpy(tmp->str, SCAN_attr.str);
			Tree_AddVal(token, tmp);
			SCAN_attr.str = MEM_malloc(ATTR_SIZE*(sizeof(char)));
		}
	}
	Tree_RemoveParen();
	Tree_Print();
	printf("RESULT: %d\n", EXPR_IntSolve());
	Tree_Dispose();
	MEM_clearAll();
	FIO_Close();
	return 0;
}

void test_VARTAB()
{
	VT_InitTabs();
	for (int i = 0; i < 50; i++)
		VT_AddInt(i*2);
	for (int i = 0; i < 5; i++)
		VT_AddDouble(i*2.5);
	for (int i = 0; i < 15; i++)
		VT_AddStr(STR_Create("necum"));
	for (int i = 0; i < 5; i++)
		VT_AddBool(i%2 ? true:false);
	VT_PrintAll();
	MEM_clearAll();
}

int main(void)
{
	test_VARTAB();
	//test_Tree();
	//test_MEM();
	//test_SCAN_GetToken();
	//test_SCAN_FindToken();
	return 0;
}