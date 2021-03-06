/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#ifndef STR_H
#define STR_H

/* --- Include knihoven --- */
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* --- Deklarace struktur --- */

typedef struct {
	char *str;
	int len;
	int size;
} S_String;

/* --- Deklarace funkci --- */

S_String *STR_GetIfj16Dot();
S_String *STR_Create(char *value);
S_String *STR_IntToString(int value);
S_String *STR_LongToString(long value);
S_String *STR_DoubleToString(double value);
S_String *STR_GetBefore(S_String *str, char ch);

void STR_Dispose(S_String *s);
void STR_PrintStr(S_String *s);
void STR_AddChar(S_String *s, int lastChar);
void STR_ChangeSize(S_String *s, int newSize);
void STR_ConCat(S_String *s1, S_String *s2);
void STR_Copy(S_String *s1, S_String *s2);
void STR_Trim(S_String *s);

int STR_Compare(S_String *s1, S_String *s2);
int STR_Compare2(S_String *s1, char *str);
int STR_FindChar(S_String *s, char subChar);
int STR_SubStr(S_String *s, S_String *s_sub, int start);
int STR_GetAfter(S_String *s, S_String **s_after, char separator);
int STR_GetBeforeEmpty(S_String *s, S_String **s_before, char separator);

int STR_StringToInt(S_String *s, int *result);
int STR_StringToLong(S_String *s, long *result);
int STR_StringToDouble(S_String *s, double *result);

#endif