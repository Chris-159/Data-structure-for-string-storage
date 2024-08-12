#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "llist.h"

/// @brief Creates a node
/// @return [Node] -> upon successful allocation
Node NodeCreate() {
    Node node_ = calloc(1, sizeof(NODE));
    
    // initialize the node members
    node_->next_ = NODE_EMPTY;
    node_->prev_ = NODE_EMPTY;
    
    return node_;
}

/// @brief Deallocates the {node_}
/// @param node_ Node*, The node that will be deallocated
/// @return [-1] -> {node_} is NULL
///         [0] -> upon successful deallocation
STATUS_t NodeDelete(Node *node_) {
    if ((*node_) == NODE_EMPTY)
        return -1;
    
    Node node_next = (*node_)->next_, node_prev = (*node_)->prev_;
    free((*node_));
    (*node_) = node_next;

    if (node_prev != NODE_EMPTY && (*node_) != NODE_EMPTY)
        (*node_)->prev_ = node_prev;
    
    return 0;
}

/// @brief Deletes all the nodes from {start_}
/// @param start_ Node*, the starting node, where the chain will be cut off
void NodeDeleteAll(Node *start_) {    
    while ((*start_) != NODE_EMPTY)
        if (NodeDelete(start_) == -1)
            break;
}
