/* LD_PRELOAD shim: snoops ForwardWithBbox ioctl against vendor
 * open_nnie.ko, then mmaps /dev/mem at CTRL.TskPhys and dumps the
 * vendor-written tskbuf tail to /tmp/bbox_tskbuf.bin. Used to RE the
 * bbox tail layout for open-source nnie_neo. */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <dlfcn.h>

#define NNIE_IOC_FORWARD              0xc6584d00u
#define NNIE_IOC_FORWARD_WITH_BBOX    0xc6c04d01u

typedef int (*orig_ioctl_t)(int, unsigned long, ...);

static void dump_phys(uint64_t phys, uint32_t size, const char *fname)
{
	const size_t PG = 4096;
	int mfd = open("/dev/mem", O_RDONLY);
	if (mfd < 0) { perror("[dump] /dev/mem"); return; }
	size_t off = phys & (PG - 1);
	size_t map_off = phys - off;
	size_t map_size = ((size + off) + PG - 1) & ~(PG - 1);
	void *m = mmap(NULL, map_size, PROT_READ, MAP_SHARED, mfd, map_off);
	if (m == MAP_FAILED) { perror("[dump] mmap"); close(mfd); return; }
	int ofd = open(fname, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (ofd >= 0) {
		write(ofd, (uint8_t *)m + off, size);
		close(ofd);
		fprintf(stderr, "[dump] %s: %u bytes from 0x%llx\n",
		        fname, size, (unsigned long long)phys);
	}
	munmap(m, map_size);
	close(mfd);
}

int ioctl(int fd, unsigned long cmd, ...)
{
	static orig_ioctl_t orig;
	va_list ap;
	void *arg;
	int ret;

	if (!orig)
		orig = (orig_ioctl_t)dlsym(RTLD_NEXT, "ioctl");

	va_start(ap, cmd);
	arg = va_arg(ap, void *);
	va_end(ap);

	if (cmd == NNIE_IOC_FORWARD_WITH_BBOX && arg) {
		const uint8_t *buf = (const uint8_t *)arg;
		uint64_t tsk_phys = *(uint64_t *)(buf + 1648 + 48);
		uint32_t tsk_size = *(uint32_t *)(buf + 1648 + 64);
		uint64_t tmp_phys = *(uint64_t *)(buf + 1648 + 24);
		uint32_t tmp_size = *(uint32_t *)(buf + 1648 + 40);
		uint32_t src_num  = *(uint32_t *)(buf + 1648 + 0);
		uint32_t dst_num  = *(uint32_t *)(buf + 1648 + 4);
		uint32_t prop_num = *(uint32_t *)(buf + 1648 + 8);
		uint32_t seg_idx  = *(uint32_t *)(buf + 1648 + 12);
		fprintf(stderr,
		        "[dump_bbox] PRE  src=%u dst=%u prop=%u seg=%u "
		        "tsk=0x%llx/%u tmp=0x%llx/%u\n",
		        src_num, dst_num, prop_num, seg_idx,
		        (unsigned long long)tsk_phys, tsk_size,
		        (unsigned long long)tmp_phys, tmp_size);

		/* Capture the input buffer before vendor mutates it. */
		int afd = open("/tmp/bbox_arg.bin", O_CREAT|O_WRONLY|O_TRUNC, 0644);
		if (afd >= 0) { write(afd, buf, 1728); close(afd); }

		ret = orig(fd, cmd, arg);

		fprintf(stderr, "[dump_bbox] POST ret=%d — pausing 30s; "
		        "run: insmod nnie_phys_dump.ko phys=0x%llx size=%u\n",
		        ret, (unsigned long long)tsk_phys, tsk_size);
		sleep(30);
		return ret;
	}
	if (cmd == NNIE_IOC_FORWARD && arg) {
		/* Capture Forward args too — useful to compare RPN stage. */
		const uint8_t *buf = (const uint8_t *)arg;
		uint64_t tsk_phys = *(uint64_t *)(buf + 1552 + 40);
		uint32_t tsk_size = *(uint32_t *)(buf + 1552 + 56);
		ret = orig(fd, cmd, arg);
		fprintf(stderr, "[dump_bbox] FWD  tsk=0x%llx/%u ret=%d\n",
		        (unsigned long long)tsk_phys, tsk_size, ret);
		if (ret == 0 && tsk_size > 0 && tsk_size < (8 << 20))
			dump_phys(tsk_phys, tsk_size, "/tmp/fwd_tskbuf.bin");
		return ret;
	}
	return orig(fd, cmd, arg);
}
