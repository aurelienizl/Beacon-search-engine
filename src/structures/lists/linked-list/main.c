/*
 * Author: Izoulet Aurélien
 * Purpose: Linked list implementation 
 * Language: C.
 */
#include "list.h"

/**
** @brief         Print the data of all nodes in the standard output. 
** @param list    A list pointer, the head.
** @return        Void
*/

void print_list(struct list *list)
{
    if(list == NULL)
    {
        printf("[NULL]");
        return;
    }
    printf("[%s] ",(char*) list->data);
    list = list->next;
    while(list != NULL)
    {
        if(list->next != NULL)
        {
            printf(" [%s]->[%s] ",(char*) list->data,(char*) list->next->data);
        }
        else
        {
            printf(" [%s] ", (char*) list->data);
        }
        list = list->next;
    }
    printf(" [NULL] \n");
}

/**
** @brief         Add n element with random data to the list
** @param list    The node to add elements 
** @param n       The number of nodes to add 
** @return        Void
*/

int main()
{

}

