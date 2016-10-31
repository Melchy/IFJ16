#include "HASH.h"
#include "STR.h"

#include <stdio.h>

int HASH_Spread(S_String *ID)
{
    int hash = 0;
    for (int i = 0 ; ID->str[i] != '\0' ; i++)
        hash = 31*hash + ID->str[i];
    hash = hash < 0 ? -hash : hash;
    return hash % SIZETAB;
}
