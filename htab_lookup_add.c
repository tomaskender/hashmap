#include "htab.h"
#include "htab_definition.h"
#include <stdlib.h>
#include <assert.h>

htab_iterator_t htab_lookup_add(htab_t * t, const char *key) {
    if(t == NULL || key == NULL) {
        htab_iterator_t err_it = {.t = t, .ptr=NULL};
        return err_it;
    }
    
    unsigned list_id = htab_hash_function(key) % (t->arr_size);
    htab_iterator_t iterator = { .t=t, .idx=list_id };
    
    // last non-null entry in list
    struct htab_item *last = t->table[list_id];
    
    for(struct htab_item *i = last; i != NULL; i=i->next) {
        if(strcmp(i->key, key) == 0) {
            iterator.ptr = i;
            return iterator;
        } else if(i->next == NULL) {
            last = i;
        }
    }

    // entry not found, creating new one
    struct htab_item *new_item = malloc(sizeof(struct htab_item));
    if(new_item == NULL) {
        //error
        iterator.ptr = NULL;
        return iterator;
    }
    new_item->key = malloc(strlen(key)+1);
    if(new_item->key == NULL) {
        //error
        free(new_item);
        iterator.ptr = NULL;
        return iterator;
    }
    strcpy(new_item->key, key);
    new_item->data = 0;
    new_item->next = NULL;


    if(last == NULL) {
        t->table[list_id] = new_item;
    } else {
        last->next = new_item;
    }

    //increase population counter of htab
    t->size++;
    iterator.ptr = new_item;
    return iterator;
}
