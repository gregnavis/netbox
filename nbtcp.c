#define _BSD_SOURCE

#include <stdio.h>

#include <netinet/tcp.h>

#include "config.h"
#include "common.h"

const char *expected_protocol = "tcp";

void process_packet(void)
{
	struct tcphdr *tcphdr = (struct tcphdr *) packet;

	printf("tcp.src %u "
			"tcp.dst %u "
			"tcp.seq %u "
			"tcp.ack %u "
			"tcp.offset %u "
			"tcp.flags %c%c%c%c%c%c "
			"tcp.window %u "
			"tcp.check %04x "
			"tcp.urg %u ",
			tcphdr->th_sport,
			tcphdr->th_dport,
			tcphdr->th_seq,
			tcphdr->th_ack,
			tcphdr->th_off,
			flag('u', tcphdr->th_flags & TH_URG),
			flag('a', tcphdr->th_flags & TH_ACK),
			flag('p', tcphdr->th_flags & TH_PUSH),
			flag('r', tcphdr->th_flags & TH_RST),
			flag('s', tcphdr->th_flags & TH_SYN),
			flag('f', tcphdr->th_flags & TH_FIN),
			tcphdr->th_win,
			tcphdr->th_sum,
			tcphdr->th_urp);
	print_data(sizeof(*tcphdr), "unknown");
}

