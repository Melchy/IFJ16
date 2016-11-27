/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#include "JL.h"

static S_JMP * active = NULL;

	void JL_Add(long Offset, int JumpType, int NestingLevel){
		S_JMP * new = MEM_malloc(sizeof(S_JMP));
		new->Offset = Offset;
		new->JumpType = JumpType;
		new->NestingLevel = NestingLevel;
		new->prev = active;
		active = new;
	}

	void JL_Remove(){
		if(active != NULL){
			S_JMP * foo = active;
			active = active->prev;
			MEM_free(foo);
		}
	}
	S_JMP * JL_Get(){
		return active;
	}
	long JL_GetOffset(){
		if(active == NULL){
			return -10;
		}
		return active->Offset;
	}

	int JL_GetJumpType(){
		if(active == NULL){
			return -10;
		}
		return active->JumpType;
	}

	int JL_GetNestingLevel(){
		if(active == NULL){
			return -10;
		}
		return active->NestingLevel;
	}