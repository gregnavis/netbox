#define _BSD_SOURCE

#include <stdio.h>

#include <arpa/inet.h>
#include <net/if_arp.h>

#include "config.h"
#include "common.h"

static struct value_name htype_names[] = {
	pair(ARPHRD_ETHER, "eth"),
	end_pair
};

static struct value_name op_names[] = {
	pair(ARPOP_REQUEST, "request"),
	pair(ARPOP_REPLY, "reply"),
	end_pair
};

void process_packet(void)
{
	struct arphdr *arphdr = (struct arphdr *) packet;
	int htype = htons(arphdr->ar_hrd),
	    ptype = htons(arphdr->ar_pro),
	    op = htons(arphdr->ar_op);

	printf("arp.htype %04x %s "
			"arp.ptype %04x %s "
			"arp.hlen %u "
			"arp.plen %u "
			"arp.oper %u %s ",
			htype,
			value_to_name(htype_names, htype, "unknown"),
			ptype,
			value_to_name(eth_protocols, ptype, "unknown"),
			arphdr->ar_hln,
			arphdr->ar_pln,
			op,
			value_to_name(op_names, op, "unknown"));

	print_data(sizeof(*arphdr));
}

