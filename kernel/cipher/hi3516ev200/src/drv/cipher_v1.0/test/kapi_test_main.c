#include "drv_osal_lib.h"

extern hi_void kapi_test(hi_void);

hi_s32 kapi_test_main(hi_void)
{
    kapi_test();

    return 0;
}
