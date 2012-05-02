#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <getopt.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include "config.h"

static unsigned char pkt[PACKET_BUFFER_SIZE];
static size_t pktlen;
static int keep_flag = 0;

static void dumppkt(size_t offset)
{
	while (offset < pktlen) {
		printf("%02x", pkt[offset++]);
	}
}

static void printpkt(void)
{
	struct ethhdr *eth = (struct ethhdr *) pkt;
	printf("eth.dst=%02x:%02x:%02x:%02x:%02x:%02x ",
		eth->h_dest[0],
		eth->h_dest[1],
		eth->h_dest[2],
		eth->h_dest[3],
		eth->h_dest[4],
		eth->h_dest[5]);
	printf("eth.src=%02x:%02x:%02x:%02x:%02x:%02x ",
		eth->h_source[0],
		eth->h_source[1],
		eth->h_source[2],
		eth->h_source[3],
		eth->h_source[4],
		eth->h_source[5]);
	printf("eth.proto=%04x ", htons(eth->h_proto));
	dumppkt(sizeof(*eth));
}

static void error(const char *format, ...)
{
	va_list va;
	va_start(va, format);
	fprintf(stderr, "error: ");
	vfprintf(stderr, format, va);
	fputc('\n', stderr);
	va_end(va);
	exit(1);
}

int main(int argc, char *argv[])
{
	char buf[HEXDUMP_BUFFER_SIZE], *ptr;
	int i;

	while ((i = getopt(argc, argv, "k")) != -1) {
		switch (i) {
		case 'k':
			keep_flag = 1;
			break;

		default:
			exit(1);
			break;
		}
	}

	while (!feof(stdin) && fgets(buf, sizeof(buf), stdin)) {
		if (ferror(stdin)) {
			clearerr(stdin);
			perror("fgets");
			continue;
		}

		ptr = strchr(buf, '\n');
		if (ptr) {
			*ptr = '\0';
		} else if (!feof(stdin)) {
			error("line is longer than the maximum of %d characters\n", MAXIMUM_LINE_SIZE);
		}

		ptr = strrchr(buf, ' ');
		if (ptr) {
			if (keep_flag) {
				*ptr = '\0';
				printf("%s ", buf);
			}
			ptr++;
			memmove(buf, ptr, strlen(ptr) + 1);
		}

		for (pktlen = 0; buf[2 * pktlen] && buf[2 * pktlen + 1]; pktlen++) {
			sscanf(buf + 2 * pktlen, "%02x", &i);
			pkt[pktlen] = i;
		}

		printpkt();
		putchar('\n');
	}

	return 0;
}

