#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

// Dynamic list object (basically a C++ vector) for sets of nodes
// Does not allow duplicates (to simulate a set)
typedef struct fa_list {
    size_t size;
    size_t capacity;
    int* data;          // Always points to a list of node IDs
    char sorted;
} list;

// 'Member' functions
list* create_list();
int find(list* l, int x);
int find_sorted(list* l, int x, int* index);
void print_list(list* l);
void expand_list(list* l);
void append(list* l, int x);
void insert(list* l, int x);
void insert_at(list* l, int x, int index);
//void remove(list* l, size_t i);
void sort(list* l);    // Insertion sort
int compare(list* left, list* right);
void destroy_list(list** l);

#endif // LIST_H
