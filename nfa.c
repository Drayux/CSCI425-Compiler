#include "nfa.h"

// FUNCTION TODO
// Build from regex


// Creates a 'container' for NFA states
// Returns a pointer to the new container
// Saves a bit of memory so each state can reference the same location for sigma
// Allows states to find each other when building their transition tables
nfa* create_container(char* sigma) {
    // Prepare the transition character set
    int len = strnlen(sigma, TC_RANGE);
    char* sigma_n;

    if (!(sigma_n = str_sort(sigma, len))) {
        fprintf(stderr, "Invalid character set provided\n");
        return NULL;
    }

    // Copy the string (and exclude the null char)
    // char* sigma_n = (char*) malloc(sizeof(char) * len);
    // memcpy(sigma_n, sigma, (size_t) len);
    char* sigma_o = (char*) calloc(len + 1, sizeof(char));
    memcpy(sigma_o, sigma, (size_t) len);

    // Create the empty container
    nfa* container = (nfa*) calloc(1, sizeof(nfa));
    container->sigma = sigma_n;
    container->orig = sigma_o;
    container->length = (size_t) len;

    container->size = (size_t) 0;
    container->capacity = (size_t) 4;       // Init with space for 4 states

    // Allocate space for state pointers
    container->states = (nfa_state**) calloc(container->capacity, sizeof(nfa_state*));

    return container;
}

// Read an NFA definition from a file (Hellman format)
// Creates and returns a pointer to a new container with the respective definition
nfa* parse_file(char* path) {
    FILE* inf = fopen(path, "r");

    // Check that the file opened successfully
    if (!inf) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        exit(1);        // Exit the program if file cannot be accessed, as per project requirements
    } printf("Parsing definition from %s ...\n", path);

    // Using getline
    int lc = 0;
    char* linebuf = NULL;
    size_t buflen = 0;
    ssize_t nread;

    // Read the first line
    while (1) {
        nread = getline(&linebuf, &buflen, inf);
        lc++;

        if (nread == -1) {
            fprintf(stderr, "Definition file is empty\n");
            exit(1);    // Exit the program if file is empty, as per project requirements

        } else if (nread > 1) break;     // Ignore empty lines (does not account for CRLF)
    }

    // Parse first line
    linebuf[nread] = 0;     // Replace newline character with string terminator
    size_t pcount;
    char** params = split(linebuf, ' ', &pcount);

    // First line has at least 3 parameters
    if (pcount < 3) {
        fprintf(stderr, "Invalid NFA definition file\n");
        exit(-1);
    }

    // Character used to denote lambda in the config file
    char lambda_c = params[1][0];

    // Create sigma
    char* sigma = calloc(pcount - 1, sizeof(char));
    for (int i = 2; i < pcount; i++)
        sigma[i - 2] = params[i][0];

    // Create NFA and insert empty states
    nfa* container = create_container(sigma);
    int num_states = atoi(params[0]);
    if (num_states < 1) {
        fprintf(stderr, "Invalid NFA definition file\n");
        exit(-1);
    }
    for (int i = 0; i < num_states; i++) create_state(container);

    // Add the specified transitions
    char** line;
    size_t count;
    while ((nread = getline(&linebuf, &buflen, inf)) != -1) {
        lc++;       // NOTE: Currently not removing newline character from linebuf

        // Skip empty lines
        if (nread < 2) continue;

        // Split the new line
        line = split(linebuf, ' ', &count);
        if (count < 3) {
            // Invalid transition
            fprintf(stderr, "WARNING: Invalid transition (line %d)\n", lc);
            free_split(&line, count);
			continue;
        }

        // Get the specified states
        int from_state = atoi(line[1]);
        int to_state = atoi(line[2]);

        nfa_state* parent = get_state(container, from_state);
        nfa_state* child = get_state(container, to_state);

        // Insert transition rule
        char tc;
        for (int i = 3; i < count; i++) {
            tc = line[i][0];
            if (tc == lambda_c) tc = 0;
            add_transition(tc, parent, child);
        }

        // Set flags
        int flags = !(from_state) * 2;      // State 0 is implied entry state
        if (line[0][0] == '+') flags++;     // '+' denotes accepting state
        parent->flags = flags;

        free_split(&line, count);
    }

    // Resource cleanup
    free_split(&params, pcount);
    free(sigma);
    free(linebuf);
    fclose(inf);

    return container;
}

// Output the contents of a container (simplification of a for loop)
void print_container(nfa* container) {
    printf("NFA CONTAINER:\n");
    for (int i = 0; i < container->size; i++) {
        print_state(container->states[i]);
        printf("\n");
    }
}

// Creates a new NFA state for a given character set
// Returns a pointer to the new state
// Sigma parameter will only be used once
nfa_state* create_state_f(nfa* container, char isEntry, char isAccept) {
    nfa_state* state = (nfa_state*) calloc(1, sizeof(nfa_state));

    // Init the transitions arrays
    state->counts = (int*) calloc(container->length + 1, sizeof(int));
    state->data = (nfa_state***) calloc(container->length + 1, sizeof(nfa_state**));

    // Place the new state into the container
    // Expand the container if need be (kinda like C++ vector)
    if (container->size == container->capacity) {
        nfa_state** data_n = (nfa_state**) realloc(container->states, 2 * sizeof(nfa_state*) * container->capacity);
        if (data_n) {
            container->states = data_n;
            container->capacity *= 2;
        } else {
            fprintf(stderr, "Realloc failed (expansion of NFA container)\nTODO FREE MEMORY WHEN THIS HAPPENS\n");
            return NULL;
        }
    }

    // Insert the new state
    *(container->states + container->size) = state;
    state->id = container->size++;
    state->container = container;

    // Set the state flags
    state->flags = 0;
    if (isEntry) state->flags += 2;
    if (isAccept) state->flags += 1;

    return state;
}

nfa_state* create_state(nfa* container) {
    return create_state_f(container, 0, 0);
}

// Show a table of transitions of a given state
void print_state(nfa_state* state) {
    // Make sure state exists
    if (!state) {
        fprintf(stderr, "WARNING: print_state called on non-existent state\n");
        return;
    }

    // Print the title
    printf(" -- STATE %d: --\n", state->id);

    // Print the flags
//    printf("ENTRY: ");
//    printf((state->flags & 2) ? "TRUE\t" : "FALSE\t");
//    printf("ACCEPT: ");
//    printf((state->flags & 1) ? "TRUE\n" : "FALSE\n");
    printf("FLAGS:\t");
    if (!state->flags) printf("(none)\n");
    else {
        printf((state->flags & 2) ? "ENTRY  " : "");
        printf((state->flags & 1) ? "ACCEPT\n" : "\n");
    }

    // Print the transition labels
    int length = state->container->length;
    for (int i = 0; i < length; i++)
        printf("%c\t", state->container->sigma[i]);
    printf("lambda\n");

    for (int i = 0; i <= length; i++)
        printf("========");
    printf("\n");

    // Print the transition data
    char output = 1;
    int row = 0;
    while (output) {
        output = 0;
        for (int i = 0; i <= length; i++) {
            if (row >= state->counts[i]) {
                printf("-\t");
                continue;
            }

            printf("%d\t", (state->data[i][row])->id);
            output = 1;
        }
        row++;
        printf("\n");
    }
}

// Gets a pointer to an NFA state from its container class by ID
nfa_state* get_state(nfa* container, int id) {
    // Check bounds of parameters
    if (id >= container->size) {
        fprintf(stderr, "WARNING: Attempted to access NFA state out of bounds\n");
        return NULL;
    }

    // Get the specified state
    nfa_state* s = container->states[id];

    // Check table integrity (mostly for debug)
    if (id - s->id) {
        fprintf(stderr, "WARNING: NFA state container is (probably) corrupt\n");
        return NULL;
    }

    return s;
}

// Follow the transition of a specified character
//nfa_state* follow_transition(nfa* container, nfa_state* state, int char_i) {}

// Adds the child state as a transition to the parent state
void add_transition(char tc, nfa_state* parent, nfa_state* child) {
    // Setup
    int char_i;
    if (!tc) char_i = parent->container->length;
    else char_i = find_char(tc, parent->container->sigma, parent->container->length);

    // Error checking (TODO add verbosity)
    if (!parent || !child) return;
    if (char_i < 0) {
        fprintf(stderr, "WARNING: Could not find specified character (%c) in transition set\n", tc);
        return;
    }

    // -- Append the child to the parent --
    int count;
    if (!(count = parent->counts[char_i])) {
        // Array not yet created (count is zero)
        parent->data[char_i] = (nfa_state**) malloc(sizeof(nfa_state*));
        goto add_transition_ret;
    }

    // Ensure duplicate is not added
    for (int i = 0; i < count; i++)
        if (parent->data[char_i][i] == child) {
            fprintf(stderr, "WARNING: Attempt to insert duplicate state transition\n");
            return;
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
void destroy_state(nfa_state** state_p) {
    nfa_state* state = *state_p;

    // Clean up the transitions array
    // (Recall that we should *not* clean up the linked transitions recursively)
    for (int i = 0; i < state->container->length + 1; i++) {
        //printf("i: %d, count: %d ; address: %p\n", i, state->counts[i], state->data[i]);
        if (state->data[i]) free(state->data[i]);
    } free(state->data);

    // Clean up counts array
    free(state->counts);

    // Update the container to point to nothing
    // (Might not be necessary)
    state->container->states[state->id] = NULL;

    // Free self
    free(state);
    *state_p = 0;
}

// Free a NFA tree from memory (by container so it's easy)
void destroy_container(nfa** container_p) {
    nfa* container = *container_p;

    // Destroy array of states
    for (int i = 0; i < container->size; i++) {
        nfa_state* state;
        if ((state = container->states[i])) destroy_state(&state);
    } free(container->states);

    // Free transition set
    free(container->sigma);
    free(container->orig);

    // Finally free self
    free(container);
    *container_p = NULL;
}
