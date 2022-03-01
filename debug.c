#include <stdio.h>
#include <stdlib.h>

#include "src/list.h"
#include "src/stack.h"
#include "src/util.h"
#include "src/nfa.h"
#include "src/dfa.h"
#include "src/convert.h"

int main(int argc, char** argv) {
	nfa* testnfa = parse_file("automata/match-a.nfa");
	dfa* testdfa = convert_nfa(testnfa);

	print_container(testnfa);
	print_table(testdfa);
	optimize_table(testdfa);
	print_table(testdfa);

	destroy_container(&testnfa);
	destroy_table(&testdfa);
	return 0;
}
