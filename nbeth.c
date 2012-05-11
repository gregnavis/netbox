#include <stdio.h>

#include <getopt.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include "config.h"
#include "common.h"

void printpkt(void)
{
	struct ethhdr *eth = (struct ethhdr *) pkt;
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
	printf("eth.proto %04x ", htons(eth->h_proto));
	dumppkt(sizeof(*eth));
}

