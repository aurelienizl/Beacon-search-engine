/*
 * Author: Izoulet Aurélien
 * Purpose: Stack implementation 
 * Language: C.
 */

#include "stack.h"

int is_empty_stack(struct stack *stack)
{
    if(stack->last == NULL)
    {
        return 1;
    }
    return 0;
}

int count_stack(struct stack *stack)
{
    int count = 0;
    struct node *current = stack->last;
    while(current != NULL)
    {
        count++;
        current = current->prev;
    }
    return count;
}