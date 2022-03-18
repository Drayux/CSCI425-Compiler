#include "dfa.h"

// Outputs a the provided DFA transition table
void print_table(dfa* table) {
    // Print the title
    printf("TRANSITION TABLE:\n");
    if (!table) {
        printf("NULL\n\n");
        return;
    }

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

    printf("\n");
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
    char* buf = (char*) calloc(24, sizeof(char));
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
        snprintf(buf, 24, "%d", i);         // 2^64 consists of 20 characters in base 10
        write(fd, buf, strnlen(buf, 24));

        // Write the transitions
        for (int j = 1; j <= table->length; j++) {
            int tr = row[j];
            write(fd, &space, 1);

            // Empty transition
            if (tr < 0) {
                *buf = 'E';
                write(fd, buf, 1);

            // Non-empty transition
            } else {
                snprintf(buf, 24, "%d", tr);
                write(fd, buf, strnlen(buf, 24));
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

// Read an transition table from a specified file
// Creates and returns a pointer to a new table with the respective definition
dfa* parse_table(char* path, char* sigma) {
    FILE* inf = fopen(path, "r");

    // Check that the file opened successfully
    if (!inf) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        exit(1);        // Exit the program if file cannot be accessed, as per project requirements
    } printf("Parsing table from %s ...\n", path);

    // Using getline
    int lc = 0;
    char* linebuf = NULL;
    size_t buflen = 0;
    ssize_t nread;

    dfa* table = create_table(sigma);

    char** line;
    size_t count;
    list* state_ids = create_list();
    while ((nread = getline(&linebuf, &buflen, inf)) != -1) {
        lc++;
        if (nread < 2) continue;

        // Prepare linebuf for parsing
        clean(linebuf, ' ');

        // Split the new line
        line = split(linebuf, ' ', &count);
        if (count < table->length + 2) {
            // Invalid transition
            fprintf(stderr, "WARNING: Invalid transition (line %d)\n", lc);
            free_split(&line, count);
			continue;
        }

        // -- Parse the transition --
        int index;
        int s_id = atoi(line[1]);
        if ((index = find(state_ids, s_id)) < 0) {
            create_transition(table);
            append(state_ids, s_id);
            index = state_ids->size - 1;
        }

        // TODO: Integrity check
        // Needs to keep a list of rows created because they can be added out of order
        // Currently later transitions will just overwrite prior transitions

        int* row = table->data[index];

        // State flags
        int flags = !s_id * 2;
        if (line[0][0] == '+') flags++;

        int t_id;
        char* tn;
        row[0] = flags;
        for (int i = 1; i <= table->length; i++) {
            tn = line[i + 1];
            if (tn[0] == 'E') continue;         // No transition, skip

            // Find ID as index of transition table
            t_id = atoi(tn);                    // TODO: Make sure this is a valid thing to convert
            if (find(state_ids, t_id) < 0) {
                create_transition(table);
                append(state_ids, t_id);
            }

            t_id = find(state_ids, t_id);
            row[i] = t_id;
        }
        free_split(&line, count);
    }

    // If the table has no rows, then the file must have been empty
    if (table->size < 1) {
        fprintf(stderr, "Transition table file is invalid or empty\n");
        destroy_table(&table);
    }

    // Memory cleanup
    fclose(inf);
    free(linebuf);
    destroy_list(&state_ids);

    return table;
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

// Remove a row from the transition table
// ID is index of row to remove
// REP is value to replace transitions (for use as subroutine of merge_states)
void remove_transition(dfa* table, int id, int rep) {
    if (id >= table->size) {
        fprintf(stderr, "WARNING: Attempt to remove transition row that does not exist\n");
        return;
    }

    // Decrease size and shift subsequent rows
    table->size--;
    free(table->data[id]);
    for (int i = id; i < table->size; i++)
        table->data[i] = table->data[i + 1];

    // Update transition values
    int tr;
    for (int i = 0; i < table->size; i++) {
        for (int j = 1; j <= table->length; j++) {
            tr = table->data[i][j];
            if (tr == id) table->data[i][j] = rep;
            else if (tr > id) table->data[i][j]--;
        }
    }
}

// void update_transition(dfa* table, ...) {}

// Merge duplicate states within a DFA
// Returns -1 if something went wrong
//          0 if no changes were made
//          1 if changes were made
int merge_states(dfa* table) {
    if (!table) return -1;

    int size = (int) table->size;          // Length of transition table
    size_t ss_cap = 8;                  // State set list capacity
    size_t ss_size = 2;                 // Setup will add first two elements
    list** state_sets = (list**) calloc(ss_cap, sizeof(list*));

    stack* states = NULL;               // Pseudocode L (S) ; Indexed as index of state_sets
    stack* tchars = NULL;               // Pseudocode L (C) ; Indexed as offset of sigma
    list* merge_sets = create_list();    // Pseudocode M

    state_sets[0] = create_list();      // Accepting states
    state_sets[1] = create_list();      // Non-accepting states

    // Setup
    for (int i = 0; i < table->size; i++) {
        int flags = table->data[i][0];
        if (flags & 1) insert(state_sets[0], i);
        else insert(state_sets[1], i);
    }

    stack_push(&states, 0);
    stack_push(&tchars, 0);

    stack_push(&states, 1);
    stack_push(&tchars, 0);

    // Create the merge set
    int state_i;
    int sigma_i;
    int tr_i;
    while (states) {
        list** splits = (list**) calloc(size + 1, sizeof(list*));    // Segregated states (guaranteed no more sets that unique states)
        for (int i = 0; i < size + 1; i++) splits[i] = create_list();

        state_i = stack_pop(&states);
        sigma_i = stack_pop(&tchars);

        // Segregate states
        list* set = state_sets[state_i];
        for (int i = 0; i < set->size; i++) {
            int row_i = set->data[i];
            int* row = table->data[row_i];

            tr_i = row[sigma_i + 1];
            if (tr_i < 0) tr_i = size;

            insert(splits[tr_i], row_i);
        }

        // Iterate through state groups (grouped by transition)
        for (int i = 0; i < size + 1; i++) {
            list* split = splits[i];
            if (split->size < 2) continue;

            // Add state as a state set
            int ss_i = find_ss(state_sets, split, ss_size);
            if (ss_i < 0) {
                // Does not exist: Copy and insert
                if (ss_size >= ss_cap) expand_ss(&state_sets, &ss_cap);
                state_sets[ss_size] = copy(split);
                ss_i = ss_size;
                ss_size++;
            }

            // Debug output
            // printf("SPLITS:\n");
            // for (int i = 0; i < size + 1; i++) {
            //     printf("%d: ", i);
            //     print_list(splits[i]);
            // }
            // printf("\n");

            // Update data structs
            if ((sigma_i + 1) >= table->length) insert(merge_sets, ss_i);
            else {
                stack_push(&states, ss_i);
                stack_push(&tchars, sigma_i + 1);
            }
        }
        // Cleanup
        destroy_ss(&splits, size + 1);
    }

    // Debug output
    // printf("STATE SETS:\n");
    // for (int i = 0; i < ss_size; i++) {
    //     list* set = state_sets[i];
    //     print_list(set);
    // } printf("\n");
    // printf("MERGE SETS:\n");
    // for (int i = 0; i < merge_sets->size; i++) print_list(state_sets[merge_sets->data[i]]);
    // printf("\n");

    // Perform the merge
    for (int i = 0; i < merge_sets->size; i++) {
        // Merge every element of each set
        list* set = state_sets[merge_sets->data[i]];
        int merge_to = set->data[0];
        for (int j = 1; j < set->size; j++)
            remove_transition(table, set->data[j] + 1 - j, merge_to);
    }

    int ret = (merge_sets->size) ? 1 : 0;

    // -- MEMORY CLEANUP --
    destroy_list(&merge_sets);
    destroy_ss(&state_sets, ss_cap);

    return ret;
}

// Recursive subroutine of optimize_table to remove dead states
// Initial call should be performed with id = 0
void prune_states(dfa* table) {
    size_t size = table->size;
    list** tree = (list**) calloc(size, sizeof(list*));
    for (int i = 0; i < size; i++) tree[i] = create_list();

    // Build graph a traversal tree
    int row_i;
    int* row;
    int queue_i = 0;
    list* queue = create_list();
    list* visited = create_list();
    append(queue, 0);       // 0 is implied starting state
    insert(visited, 0);     // ^^
    while (queue_i < queue->size) {
        row_i = queue->data[queue_i];
        row = table->data[row_i];
        queue_i++;

        // Append every state with a new transition
        // Update parent tree
        for (int i = 1; i <= table->length; i++) {
            int tr = row[i];
            if (tr < 0) continue;

            insert(tree[tr], row_i);
            if (find(visited, tr) < 0) {
                insert(visited, tr);
                append(queue, tr);
            }
        }
    }

    // Debug output
    // printf("TRAVERSAL TREE:\n");
    // for (int i = 0; i < size; i++) {
    //     printf("Row %d: ", i);
    //     print_list(tree[i]);
    // } printf("\n");

    // Create set of accepting states
    list* accepts = create_list();
    for (int i = 0; i < size; i++) {
        row = table->data[i];
        if (row[0] & 1) insert(accepts, i);
    }

    // Edge case handling
    if (!accepts->size) {
        fprintf(stderr, "WARNING: DFA has no accepting states\n");
        goto cleanup;
    }

    // Traceback on traversal tree and determine dead states (setup)
    destroy_list(&visited);
    visited = copy(accepts);

    int* dead = (int*) calloc(size, sizeof(int));  // 0 : bad ; 1 : good
    stack* traverse = NULL;
    dead[0] = 1;       // 0 is implied starting state

    for (int i = 0; i < accepts->size; i++) {
        int tr = accepts->data[i];
        dead[tr] = 1;
        stack_push(&traverse, tr);
    }

    // Actual traceback step
    while (traverse) {
        int state = stack_pop(&traverse);

        list* parents = tree[state];
        for (int i = 0; i < parents->size; i++) {
            int tr = parents->data[i];
            if (find(visited, tr) >= 0) continue;

            dead[tr] = 1;
            insert(visited, tr);
            stack_push(&traverse, tr);
        }
    }

    // More debug output
    // printf("DEAD TREE:\n");
    // for (int i = 0; i < size; i++) {
    //     printf("Row %d: %d\n", i, dead[i]);
    // } printf("\n");

    // Prune dead/unreachable states (according to dead array)
    for (int i = size - 1; i > 0; i--)
        if (!dead[i]) remove_transition(table, i, -1);

    // -- MEMORY CLEANUP --
    free(dead);
    cleanup:
    destroy_list(&accepts);
    destroy_list(&queue);
    destroy_list(&visited);
    destroy_ss(&tree, size);
}

// Prune unreachable/dead states and merge duplicate states
void optimize_table(dfa* table) {
    if (!table) goto fail;

    int success;
    while ((success = merge_states(table)))
        if (success < 0) goto fail;

    // -- STATE PRUNING --
    prune_states(table);
    return;

    // Error handling
    fail:
    fprintf(stderr, "WARNING: Attempt to merge invalid DFA\n");
}

// Determine if the specified token is a part of the DFA's regular set
// Returns -1 if so, or the position of the first character in which the matching fails
// This function is technically not memory-safe!!
// However, it is incredibly unlikely that memory garbage will match a DFA for long
int match_token(dfa* table, char* tk) {
    char tc;
    int tc_i;
    int* row;

    int length = 0;     // Length of match so far
    int state = 0;      // 0 is assumed entry state

    while ((tc = tk[length])) {
        row = table->data[state];
        tc_i = find_char(tc, table->sigma, table->length);
        length++;
        //printf("Char: %c\n", tc);

        state = row[tc_i + 1];
        if (state < 0) return length;
        //printf("State: %d\n\n", state);
    }

    if (tk[0]) length++;     // Edge case to account for project requirements
    return (table->data[state][0] & 1) ? -1 : length;
}

// Safely remove a table from memory if no longer needed
// Kinda vibes the same as destroying the death star
void destroy_table(dfa** table_p) {
    dfa* table = *table_p;
    if (!table) return;

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
    *table_p = NULL;
}
