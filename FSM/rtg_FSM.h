#ifndef rtg_FSM_H
#define rtg_FSM_H

#include <stdio.h>

#define tkn_ID		0 	// identifikator
#define tkn_NUM		1	// 60
#define tkn_WORD	2	// "blablabla"
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

#define LEX_ERR		-2
#define SYN_ERR		-3

#define st_NULL		0
#define st_SLASH	1
#define st_LCOMM	2
#define st_BCOMM	3
#define st_BCOMM_0	4
#define st_NUM		5
#define st_REALE	6
#define st_REAL 	7
#define st_WORD		8
#define st_LONGID	9

void set_file(FILE *);
void read_garbage();
int tkn_word(char *);
int get_token(char *);

#endif