#include "FIO.h"
#include "SCAN.h"
#include "STR.h"
#include "MEM.h"
#include "ERROR.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR -1

int test_MEM()
{
	char *ptr;
	if(!FIO_Open("java.code"))
		return ERR;
	int token;
	SCAN_attr.str = MEM_malloc(ATTR_SIZE*(sizeof(char)));
	char *cc;
	while((token = SCAN_GetToken()) != EOF)
	{
		if (token == tkn_ID)
		{
			ptr = MEM_malloc(SCAN_attr.len);
			if(!strcmp(SCAN_attr.str, "run"))
				cc = ptr;
			strcpy(ptr, SCAN_attr.str);
			puts(ptr);
		}
	}
	MEM_free(cc);
	MEM_printAll();
	MEM_clearAll();
	if(!FIO_Close())
		return ERR;	
	return 0;
}

int test_SCAN_GetToken() // vypisuje hodnoty tokenu a atributu na stdout
{
	if(!FIO_Open("java.code"))
		return ERR;
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
	if(!FIO_Close())
		return ERR;	
	return 0;
}

int test_SCAN_FindToken()
{
	if(!FIO_Open("java.code"))
		return ERR;
	SCAN_attr.str = MEM_malloc(ATTR_SIZE*(sizeof(char)));
	if(SCAN_FindToken(tkn_LBLOCK)){
		printf("Ve slozene zavorce\n");
		if (SCAN_FindToken(tkn_RBLOCK))
			printf("Venku\n");
	}
	MEM_clearAll();
	if(!FIO_Close())
		return ERR;	
	return 0;
}

void test_FIO(){

}

void test_ERROR_exit(){
	ERROR_exit(LEX_ERR);
}

void test_ERROR_exit2(){
	int * ptr = malloc(sizeof(int));
	ERROR_exit2(ENV_ERR,ptr);
}

void test_MEM_malloc(){
	int * a = MEM_malloc(sizeof(int));
	char * b = MEM_malloc(sizeof(char));
	double * c = MEM_malloc(sizeof(double));
	MEM_printAll();
}


//Zvlastni chovani ale funguje
void test_MEM_free(){
	char * b = MEM_malloc(sizeof(char));
	double * c = MEM_malloc(sizeof(double));
	printf("b c alloc\n");
	MEM_printAll();
	MEM_free(c);
	printf("free c\n");
	MEM_printAll();
	int * a = MEM_malloc(sizeof(int));
	printf("alloc a\n");
	MEM_printAll();
	MEM_free(a);
	MEM_free(b);
	printf("free a b\n");
	MEM_printAll();
}
//nejde (SIGSEGV)
void test_MEM_realloc(){
	char * a = MEM_malloc(sizeof(char));
	printf("a alloc\n");
	MEM_printAll();
	a = MEM_realloc(a,sizeof(char)*2);
	printf("a realloc\n");
	MEM_printAll();
	printf("free a\n");
	MEM_free(a);
	MEM_printAll();
	a = MEM_realloc(a,sizeof(char)*2);
	printf("free realloc a\n");
	MEM_printAll();
}

void test_MEM_clearAll(){
	
}
int main(void)
{
	test_MEM_realloc();
	//test_ERROR_exit();
	//test_ERROR_exit2();
	//test_SCAN_GetToken();
	//test_SCAN_FindToken();
	return 0;
}