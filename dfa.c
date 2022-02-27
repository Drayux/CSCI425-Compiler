#include "dfa.h"

// Outputs a the provided DFA transition table
void print_table(dfa* table) {
    // Print the title
    printf("TRANSITION TABLE:\n");

    // Print the column names
    printf("ID\tEntry\tAccept\t  ||\t");
    for (int i = 0; i < table->length; i++)
        printf("%c\t", table->sigma[i]);
    printf("\n");

    // Print a separator row
    for (int i = 0; i < table->length + 4; i++)
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
        for (int j = 1; j <= table->length; j++) {
            int tr = table->data[i][j];
            if (tr < 0) printf("-\t");
            else printf("%d\t", tr);
        }

        printf("\n");
    }
}

// Format the DFA into minimal format for file IO
// Writes the formatted table to the specified file descriptor
void output_table(dfa* table, char* path) {
    FILE* outf = fopen(path, "w");
    if (!outf) {
        fprintf(stderr, "Failed to open %s\n", path);
        return;
    }

    int fd = fileno(outf);
    char space = ' ';
    char newline = '\n';
    char* buf = (char*) calloc(20, sizeof(char));
    int* row;
    //int row_len = (table->length + 2) * 2;      // Flag + ID + TChars (*2 for delims space/newline)

    // TODO GENERATE AND OUTPUT THE HEADER (state count and transition set)

    for (int i = 0; i < table->size; i++) {
        row = table->data[i];

        // Write the flag
        *buf = (row[0] & 1) ? '+' : '-';
        write(fd, buf, 1);
        write(fd, &space, 1);

        // Write the ID
        snprintf(buf, 20, "%d", i);         // 2^64 consists of 20 characters in base 10
        write(fd, buf, strnlen(buf, 20));

        // Write the transitions
        for (int j = 1; j <= table->length; j++) {
            int tr = row[j];
            write(fd, &space, 1);

            if (tr < 0) {
                *buf = 'E';
                write(fd, buf, 1);

            } else {
                snprintf(buf, 20, "%d", i);
                write(fd, buf, strnlen(buf, 20));
            }
        }

        // Newline
        write(fd, &newline, 1);
    }

    // Resource cleanup
    printf("Transition table saved to %s\n", path);
    close(fd);
    free(buf);
}

// Dynamically allocates and initializes a new table with a given character set
dfa* create_table(char* sigma) {
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
    //char* sigma_o = (char*) calloc(len + 1, sizeof(char));
    //memcpy(sigma_o, sigma, (size_t) len);

    dfa* table = (dfa*) calloc(1, sizeof(dfa));
    table->sigma = sigma_n;
    //table->orig = sigma_o;
    table->length = len;
    //table->width = (size_t) (len + 1);  // Width to use every time a new row is allocated

    // Allocate the 2D table array
    table->size = (size_t) 0;
    table->capacity = (size_t) 4;       // Init with space for 4 states

    table->data = (int**) calloc(table->capacity, sizeof(int*));

    return table;
}

// Creates and appends a new, empty state into the specified table
// Returns the pointer for insertion of state data
int* create_transition(dfa* table) {
    // Expand the table if need be (kinda like C++ vector)
    if (table->size == table->capacity) {
        int** data_n = (int**) realloc(table->data, 2 * sizeof(int*) * table->capacity);
        if (data_n) {
            //free(table->data);   // Supposedly not needed (leaving this as a reminder in case I'm wrong)
            table->data = data_n;
            table->capacity *= 2;
            //printf("Table capacity is now %d\n", (int) table->capacity);

        } else {
            fprintf(stderr, "Realloc failed (expansion of DFA table)\nTODO FREE MEMORY WHEN THIS HAPPENS\n");
            return NULL;
        }
    }

    // Create the new empty row
    int* row = calloc(table->length + 1, sizeof(int));
    *(table->data + table->size) = row;
    table->size++;

    // Populate with empty transitions
    for (int i = 1; i < table->length + 1; i++)
        row[i] = -1;

    return row;
}

// Prune unreachable/dead states and merge duplicate states
void optimize_table(dfa* table) {
    printf("TODO optimize_table()\n");
}

// Determine if the specified token is a part of the DFA's regular set
// Returns -1 if so, or the position of the first character in which the matching fails
// This function is technically not memory-safe!!
// However, it is incredibly unlikely that memory garbage will match a DFA for long
int match_token(dfa* table, char* token) {
    // Edge case to account for project requirements
    if (!token[0]) return 0;    // Empty string

    char tc;
    int tc_i;
    int* row;

    int length = 0;     // Length of match so far
    int state = 0;      // 0 is assumed entry state

    while ((tc = token[length])) {
        row = table->data[state];
        tc_i = find_char(tc, table->sigma, table->length);
        length++;
        //printf("Char: %c\n", tc);

        state = row[tc_i + 1];
        if (state < 0) return length;
        //printf("State: %d\n\n", state);
    }

    return (table->data[state][0] & 1) ? -1 : length + 1;
}

// Safely remove a table from memory if no longer needed
// Kinda vibes the same as destroying the death star
void destroy_table(dfa** table_p) {
    dfa* table = *table_p;

    // Free each row of the table
    for (int i = 0; i < table->size; i++)
        free(table->data[i]);

    // Free the outer table shell
    free(table->data);

    // Free transition char string
    free(table->sigma);
    //free(table->orig);

    // Free self
    free(table);
    *table_p = 0;
}
