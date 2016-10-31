#include "FIO.h"
#include "SCAN.h"
#include "STR.h"
#include "MEM.h"
#include "Tree.h"
#include "EXPR.h"
#include "Tokens.h"
#include "VARTAB.h"
#include "HASHVAR.h"
#include "IDLogic.h"
#include "HASHFCE.h"

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
	HASHVAR_InitGlobal();
	HASHFCE_InitFce();
}

int test_MEM()
{
 	char *a = MEM_malloc(sizeof(char));
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

static void defineVars(){
	IL_AllocVar(STR_Create("i1"), tkn_NUM, true);
	IL_AllocVar(STR_Create("i2"), tkn_NUM, true);
	IL_AllocVar(STR_Create("b1"), tkn_BOOL, true);
	IL_AllocVar(STR_Create("b2"), tkn_BOOL, true);
	IL_AllocVar(STR_Create("d1"), tkn_REAL, true);
	IL_AllocVar(STR_Create("d2"), tkn_REAL, true);
	IL_AllocVar(STR_Create("s1"), tkn_LIT, true);
	IL_AllocVar(STR_Create("s2"), tkn_LIT, true);
}

int test_Expr()
{
	initAll();
	int token;
	IL_SetClass(STR_Create("trida"));
	defineVars();

	EXPR_Create();
	S_String *p = STR_Create("show");
	strelbadonohy:
	while((token = SCAN_GetToken()) != EOF)
	{
		if(token == tkn_ID && !STR_Compare(SCAN_attr, p)){
			SCAN_GetToken(); SCAN_GetToken(); STR_PrintStr(SCAN_attr); printf(" = "); VT_PrintOne(IL_GetVal(SCAN_attr));
			while(SCAN_GetToken() != tkn_SEMI)
				;
			goto strelbadonohy;
		}
		else if(token == tkn_LPAREN || token == tkn_RPAREN || (token >= tkn_PLUS && token <= tkn_ASSIGN))
			EXPR_AddSym(token);
		else if(token == tkn_NUM || token == tkn_REAL || token == tkn_TRUE || token == tkn_FALSE || token == tkn_ID || token == tkn_LIT)
			EXPR_AddVal(token, SCAN_attr);
		else if(token == tkn_SEMI){
			EXPR_Solve();
			EXPR_Create();
		}
	}
	//HASHVAR_PrintG();
	//HASHFCE_Print();
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
	test_Expr();
	//test_MEM();
	//test_SCAN_GetToken();
	//test_SCAN_FindToken();
	return 0;
}