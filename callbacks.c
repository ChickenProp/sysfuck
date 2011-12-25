#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "callbacks.h"
#include "str_to_syscall.c"

int g_argc;
char **g_argv;

callback_t getcallback (const char *str) {
	if (! strcmp("memread", str))
		return memread;
	if (! strcmp("memwrite", str))
		return memwrite;
	if (! strcmp("argc", str))
		return argc;
	if (! strcmp("argv", str))
		return argv;
	if (! strcmp("getenv", str))
		return c_getenv;
	if (! strcmp("strlen", str))
		return c_strlen;
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

void argc (const char *name, int n, buf_t *buf) {
	printlong(g_argc);
}

void argv (const char *name, int n, buf_t *buf) {
	int i;
	for (i = 0; i < n; i += 4) {
		unsigned long idx = * (unsigned long *) (buf->data + i);
		printlong((long) g_argv[idx]);
	}
}

// For simplicity of implementation, this can only be used for environment
// variables whose names are fewer than 255 characters in length.
void c_getenv (const char *name, int n, buf_t *buf) {
	buf->data[255] = 0;
	printlong((long) getenv(buf->data));
}

void c_strlen (const char *name, int n, buf_t *buf) {
	char *str = (char *) * (long *) buf;
	printlong(strlen(str));
}
