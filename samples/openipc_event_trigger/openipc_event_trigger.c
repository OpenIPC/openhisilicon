// SPDX-License-Identifier: GPL-2.0
/*
 * openipc_event_trigger — correlate an external GPIO pulse with the
 * camera's nearest sensor frame.
 *
 * Use case: PLC / photoeye / strobe-fired external event needs to be
 * matched to "which frame was the camera capturing at that moment".
 *
 *   1. Configure a SoC GPIO as input + rising-edge IRQ (sysfs).
 *   2. Subscribe to /dev/openipc-frame-ts for MIPI_FS events.
 *   3. poll() both. On GPIO rising edge, sample CLOCK_REALTIME and
 *      match against the most recent FS event ("frame being exposed
 *      when the trigger fired") AND the next FS event ("first frame
 *      that started fully after the trigger").
 *
 * Output line per trigger event:
 *   trigger=<ns> last_fs={seq=N pts=us wall=ns dt=ms before_trigger}
 *                next_fs={seq=N+1 pts=us wall=ns dt=ms after_trigger}
 *
 * Limitations:
 *   - The GPIO sysfs path adds ~µs to low-ms jitter on the trigger
 *     timestamp (kernel→user transit + scheduler latency). For
 *     PLC-class accuracy that's fine; for sub-µs you'd need kernel-
 *     side correlation.
 *   - This is NOT hardware trigger IN: the camera keeps free-running
 *     at its configured rate. We're tagging the closest frame, not
 *     gating capture.
 *   - Requires a SoC GPIO physically reachable on the camera board.
 *     Most consumer IP-cameras don't expose any extra GPIOs; the
 *     IR-cut filter GPIO is the universal one and can be repurposed
 *     during testing.
 *
 *   build: $(CROSS_COMPILE)gcc -O2 -Wall -o openipc_event_trigger \
 *          openipc_event_trigger.c
 *   run:   ./openipc_event_trigger -g 16            (default chn 0, no timeout)
 *          ./openipc_event_trigger -g 16 -t 30      (auto-exit after 30 s)
 *          ./openipc_event_trigger -g 16 -c 0x3     (channels 0+1)
 */
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "../../include/openipc_frame_ts.h"

#define DEV_PATH        "/dev/openipc-frame-ts"
#define GPIO_SYSFS      "/sys/class/gpio"
#define MAX_CHN         8
#define EV_BATCH        16

struct fs_cache {
	uint64_t pts_us;
	uint64_t wall_ns;
	uint32_t seq;
	int      valid;
};

static volatile sig_atomic_t g_stop;
static void on_signal(int sig) { (void)sig; g_stop = 1; }

static uint64_t now_realtime_ns(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_REALTIME, &ts);
	return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

/* Best-effort write-string-to-sysfs helper. Returns 0 on success. */
static int sysfs_write(const char *path, const char *val)
{
	int fd = open(path, O_WRONLY);
	ssize_t n;

	if (fd < 0)
		return -1;
	n = write(fd, val, strlen(val));
	close(fd);
	return (n == (ssize_t)strlen(val)) ? 0 : -1;
}

static int gpio_export(unsigned int n)
{
	char buf[16];
	struct stat st;
	char dir[64];

	snprintf(dir, sizeof(dir), GPIO_SYSFS "/gpio%u", n);
	if (access(dir, F_OK) == 0)
		return 0;	/* already exported */

	snprintf(buf, sizeof(buf), "%u", n);
	if (sysfs_write(GPIO_SYSFS "/export", buf) < 0) {
		fprintf(stderr, "gpio %u export: %s\n", n, strerror(errno));
		return -1;
	}
	/* sysfs takes a moment to populate the per-gpio directory after
	 * export — busybox / hotplug latency. Up to 10×10 ms wait. */
	for (int i = 0; i < 10; i++) {
		if (stat(dir, &st) == 0)
			return 0;
		usleep(10000);
	}
	return -1;
}

static int gpio_setup_edge(unsigned int n)
{
	char path[80];

	snprintf(path, sizeof(path), GPIO_SYSFS "/gpio%u/direction", n);
	if (sysfs_write(path, "in") < 0) {
		fprintf(stderr, "gpio %u direction=in: %s\n", n, strerror(errno));
		return -1;
	}
	snprintf(path, sizeof(path), GPIO_SYSFS "/gpio%u/edge", n);
	if (sysfs_write(path, "rising") < 0) {
		fprintf(stderr,
		        "gpio %u edge=rising: %s (kernel may lack GPIO_SYSFS_EDGE)\n",
		        n, strerror(errno));
		return -1;
	}
	return 0;
}

static int gpio_open_value(unsigned int n)
{
	char path[80];
	int fd;
	char dummy[8];

	snprintf(path, sizeof(path), GPIO_SYSFS "/gpio%u/value", n);
	fd = open(path, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		fprintf(stderr, "open %s: %s\n", path, strerror(errno));
		return -1;
	}
	/* Drain any pending edge captured during setup so the first
	 * poll() returns only on a real new event. */
	(void)read(fd, dummy, sizeof(dummy));
	return fd;
}

/* Re-arm the GPIO sysfs poll: lseek to 0 + read drains the latched
 * edge so the next poll() blocks until the *next* edge. */
static void gpio_rearm(int fd)
{
	char dummy[8];

	(void)lseek(fd, 0, SEEK_SET);
	(void)read(fd, dummy, sizeof(dummy));
}

static void print_trigger(uint64_t trig_ns,
                          const struct fs_cache *last,
                          const struct fs_cache *next)
{
	double dt_last_ms = 0.0, dt_next_ms = 0.0;

	printf("trigger=%llu", (unsigned long long)trig_ns);

	if (last->valid) {
		dt_last_ms = ((double)(int64_t)(trig_ns - last->wall_ns)) / 1e6;
		printf(" last_fs={seq=%u pts=%llu wall=%llu dt=%+.3f ms}",
		       last->seq,
		       (unsigned long long)last->pts_us,
		       (unsigned long long)last->wall_ns,
		       dt_last_ms);
	} else {
		printf(" last_fs=(none yet)");
	}
	if (next->valid) {
		dt_next_ms = ((double)(int64_t)(next->wall_ns - trig_ns)) / 1e6;
		printf(" next_fs={seq=%u pts=%llu wall=%llu dt=%+.3f ms}",
		       next->seq,
		       (unsigned long long)next->pts_us,
		       (unsigned long long)next->wall_ns,
		       dt_next_ms);
	} else {
		printf(" next_fs=(pending)");
	}
	printf("\n");
	fflush(stdout);
}

int main(int argc, char **argv)
{
	int opt;
	int gpio = -1, seconds = 0;
	uint32_t chn_mask = 0xFFFFFFFFu;
	uint32_t evt_mask = 1u << OPENIPC_FT_EVT_MIPI_FS;
	int fd_ft = -1, fd_gpio = -1;
	struct fs_cache last[MAX_CHN] = {{0}};
	/* Pending trigger: set when GPIO fires; we then wait for the
	 * next FS event to fill `next_fs` before printing. One slot
	 * is enough — extra triggers during the gap are coalesced. */
	struct {
		int       pending;
		uint64_t  trig_ns;
		struct fs_cache last_snapshot[MAX_CHN];
	} t = { 0 };

	while ((opt = getopt(argc, argv, "g:c:t:h")) != -1) {
		switch (opt) {
		case 'g': gpio = atoi(optarg); break;
		case 'c': chn_mask = strtoul(optarg, NULL, 0); break;
		case 't': seconds = atoi(optarg); break;
		default:
			fprintf(stderr,
			        "usage: %s -g <gpio> [-c <chn_mask>] [-t <seconds>]\n",
			        argv[0]);
			return 1;
		}
	}
	if (gpio < 0) {
		fprintf(stderr, "must specify -g <gpio>\n");
		return 1;
	}

	signal(SIGINT, on_signal);
	signal(SIGTERM, on_signal);

	if (gpio_export(gpio) < 0 ||
	    gpio_setup_edge(gpio) < 0)
		return 1;

	fd_gpio = gpio_open_value(gpio);
	if (fd_gpio < 0)
		return 1;

	fd_ft = open(DEV_PATH, O_RDONLY | O_NONBLOCK);
	if (fd_ft < 0) {
		fprintf(stderr, "open %s: %s\n", DEV_PATH, strerror(errno));
		return 1;
	}
	if (ioctl(fd_ft, OPENIPC_FT_IOC_SET_CHANNEL_MASK, &chn_mask) < 0) {
		fprintf(stderr, "SET_CHANNEL_MASK: %s\n", strerror(errno));
		return 1;
	}
	/* MIPI_FS only — we want sensor-row-0 events for correlation,
	 * not ISP back-end completion. */
	if (ioctl(fd_ft, OPENIPC_FT_IOC_SET_EVENT_MASK, &evt_mask) < 0 &&
	    errno != ENOTTY) {
		fprintf(stderr, "SET_EVENT_MASK: %s\n", strerror(errno));
		/* ENOTTY = v1 kernel; only emits FS anyway, proceed */
	}

	time_t deadline = seconds > 0 ? time(NULL) + seconds : 0;

	fprintf(stderr,
	        "openipc_event_trigger: watching gpio=%d, chn_mask=0x%x\n",
	        gpio, chn_mask);

	struct pollfd pfd[2];
	pfd[0].fd = fd_gpio;
	pfd[0].events = POLLPRI | POLLERR;	/* sysfs GPIO edge wait */
	pfd[1].fd = fd_ft;
	pfd[1].events = POLLIN;

	while (!g_stop) {
		if (deadline && time(NULL) >= deadline)
			break;

		int pr = poll(pfd, 2, 1000);

		if (pr < 0) {
			if (errno == EINTR)
				continue;
			fprintf(stderr, "poll: %s\n", strerror(errno));
			break;
		}

		/* GPIO edge — sample wall clock IMMEDIATELY so the trigger
		 * timestamp is as close as possible to the actual edge.
		 * Re-arm sysfs (lseek+read) before further work. */
		if (pfd[0].revents & (POLLPRI | POLLERR)) {
			uint64_t trig_ns = now_realtime_ns();

			gpio_rearm(fd_gpio);
			/* If a previous trigger is still waiting for its
			 * next_fs, emit it with what we have and overwrite.
			 * (Practically rare: the next FS arrives within
			 * one frame period of the trigger.) */
			if (t.pending) {
				struct fs_cache no_next = {0};

				print_trigger(t.trig_ns,
				              &t.last_snapshot[0], &no_next);
			}
			t.pending = 1;
			t.trig_ns = trig_ns;
			memcpy(t.last_snapshot, last, sizeof(last));
		}

		/* Drain available FS events. */
		if (pfd[1].revents & POLLIN) {
			struct openipc_frame_ts_event ev[EV_BATCH];
			ssize_t n;

			while ((n = read(fd_ft, ev, sizeof(ev))) > 0) {
				ssize_t nev = n / (ssize_t)sizeof(ev[0]);

				for (ssize_t i = 0; i < nev; i++) {
					unsigned int c = ev[i].vi_chn;

					if (c >= MAX_CHN)
						continue;
					if (ev[i].event_type !=
					    OPENIPC_FT_EVT_MIPI_FS)
						continue;
					last[c].pts_us  = ev[i].pts_us;
					last[c].wall_ns = ev[i].wall_ns;
					last[c].seq     = ev[i].seq;
					last[c].valid   = 1;

					/* If a trigger is pending and this FS
					 * arrived after it, that's the
					 * "next_fs". Emit and clear. */
					if (t.pending &&
					    ev[i].wall_ns > t.trig_ns) {
						struct fs_cache next = {
							.pts_us  = ev[i].pts_us,
							.wall_ns = ev[i].wall_ns,
							.seq     = ev[i].seq,
							.valid   = 1,
						};
						print_trigger(t.trig_ns,
						              &t.last_snapshot[c],
						              &next);
						t.pending = 0;
					}
				}
			}
			if (n < 0 && errno != EAGAIN && errno != EINTR) {
				fprintf(stderr, "read: %s\n", strerror(errno));
				break;
			}
		}
	}

	/* Emit any still-pending trigger with no next_fs match. */
	if (t.pending) {
		struct fs_cache no_next = {0};
		print_trigger(t.trig_ns, &t.last_snapshot[0], &no_next);
	}

	close(fd_ft);
	close(fd_gpio);
	return 0;
}
