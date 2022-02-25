#include <stdio.h>
#include <stdlib.h>

#include "nfa.h"
#include "dfa.h"
#include "list.h"
#include "stack.h"
#include "util.h"

// Follow character transitions for all states in a given set
// Returns a new set of state IDs
// Set (list) should be destroyed after use
list* follow_character(nfa* container, list* start, char tc) {
	if (!container || !start) return NULL;

	// Setup
	int char_i;
    if (!tc) char_i = container->length;
    else char_i = find_char(tc, container->sigma, container->length);
	list* follow = create_list();

	// Iterate every state in the starting set
	for (int i = 0; i < start->size; i++) {
		nfa_state* state = get_state(container, start->data[i]);
		nfa_state** transitions = state->data[char_i];
		int count = state->counts[char_i];

		// Add every transition to the set
		for (int j = 0; j < count; j++)
			insert(follow, transitions[j]->id);
	}

	return follow;
}

// Follow lambda transitions recursively
// Returns set of state IDs
// Subroutine of convert_nfa()
void follow_lambda(nfa* container, list* set) {
	int lambda_i = container->length;
	stack* to_follow = NULL;

	for (int i = 0; i < set->size; i++) {
		nfa_state* state = get_state(container, set->data[i]);
		stack_push(&to_follow, state->id);
	}

	while (to_follow) {
		int id = stack_pop(&to_follow);
		nfa_state* state = get_state(container, id);
		nfa_state** transitions = state->data[lambda_i];
		int count = state->counts[lambda_i];

		for (int i = 0; i < count; i++) {
			nfa_state* t = transitions[i];
			int t_id = t->id;

			if (find(set, t_id) < 0) {
				insert(set, t_id);
				stack_push(&to_follow, t_id);
			}
		}
	}
}

// Expand the allocation of a state set list
// Subroutine of convert_nfa()
void expand_ss(list*** ss, size_t* capacity) {
	size_t capacity_n = capacity * 2;
	list** ss_new = (list**) realloc(*ss, sizeof(list*) * capacity_n);

	if (!ss_new) {
		fprintf(stderr, "Realloc failed (expansion of state set list)\n");
		return;
	}

	// Create new empty lists for every new allocation
	//for (int i = capacity; i < capacity_n; i++) {}

	*ss = ss_new;
	*capacity = capacity_n;
}

// Find a state set within a list
// Returns the index of the set if found, else -1
// Subroutine of convert_nfa()
int find_ss(list** ss, list* set, size_t size) {
	for (int i = 0; i < size; i++)
		if (compare(ss[i]), set) return i;

	return -1;
}

// Converts an NFA to a DFA via the algorithm provided in class
// Creates a new DFA table, but will not free the provided NFA container
// *This function may be relocated at some point*
dfa* convert_nfa(nfa* input) {
	if (!input) return NULL;

	// List of state sets
	// DFA is void of set IDs (which is unnecessary after conversion)
	size_t capacity = 4;
	list** state_sets = (list**) calloc(capacity, sizeof(list*));
	//list** state_sets = expand_ss(&ss, &capacity);

	// Initialize the data structures
	dfa* table = create_table(input->orig);		// Pseudocode T
	stack* convert = NULL;						// Pseudocode L
	list* accepts = create_list();				// Pseudocode A
	list* set = create_list();					// Pseudocode B

	// Find the starting state
	for (int i = 0; i < input->size; i++) {
		nfa_state* state = input->states[i];
		if (state->flags & 2) {
			insert(set, state->id);
			break;
		}
	}

	// Begin conversion

	return table;
}

int main() {
	list* set = create_list();
	insert(set, 9);
	print_list(set);

    nfa* container = parse_file("automata/cblock.nfa");
	print_container(container);

	follow_lambda(container, set);
	print_list(set);

	destroy_list(&set);
    destroy_container(&container);
    return 0;
}
