#include "htab.h"
#include "htab_definition.h"
#include <stdlib.h>

void htab_free(htab_t * t) {
    if(t == NULL)
        return;

    htab_clear(t);

    free(t);
}
