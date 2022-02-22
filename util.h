#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 255 is max number of possible unique character values
// Not confident this is supposed to work across files?
const static size_t TC_RANGE = 256;

// Function declarations
char* str_sort(char* str, int len);
int find_char(char tc, char* str, size_t len);
char** split(char* str, char delim, size_t* count);
void free_split(char*** str, size_t count);
size_t calc_alloc_size(unsigned int size);

#endif // UTIL_H
