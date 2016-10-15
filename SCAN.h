#ifndef SCAN_H
#define SCAN_H

#include "STR.h"

#include <stdio.h>
#include <stdbool.h>

/* Seznam definovanych tokenu */
#define tkn_ID		0 	// identifikator
#define tkn_NUM		1	// 60
#define tkn_LIT		2	// "string"
#define tkn_REAL	3	// 1.35 nebo 3.15e64 nebo 3e-3 nebo 4.78E-35

#define tkn_BOOL	4
#define tkn_INT		5
#define tkn_DOUBLE	6
#define tkn_STRING	7

#define tkn_LPAREN	8	// '('
#define tkn_RPAREN	9	// ')'
#define	tkn_LBLOCK	10	// '{'
#define tkn_RBLOCK	11	// '}'

#define tkn_PLUS	12	// '+'
#define tkn_MINUS	13	// '-'
#define tkn_MUL		14	// '*'
#define tkn_DIV		15	// '/'

#define tkn_ASSIGN	16	// '='
#define tkn_HIGHER	17	// '>'
#define tkn_LOWER	18	// '<'
#define tkn_EHIGHER	19	// '>='
#define tkn_ELOWER	20	// '<='
#define tkn_EQUAL	21	// '=='
#define tkn_NEQUAL	22	// '!='

#define tkn_BREAK	23
#define tkn_CLASS	24
#define tkn_CONT	25	// 'continue'
#define tkn_DO 		26
#define tkn_ELSE	27
#define tkn_FALSE	28
#define tkn_FOR		29
#define tkn_IF		30
#define tkn_RET		31	// 'return'
#define tkn_STATIC	32
#define tkn_TRUE	33
#define tkn_VOID	34
#define tkn_WHILE	35
#define tkn_SEMI	36
#define tkn_COMMA	37

/* Seznam definovanych stavu automatu lexikalni analyzy */
#define st_NULL		0	// vychozi stav
#define st_SLASH	1	// stav po precteni '/'
#define st_LCOMM	2	// stav v line komentu
#define st_BCOMM	3	// stav v block komentu
#define st_BCOMM_0	4	// stav, kdy v block komentu precteme '*'
#define st_NUM		5	// stav v cisle (potencialni real)
#define st_REALE	6	// stav v realu s exponentem
#define st_REAL 	7	// stav v obycejnem realu
#define st_WORD		8	// stav v identifikatoru nebo klicovem slove
#define st_LONGID	9	// stav v dlouhem identifikatoru
#define st_CMP		10	// stav po precteni '<' nebo '>'
#define st_EXCL		11	// stav po precteni '!'
#define st_EQ		12	// stav po precteni '='
#define st_LIT		13	// v retezcovem literalu
#define st_LITESC	14	// v retezcovem literalu v escape sekvenci

#define ATTR_SIZE	4

/* Konecny stavovy automat */
int SCAN_GetToken();

/* Funkce pro nalezeni konkretniho tokenu */
bool SCAN_FindToken(int token);

/* Atribut automatu */
S_String SCAN_attr;

#endif