/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#ifndef JumpLogic_H 
#define JumpLogic_H
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdbool.h>

	#include "str.h"
	#include "tokens.h"
	#include "vartab.h"

	#include "fio.h"
	#include "scan.h"
	#include "str.h"
	#include "mem.h"
	#include "tree.h"
	#include "expr.h"
	#include "tokens.h"
	#include "vartab.h"
	#include "hashvar.h"
	#include "idlogic.h"
	#include "hashfce.h"
	#include "jl.h"
	#include "ial.h"
	
	#define whileType 1
	#define forType 2
	#define doType 3
	#define ifTrueType 4
	#define ifFalseType 5

	#define st_Start 0
	#define st_Class 1
	#define st_InFce 2
	#define st_StartPrg 3

	typedef struct classNameList
	{
		S_String * name;
		struct classNameList * next;
	} classNameList;


	void PARS_Run();

	void addClass(S_String * name);
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
	void IfSt(int * nestingLevel,bool * skipElse);
	void BreakSt(int * NestingLevel);
	void ContinueSt(int * NestingLevel);
	t_Value * PH_Solve(int EndToken1,int EndToken2, int * rEndToken, bool addEndToken);
	int PH_checkValidToken(int token, bool canEq);
	void PH_AllocTable();
	void PH_MakeTree();
	void PH_DisposeTree();
	void PH_DisposeTable();
	void ElseSt(int * nestingLevel,bool * skipElse);
	void CheckReturn(S_Fce * fce,t_Value * result);
	void IntitIfj16Fce();
	t_Value * PA_ifj16(S_String * ID);

	void checkIfNextTokenIsSmt(int token);
	void FR_tknID(bool canFce);
	void FR_DECLR(int nesting);
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
	void FR_checkExpr(int EndToken1,int EndToken2, int * rEndToken, bool addEndToken,bool canEmpty, bool canEq);
	void FR_fceMat();
	void FR_load();
	bool FR_jmpEmpty();
	void FR_Solve();
#endif