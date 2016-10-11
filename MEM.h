#ifndef MEM_H
#define MEM_H

#include <stdio.h>

#define SIZE_HTAB_MEM 20

typedef struct t_Address t_Address;

struct t_Address{
	void *ptr;
	t_Address *next;
};

/* Prida item do hashovaci tabulky, nealokuje zadnou pamet */
void hash_add(t_Address *address);

/* Odstrani item z hashovaci tabulky, neuvolnuje pamet */
t_Address *hash_remove(void *ptr);

/* Hashovaci funkce */
int hashf_mem(long address);

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