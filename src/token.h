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
    int length;         // Current (or maximum) number of characters matched

	char* string;		// Pointer to start of token string, set at the start of a match
	// int failed;         // Length at which the match failed
} token;

token* create_token(char* name, char* path, char* sigma);
int advance_token(token* tk, char c);
token* match_tokens(token** tokens, int count, char* file);
// Something something match file...yeah
void reset_token(token* tk);
void reset_tokens(token** tokens, int count);
void output_token(token* tk, int fd, int l_no, int ch_no);	// Built specifically for LUTHER project
void destroy_token(token** tk);

#endif
