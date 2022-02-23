#include "stack.h"

// Push a new node onto the head of a stack
// Pointer to head is replaced with address of new node
void stack_push(stack** head, int data) {
	stack* next = *head;
	stack* new = (stack*) calloc(1, sizeof(stack));

	new->data = data;
	new->next = next;

	*head = new;
}

// Output the contents of a stack type
void print_stack(stack* head) {
	printf("STACK:\n");
	(head) ? printf("%d ", head->data) : printf("EMPTY");
	head = head->next;

	while (head) {
		printf("==> %d ", head->data);
		head = head->next;
	}

	printf("\n");
}

// Pop a node off of the top of the stack
// NOTE: Calling this on something that is not the top of a stack
//       will DEFINITELY leak
int stack_pop(stack** head) {
	stack* node = *head;
	if (!node) {
		fprintf(stderr, "WARNING: Attempt to pop empty stack\n");
		return 0;
	}

	int data = node->data;

	*head = node->next;
	free(node);

	return data;
}

// Clean up a stack from memory recursively
void destroy_stack(stack** head) {
	stack* node = *head;

	// Base case
	if (!node) return;
	destroy_stack(&(node->next));
	free(node);

	*head = NULL;
}
