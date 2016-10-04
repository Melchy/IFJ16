#include "rtg_FSM.h"

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

FILE *src;
static bool endfl = 0;

void set_file(FILE *f)
{
	src = f;
}

void read_garbage()
{
	int c;
	while((c = fgetc(src)) != EOF)
	{
		if(c == ';' || c == ' ')
			return;
	}
}

int get_token(char *attr)
{
	if(endfl) return EOF;

	attr[0] = '\0';
	int state = st_NULL;
	int cnt = 0;
	bool flag = 0;

	int c;
	while(1)
	{
		c = fgetc(src);
		if(c == EOF) { c = ' '; endfl = 1; } // nemuzeme hned vratit EOF, potrebujeme posledni token

		switch(state){
			case st_NULL:
				if(isspace(c)) 		continue;
				else if(c == '/') 	{ state = st_SLASH; continue; } // nutno rozhodnout lomitko vs. koment
				else if(isalpha(c)) { state = st_WORD;  attr[cnt++] = c; continue; }
				else if(isdigit(c)) { state = st_NUM; attr[cnt++] = c; continue; }
				else				{ read_garbage() ; return SYN_ERR; }
			break;

			case st_SLASH:	// line koment, block koment nebo lomitko (deleni)
				if(c == '/') 		{ state = st_LCOMM; continue; }
				else if(c == '*') 	{ state = st_BCOMM; continue; }
				else				{ ungetc(c, src); state = st_NULL; return tkn_DIV; }
			break;

			case st_LCOMM:	// v line komentu
				if(c != '\n') 	continue;
				else			{ state = st_NULL; continue; }
			break;

			case st_BCOMM: // v block komentu
				if(c == '*') 	{ state = st_BCOMM_0; continue; }
				else			continue;
			break;

			case st_BCOMM_0: // mozny konec block komentu (*)
				if(c == '/') 	{ state = st_NULL; continue; }
				else			continue;
			break;

			case st_NUM: // integer, mozna real
				if(isdigit(c)) 					{ attr[cnt++] = c; continue; }
				else if(c == 'E' || c == 'e') 	{ state = st_REALE; attr[cnt++] = c; continue; }
				else if(c == '.')				{ state = st_REAL; attr[cnt++] = c; continue; }
				else if(isalpha(c))				{ read_garbage(); return LEX_ERR; }
				else							{ ungetc(c, src); attr[cnt] = '\0'; return tkn_NUM; }
			break;

			case st_REALE:	// '1561e' nebo '1561.34e'
				if(c == '+' || c == '-' || !flag)	{ attr[cnt++] = c; flag = 1; continue; }
				else if(isdigit(c))					{ attr[cnt++] = c; flag = 1; continue; }
				else if(isalpha(c))					{ read_garbage(); return LEX_ERR; }
				else								{ ungetc(c, src); attr[cnt] = '\0'; return tkn_REAL; }
			break;

			case st_REAL: // '1561.'
				if(isdigit(c))							{ attr[cnt++] = c; flag = 1; continue; }
				else if((c == 'E' || c == 'e') && flag)	{ state = st_REALE; flag = 0; attr[cnt++] = c; continue; }
				else if(flag)							{ ungetc(c, src); attr[cnt] = '\0'; return tkn_REAL; }
				else									{ read_garbage(); return LEX_ERR; }

			/* to be done */
			case st_WORD:
				read_garbage(); return 42;
			break;
			default: break;
		}
	}
}