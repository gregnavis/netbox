CC?=gcc
CFLAGS?=-Wall -Wextra -Werror -ansi

TOOLS=nbdump nbeth

.PHONY: all
all: $(TOOLS)

.PHONY: clean
clean:
	-rm $(TOOLS)

nbdump: nbdump.c config.h
	$(CC) -o $@ $^

nbeth: nbeth.c config.h
	$(CC) -o $@ $^
