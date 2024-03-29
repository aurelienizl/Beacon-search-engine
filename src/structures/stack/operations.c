/*
 * Author: Izoulet Aurélien
 * Purpose: Stack implementation 
 * Language: C.
 */

#include "stack.h"

struct stack *new_stack()
{
    struct stack *stack = calloc(1, sizeof(struct stack));
    stack->last = NULL;
    return stack;
}

void addstack(struct stack *stack, void* data)
{
    struct node *top = stack->last;
    if(top == NULL)
    {
        top = calloc(1, sizeof(struct node));
        top->data = data;
        top->next = NULL;
        top->prev = NULL;
        stack->last = top;
    }
    else
    {
        struct node *new = calloc(1, sizeof(struct node));
        new->data = data;
        new->prev = top;
        new->next = NULL;
        stack->last = new;
    }
}

void* unstack(struct stack *stack)
{
    struct node *last = stack->last;
    if (last == NULL)
    {
        return NULL;
    }
    else
    {
        void *data = last->data;
        stack->last = stack->last->prev;
        free(last);
        return data;
    }
}

void freestack(struct stack *stack)
{
    struct node *top = stack->last;
    while(top != NULL)
    {
        struct node *tmp = top->prev;
        free(top->data);
        free(top);
        top = tmp;
    }
    free(stack);
}
