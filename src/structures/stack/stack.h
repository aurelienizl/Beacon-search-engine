/*
 * Author: Izoulet Aurélien
 * Purpose: Stack Graph implementation 
 * Language: C.
 */

#ifndef _STACK_H_ 
#define _STACK_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
** @brief         Linked node structure.
** @param next    Next node element.
** @param prev    Prev node element.
** @param data    Represent the data stored in the node.
*/

struct node
{
    struct node *next;
    struct node *prev;
    void* data;
};

/**
** @brief         Stack structure.
** @param last    The element at the top of the stack. 
*/

struct stack
{
    struct node *last;
};

/**
** @brief         Initialise a new stack. 
** @return        A stack structure.
*/

struct stack *new_stack();

/**
** @brief         Check if a stack is empty. 
** @param stack   A stack pointer.
** @return        1 if true, 0 if false.
*/

int is_empty_stack(struct stack *stack);

/**
** @brief         Add element to the stack.
** @param stack   A stack pointer.
** @param data    Data to add to the stack.
** @note          The data needs to be allocated on the heap. 
** @return        Void.
*/

void addstack(struct stack *stack, void* data);

/**
** @brief         Delete an element to the stack and return it's data.  
** @param stack   A stack pointer.
** @return        The data of the last element from the stack element. 
** @note          The data of the element needs to be freed. 
*/

void* unstack(struct stack *stack);

/**
** @brief         Free all data from a stack structure.  
** @param stack   A stack pointer.
** @return        Void.
*/

void freestack(struct stack *stack);

/**
 * @brief Count the number of element in a stack.
 * @param stack 
 * @return int 
 */


int count_stack(struct stack *stack);


#endif