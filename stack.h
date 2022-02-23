#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

typedef struct fa_stack {
	int data;
	struct fa_stack* next;
} stack;

void stack_push(stack** head, int data);
void print_stack(stack* head);
int stack_pop(stack** head);
void destroy_stack(stack** head);

#endif
