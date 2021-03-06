#ifndef SYSFUCK_NONSYSCALLS_H
#define SYSFUCK_NONSYSCALLS_H

#include "sysfuck.h"

typedef void (*callback_t)(const char *name, int n, buf_t *buf);

callback_t getcallback(const char *str);

void syscallback (const char *name, int n, buf_t *buf);
void memread (const char *name, int n, buf_t *buf);
void memwrite (const char *name, int n, buf_t *buf);
void argc (const char *name, int n, buf_t *buf);
void argv (const char *name, int n, buf_t *buf);
void c_getenv (const char *name, int n, buf_t *buf);
void c_strlen (const char *name, int n, buf_t *buf);
void c_stdout (const char *name, int n, buf_t *buf);

#endif
