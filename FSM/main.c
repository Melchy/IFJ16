#include "rtg_FSM.h"

#include <stdio.h>

int main(void)
{
	FILE *f = fopen("java.code", "r");
	if(f == NULL)
	{
		fprintf(stderr, "NO FILE\n");
		return 1;
	}
	set_file(f);
	int token;
	char str[100];
	while((token = get_token(str)) != EOF)
	{
		printf("tkn:%d", token);
		if(str[0] != '\0')
			printf("\t\tattr:%s", str);
		putchar('\n');
	}

	fclose(f);

	return 0;
}