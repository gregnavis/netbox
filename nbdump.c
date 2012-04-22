#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <unistd.h>
#include <arpa/inet.h>

// The default size is enought to hold a whole ethernet frame (< 1524 bytes)
#ifndef PACKET_BUFFER_SIZE
#define PACKET_BUFFER_SIZE 2048
#endif

static void print_packet(const unsigned char *pkt, size_t pktlen)
{
	while (pktlen--) {
		printf("%02x", *pkt++);
	}

	putchar('\n');
}

int main(int argc, char *argv[])
{
	unsigned char pktbuf[PACKET_BUFFER_SIZE];
	int sockfd, res = 0;

	sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (-1 == sockfd) {
		perror("socket");
		goto error;
	}

	while (1) {
		ssize_t pktlen = read(sockfd, pktbuf, sizeof(pktbuf));
		if (pktlen < 0) {
			perror("read");
			continue;
		}
		if (pktlen > 0) {
			print_packet(pktbuf, pktlen);
		}
	}

	close(sockfd);

out:
	return res;

error:
	res = 1;
	goto out;
}
