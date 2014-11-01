#include "ast_eval.h"

void ast_eval(struct node *root, struct hashmap **mappings)
{
	struct ast_iterator *it = ast_iterator_init(root, POSTORDER);
	struct stack *stack = NULL;

	struct node *temp = NULL;
	while ((temp = ast_iterator_next(it))) {
        switch (temp.type){
            case N_ATOMIC:
                // if number push to stack
                // if identifier push mapped value to stack
                break;
            case N_NEGATION:
                // negate topmost number on stack

                break;
            case N_ADDITION:
                // pop two values from stack, add, push

                break;
            case N_MULTIPLICATION:
                // pop two values from stack, mult, push

                break;
            case N_SCALAR_DECLARATION:
                // pop value from stack, set (ident, value) in hashmap

                break;
        }
	}

}
