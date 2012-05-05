#include <stdio.h>

#include "config.h"
#include "common.h"

void printpkt(void)
{
	size_t i;

	for (i = 0; i < pktlen; i++) {
		putchar(pkt[i]);
	}
}
