CC?=gcc
CFLAGS?=-Wall -Wextra -Werror -std=c99

TOOLS=nbdump
COMMON_TOOLS=nbeth nbip4

.PHONY: all
all: $(TOOLS) $(COMMON_TOOLS)

.PHONY: clean
clean:
	-rm $(TOOLS) $(COMMON_TOOLS)

$(TOOLS): %: %.c config.h
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^)

$(COMMON_TOOLS): %: %.c common.c common.h config.h
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^)
