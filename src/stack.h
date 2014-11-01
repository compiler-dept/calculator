#ifndef STACK_H
#define STACK_H

struct stack {
	void *head;
	struct stack *tail;
};

void stack_push(struct stack **, void *);
void *stack_peek(struct stack *);
void *stack_pop(struct stack **);
void stack_free(struct stack **);

#endif /* end of include guard: STACK_H */
