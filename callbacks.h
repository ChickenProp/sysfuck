#ifndef SYSFUCK_NONSYSCALLS_H
#define SYSFUCK_NONSYSCALLS_H

#include "sysfuck.h"

typedef void (*callback_t)(const char *name, int n, buf_t *buf);

int g_argc;
char **g_argv;

callback_t getcallback(const char *str);

void syscallback (const char *name, int n, buf_t *buf);
void memread (const char *name, int n, buf_t *buf);
void memwrite (const char *name, int n, buf_t *buf);
void argc (const char *name, int n, buf_t *buf);
void argv (const char *name, int n, buf_t *buf);

#endif
