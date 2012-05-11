#define _BSD_SOURCE

#include <stdio.h>

#include <arpa/inet.h>
#include <netinet/ip.h>

#include "config.h"
#include "common.h"

void printpkt(void)
{
	struct ip *iphdr = (struct ip *) pkt;

	printf("ip.version %u "
			"ip.ihl %u "
			"ip.tos %02x "
			"ip.length %u "
			"ip.id %u "
			"ip.flags %u%c%c "
			"ip.offset %u "
			"ip.ttl %u "
			"ip.protocol %u "
			"ip.checksum %04x "
			"ip.src %s ",
			iphdr->ip_v,
			iphdr->ip_hl,
			iphdr->ip_tos,
			iphdr->ip_len,
			iphdr->ip_id,
			iphdr->ip_off & IP_RF,
			flag('d', iphdr->ip_off & IP_DF),
			flag('m', iphdr->ip_off & IP_MF),
			iphdr->ip_off & IP_OFFMASK,
			iphdr->ip_ttl,
			iphdr->ip_p,
			iphdr->ip_sum,
			inet_ntoa(iphdr->ip_src));
	printf("ip.dst %s ", inet_ntoa(iphdr->ip_dst));
	dumppkt(iphdr->ip_hl * 4);
}
