#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "sts.h"
#include "llist.h"

// declaration of the private field
struct sts_pfield {
    ULONG_t table_count;        // the current number of tables that are represented in the data structure
    ULONG_t table_size;         // size of each table
};

/// @brief Creates the data structure
/// @param table_count ULONG_t, the number of tables that should be created at init
/// @return [NULL]    -> the allocation for the structure or the starting node failed;
///      [TableStore] -> upon successful allocation
TableStore TSCreate(ULONG_t table_count) {
    TableStore ts_obj = malloc(sizeof(TABLE_STORE));
    if (ts_obj == NULL)
        return ts_obj;
    
    ts_obj->start_ = NodeCreate();
    if (ts_obj->start_ == NODE_EMPTY) {
        free(ts_obj); ts_obj = NULL;
        return ts_obj;
    }

    if (table_count > 1 && table_count < 11) {
        Node node_start = ts_obj->start_;
        ULONG_t table_count_tmp = table_count;
        while (table_count_tmp > 0) {
            Node node_new = NodeCreate();
            if (node_new == NODE_EMPTY)
                break;

            node_start->next_ = node_new;            
            node_start->prev_ = node_start;
            node_start = node_start->next_;
            table_count_tmp--;
        }
    } else
        table_count = 1;
    
    ts_obj->private_field = malloc(sizeof(STSPrivateField));
    if (ts_obj->private_field == NULL) {
        NodeDelete(&(ts_obj->start_));
        free(ts_obj); ts_obj = NULL;
        return ts_obj;
    }
    
    ts_obj->private_field->table_count = table_count; // 1
    ts_obj->private_field->table_size = NODE_TABLE_DEF_SIZE;

    return ts_obj;
}

/// @brief Deallocate the data structure
/// @param ts_obj TableStore*, the data structure that should be deallocated
/// @return [-1] -> {ts_obj} is NULL;
///         [0] -> upon successful deallocation
STATUS_t TSDestroy(TableStore *ts_obj) {
    if ((*ts_obj) == NULL)
        return -1;

    // if the PRIVATE FIELD is not NULL, then dealloc
    if ((*ts_obj)->private_field != NULL) {
        free((*ts_obj)->private_field); (*ts_obj)->private_field = NULL;
    }

    // if the STARTING NODE is not NULL, then dealloc
    if ((*ts_obj)->start_ != NODE_EMPTY)
        NodeDeleteAll(&((*ts_obj)->start_));
    
    // dealloc the whole structure
    free((*ts_obj)); (*ts_obj) = NULL;
    return 0;
}

/// @brief Gets the size of the storage tables for {ts_obj} 
/// @param ts_obj TableStore, the data structure for info checking
/// @return [0] -> {ts_obj} is NULL;
///   [Size of tables] -> retreived table size
ULONG_t TSGetTableSize(TableStore ts_obj) {
    if (ts_obj == NULL)
        return 0;
    
    return ts_obj->private_field->table_size;
}

/// @brief Gets the current number of tables that is represented in the {ts_obj} data structure
/// @param ts_obj TableStore, the data structure for info checking
/// @return [0] -> {ts_obj} is NULL;
///   [Count of tables] -> retreived table count
ULONG_t TSGetTableCount(TableStore ts_obj) {
    if (ts_obj == NULL)
        return 0;
    
    return ts_obj->private_field->table_count;
}

/// @brief Saves the {word_} or increases the reference count if it is already represented in the {ts_obj} structure.
///        The index of the entry is calculated with an algorithm (we can call it hash).
///        The macro (where the algorithm is located) is placed in the |llist.h| header. 
/// @param ts_obj TableStore, the data structure where {word_} will be saved (or its reference count increased)
/// @param word_ char*, the string that will be saved (or its reference count increased)
/// @return [-1] -> {word_} or {ts_obj} is NULL;
///         [-2] -> the node, where the {word_} should be is NULL;
///         [-3] -> new node allocation failed, where the {word_} should be saved
STATUS_t TSSearchAndSaveEntry(TableStore ts_obj, char *word_) {
    if (word_ == NULL || ts_obj == NULL)
        return -1;

    LONGLONG_t hash_ = 0, hash_index;           // store the hash and the calculated index
    size_t word_length = strlen(word_);         // length of the string
    // calculate the current ts objects maximum capacity
    int current_lngthMax = ((ts_obj->private_field->table_count) * NODE_TABLE_DEF_SIZE);
    int counter = 0;    // counter to determine size of tables
    
    _NODE_HASH_WORD(word_, word_length, hash_); // calculate hash
    hash_index = hash_ % NODE_TABLE_DEF_SIZE;   // calculate the entry index based on the table size

    Node node_current = ts_obj->start_;
    if (hash_ < current_lngthMax) {       // if the hash is NOT out of bounds of the table
        LONGLONG_t hash_temp = hash_;
        
        // get the location of the searched table
        while (hash_temp > 0) {
            hash_temp -= NODE_TABLE_DEF_SIZE;
            counter++;
        }

        /* The counter tells how much step should we take
           to reach the destination of the searched table index */
        Node temp_ = node_current;            
        while (counter > 0) {
            if (node_current == NODE_EMPTY) // if the current node is NULL, break out of the loop
                break;
            else                            // else it's not NULL, set a pointer to the current node
                temp_ = node_current;
            
            node_current = node_current->next_;
            counter--;
        }

        if (temp_ != NODE_EMPTY) {                      // if the temp is not NULL, update some information 
            hash_index = hash_ % NODE_TABLE_DEF_SIZE;
            if (temp_->word_table[hash_index].taken)    // if the current table entry is NOT empty (it's taken), increase the reference count
                temp_->word_table[hash_index].ref++;
            else {                                 // else it's empty (taken), save the entry in the table at the calculated index
                strcpy(temp_->word_table[hash_index].word, word_);
                temp_->word_table[hash_index].ref++;                // increase the reference
                temp_->word_table[hash_index].word_index = hash_;   // save the hash
                temp_->word_table[hash_index].taken = true;         // set to taken
            }
        } else
            return -2;
    } else {                                                      // the hash is out of bounds of the table
        /* Calculate the length based on the max capacity of the current ts object,
           and the calculated hash
        */
        LONGLONG_t lngth_calculated = current_lngthMax - hash_;
        while (lngth_calculated < -1) {
            lngth_calculated += NODE_TABLE_DEF_SIZE;
            counter++;
        }

        // create the nodes based on the calculated new size
        for (size_t i = 0; i < counter; i++) {
            Node node_new = NodeCreate();
            if (node_new == NODE_EMPTY)
                return -3;
            
            // connect the new node at the end of the chain
            node_current->next_ = node_new;
            node_new->prev_ = node_current;
            node_current = node_new;
            ts_obj->private_field->table_count++;
        }

        strcpy(node_current->word_table[hash_index].word, word_);
        node_current->word_table[hash_index].ref++;
        node_current->word_table[hash_index].word_index = hash_;
        node_current->word_table[hash_index].taken = true;
    }

    return 0;
}
