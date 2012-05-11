#include <stdio.h>

#include <getopt.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include "config.h"
#include "common.h"

static struct namevalue proto_names[] = {
	{ "ip4", 0x0800 },
	{ "arp", 0x0806 },
	{ "rarp", 0x8035 },
	{ NULL, 0 },
};

void printpkt(void)
{
	struct ethhdr *eth = (struct ethhdr *) pkt;
	int proto = htons(eth->h_proto);

	printf("eth.dst %02x:%02x:%02x:%02x:%02x:%02x ",
		eth->h_dest[0],
		eth->h_dest[1],
		eth->h_dest[2],
		eth->h_dest[3],
		eth->h_dest[4],
		eth->h_dest[5]);
	printf("eth.src %02x:%02x:%02x:%02x:%02x:%02x ",
		eth->h_source[0],
		eth->h_source[1],
		eth->h_source[2],
		eth->h_source[3],
		eth->h_source[4],
		eth->h_source[5]);
	printf("eth.proto %04x %s ",
			proto,
			value_to_name(proto_names, proto, "unknown"));
	dumppkt(sizeof(*eth));
}

