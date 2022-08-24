#!/usr/bin/env bash

# Usage: $1 <file>, <directory> or no params in case all modified files in repo
# should checked

# tested on in Hi3516EV200_SDK_V1.0.1.2 dir:
# cp -r mpp/component/isp/user/firmware/* $REPO/libraries/isp/
# ./scripts/hi2gk.sh

do_file() {
  # dos2unix
  sed 's/\r//g' -i $1

    #-e 's/hi_snprintf/snprintf_s/' \
  sed -i \
    -e 's/HI_/GK_/g' \
    -e 's/HI//g' \
    -e 's/_hi//g' \
    -e 's/hi_//g' \
    -e 's/HiISPD/ISPD/g' \
    -e 's/hil_/mmz_/g' \
    -e 's/__HuaweiLite__/__LITEOS__/g' \
    -e 's/#include "math.h"/#include "math_fun.h"/g' \
    -e 's/typedef enum hi/typedef enum /g' \
    -e 's/typedef struct hiISP_/typedef struct ISP_/g' \
    -e 's/typedef struct hi/typedef struct /g' \
    -e 's/typedef struct hiVI_/typedef struct VI_/g' \
    -e 's/typedef union hi/typedef union /g' \
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
