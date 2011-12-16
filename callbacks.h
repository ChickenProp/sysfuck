#ifndef SYSFUCK_NONSYSCALLS_H
#define SYSFUCK_NONSYSCALLS_H

typedef struct {
	char data[255];
} buf_t;

typedef void (*callback_t)(const char *name, int n, buf_t *buf);

buf_t buf;
char *data;

int g_argc;
char **g_argv;

int getarg (char *store);
callback_t getcallback(const char *str);

void syscallback (const char *name, int n, buf_t *buf);
void memread (const char *name, int n, buf_t *buf);
void memwrite (const char *name, int n, buf_t *buf);
void argc (const char *name, int n, buf_t *buf);
void argv (const char *name, int n, buf_t *buf);

#endif
