#ifndef AST_EVAL_H
#define AST_EVAL_H

#include "ast.h"
#include "hashmap.h"
#include "stack.h"

void ast_eval(struct node *, struct hashmap **);

#endif				/* end of include guard: AST_EVAL_H */
