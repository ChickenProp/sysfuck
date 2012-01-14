#ifndef SYSFUCK_SYSFUCK_H
#define SYSFUCK_SYSFUCK_H

typedef struct {
	char data[255];
} buf_t;

buf_t buf;
char *data;

int g_argc;
char **g_argv;

FILE *sf_in;
FILE *sf_out;
int prog_stdout;


int getarg (char *store);
void sendbytes (void *src, int n);
void sendlong (long n);

#endif
