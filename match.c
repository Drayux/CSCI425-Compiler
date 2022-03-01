#include <stdio.h>
#include <stdlib.h>

#include "src/convert.h"

int main(int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <input path> <output path> [test tokens...]\n", argv[0]);
		return -1;
	}

	char* inpath = argv[1];
	char* outpath = argv[2];

    nfa* container = parse_file(inpath);
	dfa* table = convert_nfa(container);

	optimize_table(table);
	output_table(table, outpath);

	// print_container(container);
	// print_table(table);

	// Match the tokens
	int result;
	if (argc > 3) printf("OUTPUT");
	for (int i = 3; i < argc; i++) {
		result = match_token(table, argv[i]);

		if (result < 0) printf(" :M:");
		else printf(" %d", result);
	}
	if (argc > 3) printf("\n");

	// Memory cleanup
    destroy_container(&container);
	destroy_table(&table);
    return 0;
}
