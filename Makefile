CC=clang
CFLAGS=-g -Wall -std=gnu17 -ggdb3 -fsanitize=address
#INCL=-lgetline
SRCS=util.c list.c stack.c dfa.c nfa.c convert.c token.c
TARGET_DEBUG=DEBUG			# main() in debug.c
TARGET_CONVERT=CONVERT		# main() in match.c
TARGET_PARSE=LUTHER			# main() in parse.c (lexer)

$(TARGET_DEBUG): debug.c $(foreach sf,$(SRCS),src/$(sf))
# $(TARGET_CONVERT): match.c $(foreach sf,$(SRCS),src/$(sf))
# $(TARGET_PARSE): parse.c $(foreach sf,$(SRCS),src/$(sf))
	$(CC) $(CFLAGS) $^ -o $@
	#$(CC) $(CFLAGS) $^ -o $@ $(INCL)

# $(TARGET_CONVERT): match.c $(foreach sf,$(SRCS),src/$(sf))
# 	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f $(TARGET_DEBUG) $(TARGET_CONVERT) $(TARGET_TOKEN)
