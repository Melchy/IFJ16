/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#include "hash.h"
#include "str.h"

#include <stdio.h>

int HASH_Spread(S_String *ID)
{
    int hash = 0;
    for (int i = 0 ; ID->str[i] != '\0' ; i++)
        hash = 31*hash + ID->str[i];
    hash = hash < 0 ? -hash : hash;
    return hash % SIZETAB;
}
