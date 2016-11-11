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
/*
static void initAll()
{
	FIO_Open("java.code");
	SCAN_InitAttr();
	VT_InitTabs();
	HASHVAR_InitGlobal();
	HASHFCE_InitFceTab();
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
		if(SCAN_GetAttr()->str[0] != '\0')
			printf("\t\tattr:%s", SCAN_GetAttr()->str);
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
		if(token == tkn_ID && !STR_Compare(SCAN_GetAttr(), p)){
			SCAN_GetToken(); SCAN_GetToken(); STR_PrintStr(SCAN_GetAttr()); printf(" = "); VT_PrintOne(IL_GetVal(SCAN_GetAttr()));
			while(SCAN_GetToken() != tkn_SEMI)
				;
			goto strelbadonohy;
		}
		else if(token == tkn_LPAREN || token == tkn_RPAREN || (token >= tkn_PLUS && token <= tkn_ASSIGN))
			EXPR_AddSym(token);
		else if(token == tkn_NUM || token == tkn_REAL || token == tkn_TRUE || token == tkn_FALSE || token == tkn_ID || token == tkn_LIT)
			EXPR_AddVal(token, SCAN_GetAttr());
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
}*/



void test_VARTABInit()
{
	VT_InitTabs();
	MEM_clearAll();
}


void test_VARTABInt()
{
	VT_InitTabs();
	VT_AddInt(200);
	printf("%d\n",VT_GetInt(1));
	VT_AddInt(-100);
	printf("%d\n",VT_GetInt(2));
	VT_AddInt(0.2);
	printf("%d\n",VT_GetInt(3));
	printf("%d\n",VT_GetInt(4));
	printf("%d\n",VT_GetInt(-2));
	VT_PrintAll();
	MEM_clearAll();
}
void test_VARTABDouble()
{
	VT_InitTabs();
	VT_AddDouble(200);
	printf("%g\n",VT_GetDouble(0));
	VT_AddDouble(-100);
	printf("%g\n",VT_GetDouble(1));
	VT_AddDouble(0.2f);
	printf("%g\n",VT_GetDouble(2));
	printf("%g\n",VT_GetDouble(3));
	printf("%g\n",VT_GetDouble(-2));
	VT_PrintAll();
	MEM_clearAll();
}
void test_VARTABStr()
{
	VT_InitTabs();
	VT_AddStr(STR_Create("pes"));
	printf("%s\n",VT_GetStr(0)->str);
	VT_AddStr(STR_Create("kocka"));
	printf("%s\n",VT_GetStr(1)->str);
	VT_AddStr(STR_Create("asd"));
	printf("%s\n",VT_GetStr(2)->str);
	VT_PrintAll();
	MEM_clearAll();
}
void test_VARTABBool()
{
	VT_InitTabs();
	printf("%d\n",VT_GetBool(VT_AddBool(tkn_FALSE)));
	printf("%d\n",VT_GetBool(VT_AddBool(1)));
	printf("%d\n",VT_GetBool(VT_AddBool(2)));
	printf("%d\n",VT_GetBoolSafe(VT_AddBool(tkn_FALSE)));
	//VT_GetBoolSafe(VT_AddDouble(2));
	VT_PrintAll();
	MEM_clearAll();
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

void test_STRTo()
{
	S_String *test = STR_Create("ahoj");
	STR_PrintStr(test);
	putchar('\n');
	S_String *sInt = STR_IntToString(20);
	STR_PrintStr(sInt);
	putchar('\n');
	S_String *sLong = STR_LongToString(200);
	STR_PrintStr(sLong);
	putchar('\n');
	S_String *sDouble =STR_DoubleToString(2.3);
	STR_PrintStr(sDouble);
	putchar('\n');
	sDouble =STR_DoubleToString(0x23);
	STR_PrintStr(sDouble);
	putchar('\n');
	
}
void test_STRChange()
{
	S_String *test = STR_Create(" ahoj ");
	STR_PrintStr(test);
	putchar('\n');
	STR_Trim(test);
	STR_PrintStr(test);
	putchar('\n');
	S_String *test2 = STR_Create("svete");
	STR_AddChar(test, ' ');
	STR_ConCat(test, test2);
	STR_PrintStr(test);
	putchar('\n');
	STR_PrintStr(test2);
	putchar('\n');
}
void test_STRSearch()
{	
	S_String *test = STR_Create("asc");
	S_String *test2 = STR_Create("ac");
	printf("%d\n",STR_Compare(test, test2));//-1
	printf("%d\n",STR_FindChar(test, 'a'));//true
	printf("%d\n",STR_FindChar(test, 'd'));//false
	printf("%d\n",STR_SubStr(test, test2,0));//nefunguje
	printf("%d\n",STR_SubStr(test, STR_Create("asd"),0));//-1
	S_String **foo = &test2;
	printf("%d\n",STR_GetAfter(test, foo,'x'));//1
	STR_PrintStr(*foo);
	putchar('\n');
	printf("%d\n",STR_GetAfter(test, foo,'s'));//0
	STR_PrintStr(*foo);
	putchar('\n');
	printf("%d\n",STR_GetAfter(test, foo,'c'));//2
	STR_PrintStr(*foo);
	putchar('\n');
}
void test_STRStrTo()
{
	//verim tomu :D 
}



int main(void)
{
	//test_VARTAB();
	//test_Expr();
	//test_MEM();
	//test_SCAN_GetToken();
	//test_SCAN_FindToken();
	//test_VARTABBool();
	test_STRSearch();
	return 0;
}