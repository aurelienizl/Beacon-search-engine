/*
 * Author: Izoulet Aurélien.
 * Purpose: Tree implementation
 * Language: C.
 */

#ifndef _TREES_H_
#define _TREES_H_

#include <stdlib.h>
#include <stdio.h>
#include <err.h>

/**
** @brief                    Node structure.
** @param depth              Represent the eccentricity of the node from the root.
** @param data               Node data.
** @param cNode              children tree_node list.
** @param adjacencyListSize  Number of nodes in the children list.
*/

struct tree_node
{
    int depth;
    int mark;
    char *data;
    struct tree_node **cNode;
    size_t cNodeSize;
};

/**
** @brief               Represent a full tree.
** @param node          Represent the head.
** @param id            Represent the id. 
*/

struct tree
{
    struct tree_node *head;
    int id; 
};

/**
** @brief               Free node memory.
** @param node          Represent the node.
** @return              Void.
*/

void destroy_node(struct tree_node *node);

/**
** @brief               Free all node on a tree.
** @param node          Represent the head node.
** @return              Void.
*/

void destroy_tree(struct tree *head);

/**
** @brief               Create a new tree node.
** @param depth         Represent the eccentricity of the node from the root (the first is 0 by default).
** @param data          Represent the data to store in the node.
** @return              A new tree node.
*/

struct tree_node *new_node(int depth, char* data);

/**
** @brief               Create a new tree.
** @param rank          Represent the tree id.
** @return              A new tree.
*/

struct tree *new_tree(int id);

/**
** @brief               Create a new node origin to the adjacency list of the node target.
** @param target        Represent the node a node that will receive the node to add.  
** @param origin        Represent the node to add.
** @return              Return true if the node is successfully added, otherwise false.
*/

void add_to_children(struct tree_node *target, struct tree_node *origin);


#endif