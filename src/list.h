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
list* copy(list* orig);
list* intersect(list* a, list* b);
list* unite(list* a, list* b);
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

// Set state functions (list of lists)
void expand_ss(list*** ss, size_t* capacity);
int find_ss(list** ss, list* set, size_t size);
void destroy_ss(list*** ss, size_t capacity);

#endif // LIST_H



/* Some tests:
 *
 * list* a = create_list();
 * list* b = create_list();
 *
 * insert(a, 1);
 * insert(a, 2);
 * insert(a, 3);
 * insert(a, 4);
 *
 * insert(b, 3);
 * insert(b, 4);
 * insert(b, 5);
 *
 * list* is = intersect(a, b);
 * print_list(is);
 *
 * list* un = unite(a, b);
 * print_list(un);
 *
 * destroy_list(&a);
 * destroy_list(&b);
 * destroy_list(&is);
 * destroy_list(&un);
 *
 */
