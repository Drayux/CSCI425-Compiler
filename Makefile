CC=clang
CFLAGS=-g -Wall -std=gnu17 -ggdb3 -fsanitize=address
#INCL=-lgetline
SRCS=util.c list.c dfa.c nfa.c stack.c convert.c
TARGET_DEBUG=DEBUG			# main() in debug.c
TARGET_CONVERT=CONVERT		# main() in match.c
TARGET_TOKEN=LUTHER			# main() in token.c

$(TARGET_DEBUG): debug.c $(foreach sf,$(SRCS),src/$(sf))
	$(CC) $(CFLAGS) $^ -o $@
	#$(CC) $(CFLAGS) $^ -o $@ $(INCL)

# $(TARGET_CONVERT): match.c $(foreach sf,$(SRCS),src/$(sf))
# 	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f $(TARGET_DEBUG) $(TARGET_CONVERT)
