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

/*
 * ==========================================================
 *   List functions
 * ==========================================================
 */

// Creates a new list and returns its pointer
list* create_list() {
    list* l = (list*) malloc(sizeof(list));

    l->data = (int*) calloc(1, sizeof(int));
    l->size = 0;
    l->capacity = 1;
    l->sorted = 1;

    return l;
}

// Find a value inside a list
// Will return -1 if not found
int find(list* l, int x) {
    // Linear search
    if (!l->sorted) {
        for (int i = 0; i < l->size; i++)
            if (l->data[i] == x) return i;
        return -1;
    }

    // Binary search
    int tmp;
    return find_sorted(l, x, &tmp);
}

// Find a value and store the last-checked index
// Subroutine of insert (and insertion sort)
int find_sorted(list* l, int x, int* index) {
    // Dummy check
    if (!l->sorted) {
        fprintf(stderr, "WARNING: Attempted binary search on unsorted list\n");
        return find(l, x);
    }

    int len = l->size;      // Number of elements left to compare
    int off = 0;
    int val = 0;
    *index = 0;

    while (len > 0) {
        val = l->data[*index];

        if (val == x) return *index;
        if (val < x) {
            // Check to the right
            len += off - (*index + 1);
            off = *index + 1;
            *index += (len / 2) + 1;
        } else {
            // Check to the left
            len = *index - off;
            *index -= (len  + 1) / 2;
        }
    }
    return -1;
}

// Output the contents of a specified list
void print_list(list* l) {
    printf("[");
    for (int i = 1; i < l->size; i++)
        printf("% d,", l->data[i - 1]);
    if (l->size > 0) printf(" %d ", l->data[l->size - 1]);
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
    // Duplicate check
    if (find(l, x) >= 0) return;

    // Expand the list if need be
    expand_list(l);

    // Append the new element
    l->data[l->size] = x;

    if (l->sorted && l->size)      // Size > 0
        l->sorted = (x > l->data[l->size - 1]) ? 1 : 0;

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

    // Insert the new element
    insert_at(l, x, -1);
}

// Inserts a value to a specific index within a list
// Subroutine of insertion sort
// Use index = -1 for sorted insert
void insert_at(list* l, int x, int index) {
    // Make sure a valid index is selected
    if (index > (int) l->size) {
        fprintf(stderr, "WARNING: Attempt to insert into list at an invalid index\n");
        return;
    }

    // Check for duplicates
    int exists;
    if (index < 0) exists = find_sorted(l, x, &index);
    else exists = find(l, x);

    if (exists >= 0) return;

    // Expand the list if need be
    expand_list(l);

    // Clear a spot
    for (int i = l->size - 1; i >= index; i--)
        l->data[i + 1] = l->data[i];

    // Check if the list is sorted
    // If the list isn't sorted, inserting an element won't magically sort it (so skip)
    if (l->sorted && l->size) {
        char sorted = 1;
        if (index > 0 && x <= l->data[index - 1]) sorted = 0;           // Check left element
        if (index < l->size && x >= l->data[index + 1]) sorted = 0;     // Check right element

        l->sorted = sorted;
    }

    // Insert the new element
    l->data[index] = x;
    l->size++;
}

// TODO: Remove the element at a specified index from the list
//void remove(list* l, size_t i);

// Sort a specified list in place
// Currently uses insertion sort
void sort(list* l) {
    if (l->sorted) return;

    size_t size = l->size;
    l->size = 0;
    l->sorted = 1;

    // Make a copy of the list
    int* copy = (int*) malloc(l->size * sizeof(int));
    for (int i = 0; i < size; i++)
        copy[i] = l->data[i];

    // Sort the list
    for (int i = 0; i < size; i++)
        insert(l, copy[i]);

    // Deallocate memory of duplicate
    free(copy);
}

// Determine if two lists contain the same set of elements
// Takes O(n log n) time as both lists may need to be sorted
int compare(list* l, list* r) {
    // Compare list sizes
    if (l->size != r->size) return 0;

    // Sort if necessary
//    if (!l->sorted) sort(l);
//    if (!r->sorted) sort(r);
    sort(l);
    sort(r);

    // Iterate and check elements
    // TODO determine if this could be done better by generating some form of hash
    for (int i = 0; i < l->size; i++)
        if (l->data[i] != r->data[i]) return 0;

    return 1;
}

// Frees a list from memory
void destroy_list(list** l) {
    list* list_n = *l;

    // Free list data
    free(list_n->data);

    // Free self
    free(list_n);
    *l = 0;
}
