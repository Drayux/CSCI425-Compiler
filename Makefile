CC=clang
CFLAGS=-g -Wall -std=gnu17 -ggdb3 -fsanitize=address
#INCL=-lgetline
SRCS=main.c util.c list.c dfa.c nfa.c stack.c
TARGET=NFAMATCH

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@
	#$(CC) $(CFLAGS) $^ -o $@ $(INCL)

.PHONY: clean
clean:
	rm -f $(TARGET)
