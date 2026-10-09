// SPDX-License-Identifier: GPL-2.0
/*
 * npu_abort_test: drives the NPU into an error on purpose and checks that
 * it comes back. Submits a command stream of 0xff bytes (no valid NPU
 * instruction) with the DDR window narrowed to that one buffer, so the job
 * either aborts with a DFX report or never finishes; then resets through
 * xmedia_npu_reset(). Run npu_bench afterwards to confirm the NPU works.
 */

#include <stdio.h>
#include <string.h>

#include "comm_npu.h"

#define BUF_SIZE 0x10000

int xmedia_npu_open(unsigned int idx, unsigned int *fd, xmedia_npu_open_params_s *p);
int xmedia_npu_close(unsigned int fd);
int xmedia_npu_create_queue(unsigned int fd);
int xmedia_npu_destroy_queue(unsigned int fd);
int xmedia_npu_submit_job(unsigned int fd, xmedia_npu_start_params_s *p,
			  unsigned int *id);
int xmedia_npu_wait_job(unsigned int fd, xmedia_npu_job_state_t *st);
int xmedia_npu_getabort(xmedia_npu_job_state_t *st);
int xmedia_npu_reset(unsigned int fd);
int uncache_mem_alloc(int size);
int uncache_mem_free(int virt);
int xmedia_virt_to_phy(int virt);

int main(void)
{
	xmedia_npu_open_params_s open = { 0 };
	xmedia_npu_start_params_s job;
	xmedia_npu_job_state_t st;
	unsigned int fd, id;
	int virt, phys, ret;

	if (xmedia_npu_open(0, &fd, &open) || xmedia_npu_create_queue(fd))
		return 1;
	virt = uncache_mem_alloc(BUF_SIZE);
	phys = xmedia_virt_to_phy(virt);
	if (!virt || !phys)
		return 1;
	memset((void *)virt, 0xff, BUF_SIZE);

	memset(&job, 0, sizeof(job));
	job.npu_cmd_base_addr = phys;
	job.npu_cmd_data_size = 0x1000;
	memset(job.npu_dynamic_input, 0xff, sizeof(job.npu_dynamic_input));
	job.npu_base_addr = phys;
	job.npu_end_addr = phys + BUF_SIZE;
	ret = xmedia_npu_submit_job(fd, &job, &id);
	printf("submit: ret %d id %u\n", ret, id);

	memset(&st, 0, sizeof(st));
	st.job_id = id;
	st.delay = 2000;
	ret = xmedia_npu_wait_job(fd, &st);
	printf("wait: ret %d state %d abort 0x%x\n", ret, st.state,
	       st.abort_value);
	xmedia_npu_getabort(&st);

	printf("reset: %d\n", xmedia_npu_reset(fd));
	xmedia_npu_destroy_queue(fd);
	uncache_mem_free(virt);
	printf("close: %d\n", xmedia_npu_close(fd));
	return 0;
}
