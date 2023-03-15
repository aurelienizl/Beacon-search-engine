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