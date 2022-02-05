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
int str_sort(char* str);
int find_char(char tc, char* str, size_t len);
size_t calc_alloc_size(unsigned int size);

#endif // UTIL_H
