#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"
#include "io.h"

#define MAX_WORD_LENGTH 1023

/*  
    Expected max key count in hashmap is 1 million keys
    because of testing input being generated with seq 1000000 2000000|shuf.
    Rule of thumb:
    - table capacity needs to be 1.3*expected_keys_count.
    (in other words expected key count is around 75% of bucket count)
    - table capacity should be a prime number.
    ----------------------------------------------------------
    the 75% rule => 1.3*1million = 1 300 000
    the prime number rule => first prime above 1.3 million is 1 300 021
*/
#define INIT_TAB_SIZE 1300021

int main() {
    htab_t *t = htab_init(INIT_TAB_SIZE);
    if(!t) {
        fprintf(stderr, "wordcount.c: Could not allocate memory for table.\n");
        return 1;
    }

    char buffer[MAX_WORD_LENGTH+1];
    htab_iterator_t matched_iter;
    int length;
    while((length = get_word(buffer, MAX_WORD_LENGTH, stdin)) != EOF) {
        // not a word
        if(length < 1)
            continue;

        matched_iter = htab_lookup_add(t, buffer);
        if(matched_iter.ptr == NULL) {
            // error
            fprintf(stderr, "wordcount.c: htab_lookup_add failed.\n");
            htab_free(t);
            return 1;
        }

        htab_iterator_set_value(matched_iter,
                                htab_iterator_get_value(matched_iter)+1);
    }
    
#ifdef TEST
    t = htab_move(10000, t);
    if(!t) {
        fprintf(stderr, "worcount.c: Could move table.\n");
        return 1;
    }
#endif

    matched_iter = htab_begin(t);
    htab_iterator_t end = htab_end(t); 
    while(!htab_iterator_equal(matched_iter, end)) {
        printf("%s => %d matches\n",
                htab_iterator_get_key(matched_iter),
                htab_iterator_get_value(matched_iter));
        matched_iter = htab_iterator_next(matched_iter);
    }

    htab_free(t);
    return 0;
}
