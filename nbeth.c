#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "config.h"

static char hexdump[HEXDUMP_BUFFER_SIZE];
static char *hexdumpptr;

static void error(const char *format, ...)
{
	va_list valist;

	va_start(valist, format);
	vfprintf(stderr, format, valist);
	fputc('\n', stderr);
	va_end(valist);

	exit(1);
}

static void print_char(void)
{
	if (!*hexdumpptr) {
		error("no more characters");
	}
	putchar(*hexdumpptr++);
}

static void print_byte(void)
{
	print_char();
	print_char();
}

static void print_remaining(void)
{
	hexdumpptr += printf("%s", hexdumpptr);
}

static void print_packet(const char *format)
{
	for (; *format; format++) {
		if (*format != '%') {
			putchar(*format);
			continue;
		}

		switch(*(++format)) {
		case 'x':
			print_byte();
			break;

		default:
			error("invalid format specifier %c", *format);
			break;
		}
	}

	putchar(' ');
}

static void process_line(void)
{
	print_packet("eth.dst=%x:%x:%x:%x:%x:%x");
	print_packet("eth.src=%x:%x:%x:%x:%x:%x");
	print_packet("eth.typelen=%x%x");
	print_remaining();
}

int main()
{
	while (!feof(stdin)) {
		hexdumpptr = fgets(hexdump, sizeof(hexdump), stdin);
		if (ferror(stdin)) {
			clearerr(stdin);
			perror("fgets");
			continue;
		}

		process_line();
	}

	return 0;
}

