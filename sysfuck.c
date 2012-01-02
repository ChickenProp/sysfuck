#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "sysfuck.h"
#include "callbacks.h"

buf_t buf;
char *data = buf.data;

int main (int argc, char **argv) {
	g_argc = argc;
	g_argv = argv;

	char *callname = NULL;
	size_t n = 0;

	while (1) {
		int c;
		while ((c = getchar())) {
			if (c == EOF)
				return 0;
			else
				write(fake_stdout, &c, 1);
		}

		int bytesread = getdelim(&callname, &n, 0, stdin);
		if (bytesread == -1)
			return 0;
		else if (bytesread == 1) {
			char null = 0;
			write(fake_stdout, &null, 1);
			continue;
		}

		callback_t f = getcallback(callname);
		int n = getarg(data);
		(*f)(callname, n, &buf);
		fflush(stdout);
	}

	return 0;
}

int getarg (char *store) {
	memset(store, 0, 255);

	int arglen = getchar();
	if (arglen == -1)
		return 0;

	char *p = store;
	while (p - store < arglen) {
		int c = getchar();
		if (c == EOF) break;
		*p++ = c;
	}

	return arglen;
}

void printlong (long n) {
	char *s = (char *) &n;
	printf("%c%c%c%c", s[0], s[1], s[2], s[3]);
}
