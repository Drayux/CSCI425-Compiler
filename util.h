#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 255 is max number of possible unique character values
// Not confident this is supposed to work across files?
const static size_t TC_RANGE = 256;

// Dynamic list object (basically a C++ vector) for sets of nodes
// Does not allow duplicates (to simulate a set)
typedef struct fa_list {
    size_t size;
    size_t capacity;
    int* data;          // Always points to a list of node IDs
    char sorted;
} list;

// Function declarations
int str_sort(char* str);
int find_char(char tc, char* str, size_t len);
size_t calc_alloc_size(unsigned int size);

// Specific to list
list* create_list();
size_t get_index(list* l, int x);
void print_list(list* l);
void expand_list(list* l);
void append(list* l, int x);
void insert(list* l, int x);
//void remove(list* l, size_t i);
list* sort(list* l);    // Insertion sort
int compare(list* left, list* right);
void destroy_list(list** l);

#endif // UTIL_H
