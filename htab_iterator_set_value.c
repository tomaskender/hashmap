#include "htab.h"
#include "htab_definition.h"

int htab_iterator_set_value(htab_iterator_t it, int val) {
    if(it.ptr == NULL)
        return -1;
    it.ptr->data = val;
    return val;
}
