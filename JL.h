#ifndef JL_H 
#define JL_H
	
	#include <stdio.h>
	#include "MEM.h"

	void JL_Add(long Offset, int JumpType, int NestingLevel);
	void JL_Remove();
	long JL_GetOffset();
	int JL_GetJumpType();
	int JL_GetNestingLevel();
	
	typedef struct S_JMP {
		long Offset;
		int JumpType;
		int NestingLevel;
		struct S_JMP * prev;
	} S_JMP;

	
#endif