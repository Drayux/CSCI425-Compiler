#include <stdio.h>
#include <stdlib.h>

#include "src/list.h"
#include "src/util.h"
#include "src/dfa.h"
#include "src/token.h"

// LUTHER-SPECIFIC FUNCTIONS (for now)

// TODO parse scan file

// TODO create array of character lengths to line count (index)

int main(int argc, char** argv) {
	if (argc < 4) {
		fprintf(stderr, "Usage: %s <definition file> <parser source> <parser output>\n", argv[0]);
		return -1;
	}

	printf("Beginning parse!\n");
	return 0;
}
