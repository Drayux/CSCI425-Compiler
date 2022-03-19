#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 255 is max number of possible unique character values
// Not confident this is supposed to work across files?
const static size_t TC_RANGE = 256;

// Function declarations
char* str_sort(char* str, int len);
int find_char(char tc, char* str, size_t len);
void clean(char* str, char rep);
int convert_hex(char* hex, size_t len);
void decode_string(char* str, char delim);
char* encode_string(char* orig, size_t len, char delim);
char** split(char* str, char delim, size_t* count);
void free_split(char*** str, size_t count);
char* load_str(char* path, size_t* len);
size_t calc_alloc_size(unsigned int size);

// TODO: Standardized function for parsing files (something with getline and static variables or somethin)

#endif // UTIL_H
