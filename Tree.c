#include "Tree.h"
#include "SCAN.h"
#include "MEM.h"
#include "STR.h"

#include <stdio.h>

static t_Tree *T = NULL;

void Tree_Create()
{
	t_Tree *tmp = T;
	T = MEM_malloc(sizeof(t_Tree));
	T->prev = tmp;
	T->Top = NULL;
	T->Act = NULL;
}

static int priority(int token)
{
	switch(token){
		case tkn_MUL:
		case tkn_DIV:
			return 5;
		case tkn_PLUS:
		case tkn_MINUS:
			return 4;
		case tkn_HIGHER:
		case tkn_LOWER:
		case tkn_EHIGHER:
		case tkn_ELOWER:
			return 3;
		case tkn_EQUAL:
		case tkn_NEQUAL:
			return 2;
		default: break;
	}
	return 0;
}

void Tree_AddOp(int token)
{
	t_Branch *b = MEM_malloc(sizeof(t_Branch));
	b->token = token;
	if(T->Top == NULL){
		T->Top = T->Act = b; 
		b->parent = b->l_child = b->r_child = NULL;
		return;
	}
	while(T->Act->parent != NULL && (priority(token) <= priority(T->Act->parent->token)) && T->Act->parent->token != tkn_LPAREN)
		T->Act = T->Act->parent;
	b->l_child = T->Act;
	b->r_child = NULL;
	b->parent = T->Act->parent;
	if(T->Act->parent != NULL)
		T->Act->parent->r_child = b;
	T->Act->parent = b;
	if(T->Act == T->Top)
		T->Top = b;
	T->Act = b;
}

void Tree_AddVal(int token, S_String *attr){
	t_Branch *b = MEM_malloc(sizeof(t_Branch));
	b->token = token; b->attr = attr;

	if(T->Top == NULL){
		T->Top = T->Act = b; 
		b->parent = b->l_child = b->r_child = NULL;
		return;
	} else {
		T->Act->r_child = b;
	}
	b->parent = T->Act;
	b->l_child = b->r_child = NULL;
	T->Act = b;
}

void Tree_NestIn()
{
	t_Branch *b = MEM_malloc(sizeof(t_Branch));
	b->token = tkn_LPAREN; b->attr = NULL;
	b->l_child = b->r_child = NULL;
	b->parent = T->Act;
	if(T->Top != NULL)
		T->Act->r_child = b;
	else
		T->Top = b;
	T->Act = b;
}

void Tree_NestOut()
{
	while(T->Act->token != tkn_LPAREN){
		T->Act = T->Act->parent;
	}
}

static void RecursiveRemove(t_Branch *B)
{
	if(B->token == tkn_LPAREN){
		if (B->parent != NULL)
		{
			if(B->parent->r_child == B){
				B->parent->r_child = B->r_child;
				if(B->r_child != NULL)
					B->r_child->parent = B->parent;
			}
			else{
				B->parent->l_child = B->r_child;
				if(B->r_child != NULL)
					B->r_child->parent = B->parent;
			}
		}
		else
		{
			T->Top = B->r_child;
			if(B->r_child != NULL)
				B->r_child->parent = NULL;
		}
	}
	if (B->l_child != NULL)
		RecursiveRemove(B->l_child);
	if (B->r_child != NULL)
		RecursiveRemove(B->r_child);
	if(B->token == tkn_LPAREN)
		MEM_free(B);
}

void Tree_RemoveParen()
{
	if(T->Top != NULL)
		RecursiveRemove(T->Top);
}

void Tree_GoTop()
{
	T->Act = T->Top;
}

void RecursiveDispose(t_Branch *B)
{
	if(B == NULL)
		return;
	RecursiveDispose(B->r_child);
	RecursiveDispose(B->l_child);
	MEM_free(B);
}

void Tree_Dispose()
{
	if(T == NULL)
		return;
	RecursiveDispose(T->Top);
	t_Tree *tmp = T;
	T = T->prev;
	MEM_free(tmp);
}

static void padding ( char ch, int n )
{
  for (int i = 0; i < n; i++ )
    putchar ( ch );
}

static void structure ( t_Branch *root, int level )
{
  if ( root == NULL ) {
    padding ( '\t', level );
    puts ( "~" );
  }
  else {
    structure ( root->r_child, level + 1 );
    padding ( '\t', level );
    printf ( "%d", root->token ); if(root->attr != NULL) printf("-%s", root->attr->str); putchar('\n');
    structure ( root->l_child, level + 1 );
  }
}

void Tree_Print()
{
	structure(T->Top, 0);
}