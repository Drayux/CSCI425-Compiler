#ifndef DFA_H
#define DFA_H

#include "util.h"

// Transition table for discrete finite automata
// Basic operation similar to a C++ vector
typedef struct dfa_table {
    char* sigma;        // String corresponding to the possible characters for transitions
    size_t length;      // Number of characters in the transition character sets
                        //   ^^Needs +1 for row 'width' (number of cols)

    size_t size;        // Current capacity of table (rows)
    size_t capacity;    // Maximum capacity of table (rows) before realloc is required
    int** data;         //   [int*, int*, int*, ... ]
                        //     \/    \/    \/
                        //   [int.][int.][int.] ...   : flags, t1, t2, t3, ...
                        // Flags: 0..100 (4) - removed; 0..010 (2) - entry; 0..001 (1) - accept;
} dfa;

// Function declarations
void print_table(dfa* table);
dfa* create_table(char* sigma);
int* create_transition(dfa* table);
void destroy_table(dfa** table_p);

#endif // DFA_H
