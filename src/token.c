#include "token.h"

// Creates a new token associated with the DFA located at the specified filepath
token* create_token(char* name, char* path, char* sigma) {
	token* tk = (token*) calloc(1, sizeof(token));
	// TODO: Check if allocation fails

	// Attempt to create DFA
	dfa* table = parse_table(path, sigma);
	if (!table) {
		fprintf(stderr, "Failed to create token %s\n", name);
		return NULL;
	} tk->table = table;

	// Copy name string
	size_t len = strlen(name);
	tk->name = (char*) calloc(len + 1, sizeof(char));
	memcpy(tk->name, name, len * sizeof(char));
	tk->name[len] = 0;

	// String data (set this during file IO directly)
	tk->data = NULL;

	// Trivial data
	reset_token(tk);

	printf("Created token %s\n", name);
	return tk;
}

// Attempt to match the next (specified in the param) character to the token's DFA
// Updates all respective fields
// Returns 0 if match failed, else 1
int advance_token(token* tk, char c) {
	if (tk->state < 0) return 0;		// Match failed previously

	dfa* table = tk->table;
	int* row = table->data[tk->state];
	int tc_i = find_char(c, table->sigma, table->length);

	int state = row[tc_i + 1];
	tk->state = state;

	if (state < 0) return 0;

	tk->length += 1;
	if (table->data[state][0] & 1) {
		tk->accepting = 1;
		tk->failed = tk->length;

	} else tk->accepting = 0;

	return 1;
}

// Matches a list of tokens simultaneously from an input string
// Returns the longest token possible
// Count is number tokens in the list of tokens
// NOTE: Offset of input string can be found using length of the final token
token* match_tokens(token** tokens, int count, char* file) {
	int offset = 0;
	int matches = 1;
	token* tk;
	char c;
	// TODO maybe? Reset all tokens preemptively

	// Set token start of each match
	for (int i = 0; i < count; i++) tokens[i]->string = file;

	while (matches) {
		matches = 0;
		c = file[offset++];

		for (int i = 0; i < count; i++) {
			tk = tokens[i];
			matches += advance_token(tk, c);
		}
	}

	// Determine the best match once all have failed
	int index_max = 0;
	int length_max = 0;
	for (int i = 0; i < count; i++) {
		tk = tokens[i];
		if (tk->failed > length_max) {
			length_max = tk->failed;
			index_max = i;
		}
	}

	if (!length_max) return NULL;
	return tokens[index_max];
}

// Reset a single token
// Subroutine of create_token and reset_tokens: cleans some stuff up
void reset_token(token* tk) {
	tk->state = 0;
	tk->accepting = tk->table->data[0][0] & 1;
	tk->length = 0;
	tk->failed = 0;
	tk->string = NULL;
}

// Reset tokens in a list in preparation for another match
// Primarily acts as a subroutine of match_tokens
void reset_tokens(token** tokens, int count) {
	for (int i = 0; i < count; i++) reset_token(tokens[i]);
}

// Output relevent token data to the specified file descriptor
// Format is as specified in the requirements for LUTHER project
void output_token(token* tk, int fd, int l_no, int ch_no) {
	if (!tk) return;

	const size_t BUFSIZE = 24;
	char buf[BUFSIZE] = { 0 };		// For saving decimal values; 2^64 requires 20 places
	char sp = 32;					// SPACE character
	char nl = 10;					// NEWLINE character

	size_t name_len = strlen(tk->name);
	size_t data_len = (tk->data) ? strlen(tk->data) : (size_t) tk->failed;

	// Write the name
	write(fd, tk->name, name_len);
	write(fd, &sp, 1);

	// Write the token string
	if (tk->data) write(fd, tk->data, data_len);
	else write(fd, tk->string, data_len);
	write(fd, &sp, 1);

	// Write the file index
	size_t index_len = 0;

	index_len = snprintf(buf, BUFSIZE, "%d", l_no);
	write(fd, buf, index_len);
	write(fd, &sp, 1);

	index_len = snprintf(buf, BUFSIZE, "%d", ch_no);
	write(fd, buf, index_len);
	write(fd, &nl, 1);
}

// Clean up a token in memory
// Frees token name and destroys its transition table
void destroy_token(token** tk) {
	token* tmp = *tk;

	// Destroy members
	destroy_table(&(tmp->table));
	free(tmp->name);
	free(tmp);

	*tk = NULL;
}
