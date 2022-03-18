#include <stdio.h>
#include <stdlib.h>

#include "src/list.h"
#include "src/stack.h"
#include "src/util.h"
#include "src/nfa.h"
#include "src/dfa.h"
#include "src/convert.h"

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
	char* str = "abababcdbbbce";

	token* tokens[2];
	tokens[0] = create_token("AAA", "tables/match-a.tt", sigma);
	tokens[1] = create_token("oogabooga", "tables/match-b.tt", sigma);

	return 0;
}
