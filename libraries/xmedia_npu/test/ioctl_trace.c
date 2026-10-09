// SPDX-License-Identifier: GPL-2.0
/*
 * LD_PRELOAD shim that logs every ioctl a process makes (command, result,
 * and payload before/after) to $IOCTL_TRACE, or stderr. Used to diff the
 * ioctl stream of this library against the SDK's on the same workload:
 *
 *   LD_PRELOAD=./ioctl_trace.so IOCTL_TRACE=/tmp/t.log npu_bench ...
 */

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

/* the Linux ioctl number layout; musl's sys/ioctl.h has no _IOC_* */
#define IOC_SIZE(cmd)	(((unsigned int)(cmd) >> 16) & 0x3fff)
#define IOC_DIR(cmd)	((unsigned int)(cmd) >> 30)
#define IOC_WRITE	1u
#define IOC_READ	2u

static FILE *out;

static void dump(const char *tag, const unsigned char *p, unsigned int size)
{
	unsigned int i;

	fprintf(out, "  %s", tag);
	for (i = 0; i < size; i++)
		fprintf(out, "%s%02x", i % 4 ? "" : " ", p[i]);
	fputc('\n', out);
}

int ioctl(int fd, int req, ...)
{
	static int (*real)(int, int, ...);
	unsigned int size = IOC_SIZE(req);
	va_list ap;
	void *arg;
	int ret;

	va_start(ap, req);
	arg = va_arg(ap, void *);
	va_end(ap);

	if (!real)
		real = dlsym(RTLD_NEXT, "ioctl");
	if (!out) {
		const char *path = getenv("IOCTL_TRACE");

		out = path ? fopen(path, "w") : NULL;
		if (!out)
			out = stderr;
		setvbuf(out, NULL, _IOLBF, 0);
	}

	if (size > 256 || !arg)
		size = 0;
	fprintf(out, "ioctl fd=%d cmd=0x%08x\n", fd, (unsigned int)req);
	if (size && (IOC_DIR(req) & IOC_WRITE))
		dump("in ", arg, size);
	ret = real(fd, req, arg);
	fprintf(out, "  ret=%d\n", ret);
	if (size && (IOC_DIR(req) & IOC_READ))
		dump("out", arg, size);
	return ret;
}
