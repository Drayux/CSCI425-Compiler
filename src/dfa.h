#ifndef DFA_H
#define DFA_H

#include "list.h"
#include "stack.h"
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
                        //   [int.][int.][int.] ...   : flags, tr1, tr2, tr3, ...
                        // Flags: 0..100 (4) - removed; 0..010 (2) - entry; 0..001 (1) - accept;
} dfa;

// Function declarations
void print_table(dfa* table);
void output_table(dfa* table, char* path);
dfa* parse_table(char* path, char* sigma);      // TODO: After LUTHER change this to parse files with sigma as a header
dfa* create_table(char* sigma);
int* create_transition(dfa* table);
void remove_transition(dfa* table, int id, int rep);
int merge_states(dfa* table);
void prune_states(dfa* table);
void optimize_table(dfa* table);
int match_token(dfa* table, char* token);
void destroy_table(dfa** table_p);

#endif // DFA_H
