#include "nfa.h"

// FUNCTION TODO
// Add transition (to NFA state)
// Get state from container?
// Build from regex


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

// Show a table of transitions of a given state
void print_state(nfa_state* state) {

}

// Creates a new NFA state for a given character set
// Returns a pointer to the new state
// Sigma parameter will only be used once
nfa_state* create_state(nfa* container) {
    nfa_state* state = (nfa_state*) calloc(1, sizeof(state));

    // Init the transitions arrays
    state->counts = (int*) calloc(container->length + 1, sizeof(int));
    state->data = (nfa_state***) calloc(container->length + 1, sizeof(struct nfa_node**));

    // Place the new state into the container
    // Expand the container if need be (kinda like C++ vector)
    if (container->size == container->capacity) {
        nfa_state** data_n = (nfa_state**) realloc(container->states, 2 * sizeof(struct nfa_node*) * container->capacity);
        if (data_n) {
            container->states = data_n;
            container->capacity *= 2;
        } else {
            fprintf(stderr, "Realloc failed (expansion of NFA container)\n");
            return NULL;
        }
    }

    // Insert the new state
    *(container->states + container->size) = state;
    state->id = container->size++;
    state->container = container;

    return state;
}

// Gets a pointer to an NFA state from its container class by ID
nfa_state* get_state(nfa* container, int id) {
    // Check bounds of parameters
    if (id >= container->size) {
        fprintf(stderr, "Attempted to access NFA state out of bounds\n");
        return NULL;
    }

    // Get the specified state
    nfa_state* s = container->states[id];

    // Check table integrity (mostly for debug)
    if (id - s->id) {
        fprintf(stderr, "WARNING: NFA state container is corrupt!");
        return NULL;
    }

    return s;
}

// Adds the child state as a transition to the parent state
void add_transition(char tc, nfa_state* parent, nfa_state* child) {
    // Setup
    int char_i = find_char(tc, parent->container->sigma, parent->container->length);
//    nfa_state* parent = get_state(container, p);
//    nfa_state* child = get_state(container, c);

    // Error checking (TODO add verbosity)
    if (!parent || !child || char_i < 0) return;

    // -- Append the child to the parent --
    int count;
    if (!(count = parent->counts[char_i])) {
        // Array not yet created (count is zero)
        parent->data[char_i] = (nfa_state**) malloc(sizeof(nfa_state*));
        goto add_transition_ret;
    }

    // Realloc if necessary
    size_t alloc_size = calc_alloc_size(count);
    if (count >= alloc_size) {
        nfa_state** t_list = (nfa_state**) realloc(parent->data[char_i], 2 * alloc_size * sizeof(nfa_state*));

        if (t_list) parent->data[char_i] = t_list;
        else fprintf(stderr, "Realloc failed (expansion of NFA container)\n");
    }

    add_transition_ret:
    parent->data[char_i][count] = child;
    parent->counts[char_i]++;
}

// Free a state from memory
// Should be used as subroutine of destroy_container()
//   ^^because there is no need to optimize a NFA tree, just convert it to a DFA
void destroy_state(nfa_state* state) {
    printf("TODO: destroy_state()\n");
}

// Free a NFA tree from memory (by container so it's easy)
void destroy_container(nfa* container) {
    printf("TODO: destroy_container()\n");
}
