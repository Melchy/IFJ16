#include "FIO.h"
#include "SCAN.h"
#include "STR.h"
#include "MEM.h"
#include "Tree.h"
#include "EXPR.h"
#include "Tokens.h"
#include "VARTAB.h"
#include "HASHVAR.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static void initAll()
{
	FIO_Open("java.code");
	SCAN_attr = STR_Create("");
	STR_ChangeSize(SCAN_attr, ATTR_SIZE);
	VT_InitTabs();
}

int test_MEM()
{
 	char * a = MEM_malloc(sizeof(char));
 	MEM_printAll();
 	a = MEM_realloc(a,sizeof(char)*2);
 	MEM_printAll();
 	MEM_free(a);
 	MEM_printAll();
 	a = MEM_realloc(a,sizeof(char)*2);
 	MEM_printAll();
 	MEM_clearAll();
	return 0;
}

int test_SCAN_GetToken() // vypisuje hodnoty tokenu a atributu na stdout
{
	initAll();
	int token;
	while((token = SCAN_GetToken()) != EOF)
	{
		printf("tkn:%d", token);
		if(SCAN_attr->str[0] != '\0')
			printf("\t\tattr:%s", SCAN_attr->str);
		putchar('\n');
	}
	MEM_clearAll();
	FIO_Close();
	return 0;
}

int test_SCAN_FindToken()
{
	initAll();
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
	initAll();
	int token;
	Tree_Create();
	while((token = SCAN_GetToken()) != EOF)
	{
		if(token == tkn_LPAREN)
			Tree_NestIn();
		else if(token == tkn_RPAREN)
			Tree_NestOut();
		else
			Tree_Add(token);
	}
	Tree_RemoveParen();
	Tree_Print();
	VT_PrintOne(EXPR_Solve());
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
	VT_PrintAll();
	MEM_clearAll();
}

int main(void)
{
	//test_VARTAB();
	test_Tree();
	//test_MEM();
	//test_SCAN_GetToken();
	//test_SCAN_FindToken();
	return 0;
}