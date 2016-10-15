
#include "JumpLogic.h"

int main(void)
{
	S_String * class = malloc(sizeof(S_String));
	class->str = malloc(sizeof(char )* 10);
	class->len = 10;
	class->str = "foo\0";

	JL_Add(2,true,5,6,class);

	S_String * class2 = malloc(sizeof(S_String));
	class2->str = malloc(sizeof(char )* 10);
	class2->len = 10;
	class2->str = "ahdfhd\0";

	JL_Add(6,false,1,2,class2);

	
	printf("Offset:%d,ExprAtEnd:%d,NestingLevel:%d,StateAfterJump:%d,ClassAfterJMP:%s\n",
  			JL_GetOffset(),JL_GetExprAtEnd(),JL_GetNestingLevel(),JL_GetStateAfterJump(),JL_GetClassAfterJMP()->str);
	return 0;
} 