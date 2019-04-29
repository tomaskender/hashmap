#ifndef __HTABLE_DEFINITION_H__

#define __HTABLE_DEFINITION_H__

// SIZE_ERR is used for error checking in functions returning t_size value
#include <stdint.h> // contains definition of SIZE_MAX
#define SIZE_ERR SIZE_MAX
struct htab_item {
   char *key;
   int data;
   struct htab_item *next;
};


struct htab {
    int size;
    int arr_size;
    struct htab_item *table[];
};

#endif
