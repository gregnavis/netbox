extern unsigned char pkt[PACKET_BUFFER_SIZE];
extern size_t pktlen;
extern int keep_flag;

void dumppkt(size_t offset);
void error(const char *format, ...);
void printpkt(void);
char flag(char name, int value);
