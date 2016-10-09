#include "FIO.h"
#include "SCAN.h"

#include <stdio.h>
#include <stdlib.h>

#define ERR -1

int main(void)
{
	if(!FIO_Open("java.code"))
		return ERR;
	int token;
	char *str = malloc(ATTR_SIZE*(sizeof(char)));
	if (str == NULL)
	{
		fprintf(stderr, "MALLOC FAILED\n");
		return 1;
	}
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