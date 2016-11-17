#ifndef TOKENS_H
#define TOKENS_H

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

#define tkn_HIGHER	16	// '>'
#define tkn_LOWER	17	// '<'
#define tkn_EHIGHER	18	// '>='
#define tkn_ELOWER	19	// '<='
#define tkn_EQUAL	20	// '=='
#define tkn_NEQUAL	21	// '!='

#define tkn_EXCL	22 // '!'
#define tkn_AND		23 // '&&'
#define tkn_OR 		24 // '||'

#define tkn_ASSIGN	25	// '='

#define tkn_BREAK	26
#define tkn_CLASS	27
#define tkn_CONT	28	// 'continue'
#define tkn_DO 		29
#define tkn_ELSE	30
#define tkn_FALSE	31
#define tkn_FOR		32
#define tkn_IF		33
#define tkn_RET		34	// 'return'
#define tkn_STATIC	35
#define tkn_TRUE	36
#define tkn_VOID	37
#define tkn_WHILE	38
#define tkn_SEMI	39
#define tkn_COMMA	40

#define tkn_EOF -1

#endif