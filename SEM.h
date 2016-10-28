#ifndef SEM_H
#define SEM_H

#include "VARTAB.h"

#include <stdbool.h>

bool SEM_IsAllowed(t_Value *l, t_Value *r, int op);

#endif