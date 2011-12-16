#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "callbacks.h"

int main (int argc, char **argv) {
	g_argc = argc;
	g_argv = argv;

	char *callname = NULL;
	size_t n = 0;

	while (1) {
		int bytesread = getdelim(&callname, &n, 0, stdin);
		if (bytesread == -1)
			return 0;

		callback_t f = getcallback(callname);
		int n = getarg(data);
		(*f)(callname, n, &buf);
		fflush(stdout);
	}

	return 0;
}
