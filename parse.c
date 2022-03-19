#include <stdio.h>
#include <stdlib.h>

#include "src/list.h"
#include "src/util.h"
#include "src/dfa.h"
#include "src/token.h"

// LUTHER-SPECIFIC FUNCTIONS (for now)

int main(int argc, char** argv) {
	// -- SETUP --
	if (argc < 4) {
		fprintf(stderr, "Usage: %s <definition file> <parser source> <parser output>\n", argv[0]);
		return -1;
	}

	// Read the scan file
	int num_tokens = 0;
	token** token_list = parse_tokens(argv[1], &num_tokens);

	// Read the source file
	size_t src_len = 0;
	char* source = load_str(argv[2], &src_len);
	if (!src_len) {
		fprintf(stderr, "ERROR: Source file is empty\n");
		return 0;		// "User" error, by project requirements
	}

	// Attempt to open the output file
	int outfd;
	FILE* outf = fopen(argv[3], "w");
	if (!outf) {
		fprintf(stderr, "ERROR: Failed to open output file %s\n", argv[3]);
		return 1;
	} outfd = fileno(outf);

	list* char_positions = create_list();
	append(char_positions, 0);
	for (int i = 0; i < src_len; i++) {
		if (source[i] == '\n') append(char_positions, i + 1);
	} // print_list(char_positions);

	// -- CORE --
	printf("Beginning parse!\n");

	// Perform the scanning
	size_t offset = 0;		// Offset of source file pointer
	token* result;
	while (offset < src_len) {
		reset_tokens(token_list, num_tokens);
		result = match_tokens(token_list, num_tokens, source + offset);

		// Find the line number
		int lineno = -1;
		find_sorted(char_positions, offset + 1, &lineno);

		// Find the character number
		int charno = 1 + offset - char_positions->data[lineno - 1];

		output_token(result, outfd, lineno, charno);
		offset += result->failed;
	}

	// -- CLEANUP --
	fclose(outf);
	free(source);
	destroy_list(&char_positions);
	for (int i = 0; i < num_tokens; i++)
		destroy_token(token_list + i);

	return 0;
}
