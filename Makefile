CC?=gcc
CFLAGS?=-Wall -Wextra -Werror -std=c99

TOOLS=nbdump netbox
COMMON_TOOLS=nbeth nbip4 nbtcp nbudp nbarp nbprint

.PHONY: all clean test
.IGNORE: clean

all: $(TOOLS) $(COMMON_TOOLS)

clean:
	rm $(TOOLS) $(COMMON_TOOLS)

test:
	bats ./tests

$(TOOLS): %: %.c config.h
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^)

$(COMMON_TOOLS): %: %.c common.c common.h config.h
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^)
