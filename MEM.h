#ifndef MEM_H
#define MEM_H

#include <stdio.h>

#define SIZE_HTAB_MEM 20

typedef struct t_Address{
	void *ptr;
	struct t_Address *next;
} t_Address;

/* Alokuje uzitecnou pamet i pamet pro strukturu adresy a zavola hash_add */
void *MEM_malloc(size_t size);

/* Odstrani adresu z tabulky, zmeni adresu na novou a vlozi adresu zpet do tabulky */
void *MEM_realloc(void *ptr, size_t size);

/* Uvolni uzitecnou pamet, odstrani adresu z tabulky a uvolni pamet adresy */
void MEM_free(void *ptr);

/* Uvolni veskerou naalokovanou pamet */
void MEM_clearAll();

/* test */
void MEM_printAll();

#endif