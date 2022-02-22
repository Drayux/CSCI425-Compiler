#include <stdio.h>
#include <stdlib.h>

#include "nfa.h"
#include "dfa.h"
#include "list.h"
#include "util.h"

list* follow_character(nfa* container, list* set, char tc) {


	// TEMPORARY
	return NULL;
}

// Follow lambda transitions recursively
// Returns set of state IDs
// Subroutine of convert_nfa()
list* follow_lambda(nfa* container, list* set) {

	// TEMPORARY
	return NULL;
}

// Converts an NFA to a DFA via the algorithm provided in class
// Creates a new DFA table, but will not free the provided NFA container
// *This function may be relocated at some point*
dfa* convert_nfa(nfa* input) {
	if (!input) return NULL;

	// Create empty table
	dfa* table = create_table(input->sigma);

	// Begin conversion

	return table;
}

int main() {
    nfa* container = parse_file("automata/cblock.nfa");

    // nfa* container = create_container("abc");

    // nfa_state* s1 = create_state(container);
    // nfa_state* s2 = create_state(container);
    // nfa_state* s3 = create_state(container);
    // nfa_state* s4 = create_state(container);

    // nfa_state* s3 = get_state(container, 0);
    // nfa_state* s4 = get_state(container, 1);

	// size_t count;
	// char* str = "a b c d e";
	// char** splits = split(str, ' ', &count);
	//
    // add_transition(splits[0][0], s1, s2);
    // add_transition(splits[1][0], s3, s4);
    // add_transition(splits[2][0], s2, s1);

	// for (int i = 0; i < count; i++) free(splits[i]);
	// free(splits);

    print_container(container);
    destroy_container(&container);
    return 0;
}
