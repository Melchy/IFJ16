#include "FIO.h"
#include "SCAN.h"

#include <stdio.h>

#define ERR -1

int main(void)
{
	if(!FIO_Open("java.code"))
		return ERR;
	int token;
	char str[100];
	while((token = SCAN_GetToken(str)) != EOF)
	{
		printf("tkn:%d", token);
		if(str[0] != '\0')
			printf("\t\tattr:%s", str);
		putchar('\n');
	}

	if(!FIO_Close())
		return ERR;

	return 0;
}