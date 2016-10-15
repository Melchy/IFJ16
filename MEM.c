#include "ERROR.h"
#include "MEM.h"
#include "SCAN.h"

#include <stdio.h>
#include <stdlib.h>

static t_Address *all_pointers[SIZE_HTAB_MEM];

void hash_add(t_Address *address)
{
	// pridani adresy na prvni pozici v seznamu - nejefektivnejsi
	address->next = all_pointers[hashf_mem((long)address->ptr)];
	all_pointers[hashf_mem((long)address->ptr)] = address;
}

t_Address *hash_remove(void *ptr)
{
	t_Address *p = all_pointers[hashf_mem((long)ptr)];
	t_Address *p_prev = NULL;
	// hledame adresovou shodu v seznamu
	while((long)ptr != (long)p->ptr){
		p_prev = p;
		p = p->next;
	}
	if(p_prev != NULL)
		p_prev->next = p->next;
	else
		all_pointers[hashf_mem((long)ptr)] = p->next;

	return p;
}

int hashf_mem(long address)
{
	return (int)(address/7)%SIZE_HTAB_MEM;
}

void *MEM_malloc(size_t size)
{
	// alokace uzitecne pameti
	void *retval = malloc(size);
	if(retval == NULL)
		ERROR_exit(ENV_ERR);

	// alokace jedne polozky s adresou
	t_Address *address = malloc(sizeof(t_Address));
	if(address == NULL)
		ERROR_exit2(ENV_ERR, retval);

	address->ptr = retval;
	hash_add(address);

	return retval;
}

void *MEM_realloc(void *ptr, size_t size)
{
	// vytahneme si polozku ze seznamu ven
	if(ptr == NULL){
		ptr = MEM_malloc(size);
		return ptr;
	}
		printf("her\n");
	t_Address *p = hash_remove(ptr);
	if((p->ptr = realloc(p->ptr, size)) == NULL)
	{
		p->ptr = ptr;
		hash_add(p);
		ERROR_exit(ENV_ERR);
	}

	// po zmene adresy ji umistime zpet (pravdepodobne jinam)
	hash_add(p);
	return p->ptr;
}

void MEM_ffree(void *ptr)
{
	t_Address *p = all_pointers[hashf_mem((long)ptr)];
	t_Address *p_prev = NULL;
	// hledame adresovou shodu v seznamu
	while((long)ptr != (long)p->ptr){
		p_prev = p;
		p = p->next;
	}
	if(p_prev != NULL)
		p_prev->next = p->next;
	else
		all_pointers[hashf_mem((long)ptr)] = p->next;

	free(p->ptr); 
	free(p);
}

void MEM_clearAll()
{
	t_Address *p;
	t_Address *p_next;
	// projdeme cele pole seznamu a dealokujeme uzivatelskou pamet i polozky s adresami
	for (int i = 0; i < SIZE_HTAB_MEM; i++)
	{
		p = all_pointers[i];
		while(p != NULL)
		{
			free(p->ptr);
			p_next = p->next;
			free(p);
			p = p_next;
		}
	}
}

/* jen testovaci funkce */
void MEM_printAll()
{
	t_Address *p;
	for (int i = 0; i < SIZE_HTAB_MEM; i++)
	{
		p = all_pointers[i];
		while(p != NULL)
		{
			printf("HTAB INDEX: %d\t HTAB ID: %s\n",i , (char *)p->ptr);
			p = p->next;
		}
	}
}