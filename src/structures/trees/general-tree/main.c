/*
 * Author: Izoulet Aurélien
 * Purpose: Tree implementation
 * Language: C.
 */
#include "tree.h"

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
        return randomString;
    }
    return random_string_gen(length);

}

void print_node_ext(struct tree_node *head, int mark)
{
    if (head != NULL)
    {
        if (head->mark != mark)
        {
            head->mark = mark;
            printf("-------------------------\n");
            printf("Data : %s\n", head->data);
            printf("Depth: %i\n", head->depth);
            printf("Mark : %i\n", head->mark);
            printf("Connx: %li\n", head->cNodeSize);
            for (size_t i = 0; i < head->cNodeSize; i++)
            {
                print_node_ext((head->cNode)[i], mark);
            }
        }
    }
}

void print_tree(struct tree *graph)
{
    struct tree_node *current = graph->head;
    if (current->mark == 0)
    {
        print_node_ext(current, 1);
    }
    else
    {
        print_node_ext(current, 0);
    }
}

int main()
{
    //Hello World !
}