#ifndef NODE_H
#define NODE_H

#include "STR.h"
#include "HASHVAR.h"
#include "VARTAB.h"

#include <stdbool.h>

/* Uzel stromu - jedna rodicovska vetev, dva potomci */
typedef struct t_Node t_Node;
struct t_Node{
	t_Value *value;
	t_Node *parent;
	t_Node *l_child;
	t_Node *r_child;
};

/* Vraci true v pripade, ze je node operator */
bool Node_IsOp(t_Node *n);

/* Vraci true v pripade, ze node nema ani jednoho potomka */
bool Node_IsBottom(t_Node *n);

/* Vraci true v pripade, ze ma node jen praveho potomka */
bool Node_IsUnary(t_Node *n);

/* Vraci true v pripade, ze je node leva zavorka */
bool Node_IsLParen(t_Node *n);

/* Vraci hodnotu node */
t_Value *Node_GetValue(t_Node *n);

/* Vraci typ (token) node */
int Node_GetType(t_Node *n);

/* Vraci index do VARTAB node */
int Node_GetIndex(t_Node *n);

/* Vraci hodnotu node z VARTAB integeru */
int Node_GetInt(t_Node *n);

/* Vraci hodnotu node z VARTAB doublu */
double Node_GetDouble(t_Node *n);

/* Vraci hodnotu node z VARTAB stringu */
S_String *Node_GetStr(t_Node *n);

/* Vraci leveho potomka node */
t_Node *Node_GetLChild(t_Node *n);

/* Vraci praveho potomka node */
t_Node *Node_GetRChild(t_Node *n);

/* Vraci rodice node */
t_Node *Node_GetParent(t_Node *n);

#endif