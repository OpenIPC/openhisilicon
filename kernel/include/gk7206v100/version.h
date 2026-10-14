/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef _VERSION_H_
#define _VERSION_H_

#include <linux/types.h>
#include "xmedia_type.h"
#include "osal.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (defined SDK_VERSION_RELEASE)

#define VERSION_FORMAT(buff,module,module_version,ext_info) \
    osal_sprintf(buff,"Version:[%s %s],  [%s %s %s],  %s,  Build Time[%s %s]",\
              VERSION_MAJOR, VERSION_MINOR, \
              module,module_version,ext_info,"Release",__DATE__,__TIME__)

#elif (defined SDK_VERSION_DEBUG)

#define VERSION_FORMAT(buff,module,module_version,ext_info) \
    osal_sprintf(buff,"Version:[%s %s],  [%s %s %s],  %s,  Build Time[%s %s]",\
              VERSION_MAJOR, VERSION_MINOR, \
              module,module_version,ext_info,"Debug",__DATE__,__TIME__)

#else
    #warning "version type is invalid !!!"

    #define VERSION_FORMAT(buff,module,module_version,ext_info) \
    osal_sprintf(buff,"Version:[%s %s],  [%s %s %s],  %s,  Build Time[%s %s]",\
              VERSION_MAJOR, VERSION_MINOR, \
              module,module_version,ext_info,"Invalid",__DATE__,__TIME__)
#endif


#ifdef __cplusplus
}
#endif

#endif


