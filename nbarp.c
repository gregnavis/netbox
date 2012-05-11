#define _BSD_SOURCE

#include <stdio.h>

#include <arpa/inet.h>
#include <net/if_arp.h>

#include "config.h"
#include "common.h"

static struct namevalue htype_names[] = {
	{ "ethernet", 1 },
	{ NULL, 0 },
};

static struct namevalue ptype_names[] = {
	{ "ip4", 0x0800 },
	{ NULL, 0 },
};

static struct namevalue op_names[] = {
	{ "request", 1 },
	{ "reply", 2 },
	{ NULL, 0 },
};

void printpkt(void)
{
	struct arphdr *arphdr = (struct arphdr *) pkt;
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
			value_to_name(ptype_names, ptype, "unknown"),
			arphdr->ar_hln,
			arphdr->ar_pln,
			op,
			value_to_name(op_names, op, "unknown"));

	dumppkt(sizeof(*arphdr));
}
