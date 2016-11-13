#include "Pars.h"

//PARSER
void PARS_Run(){
    //TODO prvni pruchod
	SolveFce("Run");
}

t_Value * SolveFce(S_String * ID){
	int returnPos;
    S_Fce * fce = IL_GetFce(ID);
	FC_Call();
	FC_LoadArgs(fce);
	returnPos = FIO_GetPosition();
	FIO_MoveToPosition(fce->offset);
	t_Value * result = FceMat();
	CheckReturn(fce,result);
	FIO_MoveToPosition(returnPos);
	return result;
}
void CheckReturn(S_Fce * fce,result){
	if(fce->returnType != result->type){
		ERROR();
	}
}

void FC_Call(){
    PH_AllocTable();
    PH_MakeTree();
}

void FC_LoadArgs(S_Fce * fce){
    t_Value * val = MEM_malloc(sizeof(t_Value));
    int * endToken = MEM_malloc(sizeof(int));
    int argNumber = 0;
    
    bool noNextParam = false;
    val = PH_Solve(tkn_RPAREN,tkn_COMMA,endToken,false);

    while((*endToken) != tkn_RPAREN){
        if(noNextParam){
            ERROR();
        }else{
            noNextParam = FC_AddParamToTable(fce,val,argNumber);
        }
        val = PH_Solve(tkn_RPAREN,tkn_COMMA,endToken,false);
        if(val == NULL){
            ERROR();
        }
    }
    if(noNextParam){
        ERROR();
    }else{
        noNextParam = FC_AddParamToTable(fce,val,argNumber); 
    }
}

bool FC_AddParamToTable(S_Fce * fce, t_Value * val,int argNumber){
	if(val == NULL){
		return -1;
	}
    return IL_AllocParam(fce, val, argNumber);
}


t_Value * FceMat(){
	int * nestingLevel = 0;
    while(true){
        switch(SCAN_GetToken()){
            case tkn_ID: tknID();  continue;
            case tkn_BOOL: DECLR(tkn_BOOL); continue;
            case tkn_INT: DECLR(tkn_INT); continue;
            case tkn_DOUBLE: DECLR(tkn_DOUBLE); continue;
            case tkn_STRING: DECLR(tkn_STRING); continue;
            case tkn_BREAK: BreakSt(nestingLevel); continue;
            case tkn_CONT: ContinueSt(nestingLevel); continue;

            case tkn_DO: cycleDo(*nestingLevel); continue;
            case tkn_FOR: cycleFor(*nestingLevel); continue;
            case tkn_WHILE: cycleWhile(*nestingLevel); continue;

            case tkn_IF: IfSt(nestingLevel); continue;
            case tkn_RET: return ReturnSt();
            case tkn_RBLOCK: if(RBool(nestingLevel))  return NULL; else continue;

            case tkn_ELSE:ElseSt(); continue; //muze byt jelikoz vime ze syntax je spravny

            case tkn_SEMI: continue;
            default: ERROR();
        }
    }
}

t_Value * ReturnSt(){
	return PH_Solve(tkn_SEMI,-1, NULL, false);
}

void ElseSt(){
	SCAN_GetToken();
}

bool RBool(int * nestingLevel){
	if((*nestingLevel) == 0){//konec fce
		return true;
	}
	if((*nestingLevel) != JL_GetNestingLevel()){
		nestingLevel--;
	}
	int type = JL_GetJumpType();
	if(type == 3){//do
		if(VT_GetBoolSafe(PH_Solve(tkn_RPAREN, -1, NULL, true))){
			FIO_MoveToPosition(JL_GetOffset());
		}else{
			nestingLevel--;
			JL_Remove();
		}
	}else if(type == 1){//while
		int position = FIO_GetPosition();
		FIO_MoveToPosition(JL_GetOffset());
		if(!(VT_GetBoolSafe(PH_Solve(tkn_RPAREN, -1, NULL, true)))){
			nestingLevel--;
			JL_Remove();
			FIO_MoveToPosition(position);
		}else{
			SCAN_GetToken();
		}
	}else if(type == 2){//for
		int position = FIO_GetPosition();
		FIO_MoveToPosition(JL_GetOffset());
		if(!(VT_GetBoolSafe(PH_Solve(tkn_SEMI, -1, NULL, false)))){
			nestingLevel--;
			JL_Remove();
			FIO_MoveToPosition(position);
		}else{
			PH_Solve(tkn_RPAREN, -1, NULL, false);
			SCAN_GetToken();
		}
	}
    return false;
}

void ERROR(){
    ERROR_exit(SEM_ERR_TYPE);
}

void tknID(){
    S_String * ID = STR_Create(SCAN_GetAttr()->str);
    int token = SCAN_GetToken();
    if(token == tkn_LPAREN){ 
        SolveFce(ID);
    }else if(token == tkn_ASSIGN){
        EXPR_AddVal(tkn_ID,ID);
        MEM_free(ID);
        EXPR_AddVal(tkn_ASSIGN,NULL);
        PH_Solve(tkn_SEMI, -1, NULL, false);
    }
}

//DECLR
void DECLR(int type){
    int token = SCAN_GetToken();
    S_String * ID = STR_Create(SCAN_GetAttr()->str);
    IL_AllocVar(ID,type,false);
	if(token == tkn_ASSIGN){
        EXPR_AddVal(tkn_ID,ID);
        MEM_free(ID);
        EXPR_AddVal(tkn_ASSIGN,NULL);
        PH_Solve(tkn_SEMI, -1, NULL, false);
    }
}

//Cyccles
void cycleDo(int nestingLevel){
    SCAN_GetToken();
    MakeJump(3,nestingLevel);
}
void cycleWhile(int nestingLevel){
    MakeJump(1,nestingLevel);
    if(!VT_GetBoolSafe(PH_Solve(tkn_LBLOCK, -1, NULL, false))){
        JL_Remove();
        SCAN_FindToken(tkn_RBLOCK);
    }
}
void cycleFor(int nestingLevel){
	int token = SCAN_GetToken();
	//for("zde")
    if(token == tkn_BOOL || token == tkn_DOUBLE || token == tkn_INT || token == tkn_STRING){
    	DECLR(token);
    }else{
    	EXPR_AddVal(token, SCAN_GetAttr());
    	PH_Solve(tkn_SEMI, -1, NULL, false);
    }
    //for("";"zde")
    bool foo = VT_GetBoolSafe(PH_Solve(tkn_SEMI, -1, NULL, false));
    MakeJump(2,nestingLevel);
     //for("";"";"preskoci"){
    SCAN_FindToken(tkn_LBLOCK);
    if(!foo){
        JL_Remove();
        SCAN_FindToken(tkn_SEMI);
    }
}

void MakeJump(int JumpType, int nestingLevel){
    JL_Add(FIO_GetPosition(),JumpType,nestingLevel);
}

void IfSt(int * nestingLevel){
	if(VT_GetBoolSafe(PH_Solve(tkn_SEMI, -1, NULL, false))){
		(*nestingLevel)++;
    }else{
    	SCAN_FindToken(tkn_RBLOCK);
    }
}

void BreakSt(int * NestingLevel){
	int neccesaryNesting = JL_GetNestingLevel();
	while(neccesaryNesting > (*NestingLevel)){
		SCAN_FindToken(tkn_RBLOCK);
		(*NestingLevel)--;
	}
	SCAN_FindToken(tkn_RBLOCK);
	(*NestingLevel)--;
	JL_Remove();
}
void ContinueSt(int * NestingLevel){
	(*NestingLevel) = JL_GetNestingLevel();
	FIO_MoveToPosition(JL_GetOffset());
}



t_Value * PH_Solve(int EndToken1,int EndToken2, int * rEndToken, bool addEndToken){
    int token;
    S_String * ID;
    bool skipToken = false;
    while(true)
    {
        if(!skipToken){
            token = SCAN_GetToken();
        }
        skipToken = false;        
        if(token == EndToken1 || token == EndToken2){
        	if(rEndToken != NULL){
        		(*rEndToken) = token;
        	}
            if(addEndToken){
                EXPR_AddVal(token,NULL);
            }
            return EXPR_Solve();
        }
        PH_checkValidToken(token);
        if(token == tkn_ID){
            ID = STR_Create(SCAN_GetAttr()->str);
            token = SCAN_GetToken();
	        if(token != tkn_LPAREN){
	            skipToken = true;
                EXPR_AddVal(tkn_ID, ID);
                MEM_free(ID);
	        }else{
	        	EXPR_AddVal2(SolveFce(ID));
	        }
	        continue;
        }
        EXPR_AddVal(token,SCAN_GetAttr());
    }

}

int PH_checkValidToken(int token){
    if(token == tkn_ID || 
        token == tkn_NUM ||
        token == tkn_LIT ||
        token == tkn_REAL ||
        token == tkn_LPAREN ||
        token == tkn_RPAREN ||
        token == tkn_PLUS ||
        token == tkn_MINUS ||
        token == tkn_MUL ||
        token == tkn_DIV ||
        token == tkn_HIGHER ||
        token == tkn_LOWER ||
        token == tkn_EHIGHER ||
        token == tkn_ELOWER ||
        token == tkn_EQUAL ||
        token == tkn_NEQUAL ||
        token == tkn_EXCL ||
        token == tkn_AND ||
        token == tkn_OR ||
        token == tkn_ASSIGN){
        return true;
    }
    return false;
}

void PH_AllocTable(){
    HASHVAR_AddTable();
}

void PH_MakeTree(){
    EXPR_Create();
}