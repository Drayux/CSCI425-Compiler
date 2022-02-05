#include <stdio.h>
#include <stdlib.h>

#include "nfa.h"
#include "dfa.h"
#include "list.h"

// Converts an NFA to a DFA via the algorithm provided in class
// Creates a new DFA table, but will not free the provided NFA container
// *This function may be relocated at some point*
dfa* convert_nfa(nfa* input) {
    //dfa* table = create_table(input->sigma);
    return NULL;
}

int main() {
    char sigma[] = "abcdef";
    printf("sigma: %s\n\n", sigma);

//    int x;
//    x = find_char('a', sigma, (size_t) 6);
//    printf("char: a | index: %d\n\n", x);
//
//    x = find_char('b', sigma, (size_t) 6);
//    printf("char: b | index: %d\n\n", x);
//
//    x = find_char('g', sigma, (size_t) 6);
//    printf("char: g | index: %d\n\n", x);


    // Insert some sample rows to a test table
//    dfa* table = create_table(sigma);
//    int* row;
//
//    row = create_transition(table);
//    row[4] = 16;
//
//    row = create_transition(table);
//    row[0] = 3;
//
//    row = create_transition(table);
//    row[0] = 1;
//
//    row = create_transition(table);
//    row[2] = 9;
//
//    row = create_transition(table);
//    row[5] = 3;
//
//    printf("table size: %d, capacity: %d\n", (int) table->size, (int) table->capacity);
//    print_table(table);

//    nfa* container = create_container(sigma);
//
//    nfa_state* s1 = create_state(container);
//    nfa_state* s2 = create_state(container);
//    nfa_state* s3 = create_state(container);
//
//    add_transition('b', s1, s1);
//    add_transition('b', s1, s2);
//    add_transition('b', s1, s2);  // Duplicate test
//    add_transition('w', s1, s2);  // Nonexistent transition test
//    add_transition('d', s1, s2);
//    add_transition('f', s1, s3);
//    add_transition(0, s1, s3);    // Add a lambda transition
//
//    s2->flags = 1;
//    add_transition('c', s2, s2);
//
//    s3->flags = 2;
//    add_transition('a', s3, s1);

//    printf("char is: %c\n", state->sigma[1]);

//    print_container(container);
//    destroy_container(&container);

//    // Clean up the table (probably not actually needed but who knows *shrug*)
//    destroy_table(table);

    // List testing
    list* l = create_list();

    append(l, 6);
    append(l, 2);
    append(l, 7);
    append(l, 1);
    append(l, 0);
    append(l, 18);

    print_list(l);

//    int x = compare(l, r);
//    printf("Lists are equal: ");
//    x ? printf("TRUE\n") : printf("FALSE\n");

    return 0;
}
