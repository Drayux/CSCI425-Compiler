#include "nfa.h"

// Creates a 'container' for NFA states
// Returns a pointer to the new container
// Saves a bit of memory so each state can reference the same location for sigma
// Allows states to find each other when building their transition tables
nfa* create_container(char* sigma) {
    // Prepare the transition character set
    int len;
    if (!(len = str_sort(sigma))) {
        fprintf(stderr, "Invalid character set provided\n");
        return NULL;
    }

    // Copy the string (and exclude the null char)
    char* sigma_n = (char*) malloc(sizeof(char) * len);
    memcpy(sigma_n, sigma, (size_t) len);

    // Create the empty container
    nfa* container = (nfa*) calloc(1, sizeof(nfa));
    container->sigma = sigma_n;
    container->length = (size_t) len;

    container->size = (size_t) 0;
    container->capacity = (size_t) 4;       // Init with space for 4 states

    // Allocate space for state pointers
    container->states = (nfa_state**) calloc(container->capacity, sizeof(nfa_state*));

    return container;
}

// Creates a new NFA state for a given character set
// Returns a pointer to the new state
// Sigma parameter will only be used once
nfa_state* create_state(nfa* container) {
    nfa_state* state = (nfa_state*) calloc(1, sizeof(state));

    // Init the transitions arrays
    state->counts = (int*) calloc(container->length + 1, sizeof(int));
    state->data = (nfa_state**) calloc(container->length + 1, sizeof(struct nfa_node*));

    // Place the new state into the container
    // Expand the container if need be (kinda like C++ vector)
    if (container->size == table->capacity) {
        nfa_state** data_n = (nfa_state**) realloc(container->data, 2 * sizeof(struct nfa_node*) * container->capacity);
        if (data_n) {
            container->states = data_n;
            container->capacity *= 2;
        } else {
            fprintf(stderr, "Realloc failed (expansion of NFA container)\n");
            return NULL;
        }
    }

    // Insert the new state
    *(container->data + container->size) = state;
    state->id = container->size++;

    return state;
}

// FUNCTION TODO
// Add transition (to NFA state)
// Destroy state
