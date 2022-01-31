#ifndef DFA_H
#define DFA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 255 is max number of possible unique character values
const static size_t RANGE = 256;  // Not confident this is supposed to work across files?

// Transition table for discrete finite automata
// Basic operation similar to a C++ vector
typedef struct dfa_table {
    char* sigma;        // String corresponding to the possible characters for transitions
    size_t width;       // Calculated transition table row width (number of cols)

    size_t capacity;    // Maximum capacity of table (rows) before realloc is required
    size_t size;        // Current capacity of table (rows)
    int** data;         //   [int*, int*, int*, ... ]
                        //     \/    \/    \/
                        //   [int.][int.][int.] ...   : flags, t1, t2, t3, ...
                        // Flags: 0..100 (4) - removed; 0..010 (2) - entry; 0..001 (1) - accept;
} dfa;

// Utility function declarations
int str_sort(char* str);
void print_table(dfa* table);
dfa* create_table(char* sigma);
int* create_state(dfa* table);
void destroy_table(dfa* table);

#endif // DFA_H
