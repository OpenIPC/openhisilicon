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

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "npu_ioctl.h"

int main(int argc, char **argv)
{
	struct npu_open_params op = { .base_addr = 0, .end_addr = 0xffffffff };
	volatile uint32_t *regs;
	unsigned long off;
	int fd, i, hold = 0;

	if (argc < 2) {
		fprintf(stderr, "usage: %s [-hold SECS] {r OFF | w OFF VAL | "
			"open | reset}...\n", argv[0]);
		return 2;
	}

	fd = open("/dev/npu.0", O_RDWR);
	if (fd < 0) {
		perror("/dev/npu.0");
		return 1;
	}
	regs = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (regs == MAP_FAILED) {
		perror("mmap");
		return 1;
	}
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-hold") && i + 1 < argc) {
			hold = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "r") && i + 1 < argc) {
			off = strtoul(argv[++i], NULL, 16);
			if (off >= 0x1000 || off % 4)
				goto bad_off;
			printf("[0x%03lx] = 0x%08x\n", off, regs[off / 4]);
		} else if (!strcmp(argv[i], "w") && i + 2 < argc) {
			off = strtoul(argv[++i], NULL, 16);
			if (off >= 0x1000 || off % 4)
				goto bad_off;
			regs[off / 4] = strtoul(argv[++i], NULL, 16);
			printf("[0x%03lx] <- 0x%08x\n", off, regs[off / 4]);
		} else if (!strcmp(argv[i], "open")) {
			printf("open: %d\n", ioctl(fd, NPU_IOC_OPEN, &op));
		} else if (!strcmp(argv[i], "reset")) {
			printf("reset: %d\n", ioctl(fd, NPU_IOC_RESET, 0));
		} else {
			fprintf(stderr, "bad argument %s\n", argv[i]);
			return 2;
		}
	}
	fflush(stdout);
	sleep(hold);
	return 0;

bad_off:
	fprintf(stderr, "offset 0x%lx is not a register in the 4 KB page\n", off);
	return 2;
}
