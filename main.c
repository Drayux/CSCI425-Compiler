#include <stdio.h>
#include <stdlib.h>

#include "nfa.h"
#include "dfa.h"

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

//    nfa* container = create_container("abcdef");
//    nfa_state* state;
//    state = create_state(container);
//    state = create_state(container);
//    state = create_state(container);

//    printf("char is: %c\n", state->sigma[1]);

//    // Clean up the table (probably not actually needed but who knows *shrug*)
//    destroy_table(table);

    return 0;
}
