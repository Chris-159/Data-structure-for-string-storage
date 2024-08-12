#pragma once
#ifndef STS_TYPES_H_INCLUDED
#define STS_TYPES_H_INCLUDED

#include "types.h"
#include "llist_type.h"

typedef struct sts_pfield STSPrivateField;

// struct for the data structure (string table store)
typedef struct sts {
    Node start_;                     // starting node of the chain
    STSPrivateField *private_field;  // the private field contains the number of tables and entries
} TABLE_STORE, *TableStore;

#endif // STS_TYPES_H_INCLUDED