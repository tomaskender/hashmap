#include "htab.h"
#include "htab_definition.h"

size_t htab_bucket_count(const htab_t * t) {
    if(t == NULL) {
        return SIZE_ERR;
    }
    return t->arr_size;
}
