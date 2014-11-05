#ifndef AST_EVAL_H
#define AST_EVAL_H

#include "ast.h"
#include "hashmap.h"
#include "stack.h"

struct ast_eval_result {
	enum alternative alternative;
	union {
		double number;
		struct {
			int compc;
			double compv[];
		} vector;
	};
};

void ast_eval(struct node *, struct hashmap **);

#endif				/* end of include guard: AST_EVAL_H */
