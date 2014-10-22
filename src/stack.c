#include "stack.h"

struct stack *stack_alloc(){
    struct stack *stack = malloc(sizeof(struct stack));
    stack->head = NULL;
    stack->tail = NULL;
    return stack;
}

void stack_free(struct stack *stack){
    if (stack->tail){
        stack_free(stack->tail);
    }
    free(stack);
}

void stack_push(struct stack **stack, void *elem){
    if (!(*stack)->head){
        (*stack)->head = elem;
    } else {
        struct stack *newhead = stack_alloc();
        (*stack)->head = elem;
        (*stack)->tail = *stack;
        *stack = newhead;
    }
}

void *stack_pop(struct stack **stack){
    void *elem = (*stack)->head;
    if ((*stack)->tail){
        struct stack *temp = *stack;
        *stack = (*stack)->tail;
        free(temp);
    } else {
        (*stack)->head = NULL;
    }
    return elem;
}
