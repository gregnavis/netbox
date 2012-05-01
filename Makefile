CC?=gcc
CFLAGS?=-Wall -Wextra -Werror -std=c99

TOOLS=nbdump nbeth

.PHONY: all
all: $(TOOLS)

.PHONY: clean
clean:
	-rm $(TOOLS)

nbdump: nbdump.c config.h
	$(CC) $(CFLAGS) -o $@ $^

nbeth: nbeth.c config.h
	$(CC) $(CFLAGS) -o $@ $^
