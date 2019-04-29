#include "htab.h"
#include "htab_definition.h"
#include <stdlib.h>

void htab_clear(htab_t * t) {
    if(t == NULL)
        return;
    
    for(int arr_i=0; arr_i<t->arr_size; arr_i++) {
        struct htab_item *n;
        for(struct htab_item *i = t->table[arr_i]; i != NULL; i=n) {
            n = i->next;
            free(i->key);
            free(i);
        }
        t->table[arr_i] = NULL;
    }

    t->size = 0;
}
