/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#ifndef ERROR_H
#define ERROR_H

/* Lexikalni chyba */
#define LEX_ERR 		1

/* Chyba syntaxe */
#define SYN_ERR 		2

/* Semanticka chyba - nedefinovana, redefinice */
#define SEM_ERR_DEF 	3

/* Semanticka chyba - datovy typ, pocet parametru funkce */
#define SEM_ERR_TYPE	4

/* Ostatni semanticke chyby */
#define SEM_ERR_OTHER	6

/* Behova chyba pri nacitani ciselne hodnoty ze vstupu */
#define NUM_ERR			7

/* Behova chyba - neinicializovana promenna */
#define INIT_ERR		8

/* Behova chyba - deleni nulou */
#define DIV_NULL_ERR	9

/* Ostatni chyby */
#define RANDOM_ERR		10

/* Interni chyba - pamet, prace se souborem, spatne parametry pri spousteni atd. */
#define ENV_ERR			99

/* Korektni ukonceni programu */
void ERROR_exit(int errCode);

/* "ptrtofree"  je ukazatel na alokovanou pamet, ktery jako jediny nefiguruje v tabulce adres */
void ERROR_exit2(int errCode, void *ptrtofree);

#endif