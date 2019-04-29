#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS_IN_LINE 1023

void check_loaded_line(FILE *f, char *buff, bool *b_LineLengthWarningShown) {
        if(strchr(buff, '\0') - buff == MAX_CHARS_IN_LINE+1 && buff[MAX_CHARS_IN_LINE] != '\n') {
            if(*b_LineLengthWarningShown == 0)
                fprintf(stderr, "print_from_row/load_from_end: One or more lines in "
                "source file exceed maximum limit of %d characters!\n", MAX_CHARS_IN_LINE);
            *b_LineLengthWarningShown = true;

            // move reader back to char 1024, which is the first char character after the limit
            ungetc(buff[MAX_CHARS_IN_LINE], f);

            // ignores rest of the line
            char c;
            while((c=getc(f)) != '\n' && c != '\0') {
                printf("skipping %c\n", c);
            }
            
            // trimming the loaded line
            //
            buff[MAX_CHARS_IN_LINE] = '\n';
            buff[MAX_CHARS_IN_LINE+1] = '\0';
        }
}

/* USED FOR PRINTING LAST X LINES USING A MODIFIED QUEUE IMPLEMENTATION */
/* -----------------------------------------------------------------------------*/
typedef struct {
    int count;      // current size
    int capacity;   // max size
    int front;      // index with first element in array
    int rear;       // index with last element in array
    char *arr[];    // lines
} Queue;

void free_queue(Queue *q) {
    if(q != NULL) {
        for(int i=0; i < q->count; i++) {
            if(q->arr[i])
                free(q->arr[i]);
        }
        free(q);
    }
}

int add_entry(Queue *q, char line[]) {
    char *a_line = malloc(strlen(line)+1);

    if(a_line == NULL) {
        fprintf(stderr, "add_entry: Could not allocate memory for line.\n");
        return 0;
    }
    
    strcpy(a_line, line);

    q->rear = (q->rear+1) % q->capacity;
    
    if(q->arr[q->rear] != NULL)
        free(q->arr[q->rear]);

    q->arr[q->rear] = a_line;

    if(q->front == -1)
        q->front = 0;

    if(q->count == q->capacity) {
        q->front = (q->front+1) % q->capacity;
    } else {
        q->count++;
    }

    return 1; // 1=success? nemalo by to byt opacne?
}

int setup(Queue **q, int capacity) {
    if(capacity<1) {
        fprintf(stderr, "setup: Incorrect capacity\n");
        return 0;
    }
    
    *q = malloc(sizeof(Queue)+capacity*sizeof(char*));
    
    for(int i=0; i<(*q)->capacity; i++)
        (*q)->arr[i] = NULL;

    if(*q==NULL) {
        fprintf(stderr, "setup: Could not allocate memory for Queue\n");
        return 0;
    }
    (*q)->front = -1;
    (*q)->rear = -1;
    (*q)->count = 0;
    (*q)->capacity = capacity;
    return 1;
}

void load_from_end(FILE *f, Queue *q) {
    // buff[char_limit+overflowing_char+'\0']
    char buff[MAX_CHARS_IN_LINE+2];
    bool b_LineLengthWarningShown = false;

    // load one more char than the limit to see if line overflows the limit
    while(fgets(buff, MAX_CHARS_IN_LINE+2, f) != NULL) {
        check_loaded_line(f, buff, &b_LineLengthWarningShown);
        // if could not add entry, clear the queue and end the program
        if(!add_entry(q, buff)) {
           free_queue(q);
           exit(1);
        }
    }

}

void print_queue(Queue *q) {
    for(int i=0; i<q->count; i++) {
        printf("%s", q->arr[(q->front+i) % q->capacity]);
    }
}

/* -----------------------------------------------------------------------------*/
/* END OF QUEUE IMPLEMENTATION FOR PRINTING LAST X LINES */

void print_from_row(FILE *f, int starting_row) {
    bool b_LineLengthWarningShown = false;
    char buff[MAX_CHARS_IN_LINE+2];
    for(int i=1; fgets(buff, MAX_CHARS_IN_LINE+2, f) != NULL; i++) {
        // skip first lines until reading starting row
        if(i<starting_row)
            continue;

        check_loaded_line(f, buff, &b_LineLengthWarningShown);
        printf("%s", buff);
    }

}

int main(int argc, char *argv[]) {
    int line_cnt = 10;
    bool b_ReadFromBeginning;
    FILE *f = NULL;
    
    if(argc > 4) {
        fprintf(stderr, "Max 3 arguments expected, which consist of \"FILENAME\" and \"-n NUMBER\" in random order\n");
        exit(1);
    }

    for(int i=1; i<argc; i++) {
        if(strcmp(argv[i], "-n") == 0) {
            // move to VALUE that is supposed to be found after -n
            i++;

            if(i >= argc) {
                fprintf(stderr, "-n VALUE not found.\n");
                return 1;
            }

            // VALUE argument format is -x, x, +x or -+x, where x is a number
            int p = 0;

            if(argv[i][p] == '-') {;
                p++;
            }

            if(argv[i][p] == '+') {
                b_ReadFromBeginning = true;
                p++;
            }

            char *end_ptr;
            line_cnt = strtol(&(argv[i][p]), &end_ptr, 10);

            if(*end_ptr != '\0') {
                fprintf(stderr, "-n VALUE is not a number.\n");
                exit(1);
            }
            
            // throws error for non-positive values
            if(line_cnt <= 0) {
                fprintf(stderr, "-n VALUE must be positive and it is %d.\n", line_cnt);
                exit(1);
            }
        } else {
            f = fopen(argv[i], "r");
            if(f == NULL) {
                fprintf(stderr, "File %s could not be loaded!\n", argv[i]);
                exit(1);
            }
        }
    }
    
    // f not loaded -> load from stdin
    if(!f)
        f = stdin;
    
    if(b_ReadFromBeginning) {
        print_from_row(f, line_cnt);
    } else {
        Queue *q;
        if(!setup(&q, line_cnt))
            exit(1);
        load_from_end(f, q);
        print_queue(q);
        free_queue(q);
    }
    fclose(f);
    return 0;
}
