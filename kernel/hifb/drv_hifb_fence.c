/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
#ifdef CONFIG_HIFB_FENCE_SUPPORT
#include <linux/file.h>
#include "drv_hifb_fence.h"

/********************** Global Variable declaration **********************************************/

HI_S32 DRV_HIFB_FENCE_Init(HIFB_PAR_S *par)
{
    if (NULL == par)
    {
        return HI_FAILURE;
    }

    if (!(IS_HD_LAYER(par->u32LayerID)))
    {
        return HI_SUCCESS;
    }

    par->pFenceRefreshWorkqueue = create_singlethread_workqueue("HIFB_REFRESH_WorkQueque");
    if (par->pFenceRefreshWorkqueue == NULL)
    {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_VOID DRV_HIFB_FENCE_DeInit(HIFB_PAR_S *par)
{
    if ((par != NULL) && (NULL != par->pFenceRefreshWorkqueue))
    {
        destroy_workqueue(par->pFenceRefreshWorkqueue);
        par->pFenceRefreshWorkqueue = NULL;
    }

    return ;
}
#endif
