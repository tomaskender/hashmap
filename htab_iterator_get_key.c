#include "htab.h"
#include "htab_definition.h"

const char * htab_iterator_get_key(htab_iterator_t it) {
    if(it.ptr == NULL)
        return NULL;
    return it.ptr->key;
}
