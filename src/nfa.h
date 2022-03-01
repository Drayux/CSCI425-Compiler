#ifndef NFA_H
#define NFA_H

#include "list.h"
#include "util.h"

// State class for non-discrete finite automata
// Behaves similar to a tree (so no parent table is needed)
typedef struct nfa_node {
    int id;
    int flags;                  // Flags: 0..010 (2) - entry; 0..001 (1) - accept;
    int* counts;                // The number of transitions for each character
    struct nfa_node*** data;    // Array of pointers to arrays of transitions
                                //    indexed by index of character transition
    struct nfa_container* container;
} nfa_state;

// Container class to house NFA states in a memory-safe fashion :)
// Behaves somewhat like a C++ vector
typedef struct nfa_container {
    char* sigma;        // Transition character set
    char* orig;         // Copy of TC set, but in the order specified in the input file
    size_t length;      // Number of characters within the transition set
                        //   ^^Each node needs one additional space for lambda
    size_t size;        // Current capacity of table (rows)
    size_t capacity;    // Maximum capacity of table (rows)

    nfa_state** states;
} nfa;

// Function declarations
nfa* create_container(char* sigma);
nfa* parse_file(char* path);
void print_container(nfa* container);
nfa_state* create_state_f(nfa* container, char isEntry, char isAccept);
nfa_state* create_state(nfa* container);
void print_state(nfa_state* state);
nfa_state* get_state(nfa* container, int index);
//nfa_state* follow_transition(nfa* container, nfa_state* state, int char_i);
//void add_transition(char tc, nfa* container, int parent, int child);
void add_transition(char tc, nfa_state* parent, nfa_state* child);
void destroy_state(nfa_state** state_p);
void destroy_container(nfa** container_p);

#endif // NFA_H


/* Memory leak testing:
 * nfa* container = create_container("abc");
 * nfa_state* s1 = create_state(container);
 * nfa_state* s2 = create_state(container);
 * nfa_state* s3 = create_state(container);
 * nfa_state* s4 = create_state(container);
 * nfa_state* s3 = get_state(container, 0);
 * nfa_state* s4 = get_state(container, 1);
 * size_t count;
 * char* str = "a b c d e";
 * char** splits = split(str, ' ', &count);
 * add_transition(splits[0][0], s1, s2);
 * add_transition(splits[1][0], s3, s4);
 * add_transition(splits[2][0], s2, s1);
 * for (int i = 0; i < count; i++) free(splits[i]);
 * free(splits);
 *
 */
