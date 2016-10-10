#include "FIO.h"
#include "SCAN.h"
#include "STR.h"

#include <stdio.h>
#include <stdlib.h>

#define ERR -1

int test_SCAN_GetToken() // vypisuje hodnoty tokenu a atributu na stdout
{
	if(!FIO_Open("java.code"))
		return ERR;
	int token;
	SCAN_attr.str = malloc(ATTR_SIZE*(sizeof(char)));
	if (SCAN_attr.str == NULL)
	{
		fprintf(stderr, "MALLOC FAILED\n");
		return 1;
	}
	while((token = SCAN_GetToken()) != EOF)
	{
		printf("tkn:%d", token);
		if(SCAN_attr.str[0] != '\0')
			printf("\t\tattr:%s", SCAN_attr.str);
		putchar('\n');
	}

	free(SCAN_attr.str);
	if(!FIO_Close())
		return ERR;	
	return 0;
}

int test_SCAN_FindToken()
{
	if(!FIO_Open("java.code"))
		return ERR;
	SCAN_attr.str = malloc(ATTR_SIZE*(sizeof(char)));
	if (SCAN_attr.str == NULL)
	{
		fprintf(stderr, "MALLOC FAILED\n");
		return 1;
	}

	if(SCAN_FindToken(tkn_LBLOCK)){
		printf("Ve slozene zavorce\n");
		if (SCAN_FindToken(tkn_RBLOCK))
			printf("Venku\n");
	}

	free(SCAN_attr.str);
	if(!FIO_Close())
		return ERR;	
	return 0;
}

int main(void)
{
	//test_SCAN_GetToken();
	test_SCAN_FindToken();
	return 0;
}