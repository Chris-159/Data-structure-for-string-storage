#ifndef WORD_TYPE_H_INCLUDED
#define WORD_TYPE_H_INCLUDED

#include "types.h"

#define WORD_MAX_LENGTH        30       // max length of each stored entry (string)

// struct to store each table entry (word data)
typedef struct wordd {
    ULONG_t word_index;             // the calculated index of the entry
    ULONG_t ref;                    // the number of reference to the current entry
    boolean_t taken;                // the indicator if the current element was referenced or not
    char word[WORD_MAX_LENGTH];     // the entry data type (string)
} WORD_TABLE_DATA, *WordTableData;

#endif // WORD_TYPE_H_INCLUDED