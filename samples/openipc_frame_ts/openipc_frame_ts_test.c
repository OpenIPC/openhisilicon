// SPDX-License-Identifier: GPL-2.0
/*
 * openipc_frame_ts_test: read /dev/openipc-frame-ts and either log each
 * event or print periodic per-channel jitter summaries.
 *
 *   build: $(CROSS_COMPILE)gcc -O2 -Wall -o openipc_frame_ts_test \
 *          openipc_frame_ts_test.c
 *   run:   ./openipc_frame_ts_test            (log every event)
 *          ./openipc_frame_ts_test -s         (per-second summary only)
 *          ./openipc_frame_ts_test -c 0x3     (channels 0 and 1 only)
 */
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include "../../include/openipc_frame_ts.h"

#define DEV_PATH      "/dev/openipc-frame-ts"
#define MAX_CHN       8

struct chn_state {
	uint64_t last_wall_ns;
	uint64_t count;
	uint64_t min_dt_ns;
	uint64_t max_dt_ns;
	double   sum_dt_ns;
	double   sum_sq_dt_ns;
};

static volatile sig_atomic_t g_stop;

static void on_signal(int sig) { (void)sig; g_stop = 1; }

/* Newton-Raphson sqrt — avoids -lm. */
static double sqrt_approx(double x)
{
	double r = x > 1 ? x / 2 : 1;
	int i;

	for (i = 0; i < 40; i++)
		r = 0.5 * (r + x / r);
	return r;
}

static void update_chn(struct chn_state *s, uint64_t wall_ns)
{
	if (s->last_wall_ns) {
		uint64_t dt = wall_ns - s->last_wall_ns;

		s->count++;
		s->sum_dt_ns += (double)dt;
		s->sum_sq_dt_ns += (double)dt * (double)dt;
		if (!s->min_dt_ns || dt < s->min_dt_ns) s->min_dt_ns = dt;
		if (dt > s->max_dt_ns) s->max_dt_ns = dt;
	}
	s->last_wall_ns = wall_ns;
}

static void print_summary(const struct chn_state *st)
{
	unsigned i;

	for (i = 0; i < MAX_CHN; i++) {
		const struct chn_state *s = &st[i];
		double mean, var, stddev;

		if (s->count < 2)
			continue;

		mean = s->sum_dt_ns / (double)s->count;
		var  = s->sum_sq_dt_ns / (double)s->count - mean * mean;
		stddev = var > 0 ? sqrt_approx(var) : 0;

		printf("chn%u: n=%llu mean=%.3f ms min=%.3f ms max=%.3f ms sigma=%.0f us\n",
		       i, (unsigned long long)s->count, mean / 1e6,
		       s->min_dt_ns / 1e6, s->max_dt_ns / 1e6,
		       stddev / 1e3);
	}
}

int main(int argc, char **argv)
{
	int opt, fd;
	int summary_only = 0;
	uint32_t mask = 0xFFFFFFFFu;
	struct chn_state st[MAX_CHN] = {0};
	time_t last_print = 0;
	struct pollfd pfd;
	uint64_t dropped = 0;

	while ((opt = getopt(argc, argv, "sc:")) != -1) {
		switch (opt) {
		case 's': summary_only = 1; break;
		case 'c': mask = strtoul(optarg, NULL, 0); break;
		default:
			fprintf(stderr, "usage: %s [-s] [-c <mask>]\n", argv[0]);
			return 1;
		}
	}

	signal(SIGINT, on_signal);
	signal(SIGTERM, on_signal);

	fd = open(DEV_PATH, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "open %s: %s\n", DEV_PATH, strerror(errno));
		return 1;
	}

	if (ioctl(fd, OPENIPC_FT_IOC_SET_CHANNEL_MASK, &mask) < 0) {
		fprintf(stderr, "ioctl SET_CHANNEL_MASK: %s\n", strerror(errno));
		close(fd);
		return 1;
	}

	pfd.fd = fd;
	pfd.events = POLLIN;

	while (!g_stop) {
		struct openipc_frame_ts_event ev;
		ssize_t n;
		int pr = poll(&pfd, 1, 1000);

		if (pr < 0) {
			if (errno == EINTR)
				continue;
			fprintf(stderr, "poll: %s\n", strerror(errno));
			break;
		}

		while ((n = read(fd, &ev, sizeof(ev))) == sizeof(ev)) {
			if (ev.vi_chn < MAX_CHN)
				update_chn(&st[ev.vi_chn], ev.wall_ns);
			if (!summary_only)
				printf("chn%u seq=%u pts=%llu wall_ns=%llu\n",
				       ev.vi_chn, ev.seq,
				       (unsigned long long)ev.pts_us,
				       (unsigned long long)ev.wall_ns);
		}
		if (n < 0 && errno != EAGAIN && errno != EINTR) {
			fprintf(stderr, "read: %s\n", strerror(errno));
			break;
		}

		if (summary_only) {
			time_t now = time(NULL);

			if (now != last_print) {
				printf("--- t=%ld ---\n", (long)now);
				print_summary(st);
				last_print = now;
			}
		}
	}

	printf("\n=== final summary ===\n");
	print_summary(st);

	if (ioctl(fd, OPENIPC_FT_IOC_GET_DROPPED, &dropped) == 0)
		printf("dropped (all channels, all time): %llu\n",
		       (unsigned long long)dropped);

	close(fd);
	return 0;
}
