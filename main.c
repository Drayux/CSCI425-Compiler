#include <stdio.h>
#include <stdlib.h>

#include "nfa.h"
#include "dfa.h"

int main() {
    char sigma[] = "abcdef";
    printf("sigma: %s\n", sigma);

    // Insert some sample rows to a test table
    dfa* table = create_table(sigma);
    int* row;

    row = create_transition(table);
    row[4] = 16;

    row = create_transition(table);
    row[0] = 3;

    row = create_transition(table);
    row[0] = 1;

    row = create_transition(table);
    row[2] = 9;

    row = create_transition(table);
    row[5] = 3;

    printf("table size: %d, capacity: %d\n", (int) table->size, (int) table->capacity);
    print_table(table);

    nfa* state;
    state = create_state("abc");
    state = create_state("def");
    state = create_state("ghi");

    printf("char is: %c\n", state->sigma[1]);

    // Clean up the table (probably not actually needed but who knows *shrug*)
    destroy_table(table);

    return 0;
}
