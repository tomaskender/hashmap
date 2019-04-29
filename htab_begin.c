#include "htab.h"
#include "htab_definition.h"

htab_iterator_t htab_begin(const htab_t * t) {
    htab_iterator_t iter;
    // setting t
    iter.t = t;
    // setting idx and ptr
    if(t->size == 0) {
        iter.ptr = NULL;
    } else {
        for(iter.idx=0;; iter.idx++) {
            iter.ptr = t->table[iter.idx];
            if(iter.ptr != NULL)
                break;
        }
    }
    return iter;
}
