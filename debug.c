#include <stdio.h>
#include <stdlib.h>

#include "src/list.h"
#include "src/stack.h"
#include "src/util.h"
#include "src/nfa.h"
#include "src/dfa.h"
#include "src/convert.h"

int main(int argc, char** argv) {
	// char* str = "Hello\t world\n t\rhis is a  te\nst   ";
	// size_t len = strlen(str) + 1;
	// char* str2 = (char*) calloc(len, sizeof(char));
	// memcpy(str2, str, len * sizeof(char));
	// printf("'%s'\n", str2);
	//
	// clean(str2, 32);
	// printf("'%s'\n", str2);
	//
	// free(str2);
	// return 0;

	nfa* testnfa = parse_file("automata/match-a.nfa");
	dfa* testdfa = convert_nfa(testnfa);

	print_container(testnfa);
	print_table(testdfa);
	// optimize_table(testdfa);
	// print_table(testdfa);

	destroy_container(&testnfa);
	destroy_table(&testdfa);
	return 0;
}
