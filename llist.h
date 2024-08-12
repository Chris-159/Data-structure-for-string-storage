#pragma once
#ifndef LLIST_H_INCLUDED
#define LLIST_H_INCLUDED

#include "llist_type.h"

Node NodeCreate();
STATUS_t NodeDelete(Node *node_);
void NodeDeleteAll(Node *start_);

//     ***Macro to calculate the hash of the current word***
#define _NODE_HASH_WORD(word__, wrd_length__, hash_out__)       \
            for (size_t i__ = 0; i__ < (wrd_length__); i__++)   \
                (hash_out__) += (word__[i__] * (i__ + 1))

#endif // LLIST_H_INCLUDED