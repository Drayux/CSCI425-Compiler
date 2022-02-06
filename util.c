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

// Binary search (the sorted string) for the index of a character
// Returns -1 if char not found
int find_char(char tc, char* str, size_t len) {
    // Base case
    if (len <= 0) return -1;  // This should never be true
    if (len == 1) return (str[0] == tc) ? 0 : -1;

    // Compare the value
    int index = len / 2;
    char c = str[index];

    // Debug stuff
//    printf("Current string: %.*s\n", len, str);
//    printf("Checking index: %d (%c)\n", index, c);

    if (c == tc) return index;
    if (c > tc) {
        // Check left half
        return find_char(tc, str, index);

    } else {
        // Check right half
        int x = find_char(tc, str + index + 1, index - ((len % 2) ? 0 : 1));
        return (x < 0) ? x : (x + index + 1);
    }
}

// Split a string by a specified delimiter char
// Returns an array of null-terminated strings
// TODO create a memory-safe version of this
char** split(char* str, char delim, size_t* count) {
    int start = 0;
    int index = 0;
    int len = 0;
    char c;

    *count = 0;
    size_t capacity = 1;
    char** split_str = (char**) calloc(capacity, sizeof(char*));

    while (1) {
        c = str[index];
        //printf("%d: %c\n", index, c);
        if (c == delim || c == 0) {
            // Create new string
            len = index - start;
            if (len < 1) {
                start = index + 1;
                index++;
                continue;
            }

            char* ptr = (char*) calloc(len + 1, sizeof(char));
            memcpy(ptr, str + start, len);

            // -- Place the string into the array --
            // Expand if need be
            if (*count >= capacity) {
                char** split_str_n = (char**) realloc(split_str, 2 * sizeof(char*) * capacity);

                if (split_str_n) {
                    capacity *= 2;
                    split_str = split_str_n;

                } else {
                    fprintf(stderr, "Realloc failed (splitting string: %s into substrings)\n", str);
                    free(split_str);
                    return NULL;
                }
            }

            ptr[len] = 0;
            split_str[*count] = ptr;

            // Prepare for next loop
            start = index + 1;
            *count += 1;
        }

        // Exit if null terminator found
        if (c == 0) break;
        index++;
    }

    return split_str;
}

// Use some math to guess the size of an allocation
// Mostly used as a subroutine of NFA states
// This is arguably a bit hackjob, but these arrays are not intended for elements to be removed
size_t calc_alloc_size(unsigned int size) {
    size--;

    int shifts = 0;
    while (size > 0) {
        size = size >> 1;
        shifts++;
    }
    return pow(2, shifts);
}
