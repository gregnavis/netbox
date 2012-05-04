#include <stdio.h>

#include <netinet/ip.h>

#include "config.h"
#include "common.h"

#define getbyte(value, byte) ((value >> (byte * 8)) & 0xFF)

void printpkt(void)
{
	struct iphdr *iphdr = (struct iphdr *) pkt;

	printf("ip.version=%d "
			"ip.ihl=%d "
			"ip.length=%d "
			"ip.id=%d "
			"ip.offset=%d "
			"ip.ttl=%d "
			"ip.protocol=%d "
			"ip.checksum=%04x "
			"ip.src=%d.%d.%d.%d "
			"ip.dst=%d.%d.%d.%d ",
			iphdr->version,
			iphdr->ihl,
			iphdr->tot_len,
			iphdr->id,
			iphdr->frag_off,
			iphdr->ttl,
			iphdr->protocol,
			iphdr->check,
			getbyte(iphdr->saddr, 0),
			getbyte(iphdr->saddr, 1),
			getbyte(iphdr->saddr, 2),
			getbyte(iphdr->saddr, 3),
			getbyte(iphdr->daddr, 0),
			getbyte(iphdr->daddr, 1),
			getbyte(iphdr->daddr, 2),
			getbyte(iphdr->daddr, 3));
	dumppkt(iphdr->ihl * 4);
}
