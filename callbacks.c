#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "callbacks.h"
#include "str_to_syscall.c"

buf_t buf;
char *data = buf.data;

int getarg (char *store) {
	int arglen = getchar();
	if (arglen == -1)
		return 0;

	char *p = store;
	while (p - store < arglen) {
		int c = getchar();
		if (c == -1) break;
		*p++ = c;
	}

	return arglen;
}

callback_t getcallback (const char *str) {
	if (! strcmp("memread", str))
		return memread;
	if (! strcmp("memwrite", str))
		return memwrite;
	return syscallback;
}

long syscallback (const char *callname) {
	getarg(data);
	return syscall(str_to_syscall(callname), buf);
}

long memread (const char *name) {
	return 0;
}

long memwrite (const char *name) {
	return 0;
}
