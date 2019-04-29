#include "htab.h"
#include "htab_definition.h"
#include <stdlib.h>

htab_t *htab_init(size_t n) {
    // SIZE_ERR aka SIZE_MAX is reserved for error checking
    if(n < 1 || n == SIZE_ERR)
        return NULL;

    htab_t *h = malloc(sizeof(struct htab)+n*sizeof(struct htab_item*));

    if(h == NULL)
        return NULL;

    h->size = 0;
    h->arr_size = n;
    for(unsigned i=0; i<n; i++)
        h->table[i] = NULL;
    return h;
}
