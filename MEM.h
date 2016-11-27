/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#ifndef MEM_H
#define MEM_H

#include <stdio.h>

#define SIZE_HTAB_MEM 20

#define MEM_free(p) MEM_ffree(p); p = NULL

typedef struct t_Address{
	void *ptr;
	struct t_Address *next;
} t_Address;

typedef t_Address* t_listAddress;

/* Alokuje uzitecnou pamet i pamet pro strukturu adresy a zavola hash_add */
void *MEM_malloc(size_t size);

/* Odstrani adresu z tabulky, zmeni adresu na novou a vlozi adresu zpet do tabulky */
void *MEM_realloc(void *ptr, size_t size);

/* Uvolni uzitecnou pamet, odstrani adresu z tabulky a uvolni pamet adresy */
void MEM_ffree(void *ptr);

/* Uvolni veskerou naalokovanou pamet */
void MEM_clearAll();

/* test */
void MEM_printAll();

#endif