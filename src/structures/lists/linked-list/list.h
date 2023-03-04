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

// Initialize the sentinel of an empty list.
struct list *new_list();

// Return a new element 
struct list *new_element(void* data);

// Delete the element 
void remove_element(struct list *element);

// Add the element on the top of the list, just after the sentinel.
void add_top(struct list *list, struct list *element);

// Add the element at the end of the list.
void add_end(struct list *list, struct list *element);

// Add the node next to the element
void add_to(struct list *element, struct list *node);

// Swap two element in the list if they exists. 
// Otherwise, do nothing. 
void swap(struct list *element1, struct list *element2);

// Sort the list. 
void sort(struct list *list);

// Sort the list in descending order.
void sort_reverse(struct list *list);

// Remove the element at index
// If index is out of range, do nothing
void remove_at(struct list *list, int index);

// Remove the last element of the list 
void remove_last(struct list *list);

// Remove the first element of the list
void remove_first(struct list *list);

// Remove the list 
void remove_list(struct list *list);

// Clear the list, keep only the sentinel
void clear(struct list **list);

// Insert the element at index, if index is out of range do nothing 
void insert_at(struct list *list, struct list *element, int index);

// Insert the element before the node 
void insert_before(struct list *list, struct list *node, struct list *element);

// Insert the element after the node 
void insert_after(struct list *list, struct list *node, struct list *element);

// Return the element at index 
// If index is out of range return the last element
struct list *get_at(struct list *list, int index);

#endif

