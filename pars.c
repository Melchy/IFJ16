/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#include "pars.h"

int State = -1;

classNameList * firstClass = NULL;

//PARSER
void PARS_Run(){
    //TODO po urcite dobe behu nekonecneho cyklu naskoci nedostatek pameti (zpusobeno spatnym odstranovanim stringu)
    //TODO mazani struktury jumpu
    EXPR_Create();
    S_String * foo = STR_Create("Main");
    State = st_Start;
    FR_load();
    IntitIfj16Fce();
    IL_SetClass(foo);
    foo->str = "run";
    PH_DisposeTree();
    SolveFce(foo,true);
}

void IntitIfj16Fce(){
    S_String * cl = STR_Create("ifj16");
    S_String * readInt = STR_Create("readInt");
    S_String * readDouble = STR_Create("readDouble");
    S_String * readString = STR_Create("readString");
    S_String * prnt = STR_Create("print");
    S_String * length = STR_Create("length");
    S_String * substr = STR_Create("substr");
    S_String * compare = STR_Create("compare");
    S_String * find = STR_Create("find");
    S_String * sort = STR_Create("sort");

    S_String * frst = STR_Create("first");
    S_String * second = STR_Create("second");
    S_String * thr = STR_Create("third");

    IL_SetClass(cl);
    IL_InitFce(readInt,tkn_INT, -1,NULL);
    IL_InitFce(readDouble,tkn_DOUBLE, -1,NULL);
    IL_InitFce(readString,tkn_STRING, -1,NULL);

    S_Param * str1 = MEM_malloc(sizeof(S_Param));
    str1->ID = frst;
    str1->type = tkn_STRING;

    IL_InitFce(length,tkn_INT, -1,str1);
    IL_InitFce(sort,tkn_STRING, -1,str1);

    str1 = MEM_malloc(sizeof(S_Param));
    str1->ID = frst;
    str1->type = tkn_STRING;

    S_Param * str2 = MEM_malloc(sizeof(S_Param));
    str2->ID = second;
    str2->type = tkn_STRING;

    str1->next = str2;
    
    IL_InitFce(compare,tkn_INT, -1,str1);//str str
    IL_InitFce(find,tkn_INT, -1,str1);//str str
    str1 = MEM_malloc(sizeof(S_Param));
    str1->ID = frst;
    str1->type = tkn_STRING;

    S_Param * int1 = MEM_malloc(sizeof(S_Param));
    int1->ID = second;
    int1->type = tkn_INT;

    S_Param * int2 = MEM_malloc(sizeof(S_Param));
    int2->ID = thr;
    int2->type = tkn_INT;

    str1->next = int1;
    int1->next = int2;
    IL_InitFce(substr,tkn_STRING, -1,str1);//str int int

    S_Param * bool1 = MEM_malloc(sizeof(S_Param));
    bool1->ID = frst;
    bool1->type = tkn_ALLTYPES;

    IL_InitFce(prnt,tkn_VOID, -1,bool1);

    STR_Dispose(cl);
    STR_Dispose(readInt);
    STR_Dispose(readDouble);
    STR_Dispose(readString);
    STR_Dispose(prnt);
    STR_Dispose(length);
    STR_Dispose(substr);
    STR_Dispose(compare);

    STR_Dispose(find);
    STR_Dispose(sort);
}


void FR_load(){
    while(true){
        switch(State){
            case st_Start: FR_HandleClass(); continue;
            case st_Class:FR_InClass(); continue;
            case st_InFce: FR_fceMat(); continue;
            case st_StartPrg: return;
            default: FR_SynError();
        }
    }

}

void FR_fceMat(){
    int * nestingLevel = MEM_malloc(sizeof(int));
    (*nestingLevel) = 0;
    bool * canElse = MEM_malloc(sizeof(bool));
    (*canElse) = false;
    bool end = false;
    int prevCanElse = false;
    while(true){
        prevCanElse = (*canElse);
        switch(SCAN_GetToken()){
            case tkn_ID: FR_tknID(true);  break;
            case tkn_BOOL: FR_DECLR(*nestingLevel); break;
            case tkn_INT: FR_DECLR(*nestingLevel); break;
            case tkn_DOUBLE: FR_DECLR(*nestingLevel); break;
            case tkn_STRING: FR_DECLR(*nestingLevel); break;
            case tkn_BREAK: FR_BreakSt(nestingLevel); break;
            case tkn_CONT: FR_ContinueSt(nestingLevel); break;

            case tkn_DO: FR_cycleDo(nestingLevel); break; 
            case tkn_FOR: FR_cycleFor(nestingLevel); break;
            case tkn_WHILE: FR_cycleWhile(nestingLevel); break;

            case tkn_IF: FR_IfSt(nestingLevel); break;
            case tkn_RET: FR_ReturnSt(); break;
            case tkn_RBLOCK: end = FR_RBool(nestingLevel,canElse); break;

            case tkn_ELSE:FR_ElseSt(nestingLevel,*canElse); break;

            case tkn_SEMI: break;
            default: FR_SynError();
        }
        if(end){
            MEM_free(nestingLevel);
            MEM_free(canElse);
            return;
        }
        if(prevCanElse == (*canElse)){
            (*canElse) = false;
        }
        continue;
    }
}

void FR_ElseSt(int * nestingLevel,bool canElse){
    if(!canElse){
        FR_SynError();
    }
    checkIfNextTokenIsSmt(tkn_LBLOCK);
    (*nestingLevel)++;
    JL_Add(0, ifTrueType, (*nestingLevel));
}
void FR_IfSt(int * nestingLevel){
    FR_checkExpr(tkn_LBLOCK,-1, NULL, false,false,false);
    JL_Add(0, ifFalseType, (*nestingLevel));
    (*nestingLevel)++;
}

void FR_ReturnSt(){
    FR_checkExpr(tkn_SEMI,-1, NULL, false,true,false);
}

bool FR_RBool(int * nestingLevel,bool * canElse){
    if(FR_jmpEmpty()){
        State = st_Class;
        return true;
    }
    int type = JL_GetJumpType();
    if(type == doType){ 
        FR_checkExpr(tkn_RPAREN,-1, NULL, true,false,false);//"do" condition
    }else if(type == ifFalseType){ 
        (*canElse) = true;   
    }
    (*nestingLevel)--;
    JL_Remove();
    return false;
}
void FR_cycleWhile(int * nestingLevel){
    FR_checkExpr(tkn_LBLOCK,-1, NULL, false,false,false);
    (*nestingLevel)++;
    JL_Add(0, whileType, (*nestingLevel));
}
void FR_cycleFor(int * nestingLevel){
    checkIfNextTokenIsSmt(tkn_LPAREN);
    int token = SCAN_GetToken();
    //for("zde")
    if(FR_checkVarType(token)){
        FR_DECLR(0);
    }else{
        
        EXPR_AddVal(token, SCAN_GetAttr());
        FR_checkExpr(tkn_SEMI,-1, NULL, false,false,false);
    }
    
    //for("";"zde")
    FR_checkExpr(tkn_SEMI,-1, NULL, false,false,false);
     //for("";"";"zde")
    
    FR_checkExpr(tkn_RPAREN,-1, NULL, false,false,true);
    //for("";"";"zde"){
    checkIfNextTokenIsSmt(tkn_LBLOCK);
    (*nestingLevel)++;
    JL_Add(0, forType, (*nestingLevel));
}
void FR_cycleDo(int * nestingLevel){
    checkIfNextTokenIsSmt(tkn_LBLOCK);
    (*nestingLevel)++;
    JL_Add(0, doType, (*nestingLevel));
}


void FR_ContinueSt(){
    if(FR_jmpEmpty()){
        FR_SynError();
    }
}
void FR_BreakSt(){
    if(FR_jmpEmpty()){
        FR_SynError();
    }
}

bool FR_jmpEmpty(){
    if(JL_Get() == NULL){
        return true;
    }
    return false;
}
void checkIfNextTokenIsSmt(int token){
    if(SCAN_GetToken() != token){
        FR_SynError();
    }
}

void FR_DECLR(int nesting){
    checkIfNextTokenIsSmt(tkn_ID);
    if(nesting > 0){
        FR_SynError();
    }
    FR_tknID(false);
}

void FR_tknID(bool canFce){
    int token = SCAN_GetToken();
    if(token == tkn_LPAREN){ 
        if(!canFce){
            FR_SynError();
        }
        FR_FceParamsSyntax();
    }else if(token == tkn_ASSIGN){
        EXPR_AddVal(tkn_ID,NULL);
        EXPR_AddVal(tkn_ASSIGN,NULL);
        FR_checkExpr(tkn_SEMI,-1, NULL, false,false,false);
    }else{
        if(token == tkn_SEMI){
            return;
        }
        FR_SynError();
    }
}

void FR_checkExpr(int EndToken1,int EndToken2, int * rEndToken, bool addEndToken,bool canEmpty,bool canEq){
    int token;
    bool skipToken = false;
    int nesting = 0;
    if(EndToken1  == tkn_LPAREN || 
            EndToken1  == tkn_LBLOCK || 
            EndToken2  == tkn_LPAREN || 
            EndToken2  == tkn_LBLOCK){
        nesting--;
    }
    while(true)
    {
        if(!skipToken){
            token = SCAN_GetToken();
        }
        if(token == tkn_LPAREN || token == tkn_LBLOCK){
            nesting++;
        } else if(token == tkn_RPAREN || token == tkn_RBLOCK){
            nesting--;
        } 
        skipToken = false;     
        if(token == EndToken1 || token == EndToken2){
            if(rEndToken != NULL){
                (*rEndToken) = token;
            }

            if(addEndToken){
                EXPR_AddVal(token,NULL);
            }

            if(nesting <= 0){

                EXPR_CheckSyntax(canEmpty);
                return; 
            }  
        }else{

        }
        if(!PH_checkValidToken(token,canEq)){
            FR_SynError();
        }
        if(token == tkn_ID){
            token = SCAN_GetToken();
            if(token != tkn_LPAREN){
                EXPR_AddVal(tkn_ID,NULL);
                skipToken = true;
            }else if(token == tkn_LPAREN){
                FR_FceParamsSyntax();
                EXPR_AddVal2(VT_GetEmpty(tkn_INT));
            }else{
                FR_SynError();
            }
            continue;
        }
        EXPR_AddVal(token,NULL);
    }
}

void FR_FceParamsSyntax(){
    EXPR_Create();
    int * token = MEM_malloc(sizeof(int));
    while(true){
        FR_checkExpr(tkn_COMMA,tkn_RPAREN, token, false,true,false);
        if((*token) == tkn_RPAREN){
            PH_DisposeTree();
            return;
        }
    }
}

void FR_HandleClass(){
    int token = SCAN_GetToken();
    if(token != tkn_CLASS){
        if(token == tkn_EOF){
            State = st_StartPrg;
            return;
        }
        FR_SynError();
    }
    checkIfNextTokenIsSmt(tkn_ID);
    addClass(SCAN_GetAttr());
    IL_SetClass(SCAN_GetAttr());
    checkIfNextTokenIsSmt(tkn_LBLOCK);

    State = st_Class;
}

void addClass(S_String * name){
    classNameList * foo = NULL;
    classNameList * class;
    class = firstClass;
    while(class != NULL){
        if(STR_Compare(class->name, name) == 0){
           FR_SynError();
        }
        foo = class;
        class = class->next;
    }

    class = MEM_malloc(sizeof(classNameList));
    class->name = STR_Create(name->str);
    class->next = NULL;
    if(firstClass != NULL){
        foo->next = class;
    }else{
        firstClass = class;
    }
}

void FR_SynError(){
    ERROR_exit(SYN_ERR);
}

void FR_InClass(){
    int token = SCAN_GetToken();
    if(token == tkn_RBLOCK){
        State = st_Start;
        return;
    }else{
        if(token != tkn_STATIC){
            FR_SynError();
        }
    }
    token = SCAN_GetToken();
    if(SCAN_GetToken() != tkn_ID) FR_SynError();
    S_String * ID = STR_Create(SCAN_GetAttr()->str);
    int foo = SCAN_GetToken();
    if(foo == tkn_LPAREN){
        FR_DeclrFceGlobal(ID,token);
    }else if (foo == tkn_SEMI){
        FR_DeclrVarGlobal(ID,token,foo);
    }else if(foo == tkn_ASSIGN){
        FR_DeclrVarGlobal(ID,token,foo);
    }else{
        FR_SynError();
    }
    STR_Dispose(ID);
}

void FR_DeclrFceGlobal(S_String * ID,int type){
    int token = SCAN_GetToken();
    S_Param * param = MEM_malloc(sizeof(S_Param));
    S_Param * foo;
    S_Param * firstParam = param;
    if(!FR_checkFceType(type)){
        FR_SynError();
    }
    if(token == tkn_RPAREN){
        MEM_free(param);
        firstParam = NULL;
    }
    while(token != tkn_RPAREN){
        if(!FR_checkVarType(token)) {
        	FR_SynError();
        }
        param->type = token;
        if((token = SCAN_GetToken()) != tkn_ID) {
        	FR_SynError();
        }
        param->ID = STR_Create(SCAN_GetAttr()->str);
        token = SCAN_GetToken();
        if(token == tkn_RPAREN){
            break;
        }else if(token == tkn_COMMA){
            foo = param;
            param = MEM_malloc(sizeof(S_Param));
            foo->next = param;
        }else{
            FR_SynError();
        }
        token = SCAN_GetToken();
    }
    if(SCAN_GetToken() != tkn_LBLOCK) FR_SynError();
    IL_InitFce(ID,type, FIO_GetPosition(),firstParam);
    State = st_InFce;
}

void FR_DeclrVarGlobal(S_String * ID,int type,int token){
    IL_AllocVar(ID, type,true);
    if(token == tkn_ASSIGN){
        EXPR_AddVal(tkn_ID,ID);
        EXPR_AddVal(tkn_ASSIGN,NULL);
        FR_Solve();
    }
    State = st_Class;
}

void FR_Solve(){
    int token = SCAN_GetToken();
    while(token != tkn_SEMI)
    {
        PH_checkValidToken(token,false);
        EXPR_AddVal(token,SCAN_GetAttr());
        token = SCAN_GetToken();
    }
    EXPR_Solve();
}

bool FR_checkVarType(int token){
    if(token == tkn_BOOL || token == tkn_INT || token == tkn_DOUBLE || token == tkn_STRING){
        return true;
    }
    return false;
}

bool FR_checkFceType(int token){
    if(FR_checkVarType(token) || (token == tkn_VOID)){
        return true;
    }
    return false;
}

//second ---------------------------------------------------------------------------------------------
t_Value * SolveFce(S_String * ID,bool run){
    //printf("%s\n", "syntax good");
    int returnPos;
    int reachableNest = IL_GetReachable();
    int nest = IL_GetNesting();
    S_Fce * fce = IL_GetFce(ID);
    PH_MakeTree();
    if(!run){
        FC_LoadArgs(fce);
    }

    S_String * class = STR_Create(IL_GetClass()->str);
    //Set new CLass
    S_String * newClass;
    newClass = STR_GetBefore(ID, '.');
    if(newClass == NULL){
        newClass = STR_Create(IL_GetClass()->str);
    }
    IL_SetClass(newClass);
    //end


    PH_AllocTable(-1);
    returnPos = FIO_GetPosition();
    t_Value * result;
    S_String * fooStr = STR_Create(IL_GetClass()->str);
    STR_AddChar(fooStr, '.');
    if(STR_Compare(fooStr, STR_GetIfj16Dot()) == 0){

        result = PA_ifj16(ID);
    }else{
        FIO_MoveToPosition(fce->offset);
        result = FceMat();
        CheckReturn(fce,result);
        FIO_MoveToPosition(returnPos);
    }
    STR_Dispose(fooStr);
    IL_SetClass(class);
    STR_Dispose(class);
    STR_Dispose(newClass);
    PH_DisposeTree();
    PH_DisposeTable(reachableNest);
    if(nest != IL_GetNesting()){
        PH_DisposeTable(reachableNest);
    }
    return result;
}

t_Value * PA_ifj16(S_String * ID){
    S_String ** fceNamePP = MEM_malloc(sizeof(S_String *));
    int foo = STR_GetAfter(ID,fceNamePP,'.');

    if(foo == 1 || foo == 2){
        ERROR();
    }
    S_String * fceNameP = *(fceNamePP);

    S_String * frst = STR_Create("first");
	S_String * second = STR_Create("second");
	S_String * third = STR_Create("third");
    if(STR_Compare2(fceNameP,"readInt") == 0){
        return VT_AddInt(readInt());
    }else if(STR_Compare2(fceNameP,"readDouble") == 0){
        return VT_AddDouble(readDouble());
    }else if(STR_Compare2(fceNameP,"readString") == 0){
        return VT_AddStr(readString());
    }else if(STR_Compare2(fceNameP,"print") == 0){
    	
    	t_Value * param1 = IL_GetVal(frst);
    	if(param1->type == tkn_TRUE){
    		printf("%s", "true");
    	}else if(param1->type == tkn_FALSE){
    		printf("%s", "false");
    	}else if(param1->type == tkn_DOUBLE || param1->type == tkn_REAL){
    		printf("%g", VT_GetDouble(param1->VT_index));
    	}else if(param1->type == tkn_INT || param1->type == tkn_NUM){
    		printf("%d", VT_GetInt(param1->VT_index));
    	}else if(param1->type == tkn_STRING || param1->type == tkn_LIT){
    		printf("%s", VT_GetStr(param1->VT_index)->str);
    	}else{
    		ERROR();
    	}
    	return NULL;
    }else if(STR_Compare2(fceNameP,"length") == 0){
    	S_String * frst = STR_Create("first");
    	t_Value * param1 = IL_GetVal(frst);
        return VT_AddInt(length(VT_GetStr(param1->VT_index)));
    }else if(STR_Compare2(fceNameP,"substr") == 0){
    	t_Value * param1 = IL_GetVal(frst);
    	t_Value * param2 = IL_GetVal(second);
    	t_Value * param3 = IL_GetVal(third);
        return VT_AddStr(substr(VT_GetStr(param1->VT_index), VT_GetInt(param2->VT_index), VT_GetInt(param3->VT_index)));
    }else if(STR_Compare2(fceNameP,"compare") == 0){
    	t_Value * param1 = IL_GetVal(frst);
    	t_Value * param2 = IL_GetVal(second);
        return VT_AddInt(compare(VT_GetStr(param1->VT_index), VT_GetStr(param2->VT_index)));
    }else if(STR_Compare2(fceNameP,"find") == 0){
    	t_Value * param1 = IL_GetVal(frst);
    	t_Value * param2 = IL_GetVal(second);
        return VT_AddInt(find(VT_GetStr(param1->VT_index), VT_GetStr(param2->VT_index)));
    }else if(STR_Compare2(fceNameP,"sort") == 0){
    	t_Value * param1 = IL_GetVal(frst);
        return VT_AddStr(sort(VT_GetStr(param1->VT_index)));
    }else{
        ERROR();
    }
    return NULL;
}

void CheckReturn(S_Fce * fce,t_Value * result){
    if(result == NULL){
        if(fce->returnType == tkn_VOID){
            return;
        }
        ERROR();
    }
    if(fce->returnType == tkn_BOOL){
        if(result->type != tkn_BOOL && result->type != tkn_TRUE && result->type != tkn_FALSE){
            ERROR();
        }
        return;
    }
    if(fce->returnType == tkn_INT){
        if(result->type != tkn_INT && result->type != tkn_NUM && result->type != tkn_REAL){
            ERROR();
        }
        return;
    }
    if(fce->returnType == tkn_DOUBLE){
        if(result->type != tkn_DOUBLE && result->type != tkn_NUM && result->type != tkn_REAL){
            ERROR();
        }
        return;
    }
    if(fce->returnType == tkn_STRING){
        if(result->type != tkn_STRING && result->type != tkn_LIT){
            ERROR();
        }
        return;
    }
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
        argNumber++;
        if(val == NULL){
            ERROR();
        }
    }
    if(noNextParam){
        ERROR();
    }else{
        FC_AddParamToTable(fce,val,argNumber); 
    }
}

bool FC_AddParamToTable(S_Fce * fce, t_Value * val,int argNumber){
    if(val == NULL){
        return -1;
    }
    IL_NestUp();
    int reachable = IL_GetReachable();
    IL_SetReachable(IL_GetNesting());
    bool result = IL_AllocParam(fce, val, argNumber);
    IL_NestDown();
    IL_SetReachable(reachable);
    return result;
}


t_Value * FceMat(){
    int * nestingLevel = MEM_malloc(sizeof(int));
    *(nestingLevel) = 0;
    bool * skipElse = MEM_malloc(sizeof(bool));
    while(true){
        switch(SCAN_GetToken()){
            case tkn_ID: tknID();  continue;
            case tkn_BOOL: DECLR(tkn_BOOL); continue;
            case tkn_INT: DECLR(tkn_INT); continue;
            case tkn_DOUBLE: DECLR(tkn_DOUBLE); continue;
            case tkn_STRING: DECLR(tkn_STRING); continue;
            case tkn_BREAK: BreakSt(nestingLevel); continue;
            case tkn_CONT: ContinueSt(nestingLevel); continue;

            case tkn_DO: cycleDo(nestingLevel); continue;
            case tkn_FOR: cycleFor(nestingLevel); continue;
            case tkn_WHILE: cycleWhile(nestingLevel); continue;

            case tkn_IF: IfSt(nestingLevel,skipElse); continue;
            case tkn_RET: return ReturnSt();
            case tkn_RBLOCK: if(RBool(nestingLevel))  return NULL; else continue;

            case tkn_ELSE:ElseSt(nestingLevel,skipElse); continue; //muze byt jelikoz vime ze syntax je spravny

            case tkn_SEMI: continue;
            default: ERROR();
        }
    }
}

t_Value * ReturnSt(){
    return PH_Solve(tkn_SEMI,-1, NULL, false);
}

void ElseSt(int * nestingLevel,bool * skipElse){
    if((*skipElse)){
        SCAN_FindToken(tkn_RBLOCK);
    }else{
        SCAN_GetToken();
        (*nestingLevel)++;
    }
}

void IfSt(int * nestingLevel,bool * skipElse){
    if(VT_GetBoolSafe(PH_Solve(tkn_LBLOCK, -1, NULL, false))){
        (*nestingLevel)++;
        (*skipElse) = true;

    }else{
        SCAN_FindToken(tkn_RBLOCK);
        (*skipElse) = false;
    }
}

bool RBool(int * nestingLevel){
    if((*nestingLevel) == 0){//konec fce
        return true;
    }
    if((*nestingLevel) != JL_GetNestingLevel()){
        (*nestingLevel)--;
        return false;
    }
    int type = JL_GetJumpType();

    if(type == doType){//do

        if(VT_GetBoolSafe(PH_Solve(tkn_RPAREN, -1, NULL, true))){

            FIO_MoveToPosition(JL_GetOffset());
        }else{
            (*nestingLevel)--;
            JL_Remove();
        }
    }else if(type == whileType){//while
        int position = FIO_GetPosition();
        FIO_MoveToPosition(JL_GetOffset());
        if(!(VT_GetBoolSafe(PH_Solve(tkn_LBLOCK, -1, NULL, false)))){
            (*nestingLevel)--;
            JL_Remove();
            FIO_MoveToPosition(position);
        }
    }else if(type == forType){//for
        int position = FIO_GetPosition();
        FIO_MoveToPosition(JL_GetOffset());
        SCAN_FindToken(tkn_SEMI);
        PH_Solve(tkn_RPAREN, -1, NULL, false);
        FIO_MoveToPosition(JL_GetOffset());
        if(!(VT_GetBoolSafe(PH_Solve(tkn_SEMI, -1, NULL, false)))){
            (*nestingLevel)--;
            JL_Remove();
            PH_DisposeTable(IL_GetReachable());
            FIO_MoveToPosition(position);
        }else{
            SCAN_FindToken(tkn_LBLOCK);
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
        SolveFce(ID,false);
    }else if(token == tkn_ASSIGN){
        
        EXPR_AddVal(tkn_ID,ID);
        STR_Dispose(ID);
        EXPR_AddVal(tkn_ASSIGN,NULL);
        PH_Solve(tkn_SEMI, -1, NULL, false);
    }
}

//DECLR
void DECLR(int type){
    int token = SCAN_GetToken();
    S_String * ID = STR_Create(SCAN_GetAttr()->str);
    IL_AllocVar(ID,type,false);
    token = SCAN_GetToken();
    if(token == tkn_ASSIGN){
        EXPR_AddVal(tkn_ID,ID);
        STR_Dispose(ID);
        EXPR_AddVal(tkn_ASSIGN,NULL);
        PH_Solve(tkn_SEMI, -1, NULL, false);
    }
}

//Cyccles
void cycleDo(int * nestingLevel){
    SCAN_GetToken();
    (*nestingLevel)++;
    MakeJump(doType,*nestingLevel);
}
void cycleWhile(int * nestingLevel){
    (*nestingLevel)++;
    MakeJump(whileType,*nestingLevel);
    if(!VT_GetBoolSafe(PH_Solve(tkn_LBLOCK, -1, NULL, false))){
        JL_Remove();
        SCAN_FindToken(tkn_RBLOCK);
        (*nestingLevel)--;
    }
    
}
void cycleFor(int * nestingLevel){
    //for(
    int token = SCAN_GetToken();
    //for("zde")
    token = SCAN_GetToken();
    if(token == tkn_BOOL || token == tkn_DOUBLE || token == tkn_INT || token == tkn_STRING){
        PH_AllocTable(IL_GetNesting()); //lokalni tabulka
        DECLR(token);
    }else{
        
        EXPR_AddVal(token, SCAN_GetAttr());
        PH_Solve(tkn_SEMI, -1, NULL, false);
    }
    
    (*nestingLevel)++;
    MakeJump(forType,*nestingLevel);
    //for("";"zde")
    
    bool foo = VT_GetBoolSafe(PH_Solve(tkn_SEMI, -1, NULL, false));
     //for("";"";"preskoci"){
    SCAN_FindToken(tkn_LBLOCK);
    if(!foo){
        JL_Remove();
        SCAN_FindToken(tkn_SEMI);
        (*nestingLevel)--;
    }

}

void MakeJump(int JumpType, int nestingLevel){
    JL_Add(FIO_GetPosition(),JumpType,nestingLevel);
}

void BreakSt(int * NestingLevel){
    int neccesaryNesting = JL_GetNestingLevel();
    while((*NestingLevel) > neccesaryNesting){
        SCAN_FindToken(tkn_RBLOCK);
        (*NestingLevel)--;
    }
    if(JL_GetJumpType() == forType){
    	PH_DisposeTable(IL_GetReachable());
    }
    SCAN_FindToken(tkn_RBLOCK);
    (*NestingLevel)--;
    JL_Remove();
}

void ContinueSt(int * nestingLevel){
    (*nestingLevel) = JL_GetNestingLevel();
    int type = JL_GetJumpType();
    long position = FIO_GetPosition();
    FIO_MoveToPosition(JL_GetOffset());
    if(type == 1){//while
        if(!(VT_GetBoolSafe(PH_Solve(tkn_RPAREN, -1, NULL, true)))){
            (*nestingLevel)--;
            JL_Remove();
            FIO_MoveToPosition(position);
        }else{
            SCAN_GetToken();
        }
    }else if(type == 2){//for
        
        if(!(VT_GetBoolSafe(PH_Solve(tkn_SEMI, -1, NULL, false)))){
            (*nestingLevel)--;
            JL_Remove();
            FIO_MoveToPosition(position);
        }else{
            PH_Solve(tkn_RPAREN, -1, NULL, false);
            SCAN_GetToken();
        }
    }
}



t_Value * PH_Solve(int EndToken1,int EndToken2, int * rEndToken, bool addEndToken){
    int token;
    S_String * ID;
    bool skipToken = false;
    int nesting = 0;
    if(EndToken1  == tkn_LPAREN || 
            EndToken1  == tkn_LBLOCK || 
            EndToken2  == tkn_LPAREN || 
            EndToken2  == tkn_LBLOCK){
        nesting--;
    }
    while(true)
    {
        if(!skipToken){
            token = SCAN_GetToken();
        }
        skipToken = false;
        if(token == tkn_LPAREN || token == tkn_LBLOCK){
            nesting++;
        } else if(token == tkn_RPAREN || token == tkn_RBLOCK){
            nesting--;
        }
        if(token == EndToken1 || token == EndToken2){
            if(rEndToken != NULL){
                (*rEndToken) = token;
            }
            if(addEndToken){
                EXPR_AddVal(token,NULL);
            }
            if(nesting <= 0){
                return EXPR_Solve();
            }  
        }

        if(token == tkn_ID){
            ID = STR_Create(SCAN_GetAttr()->str);
            token = SCAN_GetToken();
            if(token != tkn_LPAREN){
                skipToken = true;
                EXPR_AddVal(tkn_ID, ID);
                STR_Dispose(ID);
            }else{
                EXPR_AddVal2(SolveFce(ID,false));
            }
            continue;
        }
        EXPR_AddVal(token,SCAN_GetAttr());
    }
    return NULL;
}

int PH_checkValidToken(int token,bool canEq){
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
        token == tkn_NEQUAL ||
        token == tkn_EXCL ||
        token == tkn_AND ||
        token == tkn_OR ||
        token == tkn_EQUAL ||
        token == tkn_TRUE ||
        token == tkn_FALSE){
        return true;
    }
    if(canEq){
    	if(token == tkn_ASSIGN){
    		return true;
    	}
    }
    return false;
}

void PH_AllocTable(int reachable){
    IL_NestUp();   
    if(reachable == -1){
        IL_SetReachable(IL_GetNesting());
    }else{
        IL_SetReachable(reachable);
    }
    
}

void PH_MakeTree(){
    EXPR_Create();
}

void PH_DisposeTree(){
    EXPR_Dispose();
}

void PH_DisposeTable(int reachable){
    IL_RemoveNest();
    IL_NestDown();
    if(reachable == -1){
        IL_SetReachable(IL_GetNesting());
    }else{
        IL_SetReachable(reachable);
    }
}