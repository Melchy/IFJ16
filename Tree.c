#include "Tree.h"
#include "SCAN.h"
#include "MEM.h"
#include "STR.h"

#include <stdio.h>

t_Tree *Tree_Init()
{
	t_Tree *T = MEM_malloc(sizeof(t_Tree));
	T->Top = NULL;
	T->Act = NULL;
	return T;
}

int priority(int token)
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

void Tree_AddOp(t_Tree *T, int token)
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

void Tree_AddVal(t_Tree *T, int token, S_String *attr){
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

void Tree_NestIn(t_Tree *T)
{
	t_Branch *b = MEM_malloc(sizeof(t_Branch));
	b->token = tkn_LPAREN; b->attr = NULL;
	b->l_child = b->r_child = NULL;
	b->parent = T->Act;
	T->Act->r_child = b;
	T->Act = b;
}

void Tree_NestOut(t_Tree *T)
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
	}
	if (B->l_child != NULL)
		RecursiveRemove(B->l_child);
	if (B->r_child != NULL)
		RecursiveRemove(B->r_child);
}

void Tree_RemoveParen(t_Tree *T)
{
	if(T->Top != NULL)
		RecursiveRemove(T->Top);
}

void Tree_GoTop(t_Tree *T)
{
	T->Act = T->Top;
}

void Tree_GoBot(t_Tree *T)
{
	t_Branch* p = T->Top;
	while(p != NULL){
		if(p->r_child == NULL){
			if (p->l_child == NULL)
			{
				T->Act = p;
				return;
			} else {
				p = p->l_child;
			}
		} else {
			p = p->r_child;
		}
	}
}

void Tree_Dispose(t_Tree *T)
{
	Tree_GoBot(T);
	while(T->Act != NULL){
		if(T->Act->l_child != NULL)
			MEM_free(T->Act->l_child);
		if(T->Act->r_child != NULL)
			MEM_free(T->Act->r_child);
		T->Act = T->Act->parent;
	}
	T->Top = NULL;
}

void padding ( char ch, int n )
{
  for (int i = 0; i < n; i++ )
    putchar ( ch );
}

void structure ( t_Branch *root, int level )
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

void Tree_Print(t_Tree *T)
{
	structure(T->Top, 0);
}