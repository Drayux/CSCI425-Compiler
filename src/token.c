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

// Parse scanner definition file and create the respective tokens
// Returns list of tokens and count (passed by reference)
token** parse_tokens(char* path, int* count) {
	FILE* inf = fopen(path, "r");

    // Check that the file opened successfully
    if (!inf) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        exit(1);        // Exit the program if file cannot be accessed, as per project requirements
    } printf("Parsing scanner definition from %s ...\n", path);

    // Using getline
    int lc = 0;
    char* linebuf = NULL;
    size_t buflen = 0;
    ssize_t nread;

    // Read the first line
    while (1) {
        nread = getline(&linebuf, &buflen, inf);
        lc++;

        if (nread == -1) {
            fprintf(stderr, "Scanner definition is empty\n");
            exit(1);    // Exit the program if file is empty, as per project requirements

        } else if (nread > 1) break;     // Ignore empty lines (does not account for CRLF)
    }

    // Parse first line
	char* sigma = calloc(nread + 1, sizeof(char));
	memcpy(sigma, linebuf, nread * sizeof(char));

	clean(sigma, 0);
    decode_string(sigma, 'x');
	str_sort(sigma, nread);			// Shouldn't be necessary but just in case

	// DEBUG
	// printf("Sigma:\n");
	// printf("'%s'\n", sigma);

	// Begin creating tokens
	size_t token_count = 0;
	size_t token_capacity = 4;
	token** tokens = calloc(token_capacity, sizeof(token*));

	char** line;
	size_t split_len;
	token* tk;
	while ((nread = getline(&linebuf, &buflen, inf)) != -1) {
        lc++;
        if (nread < 2) continue;

        // Split the new line
		clean(linebuf, ' ');
        line = split(linebuf, ' ', &split_len);
        if (split_len < 2) {
            // Invalid transition
            fprintf(stderr, "WARNING: Invalid token definition (line %d)\n", lc);
            free_split(&line, split_len);
			continue;
        }

		// DEBUG
		// printf("TEST: '%s'\n", linebuf);
		// for (int i = 0; i < split_len; i++)
		// 	printf("SPLIT %d: '%s'\n", i + 1, line[i]);

		// Create the new token and insert it into the array
		tk = create_token(line[1], line[0], sigma);
		if (split_len > 2) {
			decode_string(line[2], 'x');
			size_t data_len = strlen(line[2]);
			tk->data = (char*) calloc(data_len + 1, sizeof(char));
			memcpy(tk->data, line[2], data_len * sizeof(char));
		}

		if (token_count == token_capacity) {
			// Realloc array
			token_capacity *= 2;
			token** tokens_n = (token**) realloc(tokens, token_capacity * sizeof(token*));
			if (!tokens_n) {
	            fprintf(stderr, "Realloc failed (expansion of tokens list)\n");
				free_split(&line, split_len);
	            return tokens;
	        }
			tokens = tokens_n;
		}

		tokens[token_count++] = tk;
		free_split(&line, split_len);
	}

	// Cleanup
	free(linebuf);
	free(sigma);
	fclose(inf);

	*count = (int) token_count;
	return tokens;
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

	// Write the name
	write(fd, tk->name, name_len);
	write(fd, &sp, 1);

	// Write the token string
	char* data;
	size_t data_len = tk->failed;
	if (tk->data) {
		data = tk->data;
		data_len = strlen(data);
	}
	else data = tk->string;

	// Convert string back into alphabet encoding
	data = encode_string(data, data_len, 'x');
	data_len = strlen(data);
	write(fd, data, data_len);
	write(fd, &sp, 1);
	free(data);		// Cleanup right away

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
