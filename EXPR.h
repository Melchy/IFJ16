#ifndef EXPR_H
#define EXPR_H

#include "VARTAB.h"
#include "STR.h"

/* Vytvori novy vyraz */
void EXPR_Create();

/* Prida symbol do vyrazu (operator, zavorka) */
void EXPR_AddSym(int token);

/* Prida hodnotu do vyrazu (id nebo literal) */
void EXPR_AddVal(int token, S_String *attr);

/* Vyresi vyraz, vrati ukazatel na vysledek */
/* Vola kontrolu semantiky */ 
t_Value *EXPR_Solve();

#endif