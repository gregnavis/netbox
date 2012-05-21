/* MAC address string size (12 digits + 5 colons + NULL). */
#define MAC_BUFFER_SIZE 18

/* Return the length of the array a. */
#define lengthof(a) (sizeof(a) / sizeof(*(a)))

/* Packet hexdump. */
extern char hexdump[MAX_HEXDUMP_SIZE];

/* Hexdump length in characters. */
extern size_t hexdump_size;

/* Packet binary data. */
extern unsigned char packet[MAX_PACKET_SIZE];

/* Packet length in bytes */
extern size_t packet_size;

/* Value to name mapping. */
struct value_name {
	int value;
	const char *name;
};

/* Return a single value to name mapping. */
#define pair(value, name) { (value), (name) }

/* A pair list terminator. */
#define end_pair { 0, NULL }

/* A protocol number to name mapping. */
extern struct value_name eth_protocols[];

/*
 * Convert a value to a name or return the default if the value is not mapped.
 *
 * vn should point to an array of value_name structures terminated by an entry
 * mapping 0 to NULL.
 */
const char *value_to_name(struct value_name *vn,
	int value,
	const char *default_);

/* Report a fatal error and terminate the program with code 1. */
void fatal(const char *format, ...);

/* Add a character to the hexdump. */
void add_to_hexdump(char c);

/* Print the hexdump to stdout. */
void print_hexdump(void);

/* Clear the hexdump. */
void reset_hexdump(void);

/* Add a byte to the packet. */
void add_to_packet(unsigned char byte);

/* Clear the packet. */
void reset_packet(void);

/* Convert the hexdump into a binary packet. */
void parse_hexdump(void);

/* Print upper-case symbol if value is non-zero, lower-case otherwise. */
char flag(char symbol, int value);

/* Convert an ethernet MAC address to a string. */
char *eth_ntoa(unsigned char mac[6]);

/* Print a hexdump of the packet data starting at the specified offset. */
void print_data(size_t offset);

/* todo */
void process_packet(void);
