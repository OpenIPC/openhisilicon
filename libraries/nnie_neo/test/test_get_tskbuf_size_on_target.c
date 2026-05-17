/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * Target-runnable parity test for HI_MPI_SVP_NNIE_GetTskBufSize.
 *
 * Loads a real .wk model from disk, calls our libnnie_neo's
 * GetTskBufSize, and prints the per-segment size + total. Cross-check
 * by running the same on a board with vendor libnnie.so and comparing.
 *
 * Build:
 *   make -C libraries/nnie_neo/test test_get_tskbuf_size_on_target \
 *       CC=arm-openipc-linux-gnueabi-gcc
 *
 * Run:
 *   ./test_get_tskbuf_size_on_target <path/to/model.wk> <max_input_num> <max_roi_num>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_svp.h"
#include "hi_nnie.h"
#include "mpi_nnie.h"

int main(int argc, char *argv[])
{
	const char *path;
	int fd;
	struct stat st;
	void *file;
	SVP_SRC_MEM_INFO_S buf = {0};
	SVP_NNIE_MODEL_S model = {0};
	HI_U32 sizes[SVP_NNIE_MAX_NET_SEG_NUM] = {0};
	HI_U32 max_in, max_roi, total = 0;
	HI_S32 ret;
	HI_U32 i;

	if (argc != 4) {
		fprintf(stderr, "usage: %s <model.wk> <max_input_num> <max_roi_num>\n", argv[0]);
		return 1;
	}
	path = argv[1];
	max_in = (HI_U32)atoi(argv[2]);
	max_roi = (HI_U32)atoi(argv[3]);

	fd = open(path, O_RDONLY);
	if (fd < 0) { perror("open"); return 1; }
	if (fstat(fd, &st) < 0) { perror("fstat"); return 1; }
	file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file == MAP_FAILED) { perror("mmap"); return 1; }

	buf.u64VirAddr = (HI_U64)(uintptr_t)file;
	buf.u32Size    = (HI_U32)st.st_size;

	ret = HI_MPI_SVP_NNIE_LoadModel(&buf, &model);
	if (ret != HI_SUCCESS) {
		fprintf(stderr, "LoadModel failed: 0x%x\n", ret);
		return 1;
	}

	printf("=== model: %s ===\n", path);
	printf("  netSegNum=%u, runMode=%u, tmpBufSize=%u\n",
	       model.u32NetSegNum, model.enRunMode, model.u32TmpBufSize);
	for (i = 0; i < model.u32NetSegNum; i++) {
		printf("  seg[%u]: netType=%u srcNum=%u dstNum=%u inst@0x%x len=0x%x\n",
		       i, model.astSeg[i].enNetType,
		       model.astSeg[i].u16SrcNum, model.astSeg[i].u16DstNum,
		       model.astSeg[i].u32InstOffset, model.astSeg[i].u32InstLen);
	}

	ret = HI_MPI_SVP_NNIE_GetTskBufSize(max_in, max_roi, &model, sizes,
	                                    model.u32NetSegNum);
	if (ret != HI_SUCCESS) {
		fprintf(stderr, "GetTskBufSize failed: 0x%x\n", ret);
		return 1;
	}

	printf("\n=== GetTskBufSize(max_in=%u, max_roi=%u) ===\n", max_in, max_roi);
	for (i = 0; i < model.u32NetSegNum; i++) {
		printf("  seg[%u]: %u B (0x%x)\n", i, sizes[i], sizes[i]);
		total += sizes[i];
	}
	printf("  total: %u B (0x%x)\n", total, total);

	munmap(file, st.st_size);
	close(fd);
	return 0;
}
