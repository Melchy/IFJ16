#include "SCAN.h"
#include "FIO.h"
#include "STR.h"
#include "MEM.h"
#include "ERROR.h"
#include "Tokens.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define Add_Char STR_AddChar(attr, c)

#define Add_Null STR_AddChar(attr, '\0')

#define ATTR_SIZE	128
						
static bool endfl = 0;	// end flag pro indikaci nalezu EOF
static int state;
static S_String *attr;

/* Detekce klicovych slov */
static int tkn_word()
{
	if (!strcmp("boolean", attr->str))
		return tkn_BOOL;
	if (!strcmp("break", attr->str))
		return tkn_BREAK;
	if (!strcmp("class", attr->str))
		return tkn_CLASS;
	if (!strcmp("continue", attr->str))
		return tkn_CONT;
	if (!strcmp("do", attr->str))
		return tkn_DO;
	if (!strcmp("double", attr->str))
		return tkn_DOUBLE;
	if (!strcmp("else", attr->str))
		return tkn_ELSE;
	if (!strcmp("false", attr->str))
		return tkn_FALSE;
	if (!strcmp("for", attr->str))
		return tkn_FOR;
	if (!strcmp("if", attr->str))
		return tkn_IF;
	if (!strcmp("int", attr->str))
		return tkn_INT;
	if (!strcmp("return", attr->str))
		return tkn_RET;
	if (!strcmp("String", attr->str))
		return tkn_STRING;
	if (!strcmp("static", attr->str))
		return tkn_STATIC;
	if (!strcmp("true", attr->str))
		return tkn_TRUE;
	if (!strcmp("void", attr->str))
		return tkn_VOID;
	if (!strcmp("while", attr->str))
		return tkn_WHILE;

	return tkn_ID;
}

/* Pro zjednoduseni v pripade jednoznakovych tokenu */
static int onechar_tkn(int c)
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

/* Abychom rozeznali lexikalni chybu od korektniho ukonceni tokenu (nekdy za tokenem musi byt whitespace, jindy ne, zalezi co nasleduje) */
static bool is_tokenchar(int c)
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
	if(c == '"')
		return true;
	if(c == '|')
		return true;
	if(c == '&')
		return true;
	return false;
}

/* Vraci true v pripade ze znak je platna oktalova cislice */
static bool is_octdigit(int c)
{
	return (c >= '0' && c <= '7');
}

/* Precte znak(y) za '\' a vyresi je jako escape sekvenci, pri uspechu ho vrati skrz pointer, jinak return -1 */
static int solve_esc(int *c0)
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
	if(endfl) {
		endfl = 0;
		//return EOF;
	}
		
	

	state = st_NULL;
	attr->str[0] = '\0'; attr->len = 0;			// vycisteni atributu
	bool flag = 0;								// pomocny flag pro rozhodovani o stavu automatu

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
				else if(c == '<' || c == '>')				{ state = st_CMP; attr->str[0] = c; continue; }
				else if(c == '!')							{ state = st_EXCL; attr->str[0] = c; continue; }
				else if(c == '=')							{ state = st_EQ; attr->str[0] = c; continue; }
				else if(c == '"')							{ state = st_LIT; continue; }
				else if(c == '&')							{ state = st_AND; attr->str[0] = c; continue; }
				else if(c == '|')							{ state = st_OR; attr->str[0] = c; continue; }
				else										{ Add_Char; Add_Null; ERROR_exit(LEX_ERR); }
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
				else									{ Add_Char; Add_Null; ERROR_exit(LEX_ERR); }
			break;

			case st_REALE:	// '1561e' nebo '1561.34e'
				if((c == '+' || c == '-') && !flag)		{ Add_Char; flag = 1; continue; }
				else if(isdigit(c))						{ Add_Char; flag = 1; continue; }
				else if(is_tokenchar(c) || isspace(c))	{ FIO_UngetChar(c); Add_Null; return tkn_REAL; }
				else 									{ Add_Char; Add_Null; ERROR_exit(LEX_ERR); }
			break;

			case st_REAL: // '1561.'
				if(isdigit(c))										{ Add_Char; flag = 1; continue; }
				else if((c == 'E' || c == 'e') && flag)				{ state = st_REALE; flag = 0; Add_Char; continue; }
				else if((is_tokenchar(c) || isspace(c)) && flag)	{ FIO_UngetChar(c); Add_Null; return tkn_REAL; }
				else												{ Add_Char; Add_Null; ERROR_exit(LEX_ERR); }

			case st_WORD: // identifikator nebo keyword
				if(isalnum(c) || c == '_' || c == '$')	{ Add_Char; continue; }
				else if(c == '.')						{ state = st_LONGID; Add_Char; continue; }
				else if(is_tokenchar(c) || isspace(c))	{ FIO_UngetChar(c); Add_Null; return tkn_word(attr->str); }
				else 									{ Add_Char; Add_Null; ERROR_exit(LEX_ERR); }
			break;

			case st_LONGID:	// trida.identifikator
				if(isalnum(c) || c == '_' || c == '$')			{ Add_Char; flag = 1; continue; }
				else if(flag && (is_tokenchar(c) || isspace(c))){ FIO_UngetChar(c); Add_Null; return tkn_ID; }
				else 											{ Add_Char; Add_Null; ERROR_exit(LEX_ERR); }
			break;

			case st_CMP: // <, >
				if(c == '=')	{ attr->str[1] = c; attr->str[2] = '\0'; return (attr->str[0] == '<') ? tkn_ELOWER:tkn_EHIGHER; }
				else 			{ FIO_UngetChar(c); attr->str[1] = '\0'; return (attr->str[0] == '<') ? tkn_LOWER:tkn_HIGHER; }
			break;

			case st_EXCL: // !
				if(c == '=')	{ attr->str[1] = c; attr->str[2] = '\0'; return tkn_NEQUAL; }
				else 			{ FIO_UngetChar(c); attr->str[1] = '\0'; return tkn_EXCL; }
			break;

			case st_EQ:	// =
				if(c == '=')	{ attr->str[1] = c; attr->str[2] = '\0'; return tkn_EQUAL; }
				else 			{ FIO_UngetChar(c); attr->str[1] = '\0'; return tkn_ASSIGN; }
			break;

			case st_LIT: // v retezcovem literalu
				if(c == '"')		{ Add_Null; return tkn_LIT; }
				else if(c == '\\') 	{ state = st_LITESC; continue; }
				else if(c == '\n') 	{ Add_Null; return LEX_ERR; }
				else 				{ Add_Char; continue; }
			break;

			case st_LITESC:	// v retezcovem literalu po nalezeni escape sekvence
				if(solve_esc(&c) != -1)	{ state = st_LIT; Add_Char; continue; }
				else 					{ Add_Char; Add_Null; ERROR_exit(LEX_ERR); }
			break;

			case st_AND: // &
				if(c == '&')	{ attr->str[1] = c; attr->str[2] = '\0'; return tkn_AND; }
				else 			{ FIO_UngetChar(c); attr->str[1] = '\0'; ERROR_exit(LEX_ERR); }
			break;

			case st_OR: // |
				if(c == '|')	{ attr->str[1] = c; attr->str[2] = '\0'; return tkn_OR; }
				else 			{ FIO_UngetChar(c); attr->str[1] = '\0'; ERROR_exit(LEX_ERR); }
			break;

			default: break;
		}
	}
	return EOF;
}

void SCAN_InitAttr(){
	attr = STR_Create("");
	STR_ChangeSize(attr, ATTR_SIZE);
}

S_String *SCAN_GetAttr(){
	return attr;
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