#!/usr/bin/env bash

# Usage: $1 <file>, <directory> or no params in case all modified files in repo
# should checked

do_file() {
  # dos2unix
  sed 's/\r//g' -i $1

  sed -i \
    -e "s/HI_FAILURE/GK_FAILURE/g" \
    -e "s/HI_SUCCESS/GK_SUCCESS/g" \
    -e 's/HI_NULL/GK_NULL/g' \
    -e 's/HI_BOOL/GK_BOOL/g' \
    -e 's/HI_FALSE/GK_FALSE/g' \
    -e 's/HI_TRUE/GK_TRUE/g' \
    -e 's/HI_U8/GK_U8/g' \
    -e 's/HI_S8/GK_S8/g' \
    -e 's/HI_U16/GK_U16/g' \
    -e 's/hi_u16/gk_u16/g' \
    -e 's/HI_S32/GK_S32/g' \
    -e 's/HI_U32/GK_U32/g' \
    -e 's/HI_UL/GK_UL/g' \
    -e 's/hi_u32/gk_u32/g' \
    -e 's/HI_U64/GK_U64/g' \
    -e 's/HI_CHAR/GK_CHAR/g' \
    -e 's/HI_VOID/GK_VOID/g' \
    -e 's/HI_UINTPTR_T/GK_UINTPTR_T/g' \
    -e 's/HI_ID_/MOD_ID_/g' \
    -e 's/HI_ERR_/ERR_CODE_/g' \
    -e 's/__HuaweiLite__/__LITEOS__/g' \
    -e 's/HI_PRINT/GK_PRINT/g' \
    -e 's/CONFIG_HI_/CONFIG_/g' \
    -e 's/CONFIG_HISI_/CONFIG_/g' \
    -e 's/HI_DBG_/MODULE_DBG_/g' \
    -e 's/HI_ISP_/ISP_/g' \
    -e 's/HiISPD/ISPD/g' \
    -e 's/hi_isp_/isp_/g' \
    -e 's/hil_is_phys_in_mmz/mmz_is_phys_in_mmz/g' \
    -e 's/hil_map_mmz_check_phys/mmz_map_mmz_check_phys/g' \
    -e 's/typedef struct hiVI_/typedef struct VI_/g' \
    -e 's/typedef struct hiISP_/typedef struct ISP_/g' \
    -e 's/typedef struct hi/typedef struct /g' \
    -e 's/typedef struct HI_/typedef struct /g' \
    -e 's/typedef enum hi/typedef enum /g' \
    -e 's/#include "hi_/#include "/g' \
    -e 's/#include "math.h"/#include "math_fun.h"/g' \
    $1
}

if [ -d $1 ]; then
  for i in `git diff --name-only $1`; do
    echo $i
    do_file $i
    clang-format -i $i
  done
else
  do_file $1
fi
