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

// Creates a new list and returns its pointer
list* create_list() {
    list* l = (list*) malloc(sizeof(list));

    l->data = (int*) calloc(1, sizeof(int));
    l->size = 0;
    l->capacity = 1;
    l->sorted = 1;

    return l;
}

// Gets the index of a value
// Subroutine of insertion sort
// Will return location value should be inserted if not found
size_t get_index(list* l, int x) {
    // TODO CONTEMPLATE FUNCTIONALITY OF THIS
}

// Output the contents of a specified list
void print_list(list* l) {
    printf("[ ");
    for (int i = 0; i < l->size; i++)
        printf("%d ", l->data[i]);
    printf("]\n");
}

// Expand the given list if need be (in preparation for the insertion of an element)
void expand_list(list* l) {
    if (l->size >= l->capacity) {
        int* data_n = realloc(l->data, l->capacity * 2 * sizeof(int));
        if (data_n) {
            l->data = data_n;
            l->capacity *= 2;
        } else fprintf(stderr, "Realloc failed (expansion of dynamic list object)\n");
    }
}

// Place a value at the end of a specified list
void append(list* l, int x) {
    // Expand the list if need be
    expand_list(l);

    // Append the new element
    l->data[l->size] = x;

    if (l->sorted && l->size)    // Size > 0
        l->sorted = (x > l->data[l->size - 1]) 1 : 0;

    l->size++;
}

// Inserts a value into a a sorted list
// (Will append if list is not sorted)
void insert(list* l, int x) {
    // Append instead if the list isn't sorted
    if (!l->sorted) {
        append(l, x);
        return;
    }


}

// TODO: Remove the element at a specified index from the list
//void remove(list* l, size_t i);

// Sort a specified list
// Returns a new list and does not free the provided list
list* sort(list* l);

// Determine if two lists contain the same set of elements
// Takes O(n log n) time as both lists may need to be sorted
int compare(list* l, list* r) {

}


void destroy_list(list** l);
