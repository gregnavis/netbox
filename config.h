// The default size is enought to hold a whole ethernet frame (<= 1524 bytes)
#ifndef PACKET_BUFFER_SIZE
#define PACKET_BUFFER_SIZE 2048
#endif

#define HEXDUMP_BUFFER_SIZE (2 * PACKET_BUFFER_SIZE)
