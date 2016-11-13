#ifndef EXPR_H
#define EXPR_H

#include "VARTAB.h"
#include "STR.h"

/* Vytvori novy vyraz */
void EXPR_Create();

/* Prida hodnotu do vyrazu (id nebo literal) */
void EXPR_AddVal(int token, S_String *attr);

/* Prida hodnotu do vyrazu predanou skrze t_Value */
void EXPR_AddVal2(t_Value *value);

/* Vyresi vyraz, vrati ukazatel na vysledek */
t_Value *EXPR_Solve();

#endif