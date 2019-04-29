#include "htab.h"
#include "htab_definition.h"

htab_iterator_t htab_iterator_next(htab_iterator_t it) {
    struct htab_item *ptr = it.ptr;
    if(htab_iterator_equal(it, htab_end(it.t))) {
        it.ptr = NULL;
    } else if(ptr->next != NULL) {
        it.ptr = ptr->next;
    } else {
        //find next non empty bucket
        it.ptr = it.ptr+1;
        for(it.idx++; it.idx < it.t->arr_size; it.idx++) {
            if(it.t->table[it.idx] != NULL) {
                it.ptr = it.t->table[it.idx];
                break;
            }
        }
    }
    
    return it;
}
