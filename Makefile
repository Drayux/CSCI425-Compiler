CC=clang
CFLAGS=-g -Wall -std=gnu17 -ggdb3 -fsanitize=address
INCL=-lreadline
SRCS=main.c util.c list.c dfa.c nfa.c
TARGET=NFAMATCH

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(INCL)

.PHONY: clean
clean:
	rm -f $(TARGET)
