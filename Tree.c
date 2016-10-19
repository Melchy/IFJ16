#include "Tree.h"
#include "Tokens.h"
#include "MEM.h"
#include "STR.h"
#include "Node.h"

#include <stdio.h>

static t_Tree *T = NULL;

void Tree_Create()
{
	t_Tree *tmp = T;
	T = MEM_malloc(sizeof(t_Tree));
	T->prev = tmp; // nastaveni ukazatele na predchozi strom
	T->Top = NULL;
	T->Act = NULL;
}

/* Vraci true, pokud je rodic uzlu unarni operator */
static bool isParentUnary(t_Node *n)
{
	if(n == T->Top)
		return false;
	if(n->parent->l_child == NULL && n->parent->token != tkn_LPAREN)
		return true;
	return false;
}

/* Vraci true, pokud je uzel prazdna zavorka */
static bool isEmptyParen(t_Node *n)
{
	return (n->token == tkn_LPAREN && n->r_child == NULL);
}

/* Vraci true, pokud je uzel plna zavorka */
static bool isFullParen(t_Node *n)
{
	return (n->token == tkn_LPAREN && n->r_child != NULL);
}

/* Vraci vyssi hodnotu pro operator s vyssi prioritou, pro ostatni uzly vraci specialni hodnoty, aby strom fungoval */
static int priority(int token)
{
	switch(token){
		case tkn_MUL:
		case tkn_DIV:
			return 5;
		case tkn_PLUS:
		case tkn_MINUS:
		case tkn_EXCL:
			return 4;
		case tkn_HIGHER:
		case tkn_LOWER:
		case tkn_EHIGHER:
		case tkn_ELOWER:
			return 3;
		case tkn_EQUAL:
		case tkn_NEQUAL:
			return 2;
		case tkn_AND:
		case tkn_OR:
			return 1;
		case tkn_LPAREN:
			return -100; // pro levou zavorku
		default: break;
	}
	return 100; // pro hodnoty (promenne, literaly)
}

static void createTop(t_Node *n) // vytvori vrchol stromu
{
	n->parent = n->l_child = n->r_child = NULL;
	T->Top = T->Act = n;
}

/*static void addLeft(t_Node *n) // prida uzel jako leveho potomka aktivniho uzlu
{
	if(T->Top == NULL){
		createTop(n); return;
	}
	n->parent = T->Act;
	n->l_child = T->Act->l_child;
	n->r_child = NULL;
	if(n->l_child != NULL)
		n->l_child->parent = n;
	T->Act->l_child = n;
	T->Act = n;
}*/

static void addRight(t_Node *n) // prida uzel jako praveho potomka aktivniho uzlu
{
	if(T->Top == NULL){
		createTop(n); return;
	}
	n->parent = T->Act;
	n->l_child = T->Act->r_child;
	n->r_child = NULL;
	if(n->l_child != NULL)
		n->l_child->parent = n;
	T->Act->r_child = n;
	T->Act = n;
}

static void addAbove(t_Node *n) // prida uzel jako rodice aktivniho uzlu
{
	if(T->Top == NULL){
		createTop(n); return;
	}
	n->parent = T->Act->parent;
	n->l_child = T->Act;
	n->r_child = NULL;
	if(T->Act->parent == NULL)
	{
		T->Top = n;
	} else {
		if(T->Act->parent->l_child == T->Act)
			T->Act->parent->l_child = n;
		else
			T->Act->parent->r_child = n;
	}
	T->Act->parent = n;
	T->Act = n;
}

static void lookupLowPrio(t_Node *n) // hleda (smerem nahoru) operator s nizsi nebo stejnou prioritou
{
	if(T->Act == NULL)
		return;
	while(T->Act != T->Top)
	{
		if (priority(T->Act->parent->token) < priority(n->token))
			return;
		T->Act = T->Act->parent;
	}
}

void Tree_AddOp(int token)
{
	t_Node *n = MEM_malloc(sizeof(t_Node));
	n->token = token;
	if(isEmptyParen(T->Act)){
		addRight(n); return;
	}
	if(isFullParen(T->Act)){
		lookupLowPrio(n); addAbove(n); return;
	}
	if(Node_IsOp(T->Act)){
		lookupLowPrio(n); addRight(n); return;
	}
	if(isParentUnary(T->Act)){
		T->Act = T->Act->parent;
		lookupLowPrio(n);
		addAbove(n); 
		return;
	}

	lookupLowPrio(n); addAbove(n); // default, nejbeznejsi operace - aktualni Node je hodnota
}

void Tree_AddVal(int token, S_String *attr){
	t_Node *n = MEM_malloc(sizeof(t_Node));
	n->token = token; n->attr = attr;
	addRight(n);
}

void Tree_NestIn()
{
	t_Node *n = MEM_malloc(sizeof(t_Node));
	n->token = tkn_LPAREN; n->attr = NULL;
	addRight(n);
}

void Tree_NestOut()
{
	do{
		T->Act = T->Act->parent;
	}
	while(T->Act->token != tkn_LPAREN);
	printf("nesting out %d\n", T->Act->token);
}

static void RecursiveRemove(t_Node *n)
{
	if(n->token == tkn_LPAREN) // pokud narazime na zavorku
	{
		if (n->parent != NULL)
		{
			// zavorka muze nyt levy i pravy potomek
			if(n->parent->r_child == n)
			{
				n->parent->r_child = n->r_child;
				if(n->r_child != NULL)
					n->r_child->parent = n->parent;
			}
			else
			{
				n->parent->l_child = n->r_child;
				if(n->r_child != NULL)
					n->r_child->parent = n->parent;
			}
		}
		else
		{
			T->Top = n->r_child;
			if(n->r_child != NULL)
				n->r_child->parent = NULL;
		}
	}
	if (n->l_child != NULL)
		RecursiveRemove(n->l_child);
	if (n->r_child != NULL)
		RecursiveRemove(n->r_child);
	if(n->token == tkn_LPAREN)
		MEM_free(n);
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

static void RecursiveDispose(t_Node *n)
{
	if(n == NULL)
		return;
	RecursiveDispose(n->r_child);
	RecursiveDispose(n->l_child);
	MEM_free(n);
}

void Tree_Dispose()
{
	if(T == NULL)
		return;
	if(T->Top != NULL)
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

static void structure ( t_Node *root, int level )
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

t_Node *Tree_GetTopNode(){
	return T->Top;
}
