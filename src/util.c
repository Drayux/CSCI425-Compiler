#include "util.h"

// Sort a string and check for duplicates
// len is length of string to sort
// (Utility function for create_table & create_state; uses a modified counting sort)
// Returns NULL if duplicate is found else a new sorted string
// TODO: replace this with radix sort if adding support for unicode character set
char* str_sort(char* str, int len) {
    char* input = NULL;
    int* counts = NULL;

    if (!len) len = strnlen(str, TC_RANGE);
    if (len < 2) {
        // String is (technically) already sorted
        input = calloc(len + 1, sizeof(char));
        input[0] = str[0];
        return input;
    }

    counts = calloc(TC_RANGE, sizeof(int));

    // Total the counts
    for (int i = 0; i < len; i++) {
        if (counts[(unsigned int) str[i]]++)
            goto str_sort_ret;
    }

    // Adjust the counts array
    for (int i = 1; i < TC_RANGE; i++)
        counts[i] += counts[i - 1];

    input = malloc(sizeof(char) * (len + 1));
    memcpy(input, str, (size_t) len + 1);
    input[len] = 0;     // Possible to sort fewer characters than actual string length

    // Reorder input string
    for (int i = 0; i < len; i++) {
        unsigned int x = str[i];
        unsigned int y = --counts[x];
        input[y] = (char) x;
    }

    str_sort_ret:
    free(counts);
    return input;
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

// Replaces all ASCII whitespace chars (excl '\0') with the char specified in rep
// NOTE: String must be null-terminated
// NOTE: Input string must be mutable
// NOTE: Currently removes all extended characters
// No return value: Works in-place
void clean(char* str, char rep) {
    char c;
    char skip = 0;          // 1 if next whitespace should be skipped

    size_t len = 0;
    size_t capacity = strlen(str);

    for (int i = 0; i < capacity; i++) {
        c = str[i];

        if (c > 127) continue;
        if (c < 33 || c == 127) {
            if (skip) continue;

            c = rep;
            skip = 1;

        } else skip = 0;

        str[len++] = c;
    }

    // Determine where to place the null terminator
    if (skip) len--;
    str[len++] = 0;
}

// Converts an array of chars into their respective integer value
// 'len' specifies length of array
// Partial credit to stackoverflow post: https://stackoverflow.com/questions/10156409/convert-hex-string-char-to-int
int convert_hex(char* hex, size_t len) {
    int mult = 1;
    int total = 0;
    char c;
    for (int i = len - 1; i >= 0; i--) {
        c = hex[i];

        if (c >= '0' && c <= '9') c = c - '0';
        else if (c >= 'a' && c <='f') c = c - 'a' + 10;
        else if (c >= 'A' && c <='F') c = c - 'A' + 10;
        else return -1;

        // Though slower than bit shifts, this accounts for 'endian-ness'
        total += c * mult;
        mult *= 16;
    }
    return total;
}

// Replace x## tokens in strings with the indicated character
// NOTE: String must be null-terminated
// NOTE: Input string must be mutable
// No return value: Works in-place
void convert_encoded(char* str, char delim) {
    char c;
    char hex[2];
    int len = 0;        // Current index of the revised string

    for (int i = 0; (c = str[i]); i++) {
        if (c == delim) {
            hex[0] = str[i + 1];
            hex[1] = str[i + 2];

            int result = convert_hex(hex, 2);
            if (result >= 0) {
                str[len++] = (char) result;
                i += 2;
                continue;
            }
        }
        // Assume c is a normal character
        str[len++] = c;
    }

    //  Update null terminator
    str[len] = 0;
}

// Split a string by a specified delimiter char
// NOTE: String must be null-terminated
// NOTE: '/0' cannot be used as the delimiter
// Returns an array of null-terminated strings
char** split(char* str, char delim, size_t* count) {
    int start = 0;
    int index = 0;
    int len = 0;
    char c;

    *count = 0;
    size_t capacity = 1;
    char** split_str = (char**) calloc(capacity, sizeof(char*));

    while (1) {
        c = str[index];

        if (!!c * (c - delim)) {
            index++;
            continue;
        }

        // Character is a splitting character
        len = index - start;

        if (len < 1) {
            if (!c) break;          // Ignore trailing delimiter chars
            else {
                start = ++index;    // Move start forward to the next char
                continue;
            }
        }

        char* ptr = (char*) calloc(len + 1, sizeof(char));
        memcpy(ptr, str + start, len);

        // -- Place the string into the array --
        if (*count >= capacity) {
            // Expand if need be
            char** split_str_n = (char**) realloc(split_str, 2 * sizeof(char*) * capacity);

            if (split_str_n) {
                capacity *= 2;
                split_str = split_str_n;

            } else {
                fprintf(stderr, "Realloc failed (splitting string: %s into substrings)\n", str);
                free(split_str);
                return NULL;
            }
        }

        ptr[len] = 0;
        split_str[*count] = ptr;

        // Prepare for next loop
        *count += 1;
        if (!c) break;              // Exit loop if null terminator found

        start = ++index;
    }
    return split_str;
}

// Free memeory allocated by a split string
void free_split(char*** str, size_t count) {
	char** splitstr = *str;

	for (int i = 0; i < count; i++) free(splitstr[i]);
	free(splitstr);

	str = NULL;
}

// Copies contents of a file at the specifed path into a c-string
// Will always return a new string of unspecified length
// Partial credit to: https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
char* load_str(char* path, size_t* len) {
    FILE* inf = fopen(path, "r");
    char* out = calloc(1, sizeof(char));    // Empty string
    *len = 1;

    // Check that the file opened successfully
    if (!inf) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return out;
    } printf("Parsing tokens from %s ...\n", path);

    // Determine file length (without stat)
    if (fseek(inf, 0, SEEK_END)) return out;
    unsigned long length = (unsigned long) ftell(inf);
    if (fseek(inf, 0, SEEK_SET) || length < 0 /*|| length >= SIZE_MAX*/) return out;

    size_t length_s = (size_t) length;
    char* out_n = (char*) realloc(out, (length_s + 1) * sizeof(char));

    if (!out_n) {
        fprintf(stderr, "Realloc failed (reading token file to memory)\n");
        return out;
    }

    out = out_n;
    out[length_s] = 0;

    // Read in the file
    fread(out, sizeof(char), length_s, inf);
    fclose(inf);

    *len = length_s;
    return out;
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
