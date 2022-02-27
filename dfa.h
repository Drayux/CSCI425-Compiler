#ifndef DFA_H
#define DFA_H

#include "util.h"

// Transition table for discrete finite automata
// Basic operation similar to a C++ vector
typedef struct dfa_table {
    char* sigma;        // String corresponding to the possible characters for transitions
    //char* orig;         // Copy of TC set left unsorted (same order as infile)
    size_t length;      // Number of characters in the transition character sets
                        //   ^^Needs +1 for row 'width' (number of cols)

    size_t size;        // Current capacity of table (rows)
    size_t capacity;    // Maximum capacity of table (rows) before realloc is required
    int** data;         //   [int*, int*, int*, ... ]
                        //     \/    \/    \/
                        //   [int.][int.][int.] ...   : flags, tr1, tr2, tr3, ...
                        // Flags: 0..100 (4) - removed; 0..010 (2) - entry; 0..001 (1) - accept;
} dfa;

// Function declarations
void print_table(dfa* table);
void output_table(dfa* table, char* path);
dfa* create_table(char* sigma);
int* create_transition(dfa* table);
void optimize_table(dfa* table);
int match_token(dfa* table, char* token);
void destroy_table(dfa** table_p);

#endif // DFA_H
