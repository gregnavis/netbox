#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "config.h"

/* Reads a protocol name from an input line. */
static ssize_t read_protocol(const char *line, char *protocol, size_t protocol_size)
{
	const char *start, *end;
	ssize_t length;

	start = strrchr(line, ' ');
	if (start) {
		start++;
	} else {
		start = line;
	}

	end = strchr(start, ':');
	if (!end) {
		goto error;
	}

	length = end - start;
	if ((size_t) (length + 1) > protocol_size) {
		goto error;
	}

	strncpy(protocol, start, length);
	protocol[length] = '\0';

out:
	return length;

error:
	length = -1;
	goto out;
}

/* Sends a given input to a stdin of a program given by path and saves stdout
 * to output. */
static ssize_t redirect(const char *path,
		const char *input,
		size_t input_size,
		char *output,
		size_t output_size)
{
	int infd[2], outfd[2], status;
	ssize_t size;

	if (pipe(infd) == -1) {
		goto error;
	}

	if (pipe(outfd) == -1) {
		goto error_close_infd;
	}

	switch (fork()) {
	case 0:
		close(outfd[0]);
		close(infd[1]);
		dup2(outfd[1], STDOUT_FILENO);
		dup2(infd[0], STDIN_FILENO);
		execl(path, path, (char *) NULL);
		_exit(EXIT_FAILURE);

	case -1:
		goto error_close_outfd;
		break;

	default:
		close(infd[0]);
		close(outfd[1]);
		write(infd[1], input, input_size);
		close(infd[1]);
		size = read(outfd[0], output, output_size);
		close(outfd[0]);
		wait(&status);
		if (status != 0) {
			goto error;
		}

		if (size == -1) {
			goto error;
		}
		output[size] = '\0';
		break;
	}

out:
	return size;

error_close_outfd:
	close(outfd[0]);
	close(outfd[1]);

error_close_infd:
	close(infd[0]);
	close(infd[1]);

error:
	size = -1;
	goto out;
}

static void process_line(const char *line)
{
	char input[MAX_LINE_SIZE + 1], output[MAX_LINE_SIZE + 1], protocol[MAX_PROTOCOL_SIZE + 1], path[MAX_PATH_SIZE + 1];
	ssize_t size;

	if (strlen(line) > MAX_LINE_SIZE) {
		fprintf(stderr, "line too long\n");
		goto out;
	}

	strcpy(input, line);

	while (1) {
		if (read_protocol(input, protocol, sizeof(protocol)) == -1) {
			fprintf(stderr, "cannot read protocol\n");
			goto out;
		}

		strcpy(path, "nb");
		strcpy(path + 2, protocol);

		size = redirect(path, input, strlen(input), output, sizeof(output));

		if (size == -1) {
			fputs(input, stdout);
			break;
		} else {
			output[size] = '\0';
			strcpy(input, output);
		}
	}

out:
	return;
}

int main()
{
	char line[MAX_LINE_SIZE + 1];

	while (fgets(line, sizeof(line), stdin)) {
		process_line(line);
	}

	return 0;
}
