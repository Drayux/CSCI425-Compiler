#include "util.h"

// Sort a string (in place) and check for duplicates
// (Utility function for create_table & create_state; uses a modified counting sort)
// Returns 0 if duplicate is found (else the length of the string)
// TODO: replace this with radix sort if adding support for unicode character set
int str_sort(char* str) {
    int len = strnlen(str, TC_RANGE);
    if (len < 2) return len;  // String is (technically) already sorted

    int* counts = calloc(TC_RANGE, sizeof(int));
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
    for (int i = 1; i < TC_RANGE; i++)
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
