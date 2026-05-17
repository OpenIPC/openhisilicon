/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * Integration test for HI_MPI_SVP_NNIE_ForwardWithBbox userspace
 * marshalling. Runs on the target (cv500/av300) against the live
 * /dev/nnie. Kernel currently returns -EOPNOTSUPP for the bbox path
 * (see kernel/nnie_neo/nnie_neo.c::nnie_op_forward_with_bbox); this
 * test asserts that we reach the kernel and translate the errno
 * correctly — i.e. the wrapper's 1728 B arg-buffer marshalling does
 * not get rejected at the userspace layer before the ioctl.
 *
 * Build (target):
 *   make -C libraries/nnie_neo/test test_forward_with_bbox \
 *     CC=arm-openipc-linux-gnueabi-gcc
 *
 * Run (on board):
 *   insmod open_nnie_neo.ko && ./test_forward_with_bbox
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_svp.h"
#include "hi_nnie.h"
#include "mpi_nnie.h"

int main(void)
{
	SVP_SRC_BLOB_S astSrc[1];
	SVP_DST_BLOB_S astDst[1];
	SVP_NNIE_MODEL_S model;
	SVP_NNIE_FORWARD_WITHBBOX_CTRL_S ctrl;
	SVP_NNIE_HANDLE handle = 0;
	HI_S32 ret;

	memset(astSrc, 0, sizeof(astSrc));
	memset(astDst, 0, sizeof(astDst));
	memset(&model, 0, sizeof(model));
	memset(&ctrl, 0, sizeof(ctrl));

	/* Minimal valid CTRL_S: 1 src, 1 dst, no proposals, segment 0.
	 * No HW dispatch happens because the kernel rejects bbox before
	 * touching any phys/virt — we just need the validation to pass
	 * inside our wrapper. */
	ctrl.u32SrcNum      = 1;
	ctrl.u32DstNum      = 1;
	ctrl.u32ProposalNum = 0;
	ctrl.u32NetSegId    = 0;
	ctrl.enNnieId       = 0;

	ret = HI_MPI_SVP_NNIE_ForwardWithBbox(&handle, astSrc, NULL, &model,
	                                      astDst, &ctrl, HI_TRUE);

	/* Expected: -EOPNOTSUPP from kernel → HI_ERR_SVP_NNIE_NOT_SURPPORT. */
	if (ret != HI_ERR_SVP_NNIE_NOT_SURPPORT) {
		fprintf(stderr,
		        "FAIL: ForwardWithBbox returned 0x%x, expected NOT_SURPPORT 0x%x\n",
		        ret, HI_ERR_SVP_NNIE_NOT_SURPPORT);
		return 1;
	}

	printf("PASS: bbox wrapper marshalled cleanly, kernel returned NOT_SURPPORT\n");
	return 0;
}
