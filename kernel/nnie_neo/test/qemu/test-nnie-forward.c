/*
 * NNIE Forward register-level smoke test for qemu-system-arm.
 *
 * Static-linked ARM binary that runs as PID 1 in a QEMU initramfs,
 * drives the NNIE block at 0x11100000 directly via /dev/mem, and
 * asserts the model returns the known-good mnist score vector.
 *
 * Boots under `qemu-system-arm -M hi3516cv500` / `-M hi3516av300`
 * with a kernel cmdline carving out 1 MB at 0x46000000 for our
 * synthetic "MMZ" region. Tests:
 *
 *   1. Register power-on values (chip ID, HW-self-populated config)
 *   2. Task descriptor + tskbuf-tail walk by the model
 *   3. dst-blob DMA write from the stub-inference path
 *   4. IRQ_STATUS finish bit + IRQ_CLEAR W1C
 *
 * Real CNN execution is out of scope here — that's a follow-up. The
 * model's stub-inference path writes a hardcoded mnist score vector
 * to the user-supplied dst phys; matching it round-trip proves the
 * descriptor decode + tskbuf walk + dst DMA path is intact.
 *
 * Build (static, no libc deps beyond startup):
 *   arm-linux-gnueabi-gcc -static -O2 -o test-nnie-forward test-nnie-forward.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/reboot.h>
#include <linux/reboot.h>

/* NNIE register offsets — mirror kernel/nnie_neo/nnie_hw_regs.h */
#define NNIE_REG_TASK_ADDR_LO    0x20
#define NNIE_REG_TASK_ADDR_HI    0x24
#define NNIE_REG_TIMEOUT_LO      0x28
#define NNIE_REG_TIMEOUT_HI      0x2C
#define NNIE_REG_START           0x30
#define NNIE_REG_IRQ_CFG         0x34
#define NNIE_REG_IRQ_CLEAR       0x38
#define NNIE_REG_IRQ_STATUS      0x3C
#define NNIE_REG_CFG_ERR_INFO    0x40
#define NNIE_REG_TASK_ID         0x48
#define NNIE_REG_CLK_GATE        0x50
#define NNIE_REG_OUTSTANDING     0x54
#define NNIE_REG_CHECK_SUM       0x68

#define NNIE_IRQ_FINISH          (1u << 0)
#define NNIE_IRQ_CFG_ERR         (1u << 2)
#define NNIE_IRQ_ALL             0x7

#define NNIE_PHYS_BASE           0x11100000UL
#define NNIE_MMIO_SIZE           0x10000UL

/* Each MMZ buffer is one page; the descriptor and tskbuf tail are
 * small enough to fit in a page each, src is 896 B (28×28 stride 32),
 * dst is 40 B (10 × int32). */
#define BUF_SIZE                 4096
#define BUF_DESC                 0
#define BUF_TSKBUF               1
#define BUF_MODEL                2
#define BUF_SRC                  3
#define BUF_DST                  4
#define NUM_BUFS                 5

static volatile uint32_t *nnie_regs;
static uint8_t           *buf_va[NUM_BUFS];
static uint64_t           buf_pa[NUM_BUFS];
static int                is_init;

static uint32_t reg_r(uint32_t off) { return nnie_regs[off / 4]; }
static void     reg_w(uint32_t off, uint32_t v) { nnie_regs[off / 4] = v; }

/* The model writes this exact vector to dst[0] on every successful
 * dispatch. Matches what real cv500 NNIE returns for mnist against
 * an all-zero input. */
static const int32_t mnist_scores_expected[10] = {
    408, 412, 401, 401, 398, 412, 398, 405, 449, 401,
};

/* Resolve a user VA to a physical address via /proc/self/pagemap.
 * Returns 0 if the page isn't present. */
static uint64_t v2p(void *va)
{
    uint64_t entry;
    int fd = open("/proc/self/pagemap", O_RDONLY);

    if (fd < 0) {
        return 0;
    }
    if (pread(fd, &entry, 8, ((uintptr_t)va / 4096) * 8) != 8) {
        close(fd);
        return 0;
    }
    close(fd);
    return (entry & (1ULL << 63))
            ? (entry & ((1ULL << 55) - 1)) * 4096
            : 0;
}

/* Allocate one DMA-friendly page (locked + zeroed) and resolve its
 * phys via the kernel pagemap. */
static int alloc_dma_page(int idx)
{
    void *p = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED, -1, 0);
    if (p == MAP_FAILED) {
        perror("mmap dma");
        return -1;
    }
    memset(p, 0, BUF_SIZE);
    buf_va[idx] = p;
    buf_pa[idx] = v2p(p);
    if (!buf_pa[idx]) {
        fprintf(stderr, "v2p failed for buf %d\n", idx);
        return -1;
    }
    return 0;
}

/* Map NNIE MMIO + allocate DMA pages. Returns 0 on success. */
static int do_mmap(void)
{
    int i, fd;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open /dev/mem");
        return -1;
    }

    nnie_regs = mmap(NULL, NNIE_MMIO_SIZE, PROT_READ | PROT_WRITE,
                     MAP_SHARED, fd, NNIE_PHYS_BASE);
    close(fd);
    if (nnie_regs == MAP_FAILED) {
        perror("mmap NNIE");
        return -1;
    }

    for (i = 0; i < NUM_BUFS; i++) {
        if (alloc_dma_page(i) < 0) {
            return -1;
        }
    }
    return 0;
}

/* Build the 64-byte HW task descriptor. Layout mirrors
 * struct nnie_hw_task in kernel/nnie_neo/nnie_hw_task.h. */
static void build_descriptor(uint32_t slot_idx)
{
    uint8_t *d = buf_va[BUF_DESC];
    memset(d, 0, 64);
    *(uint16_t *)(d + 0)   = 1;                      /* trigger_mode=instant */
    *(uint32_t *)(d + 4)   = slot_idx;
    *(uint64_t *)(d + 16)  = buf_pa[BUF_MODEL];      /* model_file_phys */
    *(uint32_t *)(d + 24)  = 0x150;                  /* seg_inst_offset (stub) */
    *(uint32_t *)(d + 28)  = 0x100;                  /* seg_inst_len    (stub) */
    *(uint64_t *)(d + 32)  = buf_pa[BUF_TSKBUF];     /* tsk_buf_phys */
    *(uint64_t *)(d + 48)  = buf_pa[BUF_MODEL];      /* tmp_buf_phys */
    *(uint32_t *)(d + 56)  = 1;                      /* src_batch_num */
}

/* Build the tskbuf tail. Mirrors nnie_build_task_tail in
 * kernel/nnie_neo/nnie_neo.c for SrcNum=1, DstNum=1:
 *
 *   +0x00  u32 src stride (32)
 *   +0x04  u32 dst stride (48)
 *   +0x08  u64 zero pad (16-byte alignment)
 *   +0x10  u64 dst[0] phys
 *   +0x18  u64 zero pad
 *   +0x20  u64 src[0] batch[0] phys
 */
static void build_tskbuf(void)
{
    uint8_t *t = buf_va[BUF_TSKBUF];
    memset(t, 0, 256);
    *(uint32_t *)(t + 0x00) = 32;                    /* src stride */
    *(uint32_t *)(t + 0x04) = 48;                    /* dst stride */
    *(uint64_t *)(t + 0x10) = buf_pa[BUF_DST];       /* §2: dst[0] phys */
    *(uint64_t *)(t + 0x20) = buf_pa[BUF_SRC];       /* §3: src[0] batch[0] phys */
}

static int run_one_forward(uint32_t slot_idx)
{
    int32_t got[10];
    int i, poll;

    /* Sentinel dst so we know the model actually wrote. */
    memset(buf_va[BUF_DST], 0x7e, 64);

    build_descriptor(slot_idx);
    build_tskbuf();

    /* Per-task register setup, matching kernel/nnie_neo/nnie_neo.c. */
    reg_w(NNIE_REG_CLK_GATE,    reg_r(NNIE_REG_CLK_GATE) | 0x80);
    reg_w(NNIE_REG_OUTSTANDING,
          (reg_r(NNIE_REG_OUTSTANDING) & ~0x1Fu) | 0x0F | 0xF00);
    reg_w(NNIE_REG_IRQ_CFG,     reg_r(NNIE_REG_IRQ_CFG) | NNIE_IRQ_ALL);
    reg_w(NNIE_REG_TIMEOUT_LO,  0xFFFFFFFF);
    reg_w(NNIE_REG_TIMEOUT_HI,  0x000000FF);
    reg_w(NNIE_REG_CHECK_SUM,   reg_r(NNIE_REG_CHECK_SUM) & ~1u);
    reg_w(NNIE_REG_IRQ_CLEAR,   NNIE_IRQ_ALL);
    reg_w(NNIE_REG_TASK_ADDR_LO, (uint32_t)buf_pa[BUF_DESC]);
    reg_w(NNIE_REG_TASK_ADDR_HI, (uint32_t)(buf_pa[BUF_DESC] >> 32));

    /* Kick — RMW OR bit 0. */
    reg_w(NNIE_REG_START, reg_r(NNIE_REG_START) | 1);

    /* Poll IRQ_STATUS for finish bit. In QEMU the model executes
     * synchronously inside the START write, so this should hit on
     * the first read. */
    for (poll = 0; poll < 1000; poll++) {
        uint32_t status = reg_r(NNIE_REG_IRQ_STATUS);
        if (status & NNIE_IRQ_FINISH) {
            break;
        }
        if (status & NNIE_IRQ_CFG_ERR) {
            uint32_t info = reg_r(NNIE_REG_CFG_ERR_INFO);
            printf("FAIL: cfg_err info=0x%08x after START (slot=%u)\n",
                   info, slot_idx);
            return 1;
        }
        usleep(1000);
    }

    if (!(reg_r(NNIE_REG_IRQ_STATUS) & NNIE_IRQ_FINISH)) {
        printf("FAIL: no finish IRQ within 1s (status=0x%x start=0x%x)\n",
               reg_r(NNIE_REG_IRQ_STATUS), reg_r(NNIE_REG_START));
        return 1;
    }

    /* TASK_ID should match the slot_idx we put in the descriptor. */
    if (reg_r(NNIE_REG_TASK_ID) != slot_idx) {
        printf("FAIL: TASK_ID=0x%x, expected slot_idx=0x%x\n",
               reg_r(NNIE_REG_TASK_ID), slot_idx);
        return 1;
    }

    /* W1C the finish bit. */
    reg_w(NNIE_REG_IRQ_CLEAR, NNIE_IRQ_FINISH);
    if (reg_r(NNIE_REG_IRQ_STATUS) & NNIE_IRQ_FINISH) {
        printf("FAIL: IRQ_CLEAR didn't clear finish bit (status=0x%x)\n",
               reg_r(NNIE_REG_IRQ_STATUS));
        return 1;
    }

    /* Read dst, compare to expected mnist vector. */
    memcpy(got, buf_va[BUF_DST], sizeof(got));
    for (i = 0; i < 10; i++) {
        if (got[i] != mnist_scores_expected[i]) {
            printf("FAIL: dst[%d]=%d, expected %d\n",
                   i, got[i], mnist_scores_expected[i]);
            return 1;
        }
    }

    return 0;
}

/* Verify register power-on defaults — must match what the QEMU
 * machine model sets in hisi_nnie_reset. Catches silent regressions
 * where the model's reset path changes shape. */
static int check_poweron(void)
{
    static const struct { uint32_t off; uint32_t expect; } expected[] = {
        { 0x00, 0x00002018 },
        { 0x04, 0x00000130 },
        { 0x08, 0x0000b017 },
        { 0x10, 0x5a5a5a5a },
        { 0x14, 0x0000ffef },
        { 0x6c, 0xffffffff },
        { 0x70, 0x000004a8 },
        { 0x78, 0x00000662 },
        { 0x90, 0x000004a8 },
    };
    int i, fails = 0;

    for (i = 0; i < (int)(sizeof(expected) / sizeof(expected[0])); i++) {
        uint32_t v = reg_r(expected[i].off);
        if (v != expected[i].expect) {
            printf("FAIL: reg+0x%02x = 0x%08x, expected 0x%08x\n",
                   expected[i].off, v, expected[i].expect);
            fails++;
        }
    }
    return fails;
}

int main(int argc, char **argv)
{
    int total = 0, fails = 0;
    int i;

    (void)argc; (void)argv;
    is_init = (getpid() == 1);
    if (is_init) {
        mkdir("/dev", 0755);
        mkdir("/proc", 0755);
        mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);
        mount("proc", "/proc", "proc", 0, NULL);
    }

    printf("nnie-forward: starting (pid=%d)\n", (int)getpid());

    if (do_mmap() < 0) {
        printf("FAIL: mmap setup failed\n");
        fails++;
        goto done;
    }

    printf("  poweron defaults  ");
    total++;
    if (check_poweron() == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
        fails++;
    }

    /* Three back-to-back Forwards with monotonic slot_idx — exercises
     * the model's per-task state + TASK_ID tracking + IRQ_CLEAR. */
    for (i = 0; i < 3; i++) {
        printf("  forward slot=%-2d   ", i);
        total++;
        if (run_one_forward(i) == 0) {
            printf("PASS\n");
        } else {
            printf("FAIL\n");
            fails++;
        }
    }

    printf("========================================\n");
    printf("Result: %d/%d passed\n", total - fails, total);
    printf("========================================\n");

done:
    if (is_init) {
        sync();
        reboot(LINUX_REBOOT_CMD_POWER_OFF);
        for (;;) sleep(1);
    }
    return fails;
}
