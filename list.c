#include "list.h"

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

// Creates a new list as a copy of another
list* copy(list* orig) {
    if (!orig) return NULL;

    list* new = (list*) malloc(sizeof(list));
    new->size = orig->size;
    new->capacity = orig->capacity;
    new->sorted = orig->sorted;

    // Copy the data
    new->data = (int*) calloc(new->capacity, sizeof(int));
    for (int i = 0; i < new->size; i++)
        new->data[i] = orig->data[i];

    return new;
}

// Creates a new list representing intersection of two list sets
list* intersect(list* a, list* b) {
    if (!a || !b) return NULL;

    list* ret = create_list();
    for (int i = 0; i < a->size; i++) {
        int val = a->data[i];
        if (find(b, val) >= 0) insert(ret, val);
    }

    return ret;
}

// Creates a new list representing the union of two list sets
list* unite(list* a, list* b) {
    if (!a || !b) return NULL;

    // Basically merge sort
    // int a_i = 0;
    // int b_i = 0;
    // int size = a->size + b->size;
    // for (int i = 0; i < size; i++) {}
    // (Nevermind, not worth the trouble rn)

    list* ret = copy(a);
    for (int i = 0; i < b->size; i++) {
        int val = b->data[i];
        if (find(ret, val) < 0) insert(ret, val);
    }

    return ret;
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
    if (!l) {
        printf("NULL\n");
        return;
    }

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

        } else {
            fprintf(stderr, "Realloc failed (expansion of dynamic list object)\n");
            free(l->data);
        }
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
    if (!list_n) return;

    // Free list data
    free(list_n->data);

    // Free self
    free(list_n);
    *l = NULL;
}
