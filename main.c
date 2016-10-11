#include "FIO.h"
#include "SCAN.h"
#include "STR.h"
#include "MEM.h"

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

int main(void)
{
	test_MEM();
	//test_SCAN_GetToken();
	//test_SCAN_FindToken();
	return 0;
}