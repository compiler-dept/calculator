#include "stack.h"

void stack_free(struct stack *stack){
    if (stack->tail){
        stack_free(stack->tail);
    }
    free(stack);
}

void stack_push(struct stack **stack, void *elem){
    struct stack *newhead = stack_alloc(elem);
    newhead->tail = *stack;
    *stack = newhead;
}

void *stack_pop(struct stack **stack){
    void *elem = (*stack)->head;
    struct stack *temp = *stack;
    *stack = (*stack)->tail;
    free(temp);
    return elem;
}
