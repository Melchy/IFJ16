#ifndef JL_H 
#define JL_H
	
	#include <stdio.h>
	#include "MEM.h"

	typedef struct S_JMP {
		long Offset;
		int JumpType;
		int NestingLevel;
		struct S_JMP * prev;
	} S_JMP;

	void JL_Add(long Offset, int JumpType, int NestingLevel);
	S_JMP * JL_Get();
	void JL_Remove();
	long JL_GetOffset();
	int JL_GetJumpType();
	int JL_GetNestingLevel();
	
	

	
#endif