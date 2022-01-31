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

    row = create_state(table);
    row[4] = 16;

    row = create_state(table);
    row[0] = 3;

    row = create_state(table);
    row[0] = 1;

    row = create_state(table);
    row[2] = 9;

    row = create_state(table);
    row[5] = 3;

    printf("table size: %d, capacity: %d\n", (int) table->size, (int) table->capacity);
    print_table(table);

    // Clean up the table (probably not actually needed but who knows *shrug*)
    destroy_table(table);

    return 0;
}
