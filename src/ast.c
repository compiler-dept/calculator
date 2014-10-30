#include "ast.h"
#include "parser_state.h"
#include "../tests/test_helper.h"
#include <stdio.h>

char *AST_TYPE_NAMES[] =
    { "Atomic", "Primary_Expression", "Negation", "Multiplication",
	"Multiplicative_Expression", "Addition", "Additive_Expression",
	"Atomic_Expression", "Components", "Vector",
	"Vector_Primary_Expression", "Vector_Negation",
	"Vector_Scalar_Multiplication", "Vector_Addition",
	"Vector_Expression", "Scalar_Declaration", "Vector_Declaration",
	"Declaration", "Declaration_Sequence", "Translation_Unit"
};

struct node *node_next_sibling(struct node *node, struct node *parent)
{
	if (node && parent) {
		for (int i = parent->childc - 1; i > 0; i--) {
			if (parent->childv[i - 1] == node) {
				return parent->childv[i];
			}
		}
	}

	return NULL;
}

struct ast_iterator *ast_iterator_init(struct node *node,
				       enum iterator_type type)
{
	struct ast_iterator *iterator = malloc(sizeof(struct ast_iterator));
	iterator->type = type;
	iterator->current = node;
	iterator->stack = NULL;

	return iterator;
}

struct node *ast_iterator_next_postorder(struct ast_iterator *iterator)
{
	struct node *next = NULL;

	if (iterator->current) {
		while (iterator->current->childc > 0) {
			stack_push(&iterator->stack, iterator->current);
			iterator->current = iterator->current->childv[0];
		}

		next = iterator->current;

		iterator->current =
		    node_next_sibling(next, stack_peek(iterator->stack));

	} else {
		next = (struct node *)stack_pop(&iterator->stack);
		iterator->current =
		    node_next_sibling(next, stack_peek(iterator->stack));
	}

	return next;
}

struct node *ast_iterator_next(struct ast_iterator *iterator)
{
	switch (iterator->type) {
	case POSTORDER:
		return ast_iterator_next_postorder(iterator);
		break;
	}
}

void ast_free(struct node *root)
{
	struct ast_iterator *it = ast_iterator_init(root, POSTORDER);
	struct stack *stack = NULL;

	struct node *temp = NULL;

	while ((temp = ast_iterator_next(it))) {
		switch (temp->type) {
		case N_ATOMIC:
			if (temp->alternative == ALT_IDENTIFIER) {
				free((char *)temp->payload.atomic.identifier);
			}
			break;
		case N_SCALAR_DECLARATION:
			free((char *)temp->payload.scalar_declaration.
			     identifier);
			break;
		}
		free(temp->childv);
		free(temp);
	}

	free(it);
}
