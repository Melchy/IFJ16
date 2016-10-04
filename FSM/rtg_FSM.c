#include "rtg_FSM.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

FILE *src;
static bool endfl = 0;	// end flag pro indikaci nalezu EOF

void set_file(FILE *f)
{
	src = f;
}

void read_garbage()
{
	int c;
	while((c = fgetc(src)) != EOF)
	{
		if(c == ';' || isspace(c))
			return;
	}
}

void read_garbage2()
{
	int c;
	while((c = fgetc(src)) != EOF)
	{
		if(c == '"' || c == '\n')
			return;
	}
}

int tkn_word(char *attr)
{
	if (!strcmp("Boolean", attr))
		return tkn_BOOL;
	if (!strcmp("break", attr))
		return tkn_BREAK;
	if (!strcmp("class", attr))
		return tkn_CLASS;
	if (!strcmp("continue", attr))
		return tkn_CONT;
	if (!strcmp("do", attr))
		return tkn_DO;
	if (!strcmp("double", attr))
		return tkn_DOUBLE;
	if (!strcmp("else", attr))
		return tkn_ELSE;
	if (!strcmp("false", attr))
		return tkn_FALSE;
	if (!strcmp("for", attr))
		return tkn_FOR;
	if (!strcmp("if", attr))
		return tkn_IF;
	if (!strcmp("int", attr))
		return tkn_INT;
	if (!strcmp("return", attr))
		return tkn_RET;
	if (!strcmp("String", attr))
		return tkn_STRING;
	if (!strcmp("static", attr))
		return tkn_STATIC;
	if (!strcmp("true", attr))
		return tkn_TRUE;
	if (!strcmp("void", attr))
		return tkn_VOID;
	if (!strcmp("while", attr))
		return tkn_WHILE;

	return tkn_ID;
}

int onechar_tkn(int c)
{
	switch(c){
		case '+': return tkn_PLUS;
		case '-': return tkn_MINUS;
		case '*': return tkn_MUL;
		case '(': return tkn_LPAREN;
		case ')': return tkn_RPAREN;
		case '{': return tkn_LBLOCK;
		case '}': return tkn_RBLOCK;
		case ';': return tkn_SEMI;
		case ',': return tkn_COMMA;
	}
	return 0;
}

bool is_tokenchar(int c)
{
	if(c == '(')
		return true;
	if(c == ')')
		return true;
	if(c == '{')
		return true;
	if(c == '}')
		return true;
	if(c == '+')
		return true;
	if(c == '-')
		return true;
	if(c == '*')
		return true;
	if(c == '/')
		return true;
	if(c == '<')
		return true;
	if(c == '>')
		return true;
	if(c == '=')
		return true;
	if(c == '!')
		return true;
	if(c == ';')
		return true;
	if(c == ',')
		return true;
	return false;
}

bool is_octdigit(int c)
{
	return (c >= '0' && c <= '7');
}

int solve_esc(int c)
{
	switch(c){
		case '"': return '\"';
		case 'n': return '\n';
		case 't': return '\t';
		case '\\': return '\\';
	}
	return -1;
}

int get_token(char *attr)
{
	if(endfl) return EOF;

	attr[0] = '\0'; 		// vycisteni atributu

	int state = st_NULL;	// vychozi stav automatu

	int cnt = 0;			// pocitadlo znaku ulozenych v attr

	bool flag = 0;			// pomocny flag pro rozhodovani o stavu automatu

	int c;
	while(!endfl)
	{
		c = fgetc(src);
		if(c == EOF) { c = ' '; endfl = 1; } // nemuzeme hned vratit EOF, potrebujeme posledni token

		switch(state){
			case st_NULL:
				if(isspace(c)) 								continue;
				else if(isalpha(c) || c == '_' || c == '$') { state = st_WORD;  attr[cnt++] = c; continue; }
				else if(isdigit(c)) 						{ state = st_NUM; attr[cnt++] = c; continue; }
				else if(c == '/') 							{ state = st_SLASH; continue; } // nutno rozhodnout lomitko vs. koment
				else if(onechar_tkn(c))						return onechar_tkn(c);
				else if(c == '<' || c == '>')				{ state = st_CMP; attr[0] = c; continue; }
				else if(c == '!')							{ state = st_EXCL; attr[0] = c; continue; }
				else if(c == '=')							{ state = st_EQ; attr[0] = c; continue; }
				else if(c == '"')							{ state = st_LIT; continue; }
				else										{ attr[cnt++] = c; attr[cnt] = '\0'; read_garbage() ; return LEX_ERR; }
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
				if(isdigit(c)) 							{ attr[cnt++] = c; continue; }
				else if(c == 'E' || c == 'e') 			{ state = st_REALE; attr[cnt++] = c; continue; }
				else if(c == '.')						{ state = st_REAL; attr[cnt++] = c; continue; }
				else if(is_tokenchar(c) || isspace(c))	{ ungetc(c, src); attr[cnt] = '\0'; return tkn_NUM; }
				else									{ attr[cnt++] = c; attr[cnt] = '\0'; read_garbage(); return LEX_ERR; }
			break;

			case st_REALE:	// '1561e' nebo '1561.34e'
				if(c == '+' || c == '-' || !flag)		{ attr[cnt++] = c; flag = 1; continue; }
				else if(isdigit(c))						{ attr[cnt++] = c; flag = 1; continue; }
				else if(is_tokenchar(c) || isspace(c))	{ ungetc(c, src); attr[cnt] = '\0'; return tkn_REAL; }
				else 									{ attr[cnt++] = c; attr[cnt] = '\0'; read_garbage(); return LEX_ERR; }
			break;

			case st_REAL: // '1561.'
				if(isdigit(c))										{ attr[cnt++] = c; flag = 1; continue; }
				else if((c == 'E' || c == 'e') && flag)				{ state = st_REALE; flag = 0; attr[cnt++] = c; continue; }
				else if((is_tokenchar(c) || isspace(c)) && flag)	{ ungetc(c, src); attr[cnt] = '\0'; return tkn_REAL; }
				else												{ attr[cnt++] = c; attr[cnt] = '\0'; read_garbage(); return LEX_ERR; }

			case st_WORD: // identifikator nebo keyword
				if(isalnum(c) || c == '_' || c == '$')	{ attr[cnt++] = c; continue; }
				else if(c == '.')						{ state = st_LONGID; attr[cnt++] = c; continue; }
				else if(is_tokenchar(c) || isspace(c))	{ ungetc(c, src); attr[cnt] = '\0'; return tkn_word(attr); }
				else 									{ attr[cnt++] = c; attr[cnt] = '\0'; read_garbage(); return LEX_ERR; }
			break;

			case st_LONGID:	// class.identifikator
				if(isalnum(c) || c == '_' || c == '$')			{ attr[cnt++] = c; flag = 1; continue; }
				else if(flag && (is_tokenchar(c) || isspace(c))){ attr[cnt] = '\0'; return tkn_ID; }
				else 											{ attr[cnt++] = c; attr[cnt] = '\0'; read_garbage(); return LEX_ERR; }
			break;

			case st_CMP: // <, >
				if(c == '=')	{ attr[1] = c; attr[2] = '\0'; return (attr[0] == '<') ? tkn_ELOWER:tkn_EHIGHER; }
				else 			{ ungetc(c, src); attr[1] = '\0'; return (attr[0] == '<') ? tkn_LOWER:tkn_HIGHER; }
			break;

			case st_EXCL: // !
				if(c == '=')	{ attr[1] = c; attr[2] = '\0'; return tkn_NEQUAL; }
				else 			{ ungetc(c, src); attr[1] = '\0'; return LEX_ERR; }
			break;

			case st_EQ:	// =
				if(c == '=')	{ attr[1] = c; attr[2] = '\0'; return tkn_EQUAL; }
				else 			{ ungetc(c, src); attr[1] = '\0'; return tkn_ASSIGN; }
			break;

			case st_LIT: // "some words etc."
				if(c == '"')	{ attr[cnt] = '\0'; return tkn_LIT; }
				else if(c == '\\') { state = st_LITESC; continue; }
				else if(c == '\n') { attr[cnt] = '\0'; return LEX_ERR; }
				else 				{ attr[cnt++] = c; continue; }
			break;

			case st_LITESC:	// v retezcovem literalu po nalezeni escape sekvence
				if(solve_esc(c) != -1)	{ state = st_LIT; attr[cnt++] = solve_esc(c); continue; }
				else 					{ attr[cnt] = '\0'; read_garbage2(); return LEX_ERR; }
			break;

			default: break;
		}
	}
	return EOF;
}