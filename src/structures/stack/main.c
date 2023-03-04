/*
 * Author: Izoulet Aurélien
 * Purpose: Oriented Graph implementation
 * Language: C.
 */

#include "stack.h"

char *random_string_gen(size_t length)
{

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!"; // could be const
    char *randomString;

    if (length)
    {
        randomString = malloc(length + 1);
        if (randomString)
        {
            int l = (int)(sizeof(charset) - 1);
            int key;
            for (size_t n = 0; n < length; n++)
            {
                key = rand() % l;
                randomString[n] = charset[key];
            }
            randomString[length] = '\0';
        }
    }

    return randomString;
}

void print_stack(struct stack *stack)
{
    printf("STACK : \n");
    struct list *last = stack->last;
    while (last != NULL)
    {
        printf("NEW NODE WITH : %s\n", (char*) last->data);
        last = last->prev;
    }
}

int main()
{

    struct stack *stack = new_stack();

    addstack(stack, random_string_gen(10));
    addstack(stack, random_string_gen(10));
    addstack(stack, random_string_gen(10));

    char* data = unstack(stack);
    free(data);

    print_stack(stack);
    freestack(stack);
    return 0;
}   