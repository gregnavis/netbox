#include <stdio.h>

#include "config.h"
#include "common.h"

const char *expected_protocol = NULL;

void process_packet(void)
{
	size_t i;

	for (i = 0; i < packet_size; i++) {
		putchar(packet[i]);
	}
}

