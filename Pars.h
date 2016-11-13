#ifndef JumpLogic_H 
#define JumpLogic_H
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdbool.h>

	#include "STR.h"
	#include "Tokens.h"
	#include "VARTAB.h"

	#include "FIO.h"
	#include "SCAN.h"
	#include "STR.h"
	#include "MEM.h"
	#include "Tree.h"
	#include "EXPR.h"
	#include "Tokens.h"
	#include "VARTAB.h"
	#include "HASHVAR.h"
	#include "IDLogic.h"
	#include "HASHFCE.h"
	#include "JL.h"

	void PARS_Run();


	t_Value * SolveFce(S_String * ID);
	void FC_Call();
	bool FC_CheckExist(S_String * ID);
	void FC_LoadArgs(S_Fce * fce);
	bool FC_AddParamToTable(S_Fce * fce, t_Value * val,int argNumber);
	t_Value * FceMat();
	t_Value * ReturnSt();
	bool RBool(int * nestingLevel);
	bool GetBoolValue(t_Value * val);
	int getInitialState();
	void ERROR();
	void tknID();
	void DECLR(int type);
	void cycleDo();
	void cycleWhile();
	void cycleFor();
	void MakeJump(int JumpType, int nestingLevel);
	void IfSt(int * nestingLevel);
	void BreakSt(int * NestingLevel);
	void ContinueSt(int * NestingLevel);
	t_Value * PH_Solve(int EndToken1,int EndToken2, int * rEndToken, bool addEndToken);
	int PH_checkValidToken(int token);
	void PH_AllocTable();
	void PH_MakeTree();
	void ElseSt();
#endif