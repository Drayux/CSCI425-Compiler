#include "token.h"

// Creates a new token associated with the DFA located at the specified filepath
token* create_token(char* name, char* path) {

}

// Matches a list of tokens simultaneously from an input string
// Returns the longest token possible
// Count is number tokens in the list of tokens
// NOTE: Offset of input string can be found using length of the final token
token* match_tokens(token** tokens, int count, char* file) {

}

// Attempt to match the next (specified in the param) character to the token's DFA
// Updates all respective fields
// Returns 0 if match failed, else 1
int advance_token(token* t, char c) {

}

// Reset tokens in a list in preparation for another match
// Primarily acts as a subroutine of match_tokens
void reset_tokens(token** tokens, int count) {

}

// Output relevent token data to the specified file descriptor
// Format is as specified in the requirements for LUTHER project
void output_token(token* token, int fd) {

}

// Clean up a token in memory
// Frees token name and destroys its transition table
void destroy_token(token** t) {

}
