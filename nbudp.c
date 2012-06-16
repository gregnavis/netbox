#define _BSD_SOURCE

#include <stdio.h>

#include <netinet/udp.h>

#include "config.h"
#include "common.h"

const char *expected_protocol = "udp";

void process_packet(void)
{
	struct udphdr *udphdr = (struct udphdr *) packet;

	printf("udp.src %u "
			"udp.dst %u "
			"udp.length %u "
			"udp.check %04x ",
			udphdr->uh_sport,
			udphdr->uh_dport,
			udphdr->uh_ulen,
			udphdr->uh_sum);
	print_data(sizeof(*udphdr), "unknown");
}

