#define _BSD_SOURCE

#include <stdio.h>

#include <arpa/inet.h>
#include <netinet/ip.h>

#include "config.h"
#include "common.h"

const char *expected_protocol = "ip4";

static struct value_name protocols[] = {
	pair(IPPROTO_ICMP, "icmp"),
	pair(IPPROTO_TCP, "tcp"),
	pair(IPPROTO_UDP, "udp"),
	end_pair
};

void process_packet(void)
{
	struct ip *iphdr = (struct ip *) packet;
	const char *protocol = value_to_name(protocols, iphdr->ip_p, "unknown");

	printf("ip.version %u "
			"ip.ihl %u "
			"ip.tos %02x "
			"ip.length %u "
			"ip.id %u "
			"ip.flags %u%c%c "
			"ip.offset %u "
			"ip.ttl %u "
			"ip.protocol %u %s "
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
			protocol,
			iphdr->ip_sum,
			inet_ntoa(iphdr->ip_src));
	printf("ip.dst %s ", inet_ntoa(iphdr->ip_dst));
	print_data(iphdr->ip_hl * 4, protocol);
}

