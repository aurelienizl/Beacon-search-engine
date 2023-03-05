/*
 * Author: Izoulet Aurélien
 * Purpose: Linked list implementation 
 * Language: C.
 */

#ifndef _LIST_H_ 
#define _LIST_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

/**
** @brief         Linked list structure.
** @param next    Next list element.
** @param data    Represent the data stored in the node.
*/
struct list
{
    struct list *next;
    void* data;
};


// ----PROPERTIES---- //

/**
** @brief         Check if a list is empty.
** @param list    A list pointer
** @return       1 if true, 0 if false.
*/

int is_empty(struct list *list);

/**
** @brief         Check if a node with data already exists in the list.
** @param list    A list pointer.
** @param data    A data to compare.
** @return        1 if true, 0 if false.
*/

int exists(struct list *list, void* data);

/**
** @brief         Check if a list is sorted. 
** @param list    A list pointer.
** @return        1 if true, 0 if false.
*/

int is_sorted(struct list *list);

/**
** @brief         Return the size of the list. 
** @param list    A list pointer.
** @return        The number of nodes (int).
*/

int count(struct list *list);

// ----OPERATIONS---- //

/**
 * @brief Create a new list
 * @return struct list* 
 */

struct list *new_list();

/**
 * @brief Create a new element
 * @param data 
 * @return struct list* 
 */

struct list *new_element(void* data);

/**
 * @brief Remove an element from the list
 * @param element 
 */

void remove_element(struct list *element);

/**
 * @brief Add an element to the top of the list
 * @param list 
 * @param element 
 */

void add_top(struct list *list, struct list *element);

/**
 * @brief Add an element to the end of the list
 * @param list 
 * @param element 
 */

void add_end(struct list *list, struct list *element);

/**
 * @brief Add an element before the node
 * @param element 
 * @param node 
 */

void add_to(struct list *element, struct list *node);

/**
 * @brief Swap two elements in the list
 * @param element1 
 * @param element2 
 */

void swap(struct list *element1, struct list *element2);

/**
 * @brief Sort the list
 * @param list 
 */

void sort(struct list *list);

/**
 * @brief Sort the list in reverse order
 * @param list 
 */

void sort_reverse(struct list *list);

/**
 * @brief Remove an element at the given index
 * @param list 
 * @param index
 */
void remove_at(struct list *list, int index);

/**
 * @brief Remove the last element of the list
 * @param list 
 */

void remove_last(struct list *list);

/**
 * @brief Remove the first element of the list
 * @param list 
 */

void remove_first(struct list *list);

/**
 * @brief Remove the list
 * @param list 
 */

void remove_list(struct list *list);

/**
 * @brief Clear the list
 * @param list 
 */

void clear(struct list **list);

/**
 * @brief Insert an element at the given index
 * @param list 
 * @param element
 * @param index
 */

void insert_at(struct list *list, struct list *element, int index);

/**
 * @brief Insert an element before the given node
 * @param list 
 * @param node 
 * @param element 
 */

void insert_before(struct list *list, struct list *node, struct list *element);

/**
 * @brief Insert an element after the given node
 * @param list 
 * @param node 
 * @param element 
 */

void insert_after(struct list *list, struct list *node, struct list *element);

/**
 * @brief Get the element at the given index
 * @param list 
 * @param index
 * @return struct list* 
 */

struct list *get_at(struct list *list, int index);

#endif

