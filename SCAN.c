#include "SCAN.h"
#include "FIO.h"
#include "STR.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define Add_Char (bytes_allocated == SCAN_cnt) ?\
						(bytes_allocated *= 2, realloc(SCAN_attr.str, bytes_allocated) == NULL ?\
								(fprintf(stderr, "MEMORY FAIL"), exit(1), 1) :\
								(SCAN_attr.str[SCAN_cnt++] = c, 1)) :\
						(SCAN_attr.str[SCAN_cnt++] = c, 1)

#define Add_Null (bytes_allocated == SCAN_cnt) ?\
						(bytes_allocated *= 2, realloc(SCAN_attr.str, bytes_allocated) == NULL ?\
								(fprintf(stderr, "MEMORY FAIL"), exit(1), 1) :\
								(SCAN_attr.str[SCAN_cnt++] = '\0', 1)) :\
						(SCAN_attr.str[SCAN_cnt++] = '\0', 1)

static bool endfl = 0;	// end flag pro indikaci nalezu EOF
static size_t bytes_allocated = ATTR_SIZE;
static int state;

void read_garbage()
{
	int c;
	while((c = FIO_GetChar()) != EOF)
	{
		if(c == ';' || isspace(c))
			return;
	}
}

void read_garbage2() // pouzivana v retezcovem literalu
{
	int c;
	while((c = FIO_GetChar()) != EOF)
	{
		if(c == '"' || c == '\n')
			return;
	}
}

int tkn_word()
{
	if (!strcmp("Boolean", SCAN_attr.str))
		return tkn_BOOL;
	if (!strcmp("break", SCAN_attr.str))
		return tkn_BREAK;
	if (!strcmp("class", SCAN_attr.str))
		return tkn_CLASS;
	if (!strcmp("continue", SCAN_attr.str))
		return tkn_CONT;
	if (!strcmp("do", SCAN_attr.str))
		return tkn_DO;
	if (!strcmp("double", SCAN_attr.str))
		return tkn_DOUBLE;
	if (!strcmp("else", SCAN_attr.str))
		return tkn_ELSE;
	if (!strcmp("false", SCAN_attr.str))
		return tkn_FALSE;
	if (!strcmp("for", SCAN_attr.str))
		return tkn_FOR;
	if (!strcmp("if", SCAN_attr.str))
		return tkn_IF;
	if (!strcmp("int", SCAN_attr.str))
		return tkn_INT;
	if (!strcmp("return", SCAN_attr.str))
		return tkn_RET;
	if (!strcmp("String", SCAN_attr.str))
		return tkn_STRING;
	if (!strcmp("static", SCAN_attr.str))
		return tkn_STATIC;
	if (!strcmp("true", SCAN_attr.str))
		return tkn_TRUE;
	if (!strcmp("void", SCAN_attr.str))
		return tkn_VOID;
	if (!strcmp("while", SCAN_attr.str))
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
	//doplnit
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

int solve_esc(int *c0)
{
	switch(*c0){
		case '"': return *c0 = '\"';
		case 'n': return *c0 = '\n';
		case 't': return *c0 = '\t';
		case '\\': return *c0 = '\\';
	}
	int c1 = FIO_GetChar(); int c2 = FIO_GetChar();
	int ret = 0;

	if(is_octdigit(*c0)) ret += 64*(*c0-'0');
	else return -1;
	if(is_octdigit(c1)) ret += 8*(c1-'0');
	else { FIO_UngetChar(c1); return -1; }
	if(is_octdigit(c2)) ret += (c2-'0');
	else { FIO_UngetChar(c2); FIO_UngetChar(c1); return -1; }

	if(ret >= 1 && ret <= 255) // rozsah \001 - \377
	{
		*c0 = ret;
		return ret;
	}
	return -1;
}

int SCAN_GetToken()
{
	if(endfl) return EOF;

	state = st_NULL;
	SCAN_attr.str[0] = '\0'; SCAN_cnt = 0;	// vycisteni atributu
	bool flag = 0;					// pomocny flag pro rozhodovani o stavu automatu

	int c;
	while(!endfl)
	{
		c = FIO_GetChar();
		if(c == EOF) { c = ' '; endfl = 1; } // nemuzeme hned vratit EOF, potrebujeme posledni token

		switch(state){
			case st_NULL:
				if(isspace(c)) 								continue;
				else if(isalpha(c) || c == '_' || c == '$') { state = st_WORD;  Add_Char; continue; }
				else if(isdigit(c)) 						{ state = st_NUM; Add_Char; continue; }
				else if(c == '/') 							{ state = st_SLASH; continue; } // nutno rozhodnout lomitko vs. koment
				else if(onechar_tkn(c))						return onechar_tkn(c);
				else if(c == '<' || c == '>')				{ state = st_CMP; SCAN_attr.str[0] = c; continue; }
				else if(c == '!')							{ state = st_EXCL; SCAN_attr.str[0] = c; continue; }
				else if(c == '=')							{ state = st_EQ; SCAN_attr.str[0] = c; continue; }
				else if(c == '"')							{ state = st_LIT; continue; }
				else										{ Add_Char; Add_Null; read_garbage() ; return LEX_ERR; }
			break;

			case st_SLASH:	// line koment, block koment nebo lomitko (deleni)
				if(c == '/') 		{ state = st_LCOMM; continue; }
				else if(c == '*') 	{ state = st_BCOMM; continue; }
				else				{ FIO_UngetChar(c); state = st_NULL; return tkn_DIV; }
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
				if(isdigit(c)) 							{ Add_Char; continue; }
				else if(c == 'E' || c == 'e') 			{ state = st_REALE; Add_Char; continue; }
				else if(c == '.')						{ state = st_REAL; Add_Char; continue; }
				else if(is_tokenchar(c) || isspace(c))	{ FIO_UngetChar(c); Add_Null; return tkn_NUM; }
				else									{ Add_Char; Add_Null; read_garbage(); return LEX_ERR; }
			break;

			case st_REALE:	// '1561e' nebo '1561.34e'
				if(c == '+' || c == '-' || !flag)		{ Add_Char; flag = 1; continue; }
				else if(isdigit(c))						{ Add_Char; flag = 1; continue; }
				else if(is_tokenchar(c) || isspace(c))	{ FIO_UngetChar(c); Add_Null; return tkn_REAL; }
				else 									{ Add_Char; Add_Null; read_garbage(); return LEX_ERR; }
			break;

			case st_REAL: // '1561.'
				if(isdigit(c))										{ Add_Char; flag = 1; continue; }
				else if((c == 'E' || c == 'e') && flag)				{ state = st_REALE; flag = 0; Add_Char; continue; }
				else if((is_tokenchar(c) || isspace(c)) && flag)	{ FIO_UngetChar(c); Add_Null; return tkn_REAL; }
				else												{ Add_Char; Add_Null; read_garbage(); return LEX_ERR; }

			case st_WORD: // identifikator nebo keyword
				if(isalnum(c) || c == '_' || c == '$')	{ Add_Char; continue; }
				else if(c == '.')						{ state = st_LONGID; Add_Char; continue; }
				else if(is_tokenchar(c) || isspace(c))	{ FIO_UngetChar(c); Add_Null; return tkn_word(SCAN_attr.str); }
				else 									{ Add_Char; Add_Null; read_garbage(); return LEX_ERR; }
			break;

			case st_LONGID:	// trida.identifikator
				if(isalnum(c) || c == '_' || c == '$')			{ Add_Char; flag = 1; continue; }
				else if(flag && (is_tokenchar(c) || isspace(c))){ FIO_UngetChar(c); Add_Null; return tkn_ID; }
				else 											{ Add_Char; Add_Null; read_garbage(); return LEX_ERR; }
			break;

			case st_CMP: // <, >
				if(c == '=')	{ SCAN_attr.str[1] = c; SCAN_attr.str[2] = '\0'; return (SCAN_attr.str[0] == '<') ? tkn_ELOWER:tkn_EHIGHER; }
				else 			{ FIO_UngetChar(c); SCAN_attr.str[1] = '\0'; return (SCAN_attr.str[0] == '<') ? tkn_LOWER:tkn_HIGHER; }
			break;

			case st_EXCL: // !
				if(c == '=')	{ SCAN_attr.str[1] = c; SCAN_attr.str[2] = '\0'; return tkn_NEQUAL; }
				else 			{ FIO_UngetChar(c); SCAN_attr.str[1] = '\0'; return LEX_ERR; }
			break;

			case st_EQ:	// =
				if(c == '=')	{ SCAN_attr.str[1] = c; SCAN_attr.str[2] = '\0'; return tkn_EQUAL; }
				else 			{ FIO_UngetChar(c); SCAN_attr.str[1] = '\0'; return tkn_ASSIGN; }
			break;

			case st_LIT: // v retezcovem literalu
				if(c == '"')		{ Add_Null; return tkn_LIT; }
				else if(c == '\\') 	{ state = st_LITESC; continue; }
				else if(c == '\n') 	{ Add_Null; return LEX_ERR; }
				else 				{ Add_Char; continue; }
			break;

			case st_LITESC:	// v retezcovem literalu po nalezeni escape sekvence
				if(solve_esc(&c) != -1)	{ state = st_LIT; Add_Char; continue; }
				else 					{ Add_Char; Add_Null; read_garbage2(); return LEX_ERR; }
			break;

			default: break;
		}
	}
	return EOF;
}

bool SCAN_FindToken(int token)
{
	int t; 
	int nest = 1;
	while((t = SCAN_GetToken()) != EOF)
	{
		switch(token){
		case tkn_RPAREN:
			if(t == tkn_LPAREN) 					nest++;
			else if(t == tkn_RPAREN && nest == 1)	return true;
			else if(t == tkn_RPAREN && nest > 1)	nest--;
			else 									continue;
		break;
		case tkn_RBLOCK:
			if(t == tkn_LBLOCK) 					nest++;
			else if(t == tkn_RBLOCK && nest == 1)	return true;
			else if(t == tkn_RBLOCK && nest > 1)	nest--;
			else 									continue;
		break;
		default:
			if(t == token)	return true;
		break;
		}
	}
	return false;
}