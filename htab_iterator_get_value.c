#include "htab.h"
#include "htab_definition.h"

int htab_iterator_get_value(htab_iterator_t it) {
    if(it.ptr == NULL)
        return -1;
    return it.ptr->data;
}
