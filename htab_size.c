#include "htab.h"
#include "htab_definition.h"

size_t htab_size(const htab_t * t) {
    if(t == NULL) {
        // error code
        return SIZE_ERR;    
    }
    return t->size;
}
