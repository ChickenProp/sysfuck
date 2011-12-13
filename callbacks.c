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

void printlong (long n) {
	char *s = (char *) &n;
	printf("%c%c%c%c", s[0], s[1], s[2], s[3]);
}

callback_t getcallback (const char *str) {
	if (! strcmp("memread", str))
		return memread;
	if (! strcmp("memwrite", str))
		return memwrite;
	return syscallback;
}

void syscallback (const char *callname, int n, buf_t *buf) {
	long ret = syscall(str_to_syscall(callname), *buf);
	printlong(ret);
}

void memread (const char *name, int n, buf_t *buf) {
	char *ptr = (char *) * (long *) buf;
	unsigned long num = * (unsigned long *) (buf->data + 4);
	
	unsigned long i;
	for (i = 0; i < num; i++) {
		putchar(ptr[i]);
	}
}

void memwrite (const char *name, int n, buf_t *buf) {
	if (n <= 4)
		return;
	
	char *dest = (char *) * (long *) buf;
	char *src = buf->data + 4;

	memmove(dest, src, n - 4);
}
