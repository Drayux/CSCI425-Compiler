#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 255 is max number of possible unique character values
// Not confident this is supposed to work across files?
const static size_t TC_RANGE = 256;

// Function declarations
int str_sort(char* str);

#endif // UTIL_H
