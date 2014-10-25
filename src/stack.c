#include <stdlib.h>
#include "stack.h"

void stack_push(struct stack **stack, void *elem)
{
	struct stack *newhead = malloc(sizeof(struct stack));
	newhead->head = elem;
	newhead->tail = *stack;
	*stack = newhead;
}

void *stack_pop(struct stack **stack)
{
	if(*stack) {
		void *elem = (*stack)->head;
		struct stack *temp = *stack;
		*stack = (*stack)->tail;
		free(temp);
		return elem;
	}

	return NULL;
}

void stack_free(struct stack **stack)
{
	if(*stack) {
		if((*stack)->tail) {
			stack_free(&((*stack)->tail));
		}

		free(*stack);
		*stack = NULL;
	}
}
