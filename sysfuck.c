#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <readline/readline.h>
//#include <readline/history.h>
#include <sys/syscall.h>
#include <unistd.h>

int str_to_syscall (const char *str) {
	if (! strcmp("brk", str))
		return __NR_brk;
	return -1;
}

typedef struct {
	char buf[255];
} buf_t;

int main (int argc, char **argv) {
	char *callname = NULL;
	size_t n = 0;

	while (1) {
		int bytesread = getdelim(&callname, &n, 0, stdin);
		if (bytesread == -1)
			return 0;

		int arglen = getchar();
		if (arglen == -1)
			return 0;

		printf("call %s with %d bytes\n", callname, arglen);
	
		char *data = malloc(255);
		memset(data, 0, 255);

		char *p = data;
		while (p - data < arglen) {
			int c = getchar();
			if (c == -1) break;
			*p++ = c;
		}

		buf_t buf = * (buf_t*) data;

		printf("As a number, the argument is %d\n", (int)*(int*)data);
	
		long ret = syscall(str_to_syscall(callname), buf);

		printf("%ld\n", ret);
	}

	return 0;
}
