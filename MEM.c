/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#include "ERROR.h"
#include "MEM.h"

#include <stdio.h>
#include <stdlib.h>
static t_listAddress HashMem[SIZE_HTAB_MEM];

static int hashf_mem(long address)
{
	return (int)(address/7)%SIZE_HTAB_MEM;
}

static void hash_add(t_Address *address)
{
	// pridani adresy na prvni pozici v seznamu - nejefektivnejsi
	address->next = HashMem[hashf_mem((long)address->ptr)];
	HashMem[hashf_mem((long)address->ptr)] = address;
}

static t_Address *hash_remove(void *ptr)
{
	t_Address *p = HashMem[hashf_mem((long)ptr)];
	t_Address *p_prev = NULL;
	// hledame adresovou shodu v seznamu
	while((long)ptr != (long)p->ptr){
		p_prev = p;
		p = p->next;
	}
	if(p_prev != NULL)
		p_prev->next = p->next;
	else
		HashMem[hashf_mem((long)ptr)] = p->next;

	return p;
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
	if(ptr == NULL){
		ptr = MEM_malloc(size);
		return ptr;
	}
	// vytahneme si polozku ze seznamu ven
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
	t_Address *p = hash_remove(ptr);

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
		p = HashMem[i];
		HashMem[i] = NULL;
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
		p = HashMem[i];
		while(p != NULL)
		{
			printf("HTAB INDEX: %d\t HTAB ID: %s\n",i , (char *)p->ptr);
			p = p->next;
		}
	}
}