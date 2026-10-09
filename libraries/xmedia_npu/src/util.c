// SPDX-License-Identifier: GPL-2.0
/*
 * The SDK's portability layer: xmedia_* names for libc, which
 * libxmedia_cl and SDK samples call instead of libc directly.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "npu_priv.h"

int xmedia_printf(const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vprintf(fmt, ap);
	va_end(ap);
	return ret;
}

int xmedia_snprintf(char *buf, size_t size, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vsnprintf(buf, size, fmt, ap);
	va_end(ap);
	return ret;
}

int xmedia_sprintf(char *buf, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vsprintf(buf, fmt, ap);
	va_end(ap);
	return ret;
}

int xmedia_sscanf(const char *str, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vsscanf(str, fmt, ap);
	va_end(ap);
	return ret;
}

void *xmedia_malloc(size_t size) { return malloc(size); }
void xmedia_free(void *ptr) { free(ptr); }

void *xmedia_memcpy(void *d, const void *s, size_t n) { return memcpy(d, s, n); }
void *xmedia_memmove(void *d, const void *s, size_t n) { return memmove(d, s, n); }
void *xmedia_memset(void *s, int c, size_t n) { return memset(s, c, n); }
int xmedia_memcmp(const void *a, const void *b, size_t n) { return memcmp(a, b, n); }
void *xmedia_memchr(const void *s, int c, size_t n) { return memchr(s, c, n); }

size_t xmedia_strlen(const char *s) { return strlen(s); }
char *xmedia_strcpy(char *d, const char *s) { return strcpy(d, s); }
char *xmedia_strncpy(char *d, const char *s, size_t n) { return strncpy(d, s, n); }
size_t xmedia_strlcpy(char *d, const char *s, size_t n) { return strlcpy(d, s, n); }
char *xmedia_strcat(char *d, const char *s) { return strcat(d, s); }
char *xmedia_strncat(char *d, const char *s, size_t n) { return strncat(d, s, n); }
size_t xmedia_strlcat(char *d, const char *s, size_t n) { return strlcat(d, s, n); }
int xmedia_strcmp(const char *a, const char *b) { return strcmp(a, b); }
int xmedia_strncmp(const char *a, const char *b, size_t n) { return strncmp(a, b, n); }
int xmedia_strcasecmp(const char *a, const char *b) { return strcasecmp(a, b); }
int xmedia_strncasecmp(const char *a, const char *b, size_t n) { return strncasecmp(a, b, n); }
char *xmedia_strchr(const char *s, int c) { return strchr(s, c); }
char *xmedia_strrchr(const char *s, int c) { return strrchr(s, c); }
char *xmedia_strstr(const char *h, const char *n) { return strstr(h, n); }
size_t xmedia_strspn(const char *s, const char *a) { return strspn(s, a); }
size_t xmedia_strcspn(const char *s, const char *r) { return strcspn(s, r); }
char *xmedia_strpbrk(const char *s, const char *a) { return strpbrk(s, a); }
char *xmedia_strsep(char **s, const char *d) { return strsep(s, d); }
