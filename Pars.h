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

	#define st_Start 0
	#define st_Class 1
	#define st_InFce 2
	#define st_StartPrg 3
	void PARS_Run();

	
	void GetNewClass(S_String * ID,S_String * newClass);
	t_Value * SolveFce(S_String * ID,bool run);
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
	void ElseSt(int * nestingLevel);
	void CheckReturn(S_Fce * fce,t_Value * result);

	void checkIfNextTokenIsSmt(int token);
	void FR_tknID(bool canFce);
	void FR_DECLR();
	void FR_BreakSt();
	void FR_ContinueSt();
	void FR_cycleDo(int * nestingLevel);
	void FR_cycleFor(int * nestingLevel);
	void FR_cycleWhile(int * nestingLevel);
	void FR_IfSt(int * nestingLevel);
	void FR_ReturnSt();
	bool FR_RBool(int * nestingLevel,bool * canElse);
	void FR_ElseSt(int * nestingLevel,bool canElse);
	void FR_SynError();
	bool FR_checkFceType(int token);
	bool FR_checkVarType(int token);
	void FR_DeclrVarGlobal(S_String * ID,int type,int token);
	void FR_DeclrFceGlobal(S_String * ID,int type);
	void FR_InClass();
	void FR_SynError();
	void FR_HandleClass();
	void FR_FceParamsSyntax();
	void FR_checkExpr(int EndToken1,int EndToken2, int * rEndToken, bool addEndToken,bool canEmpty);
	void FR_fceMat();
	void FR_load();
	bool FR_jmpEmpty();
	void FR_Solve();
#endif