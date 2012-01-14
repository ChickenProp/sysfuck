#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "sysfuck.h"
#include "callbacks.h"

buf_t buf;
char *data = buf.data;
FILE *sf_in;
FILE *sf_out;

int main (int argc, char **argv) {
	g_argc = argc;
	g_argv = argv;

	sf_in = fdopen(3, "r");
	sf_out = fdopen(4, "a");

	if (!sf_in || !sf_out) {
		fprintf(stderr,
		        "Error: sysfuck needs to be able to read from file "
		        "descriptor 3 and write to file descriptor 4.\n"
		        "You probably want to be using `sfwrap` instead.\n");

		exit(1);
	}

	char *callname = NULL;
	size_t n = 0;

	while (1) {
		int c;
		while ((c = getc(sf_in))) {
			if (c == EOF)
				return 0;
			else
				write(prog_stdout, &c, 1);
		}

		int bytesread = getdelim(&callname, &n, 0, sf_in);
		if (bytesread == -1)
			return 0;
		else if (bytesread == 1) {
			char null = 0;
			write(prog_stdout, &null, 1);
			continue;
		}

		callback_t f = getcallback(callname);
		int n = getarg(data);
		(*f)(callname, n, &buf);
		fflush(sf_out);
	}

	return 0;
}

int getarg (char *store) {
	memset(store, 0, 255);

	int arglen = getc(sf_in);
	if (arglen == -1)
		return 0;

	char *p = store;
	while (p - store < arglen) {
		int c = getc(sf_in);
		if (c == EOF) break;
		*p++ = c;
	}

	return arglen;
}

void sendbytes (void *src, int n) {
	char *bytes = src;
	int i;
	for (i = 0; i < n; i++) {
		putc(bytes[i], sf_out);
	}
}

void sendlong (long n) {
	sendbytes(&n, 4);
}
