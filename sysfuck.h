#ifndef SYSFUCK_SYSFUCK_H
#define SYSFUCK_SYSFUCK_H

typedef struct {
	char data[255];
} buf_t;

buf_t buf;
char *data;

int getarg (char *store);
void printlong (long n);

#endif
