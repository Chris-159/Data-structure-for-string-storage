#pragma once
#ifndef LLIST_TYPES_H_INCLUDED
#define LLIST_TYPES_H_INCLUDED

#include "word_type.h"

#define NODE_TABLE_DEF_SIZE  1000       // default size of the entry table
#define NODE_EMPTY           NULL

// struct to store each node in the data structure (node)
typedef struct nd {
    void *next_;    // pointer to the NEXT element
    void *prev_;    // pointer to the PREVIOUS element
    WORD_TABLE_DATA word_table[NODE_TABLE_DEF_SIZE];     // table of entries
} NODE, *Node;

#endif // LLIST_TYPES_H_INCLUDED