#include <stdarg.h>
#include <stddef.h>

int printf(const char *fmt, ...);
int sprintf(char *buf, const char *fmt, ...);
int snprintf(char *buf, size_t bufsize, const char *fmt, ...);

int vprintf(const char *fmt, va_list vlist);
int vsprintf(char *buf, const char *fmt, va_list vlist);
int vsnprintf(char *buf, size_t bufsize, const char *fmt, va_list vlist);
