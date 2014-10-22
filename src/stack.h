#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

struct stack {
    void* head;
    struct stack *tail;
};

void stack_free(struct stack *stack);
void stack_push(struct stack **stack, void *elem);
void *stack_pop(struct stack **stack);

#endif /* end of include guard: STACK_H */
