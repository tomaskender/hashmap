#include "htab.h"
#include "htab_definition.h"

htab_iterator_t htab_end(const htab_t * t) {
    htab_iterator_t iter;
    struct htab_item *last_item = NULL;
    for(int i=0; i<t->arr_size; i++) {
        if(t->table[i] != NULL) {
            last_item = t->table[i];
            iter.idx = i;
        }
    }
    if(last_item != NULL) {
        for(;last_item->next != NULL; last_item = last_item->next);
    }
    
    if(last_item == NULL) {
        iter.ptr = NULL;
    } else {
        iter.ptr = last_item+1;
    }
    iter.t = t;
    return iter;
}
