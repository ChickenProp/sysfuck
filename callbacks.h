#ifndef SYSFUCK_NONSYSCALLS_H
#define SYSFUCK_NONSYSCALLS_H

typedef long (*callback_t)(const char *);

typedef struct {
	char data[255];
} buf_t;

buf_t buf;
char *data;

int getarg (char *store);
callback_t getcallback(const char *str);

long syscallback (const char *);
long memread (const char *);
long memwrite (const char *);

#endif
