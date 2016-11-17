#include "Pars.h"

int State = -1;

//PARSER
void PARS_Run(){
    //TODO nesting zavorek ve vyrazech v prvnim pruchodu
    //TODO neoveruje stejna jmena trid
    //TODO Expr check, do te doby nejde prirazeni
    //TODO Vestavene fce
    S_String * foo = STR_Create("Main");
    State = st_Start;
    FR_load();
    IL_SetClass(foo);
    foo->str = "Run";

    SolveFce(foo,false);
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
            case tkn_BOOL: FR_DECLR(); break;
            case tkn_INT: FR_DECLR(); break;
            case tkn_DOUBLE: FR_DECLR(); break;
            case tkn_STRING: FR_DECLR(); break;
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
}
void FR_IfSt(int * nestingLevel){
    Tree_Create();
    FR_checkExpr(tkn_LBLOCK,-1, NULL, false,false);
    (*nestingLevel)++;
}

void FR_ReturnSt(){
    Tree_Create();
    FR_checkExpr(tkn_SEMI,-1, NULL, false,true);
}

bool FR_RBool(int * nestingLevel,bool * canElse){
    bool JmpEmpty = FR_jmpEmpty();
    if((*nestingLevel) == 0){
        if(!JmpEmpty){
            FR_SynError();
        }
        State = st_Class;
        return true;
    }
    if(JmpEmpty){
        (*nestingLevel)--;
        (*canElse) = true;
    }else{
        int jmpNesting = JL_GetNestingLevel();
        if(JL_GetJumpType() == 3){ 
            Tree_Create();
            FR_checkExpr(tkn_RPAREN,-1, NULL, true,false);//"do" condition
        }
        if((*nestingLevel) == jmpNesting){
            (*nestingLevel)--;
            JL_Remove();
        }else if((*nestingLevel) > jmpNesting){
            (*canElse) = true;
            (*nestingLevel)--;
        }else{
            FR_SynError(); //divny error
        }
    }
    return false;
}
void FR_cycleWhile(int * nestingLevel){
    checkIfNextTokenIsSmt(tkn_LPAREN);
    Tree_Create();
    FR_checkExpr(tkn_RPAREN,-1, NULL, false,false);
    checkIfNextTokenIsSmt(tkn_LBLOCK);
    (*nestingLevel)++;
    JL_Add(0, 1, (*nestingLevel));
}
void FR_cycleFor(int * nestingLevel){
    checkIfNextTokenIsSmt(tkn_LPAREN);
    int token = SCAN_GetToken();
    //for("zde")
    if(FR_checkVarType(token)){
        FR_DECLR();
    }else{
        Tree_Create();
        EXPR_AddVal(token, SCAN_GetAttr());
        FR_checkExpr(tkn_SEMI,-1, NULL, false,false);
    }
    Tree_Create();
    //for("";"zde")
    FR_checkExpr(tkn_SEMI,-1, NULL, false,false);
     //for("";"";"zde")
    Tree_Create();
    FR_checkExpr(tkn_RPAREN,-1, NULL, false,false);
    //for("";"";"zde"){
    checkIfNextTokenIsSmt(tkn_LBLOCK);
    (*nestingLevel)++;
    JL_Add(0, 2, (*nestingLevel));
}
void FR_cycleDo(int * nestingLevel){
    checkIfNextTokenIsSmt(tkn_LBLOCK);
    (*nestingLevel)++;
    JL_Add(0, 3, (*nestingLevel));
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

void FR_DECLR(){
    checkIfNextTokenIsSmt(tkn_ID);
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
        Tree_Create();
        S_String * empty = STR_Create("");
        EXPR_AddVal2(empty);
        MEM_free(empty);
        EXPR_AddVal(tkn_ASSIGN,NULL);
        FR_checkExpr(tkn_SEMI,-1, NULL, false,false);
    }else{
        if(token == tkn_SEMI){
            return;
        }
        FR_SynError();
    }
}

void FR_checkExpr(int EndToken1,int EndToken2, int * rEndToken, bool addEndToken,bool canEmpty){
    int token;
    bool skipToken = false;
    bool first = true;
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
            if(first == true && canEmpty == true){
                return;
            }
            if(EXPR_Solve() == NULL){//TODO EXPR CHECK
                FR_SynError();
            }

            return; 
        }
        first = false;
        if(!PH_checkValidToken(token)){
            FR_SynError();
        }
        if(token == tkn_ID){
            token = SCAN_GetToken();
            if(token != tkn_LPAREN){
                S_String * foo = STR_Create("");
                EXPR_AddVal2(foo);
                MEM_free(foo);
                skipToken = true;
            }else{
                FR_FceParamsSyntax();
                EXPR_AddVal2(VT_GetEmpty(tkn_INT));
            }
            continue;
        }
        EXPR_AddVal(token,SCAN_GetAttr());
    }
}

void FR_FceParamsSyntax(){
    int * token = MEM_malloc(sizeof(int));
    while(true){
        Tree_Create();
        FR_checkExpr(tkn_COMMA,tkn_RPAREN, token, false,true);
        if((*token) == tkn_RPAREN){
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
    IL_SetClass(SCAN_GetAttr());
    checkIfNextTokenIsSmt(tkn_LBLOCK);

    State = st_Class;
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
    MEM_free(ID);
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

        if(!FR_checkVarType(token)) FR_SynError();

        param->type = token;
        if((token = SCAN_GetToken()) != tkn_ID) FR_SynError();
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
    }
    if(SCAN_GetToken() != tkn_LBLOCK) FR_SynError();
    IL_InitFce(ID,type, FIO_GetPosition(),firstParam);
    State = st_InFce;
}
void FR_DeclrVarGlobal(S_String * ID,int type,int token){
    IL_AllocVar(ID, type,true);
    if(token == tkn_ASSIGN){
        Tree_Create();
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
        PH_checkValidToken(token);
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
    printf("%s\n", "syntax good");
    int returnPos;
    S_String * class = STR_Create(IL_GetClass()->str);
    
    //Set new CLass
    S_String * newClass;
    newClass = STR_GetBefore(ID, '.');
    if(newClass == NULL){
        newClass = STR_Create(IL_GetClass()->str);
    }
    IL_SetClass(newClass);
    //end

    S_Fce * fce = IL_GetFce(ID);

    FC_Call();
    if(run){
        FC_LoadArgs(fce);
        SCAN_GetToken(); //strednik
    }
    returnPos = FIO_GetPosition();
    FIO_MoveToPosition(fce->offset);
    t_Value * result = FceMat();
    CheckReturn(fce,result);
    FIO_MoveToPosition(returnPos);
    IL_SetClass(class);
    MEM_free(class);
    MEM_free(newClass);
    return result;
}

void CheckReturn(S_Fce * fce,t_Value * result){
    if(result == NULL){
        if(fce->returnType == tkn_VOID){
            return;
        }
        ERROR();
    }
    if(fce->returnType == tkn_BOOL){
        if(result->type != tkn_TRUE && result->type != tkn_FALSE){
            ERROR();
        }
        return;
    }
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

    EXPR_Create();
    val = PH_Solve(tkn_RPAREN,tkn_COMMA,endToken,false);

    while((*endToken) != tkn_RPAREN){
        if(noNextParam){
            ERROR();
        }else{
            noNextParam = FC_AddParamToTable(fce,val,argNumber);
        }
        EXPR_Create();
        val = PH_Solve(tkn_RPAREN,tkn_COMMA,endToken,false);
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
    return IL_AllocParam(fce, val, argNumber);
}


t_Value * FceMat(){
    int * nestingLevel = MEM_malloc(sizeof(int));
    *(nestingLevel) = 0;
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

            case tkn_IF: IfSt(nestingLevel); continue;
            case tkn_RET: return ReturnSt();
            case tkn_RBLOCK: if(RBool(nestingLevel))  return NULL; else continue;

            case tkn_ELSE:ElseSt(nestingLevel); continue; //muze byt jelikoz vime ze syntax je spravny

            case tkn_SEMI: continue;
            default: ERROR();
        }
    }
}

t_Value * ReturnSt(){

    EXPR_Create();
    return PH_Solve(tkn_SEMI,-1, NULL, false);
}

void ElseSt(int * nestingLevel){
    SCAN_GetToken();
    (*nestingLevel)++;
}

void IfSt(int * nestingLevel){
    EXPR_Create();
    if(VT_GetBoolSafe(PH_Solve(tkn_LBLOCK, -1, NULL, false))){
        (*nestingLevel)++;
    }else{
        SCAN_FindToken(tkn_RBLOCK);
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

    if(type == 3){//do
        EXPR_Create();
        if(VT_GetBoolSafe(PH_Solve(tkn_RPAREN, -1, NULL, true))){
            FIO_MoveToPosition(JL_GetOffset());
        }else{
            (*nestingLevel)--;
            JL_Remove();
        }
    }else if(type == 1){//while
        int position = FIO_GetPosition();
        FIO_MoveToPosition(JL_GetOffset());
        if(!(VT_GetBoolSafe(PH_Solve(tkn_RPAREN, -1, NULL, true)))){
            (*nestingLevel)--;
            JL_Remove();
            FIO_MoveToPosition(position);
        }else{
            SCAN_GetToken();
        }
    }else if(type == 2){//for
        int position = FIO_GetPosition();
        FIO_MoveToPosition(JL_GetOffset());
        EXPR_Create();
        if(!(VT_GetBoolSafe(PH_Solve(tkn_SEMI, -1, NULL, false)))){
            (*nestingLevel)--;
            JL_Remove();
            FIO_MoveToPosition(position);
        }else{
            EXPR_Create();
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
        SolveFce(ID,true);
    }else if(token == tkn_ASSIGN){
        EXPR_Create();
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
        EXPR_Create();
        EXPR_AddVal(tkn_ID,ID);
        MEM_free(ID);
        EXPR_AddVal(tkn_ASSIGN,NULL);
        PH_Solve(tkn_SEMI, -1, NULL, false);
    }
}

//Cyccles
void cycleDo(int * nestingLevel){
    SCAN_GetToken();
    (*nestingLevel)++;
    MakeJump(3,*nestingLevel);
}
void cycleWhile(int * nestingLevel){
    (*nestingLevel)++;
    MakeJump(1,*nestingLevel);
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
        DECLR(token);
    }else{
        EXPR_Create();
        EXPR_AddVal(token, SCAN_GetAttr());
        PH_Solve(tkn_SEMI, -1, NULL, false);
    }
    
    (*nestingLevel)++;
    MakeJump(2,*nestingLevel);
    //for("";"zde")
    EXPR_Create();
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
    while(neccesaryNesting > (*NestingLevel)){
        SCAN_FindToken(tkn_RBLOCK);
        (*NestingLevel)--;
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
        EXPR_Create();
        if(!(VT_GetBoolSafe(PH_Solve(tkn_RPAREN, -1, NULL, true)))){
            (*nestingLevel)--;
            JL_Remove();
            FIO_MoveToPosition(position);
        }else{
            SCAN_GetToken();
        }
    }else if(type == 2){//for
        EXPR_Create();
        if(!(VT_GetBoolSafe(PH_Solve(tkn_SEMI, -1, NULL, false)))){
            (*nestingLevel)--;
            JL_Remove();
            FIO_MoveToPosition(position);
        }else{
            EXPR_Create();
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
    while(true)
    {
        if(!skipToken){
            token = SCAN_GetToken();
        }
        if(EndToken1  == tkn_RPAREN || 
            EndToken1  == tkn_RBLOCK || 
            EndToken2  == tkn_RPAREN || 
            EndToken2  == tkn_RBLOCK){
            nesting--;
        }
        if(EndToken1  == tkn_LPAREN || 
            EndToken1  == tkn_LBLOCK || 
            EndToken2  == tkn_LPAREN || 
            EndToken2  == tkn_LBLOCK){
            nesting++;
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
                return EXPR_Solve();
            }  
        }
        if(token == tkn_ID){
            ID = STR_Create(SCAN_GetAttr()->str);
            token = SCAN_GetToken();
            if(token != tkn_LPAREN){
                skipToken = true;
                EXPR_AddVal(tkn_ID, ID);
                MEM_free(ID);
            }else{
                EXPR_AddVal2(SolveFce(ID,true));
            }
            continue;
        }

        EXPR_AddVal(token,SCAN_GetAttr());
    }
    return NULL;
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
        token == tkn_ASSIGN ||
        token == tkn_TRUE ||
        token == tkn_FALSE){
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