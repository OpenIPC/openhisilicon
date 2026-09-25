/*
 * Shadows include/hicompat.h for XMedia builds: the GK_API_* calls resolve
 * to XMEDIA_API_* through gk_api_{isp,ae,awb}.h here, whose prototypes come
 * from the SDK's own xmedia_api_*.h.
 */
#ifndef HICOMPAT_H
#define HICOMPAT_H

#include "gk_api_isp.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"

#endif
