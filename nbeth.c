#define _BSD_SOURCE

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include "config.h"
#include "common.h"

const char *expected_protocol = "eth";

void process_packet(void)
{
	struct ethhdr *eth = (struct ethhdr *) packet;
	const char *protocol = value_to_name(eth_protocols, htons(eth->h_proto), "default");

	printf("eth.dst %s ", eth_ntoa(eth->h_dest));
	printf("eth.src %s eth.proto %d %s ",
			eth_ntoa(eth->h_source),
			htons(eth->h_proto),
			protocol);
	print_data(sizeof(*eth), protocol);
}

