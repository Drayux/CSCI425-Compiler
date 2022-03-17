#ifndef TOKEN_H
#define TOKEN_H

#include "dfa.h"
#include "list.h"
#include "util.h"

typedef struct token_t {
    char* name;         // Name of the token as specified in the scanner definition file
    dfa* table;         // Transition table representing the token being matched

    int state;          // Current state within the DFA (as of last iteration)
    int accepting;      // Flag representing whether current state is accepting
    int failed;         // Length at which the match failed

    int length;         // Current (or maximum) number of characters matched
} token;

token* create_token(char* name, char* path);
token* match_tokens(token** tokens, int count, char* file);
int advance_token(token* t, char c);
void reset_tokens(token** tokens, int count);
void output_token(token* token, int fd);
void destroy_token(token** t);

#endif
