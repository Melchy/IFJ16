#include "JumpLogic.h"

  void JL_Add(int Offset, bool ExprAtEnd, int StateAfterJump,int NestingLevel, S_String * ClassAfterJMP){
  	S_JMP * new = malloc(sizeof(S_JMP));
  	if(new == NULL){
  		return;
  	}
  	new->Offset = Offset;
  	new->ExprAtEnd = ExprAtEnd;
  	new->NestingLevel = NestingLevel;
  	new->ClassAfterJMP = ClassAfterJMP;
  	new->prev = active;
  	new->StateAfterJump = StateAfterJump;
  	active = new;
  }
  void JL_Remove(){
  	if(active != NULL){
  		S_JMP * foo = active;
	  	active = active->prev;
	  	free(foo);
  	}
  }
  bool JL_GetExprAtEnd(){
  	return active->ExprAtEnd;
  }
  int JL_GetStateAfterJump(){
  	return active->StateAfterJump;
  }
  int JL_GetOffset(){
  	return active->Offset;
  }
  int JL_GetNestingLevel(){
  	return active->NestingLevel;
  }
  S_String * JL_GetClassAfterJMP(){
  	return active->ClassAfterJMP;
  }

  void JL_print(){
  	S_JMP * foo = active;
  	printf("vypisuje od aktivniho prvku po prvni\n");
  	while(foo != NULL){
  		printf("Offset:%d,ExprAtEnd:%d,NestingLevel:%d,StateAfterJump:%d,ClassAfterJMP:%s\n",
  			foo->Offset,foo->ExprAtEnd,foo->NestingLevel,foo->StateAfterJump,foo->ClassAfterJMP->str);
  		foo = foo->prev;
  	}
  }