/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#ifndef ial_H
#define ial_H

/* --- Libraries list --- */

#include <string.h>
#include <stdbool.h>

#include "str.h"
#include "mem.h"
#include "error.h"

/* --- Macro declaration --- */

#define SWAP(arr, a, b) \
  do { \
    int tmp = arr[a]; \
    arr[a] = arr[b]; \
    arr[b] = tmp; \
  } while (0)

/* --- Function declaration --- */

int readInt();
double readDouble();
S_String *readString();
void print(S_String *s);

int length(S_String *s);
int compare(S_String *s1, S_String *s2);
int find(S_String *s, S_String *search);
S_String *substr(S_String *s, int i, int n);
S_String *sort(S_String *s);

void HS_Sort(S_String *s);
void HS_Heapify(S_String *s, int i);
int BM_BCR(S_String *s, S_String *search);
bool verify_integer(S_String *s, int *result);
bool verify_double(S_String *s, double *result);
char *read_line();

#endif