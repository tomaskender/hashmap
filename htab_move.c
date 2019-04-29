#include "htab.h"
#include "htab_definition.h"

htab_t *htab_move(size_t n, htab_t *from) {
    htab_t *to = htab_init(n);
    if(!to) {
        htab_free(from);
        return NULL;
    }

    htab_iterator_t iter = htab_begin(from);
    htab_iterator_t end = htab_end(from);
    while(!htab_iterator_equal(iter, end)) {
        htab_lookup_add(to, htab_iterator_get_key(iter));
        iter = htab_iterator_next(iter);
    }

    htab_free(from);
    return to;
}
