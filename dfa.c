#include "dfa.h"

// FUNCTIONS TODO!
// Follow lambda
// Convert from NFA to DFA
// Cleans up a tables rows after optimization
// Optimize table (removes duplicate states)

// Sort a string (in place) and check for duplicates
// (Utility function for create_table; uses a modified counting sort)
// Returns 0 if duplicate is found (else the length of the string)
// TODO: replace this with radix sort if adding support for unicode character set
int str_sort(char* str) {
    int len = strnlen(str, RANGE);
    if (len < 2) return len;  // String is (technically) already sorted

    int* counts = calloc(RANGE, sizeof(int));
    char* input = malloc(sizeof(char) * (len + 1));
    memcpy(input, str, (size_t) len + 1);

    // Total the counts
    for (int i = 0; i < len; i++) {
        if (counts[(unsigned int) input[i]]++) {
            len = 0;
            goto str_sort_ret;
        }
    }

    // Adjust the counts array
    for (int i = 1; i < RANGE; i++)
        counts[i] += counts[i - 1];

    // Reorder input string
    for (int i = 0; i < len; i++) {
        unsigned int x = input[i];
        unsigned int y = --counts[x];
        /////////////////////////////////////////
        // THIS WILL FAULT IF STRING IS IMMUTABLE (char* str = "string")
        // TO FIX THIS, INSTEAD USE: (char str[] = "string")
        str[y] = (char) x;
        /////////////////////////////////////////
    }

    str_sort_ret:
    free(counts);
    free(input);
    return len;
}

// Outputs a the provided DFA transition table
void print_table(dfa* table) {
    // Print the title
    printf("TRANSITION TABLE:\n");

    // Print the column names
    printf("ID\tEntry\tAccept\t  ||\t");
    for (int i = 0; i < table->width - 1; i++)
        printf("%c\t", table->sigma[i]);
    printf("\n");

    // Print a separator row
    for (int i = 0; i < table->width + 3; i++)
        printf("========");
    printf("\n");

    // Print the column values
    for (int i = 0; i < table->size; i++) {
        // State ID
        printf("%d\t", i);

        // Is ENTRY
        printf((table->data[i][0] & 2) ? "TRUE\t" : "FALSE\t");

        // Is ACCEPT
        printf((table->data[i][0] & 1) ? "TRUE\t" : "FALSE\t");

        // Separator
        printf("  ||\t");

        // Transition values
        for (int j = 1; j < table->width; j++)
            printf("%d\t", table->data[i][j]);

        printf("\n");
    }
}

// Dynamically allocates and initializes a new table with a given character set
dfa* create_table(char* sigma) {
    // Prepare the transition character set
    int len;
    if (!(len = str_sort(sigma))) {
        fprintf(stderr, "Invalid character set provided\n");
        return NULL;
    }

    // Copy the string (and exclude the null char)
    char* sigma_n = (char*) malloc(sizeof(char) * len);
    memcpy(sigma_n, sigma, (size_t) len);

    dfa* table = (dfa*) calloc(1, sizeof(dfa));
    table->sigma = sigma_n;
    table->width = (size_t) (len + 1);  // Width to use every time a new row is allocated

    // Allocate the 2D table array
    table->capacity = (size_t) 4;       // Init with space for 4 states
    table->size = (size_t) 0;

    table->data = (int**) calloc(table->capacity, sizeof(int*));

    return table;
}

// Creates and appends a new, empty state into the specified table
// Returns the pointer for insertion of state data
int* create_state(dfa* table) {
    // Expand the table if need be (kinda like C++ vector)
    if (table->size == table->capacity) {
        int** data_n = (int**) realloc(table->data, 2 * sizeof(int*) * table->capacity);
        if (data_n) {
            //free(table->data);   // Supposedly not needed (leaving this as a reminder in case I'm wrong)
            table->data = data_n;
            table->capacity *= 2;
            //printf("Table capacity is now %d\n", (int) table->capacity);

        } else {
            fprintf(stderr, "Realloc failed (expansion of DFA table)\n");
            return NULL;
        }
    }

    // Create the new empty row
    int* row = calloc(table->width, sizeof(int));
    *(table->data + table->size) = row;
    table->size++;

    return row;
}

// Safely remove a table from memory if no longer needed
// Kinda vibes the same as destroying the death star
void destroy_table(dfa* table) {
    // Free each row of the table
    for (int i = 0; i < table->size; i++)
        free(table->data[i]);

    // Free the outer table shell
    free(table->data);

    // Free transition char string
    free(table->sigma);

    // Free self
    free(table);
}
