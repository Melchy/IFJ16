/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

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

/* Kontorla syntaxe vyrazu */
void EXPR_CheckSyntax(bool emptyAllowed);

/* Uvolneni vyrazu */
void EXPR_Dispose();

#endif