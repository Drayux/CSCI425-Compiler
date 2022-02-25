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

/* Example Usage:
 *
 * stack* head = NULL;
 *
 * stack_push(&head, 4);
 * stack_push(&head, 6);
 * stack_push(&head, 7);
 * stack_push(&head, 1);
 *
 * print_stack(head);
 *
 * int x = 0;
 * x = stack_pop(&head);
 * printf("x: %d\n", x);
 *
 * x = stack_pop(&head);
 * printf("x: %d\n", x);
 *
 * print_stack(head);
 *
 * destroy_stack(&head);
 *
 */
