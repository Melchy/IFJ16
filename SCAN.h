#ifndef SCAN_H
#define SCAN_H

#include "STR.h"

#include <stdio.h>
#include <stdbool.h>

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
#define st_AND		15 	// stav po precteni '&'
#define st_OR 		16 	// stav po precteni '|'

#define ATTR_SIZE	128

/* Konecny stavovy automat */
int SCAN_GetToken();

/* Inicializace atributu - se startem programu */
void SCAN_InitAttr();

/* Vrati ukazatel na atribut */
S_String *SCAN_GetAttr();

/* Funkce pro nalezeni konkretniho tokenu */
bool SCAN_FindToken(int token);

#endif