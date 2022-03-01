#ifndef CONVERT_H
#define CONVERT_H

#include "list.h"
#include "stack.h"
#include "util.h"
#include "nfa.h"
#include "dfa.h"

list* follow_character(nfa* container, list* start, int char_i);
void follow_lambda(nfa* container, list* set);
dfa* convert_nfa(nfa* input);

#endif
