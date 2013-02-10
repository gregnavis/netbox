#define _BSD_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include <linux/if_ether.h>

#include "config.h"
#include "common.h"

char hexdump[MAX_HEXDUMP_SIZE];
size_t hexdump_size = 0;
unsigned char packet[MAX_PACKET_SIZE];
size_t packet_size = 0;
int keep_flag = 0;


struct value_name eth_protocols[] = {
	pair(ETH_P_IP, "ip4"),
	pair(ETH_P_ARP, "arp"),
	pair(ETH_P_RARP, "rarp"),
	end_pair
};

const char *value_to_name(struct value_name *vn,
	int value,
	const char *default_)
{
	const char *name = default_;
	struct value_name *p;

	for (p = vn; p->name; p++) {
		if (p->value == value) {
			name = p->name;
			break;
		}
	}

	return name;
}

void fatal(const char *format, ...)
{
	va_list val;

	va_start(val, format);
	fprintf(stderr, "Fatal: ");
	vfprintf(stderr, format, val);
	fputc('\n', stderr);
	va_end(val);
	exit(1);
}

void add_to_hexdump(char c)
{
	if (hexdump_size == lengthof(hexdump)) {
		fatal("packet is longer than the maximum of %d bytes",
				MAX_PACKET_SIZE);
	}

	hexdump[hexdump_size++] = c;
	hexdump[hexdump_size] = '\0';
}

void print_hexdump(void)
{
	printf("%s", hexdump);
}

void reset_hexdump(void)
{
	hexdump_size = 0;
	hexdump[0] = '\0';
}

void add_to_packet(unsigned char byte)
{
	if (packet_size == lengthof(packet)) {
		fatal("packet is longer than the maximum of %d",
				lengthof(packet));
	}

	packet[packet_size++] = byte;
}

void reset_packet(void)
{
	packet_size = 0;
}

void parse_hexdump(void)
{
	char *p = strchr(hexdump, ':'), *protocol;
	int byte;

	if (p) {
		*p = '\0';
		p++;
		protocol = hexdump;
	} else {
		p = hexdump;
		protocol = NULL;
	}

	if (expected_protocol) {
		if (!protocol) {
			fatal("expected a protocol specifier");
		}

		if (strcmp(protocol, expected_protocol) != 0) {
			fatal("expected protocol %s instead of %s",
					expected_protocol,
					protocol);
		}
	}

	while (*p && *(p + 1)) {
		if (sscanf(p, "%02x", &byte) != 1) {
			fatal("invalid hexdump format");
		}
		p += 2;

		add_to_packet(byte);
	}

	if (!packet_size) {
		fatal("no data");
	}
}

char flag(char symbol, int value)
{
	return value ? toupper(symbol) : tolower(symbol);
}

char *eth_ntoa(unsigned char mac[6])
{
	static char buf[MAC_BUFFER_SIZE];
	sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1],
			mac[2], mac[3], mac[4], mac[5]);
	return buf;
}

char *ip4_ntoa(unsigned char ip[4])
{
	static char buf[IP4_BUFFER_SIZE];
	sprintf(buf, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
	return buf;
}

void print_data(size_t offset, const char *protocol)
{
	printf("%s:", protocol);
	while (offset < packet_size) {
		printf("%02x", packet[offset]);
		offset++;
	}
}

int main()
{
	int c = 0;

	while (c != EOF) {
		c = getchar();

		if ((c == EOF || c == '\n')) {
			if (hexdump_size) {
				parse_hexdump();
				process_packet();
				reset_packet();
				reset_hexdump();
				putchar('\n');
			} else if (c == '\n') {
				fatal("no protocol specifier");
			}
		} else {
			add_to_hexdump(c);
		}

		if (c == ' ') {
			print_hexdump();
			reset_hexdump();
		}
	}

	return 0;
}
