#include "HASH.h"
#include "STR.h"

int HASH_Spread(S_String *ID)
{
	return ((int)ID->str[0] * 7) % SIZETAB;
}
