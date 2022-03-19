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
	//
	// printf("Length: %d\n", (int) len);
	// printf("String:\n");
	// printf("'%s'\n", str);

	// char str1[] = "Hello there \nworld! ";
	// char str2[] = "Hello there \nworld! ";
	// clean(str1, ' ');
	// clean(str2, 0);
	// printf("str1: '%s'\n", str1);
	// printf("str2: '%s'\n", str2);
	// snprintf(str2, 4, "x%02x", 255);
	// printf("sample: '%s'\n", str2);
	// char* test = encode_string(str1, strlen(str1), 'x');
	// printf("encoded: '%s'\n", test);
	// free(test);

	int count = 0;
	token** tks = parse_tokens("data/example.u", &count);

	for (int i = 0; i < count; i++) {
		output_token(tks[i], STDOUT_FILENO, 0, 0);
	}

	// char test[] = "x5cooox5cx20x5cooox5c";
	// printf("orig: '%s'\n", test);
	//
	// decode_string(test, 'x');
	// printf("decd: '%s'\n", test);
	//
	// char* test2 = encode_string(test, strlen(test), 'x');
	// printf("encd: '%s'\n", test2);
	// return 0;

	// token* tk;
	// char* sigma = "\n \\opqrs";
	// tk = create_token("whack", "data/tables/whackamole.tt", sigma);
	//
	// print_table(tk->table);
	// //token* result = match_tokens(tokens, 1, str);
	//
	// printf("Name: %s\n", tk->name);
	// printf("Length: %d\n", tk->failed);
	//
	// output_token(tk, STDOUT_FILENO, 0, 0);
	// destroy_token(&tk);

	return 0;
}
