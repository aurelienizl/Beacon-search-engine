/*
 * Author: Izoulet Aurélien
 * Purpose: Tree implementation
 * Language: C.
 */

#include "tree.h"

/**
** @brief               Free node memory.
** @param node          Represent the node.
** @return              Void.
*/

void destroy_node(struct tree_node *node)
{
    if (node != NULL)
    {
        free(node->data);
        {
            if (node->cNode != NULL)
            {
                free(node->cNode);
            }
        }
        free(node);
    }
}

/**
** @brief               Extension of : destroy_tree.
** @details             Recursively delete all nodes from the tree.
** @param head          Represent the head node.
** @param mark          Represent the mark vector.
** @return              Void.
*/

void destroy_graph_ext(struct tree_node *head, int mark)
{
    if (head != NULL)
    {
        if (head->mark != mark)
        {
            head->mark = mark;
            for (size_t i = 0; i < head->cNodeSize; i++)
            {
                struct tree_node *tmp = head->cNode[i];
                destroy_graph_ext(tmp, mark);
            }
            destroy_node(head);
        }
    }
}

/**
** @brief               Free all node on a tree.
** @param head          Represent the head node.
** @return              Void.
*/

void destroy_tree(struct tree *head)
{
    struct tree_node *current = head->head;
    int mark = current->mark;
    if (mark == 0)
    {
        destroy_graph_ext(current, 1);
    }
    else
    {
        destroy_graph_ext(current, 0);
    }
    free(head);
}

/**
** @brief               Free node memory.
** @param depth         Represent the current depth of the node. 
** @param data          Represent the data stored in the new node.     
** @note                The data need to be allocated on the heap.                          
** @return              Void.
*/

struct tree_node *new_node(int depth, char *data)
{
    struct tree_node *new_node = malloc(sizeof(struct tree_node));

    if (new_node == NULL)
    {
        errx(1, "new_node: malloc error");
    }

    new_node->depth = depth;
    new_node->data = data;
    new_node->cNode = NULL;
    new_node->cNodeSize = 0;
    new_node->mark = 0;

    return new_node;
}

/**
** @brief               Create a new tree.
** @param id          Represent the tree id.
** @return              A new tree.
*/

struct tree *new_tree(int id)
{
    struct tree *new = malloc(sizeof(struct tree));
    if (new == NULL)
    {
        errx(1, "new_tree: malloc error");
    }

    new->head = NULL;
    new->id = id;

    return new;
}

/**
** @brief               Create a new node origin to the children list of the node target.
** @note                This method will override the depth parameter of the origin node.
** @param target        Represent the node a node that will receive the node to add.
** @param origin        Represent the node to add.
** @return              Return true if the node is successfully added, otherwise false.
*/

void add_to_children(struct tree_node *target, struct tree_node *origin)
{
    if (target->cNode == NULL)
    {
        target->cNode = malloc(sizeof(struct tree_node));
        if (target->cNode == NULL)
        {
            errx(1, "add_to_children: unable to allocate memory");
        }
        target->cNode[target->cNodeSize] = origin;
        target->cNodeSize = 1;
        origin->depth = target->depth + 1;
    }
    else
    {
        struct tree_node **tmp = realloc(target->cNode, sizeof(struct tree_node) * (target->cNodeSize + 1));
        if (target->cNode == NULL)
        {
            errx(1, "add_to_children: unable to reallocate memory");
        }
        target->cNode = tmp;
        target->cNode[target->cNodeSize] = origin;
        target->cNodeSize++;
        origin->depth = target->depth + 1;
    }
}
