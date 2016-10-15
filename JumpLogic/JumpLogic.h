#ifndef JumpLogic_H 
#define JumpLogic_H
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdbool.h>

	#include "STR.h"
	
	void JL_Add(int Offset, bool ExprAtEnd, int StateAfterJump,int NestingLevel, S_String * ClassAfterJMP);
	void JL_Remove();
	bool JL_GetExprAtEnd();
	int JL_GetStateAfterJump();
	int JL_GetOffset();
	int JL_GetNestingLevel();
	S_String * JL_GetClassAfterJMP();
	void JL_print();
	
	typedef struct S_JMP {
		int Offset;
		bool ExprAtEnd;
		int NestingLevel;
		S_String * ClassAfterJMP;
		int StateAfterJump;
		struct S_JMP * prev;
	} S_JMP;

	static S_JMP * active = NULL;
#endif