#define _BSD_SOURCE

#include <stdio.h>

#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ether.h>

#include "config.h"
#include "common.h"

const char *expected_protocol = "arp";

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
	struct ether_arp *arphdr = (struct ether_arp *) packet;
	int htype = htons(arphdr->ea_hdr.ar_hrd),
	    ptype = htons(arphdr->ea_hdr.ar_pro),
	    op = htons(arphdr->ea_hdr.ar_op);
	const char *protocol = value_to_name(op_names, op, "unknown");

	printf("arp.htype %04x %s "
			"arp.ptype %04x %s "
			"arp.hlen %u "
			"arp.plen %u "
			"arp.oper %u %s ",
			htype,
			value_to_name(htype_names, htype, "unknown"),
			ptype,
			value_to_name(eth_protocols, ptype, "unknown"),
			arphdr->ea_hdr.ar_hln,
			arphdr->ea_hdr.ar_pln,
			op,
			protocol);

	printf("arp.sha %s ", eth_ntoa(arphdr->arp_sha));
	printf("arp.spa %s ", ip4_ntoa(arphdr->arp_spa));
	printf("arp.tha %s ", eth_ntoa(arphdr->arp_tha));
	printf("arp.tpa %s ", ip4_ntoa(arphdr->arp_tpa));

	print_data(sizeof(*arphdr), protocol);
}

