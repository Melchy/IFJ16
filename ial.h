#ifndef ial_H
#define ial_H

/* --- Libraries list --- */

#include <string.h>

#include "STR.h"
#include "MEM.h"
#include "ERROR.h"

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
char *read_line();

#endif