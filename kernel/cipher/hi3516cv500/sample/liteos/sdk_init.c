/******************************************************************************
  Some simple Hisilicon Hi3519A system functions.

  Copyright (C), 2017, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2017-11 Created
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include "hi_type.h"
#include "osal_mmz.h"
#include "asm/io.h"
#include "hi_module_param.h"

#include "stdlib.h"
#include "fcntl.h"
#include "string.h"

/* set liteos's mmz parameter, sync form mpp sdk_init.c */
#if defined(CHIP_TYPE_hi3519av100)
//#define TEE_VERSION
#define MMZ_START_ADDR     0x52000000
#define MMZ_MSIZE          1024
#define MAP_MMZ_PARAMETER  "0x42000000,256M"
#elif defined(CHIP_TYPE_hi3516cv500)
#define MMZ_START_ADDR     0x88000000
#define MMZ_MSIZE          336
#define MAP_MMZ_PARAMETER  "0x9d000000,48M"
#elif defined(CHIP_TYPE_hi3516ev200)
#define MMZ_START_ADDR     0x42000000
#define MMZ_MSIZE          32
#define MAP_MMZ_PARAMETER  "0x42000000,32M"
#endif

#ifdef TEE_VERSION
#include "tzasc_user.h"
#endif

static unsigned long long mmz_start = MMZ_START_ADDR;
static unsigned int mmz_size = MMZ_MSIZE;   //M Byte

/******************************************************************/

#ifdef TEE_VERSION

static HI_S32 tzasc_init(char mmzparam[])
{
    tzasc_region_attr anonymous_region, svp_region, share_region;
    tzasc_handle hdl_annoymous = tzasc_invalid_handle;
    tzasc_handle hdl_svp = tzasc_invalid_handle;
    tzasc_handle hdl_share = tzasc_invalid_handle;
    int ret;

    ret = tzasc_mod_init();
    if (ret)
    {
        printf("tzasc mod init fail. \n");
        goto tzasc_init_err;
    }

    strncpy(anonymous_region.name, "anonymous", TZASC_REGION_NAME_LENGTH);
    anonymous_region.phystart = 0x52000000;
        anonymous_region.length = 512;
    anonymous_region.type = tzasc_anonymous_type;
    hdl_annoymous = tzasc_init_one_region(&anonymous_region);
    if (hdl_annoymous == tzasc_invalid_handle)
    {
        dprintf("init anonymous region fail. \n");
        ret = -1;
        goto tzasc_init_err;
    }
    mmzparam = tzasc_concatenate_mmz_string(hdl_annoymous, mmzparam);

    strncpy(svp_region.name, "svp", TZASC_REGION_NAME_LENGTH);
    svp_region.phystart = 0x72000000;
        svp_region.length = 256;
    svp_region.type = tzasc_svp_type;
    hdl_svp = tzasc_init_one_region(&svp_region);
    if (hdl_svp == tzasc_invalid_handle)
    {
        dprintf("init svp region fail. \n");
        ret = -1;
        goto tzasc_init_err;
    }
    mmzparam = tzasc_concatenate_mmz_string(hdl_svp, mmzparam);

    strncpy(share_region.name, "share", TZASC_REGION_NAME_LENGTH);
    share_region.phystart = 0x82000000;
    share_region.length = 224;
    share_region.type = tzasc_share_type;
    hdl_share = tzasc_init_one_region(&share_region);
    if (hdl_share == tzasc_invalid_handle)
    {
        dprintf("init share region fail. \n");
        ret = -1;
        goto tzasc_init_err;
    }
    mmzparam = tzasc_concatenate_mmz_string(hdl_share, mmzparam);

    return 0;

tzasc_init_err:
    if (hdl_annoymous)
        tzasc_exit_one_region(hdl_annoymous);
    if (hdl_svp)
        tzasc_exit_one_region(hdl_svp);
    if (hdl_share)
        tzasc_exit_one_region(hdl_share);
    return ret;
}
#endif

static HI_S32 MMZ_init(void)
{
    extern int media_mem_init(void * pArgs);
    MMZ_MODULE_PARAMS_S stMMZ_Param = {0};

#ifdef TEE_VERSION
    int ret;
    ret = tzasc_init(stMMZ_Param.mmz);
    if (ret)
    {
        dprintf("tzasc init err. \n");
        return ret;
    }
#else
    snprintf(stMMZ_Param.mmz, MMZ_SETUP_CMDLINE_LEN, "anonymous,0,0x%llx,%uM", mmz_start, mmz_size);
#endif

    snprintf(stMMZ_Param.map_mmz, MMZ_SETUP_CMDLINE_LEN, MAP_MMZ_PARAMETER);

    stMMZ_Param.anony = 1;

    dprintf("mmz_start=0x%llx, mmz_size=0x%x\n", mmz_start, mmz_size);
    dprintf("mmz param= %s\n", stMMZ_Param.mmz);

    return media_mem_init(&stMMZ_Param);
}

extern void osal_proc_init(void);

extern int cipher_drv_mod_init(void);

HI_VOID SDK_init(void)
{
    HI_S32 ret = 0;

    osal_proc_init();

    ret = MMZ_init();
    if (ret != 0)
    {
        printf("MMZ init error.\n");
    }

    ret = cipher_drv_mod_init();
    if (ret != 0)
    {
        printf("cipher init error.\n");
    }

    printf("SDK init ok...\n");
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

