/*
 *  osdrv sample
 */
#include "sys_config.h"
#include "board.h"
#include "los_typedef.h"

#include "stdlib.h"
#include "stdio.h"

#ifdef LOSCFG_DRIVERS_VIDEO
#include "linux/fb.h"
#endif
#ifdef LOSCFG_SHELL
#include "shell.h"
#include "shcmd.h"
//extern UINT32 osShellInit(void);
#endif

#ifdef LOSCFG_DRIVERS_UART
#include "console.h"
#include "hisoc/uart.h"
#include "uart.h"
#endif

#if defined(LOSCFG_DRIVERS_HIGMAC) || defined(LOSCFG_DRIVERS_HIETH_SF)
#include "eth_drv.h"
#endif
#ifndef  LOSCFG_CORTEX_M7

#if defined(LOSCFG_PLATFORM_HI3559AV100) || defined(LOSCFG_PLATFORM_HI3519AV100)
extern AARCHPTR g_sys_mem_addr_end;
#endif
#endif

extern int app_main(int argc, char *argv[]);

#define ARGS_SIZE_T 20
#define ARG_BUF_LEN_T 256
static char *ptask_args[ARGS_SIZE_T];
static char *args_buf_t = NULL;

static unsigned int taskid = -1;

void com_app(unsigned long p0, unsigned long p1, unsigned long p2, unsigned long p3)
{
    int i = 0;
    unsigned int argc = p0;
    char** argv = (char**)p1;
    //Set_Interupt(0);

    for (i = 0; i < argc; i++)
    {
        dprintf("%s ", argv[i]);
    }

    dprintf("\n");
    app_main(argc, argv);
    dprintf("[END]:app_test finish!\n");
    free(args_buf_t);
    args_buf_t = NULL;
    taskid = -1;
}


unsigned int app_sample(int argc, char **argv )
{
    int i = 0;
    unsigned int ret = 0;
    int len = 0;
    char* pch = NULL;
    TSK_INIT_PARAM_S stappTask;

    if (taskid != -1)
    {
        dprintf("There's a app_main task existed.");
    }

    args_buf_t = zalloc(ARG_BUF_LEN_T);
    memset(&stappTask, 0, sizeof(TSK_INIT_PARAM_S));
    pch = args_buf_t;

    for (i = 0; i < ARGS_SIZE_T; i++)
    {
        ptask_args[i] = NULL;
    }

    argc++;
    ptask_args[0] = "sample_cipher";

    for (i = 1; i < argc; i++)
    {
        len = strlen(argv[i - 1]);
        memcpy(pch, argv[i - 1], len);
        ptask_args[i] = pch;
        //keep a '\0' at the end of a string.
        pch = pch + len + 1;

        if (pch >= args_buf_t + ARG_BUF_LEN_T)
        {
            dprintf("args out of range!\n");
            break;
        }
    }

    memset(&stappTask, 0, sizeof(TSK_INIT_PARAM_S));
    stappTask.pfnTaskEntry = (TSK_ENTRY_FUNC)com_app;
    stappTask.uwStackSize = 0x10000;
    stappTask.pcName = "sample_cipher";
    stappTask.usTaskPrio = 10;
    stappTask.uwResved = LOS_TASK_STATUS_DETACHED;
    stappTask.auwArgs[0] = argc;
    stappTask.auwArgs[1] = (unsigned long)ptask_args;
    ret = LOS_TaskCreate(&taskid, &stappTask);

    //chdir("/sd0");
    chdir("/sharefs");

    return ret;
}

void sample_command(void)
{
    osCmdReg(CMD_TYPE_EX,"sample", 0, (CMD_CBK_FUNC)app_sample);
}

extern void SDK_init(void);

void app_init(void)
{
#ifdef LOSCFG_PLATFORM_HISI_AMP
    extern int _ipcm_vdd_init(void);
    dprintf("ipcm init ...\n");
    _ipcm_vdd_init();

#ifndef LOSCFG_PLATFORM_HI3559AV100_CORTEX_M7
    extern int sharefs_client_init(const char *path);
    dprintf("sharefs init ...\n");
    sharefs_client_init("/sharefs");
#endif

    extern int virt_tty_dev_init(void);
    dprintf("virt tty init ...\n");
    virt_tty_dev_init();
#endif

#ifdef LOSCFG_DRIVERS_UART
    dprintf("uart init ...\n");
    if (uart_dev_init() != 0)
    {
        PRINT_ERR("uart_dev_init failed");
    }
    if (virtual_serial_init(TTY_DEVICE) != 0)
    {
        PRINT_ERR("virtual_serial_init failed");
    }
    if (system_console_init(SERIAL) != 0)
    {
        PRINT_ERR("system_console_init failed\n");
    }

#endif

#ifdef LOSCFG_DRIVERS_EMMC
    size_t part0_start_sector = 16 * (0x100000 / 512);
    size_t part0_count_sector = 1024 * (0x100000 / 512);
    size_t part1_start_sector = 16 * (0x100000 / 512) + part0_count_sector;
    size_t part1_count_sector = 1024 * (0x100000 / 512);
    extern struct disk_divide_info emmc;
    add_mmc_partition(&emmc, part0_start_sector, part0_count_sector);
    add_mmc_partition(&emmc, part1_start_sector, part1_count_sector);
#endif

#ifdef LOSCFG_DRIVERS_MTD_NAND
    dprintf("nand init ...\n");
    extern int nand_init(void);

    if (!nand_init())
    {
        extern int add_mtd_partition( char * type, UINT32 start_addr, UINT32 length, UINT32 partition_num);
        add_mtd_partition("nand", 0x200000, 32 * 0x100000, 0);
        add_mtd_partition("nand", 0x200000 + 32 * 0x100000, 32 * 0x100000, 1);
        extern int mount(const char * source, const char * target,
                         const char * filesystemtype, unsigned long mountflags,
                         const void * data);
        mount("/dev/nandblk0", "/yaffs0", "yaffs", 0, NULL);
        //mount("/dev/nandblk1", "/yaffs1", "yaffs", 0, NULL);
    }

#endif

#ifdef LOSCFG_DRIVERS_MTD_SPI_NOR
    dprintf("spi nor flash init ...\n");
    extern int spinor_init(void);

    if (!spinor_init())
    {
        extern int add_mtd_partition( char * type, UINT32 start_addr, UINT32 length, UINT32 partition_num);
        add_mtd_partition("spinor", 0x100000, 2 * 0x100000, 0);
        add_mtd_partition("spinor", 3 * 0x100000, 2 * 0x100000, 1);
        extern int mount(const char * source, const char * target,
                         const char * filesystemtype, unsigned long mountflags,
                         const void * data);
        mount("/dev/spinorblk0", "/jffs0", "jffs", 0, NULL);
        //mount("/dev/spinorblk1", "/jffs1", "jffs", 0, NULL);
    }
#endif

    dprintf("g_sys_mem_addr_end=%p,\n",g_sys_mem_addr_end);
    dprintf("Date:%s.\n", __DATE__);
    dprintf("Time:%s.\n", __TIME__);
    SDK_init();
    sample_command();

    return;
}



