#include "STR.h"
#include "MEM.h"
#include "ERROR.h"
#include "FIO.h"

#include <errno.h>

S_String *STR_Create(char *value)
{
	S_String *s = MEM_malloc(sizeof(S_String));
	s->len = strlen(value);
	s->size = s->len + 1;
	s->str = MEM_malloc(s->size);
	strcpy(s->str, value);
	
	return s;
}

S_String *STR_IntToString(int value)
{
  int n = snprintf(NULL, 0, "%d", value);
  char buffer[n+1];

  snprintf(buffer, n + 1, "%d", value);
  S_String *s = STR_Create(buffer);
  return s;
}

S_String *STR_LongToString(long value)
{
  int n = snprintf(NULL, 0, "%ld", value);
  char buffer[n+1];

  snprintf(buffer, n + 1, "%ld", value);
  S_String *s = STR_Create(buffer);
  return s;
}

S_String *STR_DoubleToString(double value)
{
  int n = snprintf(NULL, 0, "%g", value);
  char buffer[n+1];

  snprintf(buffer, n + 1, "%g", value);
  S_String *s = STR_Create(buffer);
  return s;
}

void STR_PrintStr(S_String *s)
{
    printf("%s", s->str);
}

void STR_AddChar(S_String *s, int lastChar)
{
  if(s->size <= s->len + 1)
  {
    STR_ChangeSize(s, s->size + 1);
    s->str[s->len] = (char)lastChar;
    s->str[s->len+1] = '\0';
    s->len = s->len + 1;
  }
  else
  {
    s->str[s->len] = (char)lastChar;
    s->str[s->len+1] = '\0';
    s->len = s->len + 1;
  }
}

void STR_ChangeSize(S_String *s, int newSize)
{
  s->str = MEM_realloc(s->str, newSize);
  s->size = newSize;
}

void STR_ConCat(S_String *s1, S_String *s2)
{
  if(s1->size <= s1->len + s2 ->len)
  {
    STR_ChangeSize(s1,(s1->len + s2->len + 1));
    strcat(s1->str, s2->str);
    s1->len = s1->len + s2->len;
  }
  else
  {
    strcat(s1->str, s2->str);
    s1->len = s1->len + s2->len;
  }
}

void STR_Copy(S_String *s1, S_String *s2)
{
  MEM_free(s1);

  s1 = STR_Create(s2->str);
}

void STR_Trim(S_String *s)
{
  int i = 0;
  int j = 0;

  while(isspace((char)s->str[i])) 
  {
    i++;
  }
  
  s->len -= i;

  while(s->str[i] != '\0')
  {
    s->str[j] = s->str[i];
    j++;
    i++;
  }
  s->str[j] = '\0';

  i = j;

  while(isspace((char)s->str[j-1]) && j > 0) 
  {
    j--;
  }

  s->len -= i - j;
  s->str[j] = s->str[i];
}

/*
Return values:
-----------------------------------------------
if Return value < 0 then it indicates s1 is less than s2.

if Return value > 0 then it indicates s2 is less than s1.

if Return value = 0 then it indicates s1 is equal to s2.
----------------------------------------------- 
*/

int STR_Compare(S_String *s1, S_String *s2)
{
  return strcmp(s1->str, s2->str);
}

/*
Return values:
-------------------------------------------------------
-1 ..... The demanded character not found.
else ... Index to the character in the observed string.
-------------------------------------------------------
*/

int STR_FindChar(S_String *s, char subChar)
{
  char *ptr;
  int index;

  ptr = strchr(s->str, subChar);

  if(ptr == NULL)
  {
    return -1;
  }

  index = ptr - s->str;
  return index;
}

/*
Return values:
------------------------------------------------------------------------
-1 ..... The occurence of substring not found.
0 ...... The demanded string may have been an empty string.
else ... Index to the beginning of the substring in the observed string.
------------------------------------------------------------------------
*/

int STR_SubStr(S_String *s, S_String *s_sub, int start)
{
  int position = 0;
  char *a;
  char *b;
  char *x;
  char *y;

  a = s->str + start;
  b = s_sub->str;
  x = a;
  y = b;

  if(!*b)
  {
    return 0;
  }

  while(*a)
  {
    while(*x == *y)
    {
      x++;
      y++;

      if(*x == '\0' || *y == '\0')
      {
        break;
      }
    }
    
    if(*y == '\0')
    {
      break;
    }

    a++;
    position++;
    x = a;
    y = b;
  }

  if(*a)
  {
    return (position + start);
  }
  else
  {
    return -1;
  }
}

/*
Return values:
------------------------------------
0 ... Success
1 ... Serapator not found.
2 ... No String after the separator.
------------------------------------
*/

int STR_GetAfter(S_String *s, S_String **s_after, char separator)
{
  char *ptr = strchr(s->str, separator);
  
  if(ptr == NULL)
  {
    return 1;
  }

  char *value = ptr+1;

  if(*value == '\0')
  {
    return 2;
  } 

  *s_after = STR_Create(value);

  return 0;
}

/*
Return values:
-----------------------------
0 ... Successful.
1 ... Unconvertable content.
2 ... Result can't be stored.
-----------------------------
Range:
-----------------------------
INT_MAX = 2,147,483,647
INT_MIN = -2,147,483,648
-----------------------------
*/

int STR_StringToInt(S_String *s, int *result)
{
  char *ptr;
  errno = 0;

  *result = (int)strtol(s->str, &ptr, 10);

  if(*ptr != '\0')
  {
    return 1;
  }

  if(errno != 0)
  {
    return 2;
  }

  return 0;
}

/*
Return values:
-----------------------------
0 ... Successful.
1 ... Unconvertable content.
2 ... Result can't be stored.
-----------------------------
Range:
----------------------------------------------
LONG_MIN
32 bit compiler ... −2,147,483,648
64 bit compiler ... −9,223,372,036,854,775,808
 
LONG_MAX
32 bit compiler ... 2,147,483,647
64 bit compiler ... 9,223,372,036,854,775,807
----------------------------------------------
*/

int STR_StringToLong(S_String *s, long *result)
{
  char *ptr;
  errno = 0;

  *result = strtol(s->str, &ptr, 10);

  if(*ptr != '\0')
  {
    return 1;
  }

  if(errno != 0)
  {
    return 2;
  }

  return 0;
}

/*
Return values:
---------------------------------
0 ... Successful.
1 ... Unconvertable content.
2 ... Result can't be stored.
---------------------------------
Range:
---------------------------------
Float range: 2.3E-308 to 1.7E+308
---------------------------------
*/

int STR_StringToDouble(S_String *s, double *result)
{
  char *ptr;
  errno = 0;

  *result = strtod(s->str, &ptr);

  if(*ptr != '\0')
  {
    return 1;
  }

  if(errno != 0)
  {
    return 2;
  }

  return 0;
}