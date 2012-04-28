CC?=gcc
CFLAGS?=-Wall -Wextra -Werror -ansi

TOOLS=nbdump

.PHONY: all
all: $(TOOLS)

.PHONY: clean
clean:
	-rm $(TOOLS)

nbdump: nbdump.c config.h
	$(CC) -o $@ $^
