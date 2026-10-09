// SPDX-License-Identifier: GPL-2.0
/*
 * npu_hang_test: gives the NPU a job that never ends and checks that the
 * driver gets the NPU back. The command stream is RISC-V "j ." (0x6f) over
 * and over, which the NPU rejects with a DFX abort while its sequencer
 * keeps running; with "mask" (interrupts masked first) nothing reaches the
 * driver at all and the job looks like it never ends. Expected:
 *
 *   - the running cycle counter keeps climbing, the wait times out;
 *   - destroying the queue gives up on the job after its timeout (-EPERM)
 *     and leaves the device draining: a new submission gets -EBUSY;
 *   - NPU_IOC_RESET alone soft-resets the NPU: the counter clears, and
 *     xmedia_npu_reset() then leaves it ready. Run npu_bench afterwards.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>

#include "comm_npu.h"
#include "npu_ioctl.h"

#define BUF_SIZE	0x10000
#define RV_J_SELF	0x0000006f

int xmedia_npu_open(unsigned int idx, unsigned int *fd, xmedia_npu_open_params_s *p);
int xmedia_npu_close(unsigned int fd);
int xmedia_npu_create_queue(unsigned int fd);
int xmedia_npu_destroy_queue(unsigned int fd);
int xmedia_npu_submit_job(unsigned int fd, xmedia_npu_start_params_s *p,
			  unsigned int *id);
int xmedia_npu_wait_job(unsigned int fd, xmedia_npu_job_state_t *st);
int xmedia_npu_reset(unsigned int fd);
int gh_npu_get_cfg_running_cycle_cnt(int fd);
int gh_npu_enable_interrupt(int fd, int done, int si, int dfx, int pc);
int uncache_mem_alloc(int size);
int uncache_mem_free(int virt);
int xmedia_virt_to_phy(int virt);

static double now_s(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1e6;
}

int main(int argc, char **argv)
{
	xmedia_npu_open_params_s open = { 0 };
	xmedia_npu_start_params_s job;
	xmedia_npu_job_state_t st;
	unsigned int fd, id, i, *code;
	int virt, phys, ret;
	double t;

	if (xmedia_npu_open(0, &fd, &open) || xmedia_npu_create_queue(fd))
		return 1;
	if (argc > 1 && !strcmp(argv[1], "mask")) {
		gh_npu_enable_interrupt(fd, 0, 0, 0, 0);
		printf("interrupts masked\n");
	}
	virt = uncache_mem_alloc(BUF_SIZE);
	phys = xmedia_virt_to_phy(virt);
	if (!virt || !phys)
		return 1;
	code = (unsigned int *)virt;
	for (i = 0; i < BUF_SIZE / 4; i++)
		code[i] = RV_J_SELF;

	memset(&job, 0, sizeof(job));
	job.npu_cmd_base_addr = 0;	/* offsets are from npu_base_addr */
	job.npu_cmd_data_size = 0x1000;
	memset(job.npu_dynamic_input, 0xff, sizeof(job.npu_dynamic_input));
	job.npu_base_addr = phys;
	job.npu_end_addr = phys + BUF_SIZE;
	ret = xmedia_npu_submit_job(fd, &job, &id);
	printf("submit: ret %d id %u\n", ret, id);

	memset(&st, 0, sizeof(st));
	st.job_id = id;
	st.delay = 500;
	ret = xmedia_npu_wait_job(fd, &st);
	printf("wait: ret %d (%s) state %d abort 0x%x\n", ret,
	       ret ? strerror(errno) : "ok", st.state, st.abort_value);
	printf("cycles: %d, 100 ms later %d\n",
	       gh_npu_get_cfg_running_cycle_cnt(fd),
	       (usleep(100000), gh_npu_get_cfg_running_cycle_cnt(fd)));

	t = now_s();
	ret = xmedia_npu_destroy_queue(fd);
	printf("destroy: ret %d (%s) after %.1f s\n", ret,
	       ret ? strerror(errno) : "ok", now_s() - t);

	xmedia_npu_create_queue(fd);
	ret = xmedia_npu_submit_job(fd, &job, &id);
	printf("submit while draining: ret %d (%s)\n", ret,
	       ret < 0 ? strerror(errno) : "accepted");

	ret = ioctl(fd, NPU_IOC_RESET, 0);
	printf("driver reset: %d, cycles now %d, 100 ms later %d\n", ret,
	       gh_npu_get_cfg_running_cycle_cnt(fd),
	       (usleep(100000), gh_npu_get_cfg_running_cycle_cnt(fd)));
	printf("library reset: %d\n", xmedia_npu_reset(fd));

	uncache_mem_free(virt);
	printf("close: %d\n", xmedia_npu_close(fd));
	return 0;
}
