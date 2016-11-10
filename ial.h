#ifndef ial_H
#define ial_H

/* --- Libraries list --- */

#include <string.h>

#include "STR.h"

/* --- Macro declaration --- */

#define SWAP(arr, a, b) \
  do { \
    int tmp = arr[a]; \
    arr[a] = arr[b]; \
    arr[b] = tmp; \
  } while (0)

/* --- Function declaration --- */

void HS_Sort(S_String *s);
void HS_Heapify(S_String *s, int i);

int BM_BCRule(S_String *s, S_String *search);

#endif