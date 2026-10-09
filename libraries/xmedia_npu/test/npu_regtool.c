// SPDX-License-Identifier: GPL-2.0
/*
 * npu_regtool: pokes /dev/npu.0 directly, to stage driver corner cases.
 *
 *   npu_regtool [-hold SECS] CMD...
 *     r OFF          read a register (hex offset into the 4 KB page)
 *     w OFF VAL      write a register (hex)
 *     open           NPU_IOC_OPEN (window 0..~0), as libxmedia_npu does
 *     reset          NPU_IOC_RESET
 *
 * Commands run in order on one descriptor; -hold keeps it open afterwards
 * (an open descriptor keeps the NPU clocked and, after "open", keeps the
 * driver from reprogramming the NPU on the next process's open).
 */

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "npu_ioctl.h"

#define NPU_REG_PAGE	0x1000

/* Whole-string unsigned parse in <base>, at most <max>; false otherwise. */
static int parse_ul(const char *str, int base, unsigned long max,
		    unsigned long *val)
{
	char *end;

	if (!*str || *str == '-' || *str == '+')
		return 0;
	errno = 0;
	*val = strtoul(str, &end, base);
	return !errno && !*end && *val <= max;
}

static int reg_off(const char *str, unsigned long *off)
{
	if (parse_ul(str, 16, NPU_REG_PAGE - 4, off) && *off % 4 == 0)
		return 1;
	fprintf(stderr, "%s is not a register offset in the 4 KB page\n", str);
	return 0;
}

/*
 * Runs the command list, or with regs == NULL only checks it: every argument
 * is validated before the first one touches the hardware, so a typo late in
 * a sequence cannot leave the earlier writes half-applied.
 */
static int run(int argc, char **argv, int fd, volatile uint32_t *regs,
	       unsigned long *hold)
{
	struct npu_open_params op = { .base_addr = 0, .end_addr = 0xffffffff };
	unsigned long off, val;
	int i, ret;

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-hold") && i + 1 < argc) {
			if (!parse_ul(argv[++i], 10, 86400, hold)) {
				fprintf(stderr, "-hold wants 0..86400 seconds, "
					"not %s\n", argv[i]);
				return 2;
			}
		} else if (!strcmp(argv[i], "r") && i + 1 < argc) {
			if (!reg_off(argv[++i], &off))
				return 2;
			if (regs)
				printf("[0x%03lx] = 0x%08x\n", off, regs[off / 4]);
		} else if (!strcmp(argv[i], "w") && i + 2 < argc) {
			if (!reg_off(argv[++i], &off))
				return 2;
			if (!parse_ul(argv[++i], 16, 0xffffffff, &val)) {
				fprintf(stderr, "%s is not a 32-bit hex value\n",
					argv[i]);
				return 2;
			}
			if (regs) {
				regs[off / 4] = val;
				printf("[0x%03lx] <- 0x%08x\n", off, regs[off / 4]);
			}
		} else if (!strcmp(argv[i], "open") || !strcmp(argv[i], "reset")) {
			if (!regs)
				continue;
			ret = !strcmp(argv[i], "open") ?
			      ioctl(fd, NPU_IOC_OPEN, &op) :
			      ioctl(fd, NPU_IOC_RESET, 0);
			if (ret) {
				fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
				return 1;
			}
			printf("%s: 0\n", argv[i]);
		} else {
			fprintf(stderr, "bad argument %s\n", argv[i]);
			return 2;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	volatile uint32_t *regs;
	unsigned long hold = 0;
	int fd, ret;

	if (argc < 2 || (ret = run(argc, argv, -1, NULL, &hold))) {
		fprintf(stderr, "usage: %s [-hold SECS] {r OFF | w OFF VAL | "
			"open | reset}...\n", argv[0]);
		return 2;
	}
	fd = open("/dev/npu.0", O_RDWR);
	if (fd < 0) {
		perror("/dev/npu.0");
		return 1;
	}
	regs = mmap(NULL, NPU_REG_PAGE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (regs == MAP_FAILED) {
		perror("mmap");
		return 1;
	}
	ret = run(argc, argv, fd, regs, &hold);
	fflush(stdout);
	if (!ret)
		sleep(hold);
	return ret;
}
