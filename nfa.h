#ifndef NFA_H
#define NFA_H

#include "util.h"

// State class for non-discrete finite automata
// Behaves similar to a tree (so no parent table is needed)
typedef struct nfa_node {
    int id;
    int* counts;                // The number of transitions for each character
    struct nfa_node*** data;    // Array of pointers to arrays of transitions
                                //    indexed by index of character transition
    struct nfa_container* container;
} nfa_state;

// Container class to house NFA states in a memory-safe fashion :)
// Behaves somewhat like a C++ vector
typedef struct nfa_container {
    char* sigma;        // Transition character set
    size_t length;      // Number of characters within the transition set
                        //   ^^Each node needs one additional space for lambda
    size_t size;        // Current capacity of table (rows)
    size_t capacity;    // Maximum capacity of table (rows)

    nfa_state** states;
} nfa;

// Function declarations
nfa* create_container(char* sigma);
void print_state(nfa_state* state);
nfa_state* create_state(nfa* container);
nfa_state* get_state(nfa* container, int index);
//void add_transition(char tc, nfa* container, int parent, int child);
void add_transition(char tc, nfa_state* parent, nfa_state* child);
void destroy_state(nfa_state* state);
void destroy_container(nfa* container);

#endif // NFA_H
