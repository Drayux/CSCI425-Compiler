#include <stdio.h>
#include <stdlib.h>

#include "src/list.h"
#include "src/stack.h"
#include "src/util.h"
#include "src/nfa.h"
#include "src/dfa.h"
#include "src/convert.h"
#include "src/token.h"

int main(int argc, char** argv) {
	// dfa* testdfa = parse_table("tables/match-a.tt", "abcdefg");
	// print_table(testdfa);
	// destroy_table(&testdfa);

	// size_t len;
	// char* str = load_str("test", &len);

	// printf("Length: %d\n", (int) len);
	// printf("String:\n");
	// printf("'%s'\n", str);

	char* sigma = "abdce";
	char* str = "cabababcdbbbce";

	token* tokens[1];
	tokens[0] = create_token("AAA", "tables/match-a.tt", sigma);
	// tokens[1] = create_token("oogabooga", "tables/match-b.tt", sigma);

	print_table(tokens[0]->table);
	token* result = match_tokens(tokens, 1, str);

	printf("Name: %s\n", result->name);
	printf("Length: %d\n", result->failed);
	// printf(" Match: %s\n", result->string);

	output_token(result, STDOUT_FILENO, 0, 0);
	destroy_token(tokens);

	return 0;
}
