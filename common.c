#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <getopt.h>

#include "config.h"
#include "common.h"

unsigned char pkt[PACKET_BUFFER_SIZE];
size_t pktlen;
int keep_flag = 0;

void dumppkt(size_t offset)
{
	while (offset < pktlen) {
		printf("%02x", pkt[offset++]);
	}
}

void error(const char *format, ...)
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

