@
@ -- FILES --
@

@ video_buf.c
@ cmpi_log.c
@ cmpi_mm.c
@ cmpi_mod.c
@ vb_drv.c
@ base_init.c
@ hi3516cv500_base.mod.c


_RODATA_0000: .asciz "VB_GetPoolInfo"
_RODATA_0010: .asciz "VB_CheckPhyAddr"
_RODATA_0020: .asciz "VB_InquirePool"
_RODATA_0030: .asciz "VB_Handle2Phys"
_RODATA_0040: .asciz "VB_Handle2Kern"
_RODATA_0050: .asciz "VB_Handle2BlkSize"
_RODATA_0064: .asciz "VB_InquireUserCnt"
_RODATA_0078: .asciz "VB_InquireOneUserCnt"
_RODATA_0090: .asciz "VB_InquireBlkCnt"
_RODATA_00a4: .asciz "VB_InquirePoolUserCnt"
_RODATA_00bc: .asciz "VB_Handle2Supplement"
_RODATA_00d4: .word 0x00000000
_RODATA_00d8: .asciz "VbSearchCommPoolBySize"
_RODATA_00f0: .asciz "VbCreatePool"
_RODATA_0100: .asciz "VB_GetBlkByPoolId"
_RODATA_0114: .asciz "VB_GetBlkBySizeAndModule"
_RODATA_0130: .asciz "VbDestroyPool"
_RODATA_0140: .asciz "VbDoVbExit"
_RODATA_014c: .asciz "VB_PutBlk"
_RODATA_0158: .asciz "VB_Phy2Handle"
_RODATA_0168: .asciz "VB_UserAdd"
_RODATA_0174: .asciz "VB_UserSub"
_RODATA_0180: .asciz "VB_CopySupplement"
_RODATA_0194: .asciz "VB_Handle2BlkId"
_RODATA_01a4: .asciz "VB_GetPoolId"
_RODATA_01b4: .asciz "VB_CreatePool"
_RODATA_01c4: .asciz "VB_GetBlkBySize"
_RODATA_01d4: .asciz "VB_IsBlkValid"
_RODATA_01e4: .asciz "VB_IsPoolIdValid"
_RODATA_01f8: .asciz "VB_GetBlkBySizeAndPoolId"
_RODATA_0214: .asciz "VB_DestroyPool"
_RODATA_0224: .asciz "VB_Handle2PoolId"
_RODATA_0238: .asciz "VB_InquirePoolBusyBlkCnt"
_RODATA_0254: .asciz "VB_GetConfig"
_RODATA_0264: .asciz "VB_SetConfig"
_RODATA_0274: .asciz "VB_GetModPoolConfig"
_RODATA_0288: .asciz "VB_SetModPoolConfig"
_RODATA_029c: .asciz "VbSetSupplementConf"
_RODATA_02b0: .asciz "BaseIoctl"
_RODATA_02bc: .asciz "VbDoVbInit"
_RODATA_02c8: .asciz "VbDoModVbInit"
_RODATA_02d8: .asciz "VbDoModVbExit"
_RODATA_02e8: .asciz "VB_Init"
_RODATA_02f0: .asciz "VB_Exit"
_RODATA_02f8: .asciz "LogWrite"
_RODATA_0304: .asciz "DoIoctl"
_RODATA_030c: .asciz "CMPI_RegisterModule"
_RODATA_0320: .asciz "CMPI_UnRegisterModule"
_RODATA_0338: .asciz "VB_CheckSupplement"
_RODATA_034c: .asciz "vb_force_exit"


_RODATA_STR1_4_0000: .asciz "[Func]:%s [Line]:%d [Info]:pstInfo is null!\n"
_RODATA_STR1_4_0030: .asciz "VB not initialized!\n"
_RODATA_STR1_4_0048: .asciz "[Func]:%s [Line]:%d [Info]:PoolId(%u) is invalid!\n"
_RODATA_STR1_4_007c: .asciz "[Func]:%s [Line]:%d [Info]:Pool %u is already destroyed !\n"
_RODATA_STR1_4_00b8: .asciz "[Func]:%s [Line]:%d [Info]:PhyAddr = %llx error! Pool[%d]: PhyAddr = %llx, Size = %llu\n"
_RODATA_STR1_4_0110: .asciz "[Func]:%s [Line]:%d [Info]:null point!\n"
_RODATA_STR1_4_0138: .asciz "[Func]:%s [Line]:%d [Info]:Pool ID [%u] is great than max pool Id [%u]!\n"
_RODATA_STR1_4_0184: .asciz "[Func]:%s [Line]:%d [Info]:pool id [%u] does not existed! \n"
_RODATA_STR1_4_01c0: .asciz "HI_NULL != s_apstPools[VB_H2POOLID(Handle)]"
_RODATA_STR1_4_01ec: .asciz "\nASSERT at:\n  >Function : %s\n  >Line No. : %d\n  >Condition: %s\n"
_RODATA_STR1_4_022c: .asciz "HI_NULL != s_apstBlks[VB_H2POOLID(Handle)]"
_RODATA_STR1_4_0258: .asciz "VB_H2BLKID(Handle) < s_apstPools[VB_H2POOLID(Handle)]->u32BlkCnt"
_RODATA_STR1_4_029c: .asciz "u32Uid < VB_MAX_USER"
_RODATA_STR1_4_02b4: .asciz "HI_NULL != s_apstPools"
_RODATA_STR1_4_02cc: .asciz "s_apstPools[i] != HI_NULL"
_RODATA_STR1_4_02e8: .asciz "[Func]:%s [Line]:%d [Info]:pool [%u] doesn't have blocks! \n"
_RODATA_STR1_4_0324: .asciz "[Func]:%s [Line]:%d [Info]:all blk size of vb pool (owner %d ) is smaller than the actual size %llu required!\n"
_RODATA_STR1_4_0394: .asciz "HI_TRUE == pstN->bIsCommPool"
_RODATA_STR1_4_03b4: .asciz "[Func]:%s [Line]:%d [Info]:no fit common pool!\n"
_RODATA_STR1_4_03e4: .asciz "[Func]:%s [Line]:%d [Info]:bIsCommPool is illegal!\n"
_RODATA_STR1_4_0418: .asciz "[Func]:%s [Line]:%d [Info]:pu32Poolid NULL!\n"
_RODATA_STR1_4_0448: .asciz "[Func]:%s [Line]:%d [Info]:enVbRemapMode:%d is illegal!\n"
_RODATA_STR1_4_0484: .asciz "[Func]:%s [Line]:%d [Info]:pBufName NULL!\n"
_RODATA_STR1_4_04b0: .asciz "[Func]:%s [Line]:%d [Info]:mmzname:%s!\n"
_RODATA_STR1_4_04d8: .asciz "[Func]:%s [Line]:%d [Info]:illegal u32BlkCnt, can not be bigger than 10240!\n"
_RODATA_STR1_4_0528: .asciz "[Func]:%s [Line]:%d [Info]:failed to malloc memory!(pstPool=%p,pstBlk=%p, BlkCnt=%u, Size=%llu)\n"
_RODATA_STR1_4_058c: .asciz "[Func]:%s [Line]:%d [Info]:[size = %llu, cnt = %u]vb mmz alloc:%s total size is larger than 4GB!\n"
_RODATA_STR1_4_05f0: .asciz "[Func]:%s [Line]:%d [Info]:[size = %llu, cnt = %u]vb mmz alloc:%s failed!\n"
_RODATA_STR1_4_063c: .asciz "%s_%s"
_RODATA_STR1_4_0644: .asciz "supNC"
_RODATA_STR1_4_064c: .asciz "[Func]:%s [Line]:%d [Info]:supplement nocache malloc(size:%u) from mmz:%s failed!\n"
_RODATA_STR1_4_06a0: .asciz "supC"
_RODATA_STR1_4_06a8: .asciz "[Func]:%s [Line]:%d [Info]:supplement cached malloc(size:%u) from mmz:%s failed!\n"
_RODATA_STR1_4_06fc: .asciz "[Func]:%s [Line]:%d [Info]:too many pools!\n"
_RODATA_STR1_4_0728: .asciz "[Func]:%s [Line]:%d [Info]:created pool %u!\n"
_RODATA_STR1_4_0758: .asciz "u32PoolId < s_stVbConf.u32MaxPoolCnt"
_RODATA_STR1_4_0780: .asciz "[Func]:%s [Line]:%d [Info]:Pool %u is dead !\n"
_RODATA_STR1_4_07b0: .asciz "[Func]:%s [Line]:%d [Info]:no free buffer in pool %u!\n"
_RODATA_STR1_4_07e8: .asciz "0 != pstPool->u32FreeBlkCnt"
_RODATA_STR1_4_0804: .asciz "[Func]:%s [Line]:%d [Info]:Blk [%u] in Pool[%u]:is allocated!\n"
_RODATA_STR1_4_0844: .asciz "HI_NULL != pstPool"
_RODATA_STR1_4_0858: .asciz "[Func]:%s [Line]:%d [Info]:Blk in this pool(%d) occupied by someone, please release first!\n"
_RODATA_STR1_4_08b4: .asciz "[Func]:%s [Line]:%d [Info]:destroyed pool %u!\n"
_RODATA_STR1_4_08e4: .asciz "[Func]:%s [Line]:%d [Info]:osal_down error !\n"
_RODATA_STR1_4_0914: .asciz "[Func]:%s [Line]:%d [Info]:vb already exited!\n"
_RODATA_STR1_4_0944: .asciz "[Func]:%s [Line]:%d [Info]:mod[%d] vb should be exit first!\n"
_RODATA_STR1_4_0984: .asciz "[Func]:%s [Line]:%d [Info]:please exit sys first!\n"
_RODATA_STR1_4_09b8: .asciz "HI_TRUE == s32Ret"
_RODATA_STR1_4_09cc: .asciz "[Func]:%s [Line]:%d [Info]:vb exited!\n"
_RODATA_STR1_4_09f4: .asciz "[Func]:%s [Line]:%d [Info]:try to put a free buffer block!\n"
_RODATA_STR1_4_0a30: .asciz "u32BlkId < pstPool->u32BlkCnt"
_RODATA_STR1_4_0a50: .asciz "[Func]:%s [Line]:%d [Info]:try to increase counter for a free buffer!\n"
_RODATA_STR1_4_0a98: .asciz "[Func]:%s [Line]:%d [Info]:try to sub user for a free buffer!\n"
_RODATA_STR1_4_0ad8: .asciz "[Func]:%s [Line]:%d [Info]:Blk [%u] in Pool[%u]:is subed!\n"
_RODATA_STR1_4_0b14: .asciz "[Func]:%s [Line]:%d [Info]:Dst VB handle is %d,PhyAddr 0x%llx!!!\n"
_RODATA_STR1_4_0b58: .asciz "[Func]:%s [Line]:%d [Info]:Src VB handle is %d,PhyAddr 0x%llx!!!\n"
_RODATA_STR1_4_0b9c: .asciz "[Func]:%s [Line]:%d [Info]:pJpegDCFVirAddr is null.\n"
_RODATA_STR1_4_0bd4: .asciz "[Func]:%s [Line]:%d [Info]:pIspInfoVirAddr is null.\n"
_RODATA_STR1_4_0c0c: .asciz "[Func]:%s [Line]:%d [Info]:pFrameDNGVirAddr is null.\n"
_RODATA_STR1_4_0c44: .asciz "VI    "
_RODATA_STR1_4_0c4c: .asciz "VO    "
_RODATA_STR1_4_0c54: .asciz "VGS   "
_RODATA_STR1_4_0c5c: .asciz "VENC  "
_RODATA_STR1_4_0c64: .asciz "VDEC  "
_RODATA_STR1_4_0c6c: .asciz "H265E "
_RODATA_STR1_4_0c74: .asciz "H264E "
_RODATA_STR1_4_0c7c: .asciz "JPEGE "
_RODATA_STR1_4_0c84: .asciz "H264D "
_RODATA_STR1_4_0c8c: .asciz "JPEGD "
_RODATA_STR1_4_0c94: .asciz "VPSS  "
_RODATA_STR1_4_0c9c: .asciz "DIS   "
_RODATA_STR1_4_0ca4: .asciz "USER  "
_RODATA_STR1_4_0cac: .asciz "PCIV  "
_RODATA_STR1_4_0cb4: .asciz "AI    "
_RODATA_STR1_4_0cbc: .asciz "AENC  "
_RODATA_STR1_4_0cc4: .asciz "RC    "
_RODATA_STR1_4_0ccc: .asciz "VFMW  "
_RODATA_STR1_4_0cd4: .asciz "GDC   "
_RODATA_STR1_4_0cdc: .asciz "AVS   "
_RODATA_STR1_4_0ce4: .asciz "RECT  "
_RODATA_STR1_4_0cec: .asciz "MATCH "
_RODATA_STR1_4_0cf4: .asciz "\n[VB] Version: [Hi3516CV500_MPP_V2.0.1.0 B090 Release], Build Time[Jan 14 2019, 20:23:44]\n"
_RODATA_STR1_4_0d50: .asciz "\n"
_RODATA_STR1_4_0d54: .asciz "-----VB PUB CONFIG--------------------------------------------------------------\n"
_RODATA_STR1_4_0da8: .asciz "MaxPoolCnt\n"
_RODATA_STR1_4_0db4: .asciz "%10d\n"
_RODATA_STR1_4_0dbc: .asciz "-----VB SUPPLEMENT ATTR---------------------------------------------------------\n"
_RODATA_STR1_4_0e10: .asciz "Size"
_RODATA_STR1_4_0e18: .asciz "Config"
_RODATA_STR1_4_0e20: .asciz "%8s%8s%8s\n"
_RODATA_STR1_4_0e2c: .asciz "VbCnt"
_RODATA_STR1_4_0e34: .asciz "%8d%8d"
_RODATA_STR1_4_0e3c: .asciz "%8d\n"
_RODATA_STR1_4_0e44: .asciz "-----COMMON POOL CONFIG---------------------------------------------------------\n"
_RODATA_STR1_4_0e98: .asciz "PoolId"
_RODATA_STR1_4_0ea0: .asciz "%10u"
_RODATA_STR1_4_0ea8: .asciz "\nSize  "
_RODATA_STR1_4_0eb0: .asciz "%10llu"
_RODATA_STR1_4_0eb8: .asciz "\nCount "
_RODATA_STR1_4_0ec0: .asciz "\n-----MODULE COMMON POOL CONFIG of VB_UID <%d> ---------------------------------\n"
_RODATA_STR1_4_0f14: .asciz "\n-----%s---------------------------------------------------------------------------\n"
_RODATA_STR1_4_0f6c: .asciz "PhysAddr"
_RODATA_STR1_4_0f78: .asciz "%-8s%-20s%-20s%-8s%-7s%-10s%-8s%-8s%-8s\n"
_RODATA_STR1_4_0fa4: .asciz "MinFree"
_RODATA_STR1_4_0fac: .asciz "Free"
_RODATA_STR1_4_0fb4: .asciz "BlkCnt"
_RODATA_STR1_4_0fbc: .asciz "BlkSz"
_RODATA_STR1_4_0fc4: .asciz "Owner"
_RODATA_STR1_4_0fcc: .asciz "IsComm"
_RODATA_STR1_4_0fd4: .asciz "VirtAddr"
_RODATA_STR1_4_0fe0: .asciz "%-8u0x%-18llx0x%-18lx%-8d%-7d%-10llu%-8d%-8u%-8u\n"
_RODATA_STR1_4_1014: .asciz "BLK   "
_RODATA_STR1_4_101c: .asciz "%6s"
_RODATA_STR1_4_1020: .asciz "Sum   "
_RODATA_STR1_4_1028: .asciz "%-6u"
_RODATA_STR1_4_1030: .asciz "[Func]:%s [Line]:%d [Info]:null point !\n"
_RODATA_STR1_4_105c: .asciz "[Func]:%s [Line]:%d [Info]:Vb remap mode:%d is illegal!\n"
_RODATA_STR1_4_1098: .asciz "HI_NULL != pu32PoolId"
_RODATA_STR1_4_10b0: .asciz "[Func]:%s [Line]:%d [Info]:VB not initialized! \n"
_RODATA_STR1_4_10e4: .asciz "[Func]:%s [Line]:%d [Info]:pool id [%u] is illegal! \n"
_RODATA_STR1_4_111c: .asciz "[Func]:%s [Line]:%d [Info]:Id%d, PhysAddr = %llx, PoolPhysAddr = 0x%llx, PoolSize = %llu\n"
_RODATA_STR1_4_1178: .asciz "[Func]:%s [Line]:%d [Info]:Pool %u is invalid !\n"
_RODATA_STR1_4_11ac: .asciz "[Func]:%s [Line]:%d [Info]:input blksize%llu is larger than pool blk size:%llu !\n"
_RODATA_STR1_4_1200: .asciz "*pu32Cnt <= pstPool->u32BlkCnt"
_RODATA_STR1_4_1220: .asciz "[Func]:%s [Line]:%d [Info]:invalid handle!\n"
_RODATA_STR1_4_124c: .asciz "[Func]:%s [Line]:%d [Info]:mmz name len it's too long\n"
_RODATA_STR1_4_1284: .asciz "[Func]:%s [Line]:%d [Info]:enRemapMode %d is illegal!\n"
_RODATA_STR1_4_12bc: .asciz "[Func]:%s [Line]:%d [Info]:VB is initialized!\n"
_RODATA_STR1_4_12ec: .asciz "[Func]:%s [Line]:%d [Info]:invalid u32Uid(%d) >= VB_MAX_USER(%d)!\n"
_RODATA_STR1_4_1330: .asciz "[Func]:%s [Line]:%d [Info]:Invalid user id %d !\n"
_RODATA_STR1_4_1364: .asciz "[Func]:%s [Line]:%d [Info]:VB is not initialized!\n"
_RODATA_STR1_4_1398: .asciz "[Func]:%s [Line]:%d [Info]:illegal argument!\n"
_RODATA_STR1_4_13c8: .asciz "[Func]:%s [Line]:%d [Info]:illegal parameter enRemapMode %u!\n"
_RODATA_STR1_4_1408: .asciz "UserPool"
_RODATA_STR1_4_1414: .asciz "[Func]:%s [Line]:%d [Info]:invlid pool ID [%u]!\n"
_RODATA_STR1_4_1448: .asciz "[Func]:%s [Line]:%d [Info]:try to destroy a NOT existed pool!\n"
_RODATA_STR1_4_1488: .asciz "[Func]:%s [Line]:%d [Info]:try to destroy a kernel pool!\n"
_RODATA_STR1_4_14c4: .asciz "[Func]:%s [Line]:%d [Info]:Someone is using vb now, please make sure to release vb block first!\n"
_RODATA_STR1_4_1528: .asciz "[Func]:%s [Line]:%d [Info]:no buffer block!\n"
_RODATA_STR1_4_1558: .asciz "[Func]:%s [Line]:%d [Info]:invalid pool id[%u]!\n"
_RODATA_STR1_4_158c: .asciz "[Func]:%s [Line]:%d [Info]:pool id[%u] is not existed!\n"
_RODATA_STR1_4_15c4: .asciz "[Func]:%s [Line]:%d [Info]:the size(%llu) is larger than the pool block size(%llu)!\n"
_RODATA_STR1_4_161c: .asciz "[Func]:%s [Line]:%d [Info]:invalid handle %u!\n"
_RODATA_STR1_4_164c: .asciz "[Func]:%s [Line]:%d [Info]:bad argument!\n"
_RODATA_STR1_4_1678: .asciz "user copy pstSupplementTmp failed!\n"
_RODATA_STR1_4_169c: .asciz "[Func]:%s [Line]:%d [Info]:please configrate VB first! \n"
_RODATA_STR1_4_16d8: .asciz "[Func]:%s [Line]:%d [Info]:VB_Init again!\n"
_RODATA_STR1_4_1704: .asciz "[Func]:%s [Line]:%d [Info]:no memory!\n"
_RODATA_STR1_4_172c: .asciz "VbPool"
_RODATA_STR1_4_1734: .asciz "[Func]:%s [Line]:%d [Info]:vb init ok!\n"
_RODATA_STR1_4_175c: .asciz "[Func]:%s [Line]:%d [Info]:u32Uid should between [0, %d]! \n"
_RODATA_STR1_4_1798: .asciz "[Func]:%s [Line]:%d [Info]:please configrate common VB first! \n"
_RODATA_STR1_4_17d8: .asciz "[Func]:%s [Line]:%d [Info]:please init common pools first!\n"
_RODATA_STR1_4_1814: .asciz "[Func]:%s [Line]:%d [Info]:please configrate mod common VB first! \n"
_RODATA_STR1_4_1858: .asciz "[Func]:%s [Line]:%d [Info]:module %d common pool init again!\n"
_RODATA_STR1_4_1898: .asciz "ModVb"
_RODATA_STR1_4_18a0: .asciz "[Func]:%s [Line]:%d [Info]:init module(%d) common pool failed!\n"
_RODATA_STR1_4_18e0: .asciz "[Func]:%s [Line]:%d [Info]:common module(%d) vb exited!\n"
_RODATA_STR1_4_191c: .asciz "[Func]:%s [Line]:%d [Info]:ioctl cmd does NOT exit!\n"
_RODATA_STR1_4_1954: .asciz "[Func]:%s [Line]:%d [Info]:supplement address is invalid!\n"
_RODATA_STR1_4_1990: .asciz "vb"
_RODATA_STR1_4_1994: .asciz "[Func]:%s [Line]:%d [Info]:VB createdev failed!\n"
_RODATA_STR1_4_19c8: .asciz "[Func]:%s [Line]:%d [Info]:VB register device failed!\n"
_RODATA_STR1_4_1a00: .asciz "[Func]:%s [Line]:%d [Info]:vb register module failed!.\n"
_RODATA_STR1_4_1a38: .asciz "[Func]:%s [Line]:%d [Info]:VB init OK!\n"
_RODATA_STR1_4_1a60: .asciz "Hi3516CV500"
_RODATA_STR1_4_1a6c: .asciz "load base.ko for %s...fail!\n"
_RODATA_STR1_4_1a8c: .asciz "[Func]:%s [Line]:%d [Info]:VB exit OK!\n"
_RODATA_STR1_4_1ab4: .asciz "wait"
_RODATA_STR1_4_1abc: .asciz "invalid value!\n"
_RODATA_STR1_4_1acc: .asciz "level: %d illegal!\n"
_RODATA_STR1_4_1ae0: .asciz "all"
_RODATA_STR1_4_1ae4: .asciz "string is unkown!\n"
_RODATA_STR1_4_1af8: .asciz "%s not found in array!\n"
_RODATA_STR1_4_1b10: .asciz "-----LOG BUFFER STATE----------------------------------------------------------\n"
_RODATA_STR1_4_1b64: .asciz "MaxLen  ReadPos WritePos ButtPos\n"
_RODATA_STR1_4_1b88: .asciz "%3d(KB) %7d %8d %7d\n"
_RODATA_STR1_4_1ba0: .asciz "\n"
_RODATA_STR1_4_1ba4: .asciz "-----CURRENT LOG LEVEL---------------------------------------------------------\n"
_RODATA_STR1_4_1bf8: .asciz "%s\t:  %d\n"
_RODATA_STR1_4_1c04: .asciz "s_stLogBuf.u32WritePos <= s_stLogBuf.u32MaxLen"
_RODATA_STR1_4_1c34: .asciz "\nASSERT at:\n  >Function : %s\n  >Line No. : %d\n  >Condition: %s\n"
_RODATA_STR1_4_1c74: .asciz "s_stLogBuf.u32ReadPos <= s_stLogBuf.u32MaxLen"
_RODATA_STR1_4_1ca4: .asciz "<%d>[%6s] "
_RODATA_STR1_4_1cb0: .asciz "[%6s]: Log message is too long!!\n"
_RODATA_STR1_4_1cd4: .asciz "illegal level: %d\n"
_RODATA_STR1_4_1ce8: .asciz "%s, line: %d, illegal parma, modeid: %d\n"
_RODATA_STR1_4_1d14: .asciz "%s, line: %d, illegal parma, bWait: %d\n"
_RODATA_STR1_4_1d3c: .asciz "logmpp"
_RODATA_STR1_4_1d44: .asciz "Log createdev failed!\n"
_RODATA_STR1_4_1d5c: .asciz "Log register device failed!\n"
_RODATA_STR1_4_1d7c: .asciz "Log buffer %dB malloc faild.\n"
_RODATA_STR1_4_1d9c: .asciz "Mmz malloc failed!\n"
_RODATA_STR1_4_1db0: .asciz "phy(0x%llx) size(0x%lx) map cache err!\n"
_RODATA_STR1_4_1dd8: .asciz "phy(0x%llx) size(0x%lx) map nocache err!\n"
_RODATA_STR1_4_1e04: .asciz "Mmz remap no cache failed!\n"
_RODATA_STR1_4_1e20: .asciz "Mmz remap cache failed!\n"
_RODATA_STR1_4_1e3c: .asciz "MOD [%s] is busy!\n"
_RODATA_STR1_4_1e50: .asciz "MOD[%s] already exited!\n"
_RODATA_STR1_4_1e6c: .asciz "MOD[%s] exit OK!\n"
_RODATA_STR1_4_1e80: .asciz "MOD[%s] pfnInit failed!\n"
_RODATA_STR1_4_1e9c: .asciz "HI_NULL != pstModule"
_RODATA_STR1_4_1eb4: .asciz "\nASSERT at:\n  >Function : %s\n  >Line No. : %d\n  >Condition: %s\n"
_RODATA_STR1_4_1ef4: .asciz "pstModule->enModId < MAX_MPP_MODULES"
_RODATA_STR1_4_1f1c: .asciz "HI_NULL != pstModule->pfnInit"
_RODATA_STR1_4_1f3c: .asciz "HI_NULL != pstModule->pfnExit"
_RODATA_STR1_4_1f5c: .asciz "HI_NULL != pstModule->pfnVerChecker"
_RODATA_STR1_4_1f80: .asciz "MOD[%s] already registered!\n"
_RODATA_STR1_4_1fa0: .asciz "MOD[%s] version doesn't match!\n"
_RODATA_STR1_4_1fc0: .asciz "enModId < MAX_MPP_MODULES"
_RODATA_STR1_4_1fdc: .asciz "MOD[%s] unregister OK!\n"
_RODATA_STR1_4_1ff4: .asciz "MOD[%d] already unregister!\n"
_RODATA_STR1_4_2014: .asciz "VB_Init failed!\n"
_RODATA_STR1_4_2028: .asciz "Log_Init failed!\n"
_RODATA_STR1_4_203c: .asciz "load hi35xx_base ... OK!\n"
_RODATA_STR1_4_2058: .asciz "Unload hi35xx_base ... OK!\n"
_RODATA_STR1_4_2074: .asciz "[Func]:%s [Line]:%d [Info]:VB supplement conf(%#x) is out of mask range(%#x)!\n"
_RODATA_STR1_4_20c4: .asciz "dev"
_RODATA_STR1_4_20c8: .asciz "debug"
_RODATA_STR1_4_20d0: .asciz "proc_message_enable"

.global DATA_ANCHOR0
.global DATA_ANCHOR1
.global DATA_ANCHOR2

.global BSS_ANCHOR0
.global BSS_ANCHOR1
.global BSS_ANCHOR2
.global BSS_ANCHOR3
.global BSS_ANCHOR4

@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0x69d0
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN VB_IsSupplementSupport
    /*00000000*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000004*/ LDR         R3, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00000008*/ LDR         R3, [R3] 
    /*0000000c*/ AND         R0, R0, R3 
    /*00000010*/ BX          LR 
FUNC_END VB_IsSupplementSupport


FUNC_BEGIN Close
    /*00000014*/ MOV         R0, #0 
    /*00000018*/ BX          LR 
FUNC_END Close


FUNC_BEGIN VB_ModInit
    /*0000001c*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000020*/ LDR         R3, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00000024*/ MOV         R2, #1 
    /*00000028*/ MOV         R0, #0 
    /*0000002c*/ STR         R2, [R3, #4] 
    /*00000030*/ BX          LR 
FUNC_END VB_ModInit


FUNC_BEGIN VB_GetVerMagic
    /*00000034*/ MOVW        R0, #61479                  @ 0xf027 
    /*00000038*/ MOVT        R0, #307                    @ 0x133 
    /*0000003c*/ BX          LR 
FUNC_END VB_GetVerMagic


FUNC_BEGIN VB_GetPoolInfo
    /*00000040*/ SUBS        R3, R0, #0 
    /*00000044*/ PUSH        {R4, R5, LR} 
    /*00000048*/ SUB         SP, SP, #12 
    /*0000004c*/ BEQ         VB_GetPoolInfo_xf8
    /*00000050*/ @ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000054*/ LDR         R2, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00000058*/ LDR         R0, [R2, #8] 
    /*0000005c*/ CMP         R0, #0 
    /*00000060*/ BEQ         VB_GetPoolInfo_xdc
    /*00000064*/ LDR         R2, [R2, #16] 
    /*00000068*/ LDR         R1, [R3] 
    /*0000006c*/ CMP         R1, R2 
    /*00000070*/ BCS         VB_GetPoolInfo_xac
    /*00000074*/ LDR         R2, [R0, R1, LSL #2] 
    /*00000078*/ CMP         R2, #0 
    /*0000007c*/ BEQ         VB_GetPoolInfo_x128
    /*00000080*/ LDRD        R4, [R2, #24] 
    /*00000084*/ MOV         R0, #0 
    /*00000088*/ STRD        R4, [R3, #16] 
    /*0000008c*/ LDRD        R4, [R2, #32] 
    /*00000090*/ STRD        R4, [R3, #24] 
    /*00000094*/ LDR         R1, [R2, #12] 
    /*00000098*/ STR         R1, [R3, #4] 
    /*0000009c*/ LDRD        R4, [R2, #16] 
    /*000000a0*/ STRD        R4, [R3, #8] 
    VB_GetPoolInfo_xa4:
    /*000000a4*/ ADD         SP, SP, #12 
    /*000000a8*/ POP         {R4, R5, PC} 
    VB_GetPoolInfo_xac:
    /*000000ac*/ STR         R1, [SP, #4] 
    /*000000b0*/ MOVW        R2, #1129                   @ 0x469 
    /*000000b4*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000000b8*/ STR         R2, [SP] 
    /*000000bc*/ ADDRL       R3, =_RODATA_0000           @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x0]
    /*000000c0*/ @ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x48] 
    /*000000c4*/ MOV         R1, #1 
    /*000000c8*/ ADDRL       R2, =_RODATA_STR1_4_0048    @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x48]
    /*000000cc*/ MOV         R0, #3 
    /*000000d0*/ BL          HI_LOG
    /*000000d4*/ MVN         R0, #0 
    /*000000d8*/ B           VB_GetPoolInfo_xa4
    VB_GetPoolInfo_xdc:
    /*000000dc*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*000000e0*/ MOV         R1, #1 
    /*000000e4*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000000e8*/ MOV         R0, #4 
    /*000000ec*/ BL          HI_LOG
    /*000000f0*/ MVN         R0, #0 
    /*000000f4*/ B           VB_GetPoolInfo_xa4
    VB_GetPoolInfo_xf8:
    /*000000f8*/ MOVW        R2, #1119                   @ 0x45f 
    /*000000fc*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000100*/ STR         R2, [SP] 
    /*00000104*/ ADDRL       R3, =_RODATA_0000           @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x0]
    /*00000108*/ @ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*0000010c*/ MOV         R1, #1 
    /*00000110*/ ADDRL       R2, =_RODATA_STR1_4_0000    @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0]
    /*00000114*/ MOV         R0, #3 
    /*00000118*/ BL          HI_LOG
    /*0000011c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000120*/ MOVT        R0, #40961                  @ 0xa001 
    /*00000124*/ B           VB_GetPoolInfo_xa4
    VB_GetPoolInfo_x128:
    /*00000128*/ STR         R1, [SP, #4] 
    /*0000012c*/ MOV         R2, #1136                   @ 0x470 
    /*00000130*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000134*/ STR         R2, [SP] 
    /*00000138*/ ADDRL       R3, =_RODATA_0000           @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x0]
    /*0000013c*/ @ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c] 
    /*00000140*/ MOV         R1, #1 
    /*00000144*/ ADDRL       R2, =_RODATA_STR1_4_007c    @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c]
    /*00000148*/ MOV         R0, #4 
    /*0000014c*/ BL          HI_LOG
    /*00000150*/ MVN         R0, #0 
    /*00000154*/ B           VB_GetPoolInfo_xa4
FUNC_END VB_GetPoolInfo


FUNC_BEGIN VB_CheckPhyAddr
    /*00000158*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*0000015c*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00000160*/ LDRD        R6, [R0, #32] 
    /*00000164*/ LDRD        R8, [R0, #24] 
    /*00000168*/ CMP         R7, R3 
    /*0000016c*/ CMPEQ       R6, R2 
    /*00000170*/ BHI         VB_CheckPhyAddr_x194
    /*00000174*/ ADDS        R4, R6, R8 
    /*00000178*/ ADC         R5, R7, R9 
    /*0000017c*/ CMP         R3, R5 
    /*00000180*/ CMPEQ       R2, R4 
    /*00000184*/ MOVCC       R0, #0 
    /*00000188*/ BCS         VB_CheckPhyAddr_x194
    VB_CheckPhyAddr_x18c:
    /*0000018c*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000190*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    VB_CheckPhyAddr_x194:
    /*00000194*/ LDR         R1, [R0] 
    /*00000198*/ MOV         R4, R2 
    /*0000019c*/ MOV         R5, R3 
    /*000001a0*/ STRD        R8, [SP, #32] 
    /*000001a4*/ MOVW        R3, #273                    @ 0x111 
    /*000001a8*/ @ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb8] 
    /*000001ac*/ STR         R1, [SP, #16] 
    /*000001b0*/ ADDRL       R2, =_RODATA_STR1_4_00b8    @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb8]
    /*000001b4*/ STR         R3, [SP] 
    /*000001b8*/ MOV         R1, #1 
    /*000001bc*/ STRD        R6, [SP, #24] 
    /*000001c0*/ MOV         R0, #3 
    /*000001c4*/ STRD        R4, [SP, #8] 
    /*000001c8*/ LDR         R3, WORD_01dc               @ LDR         R3, [PC, #12]               @ 0x00000000000001dc 
    /*000001cc*/ BL          HI_LOG
    /*000001d0*/ MOVW        R0, #32771                  @ 0x8003 
    /*000001d4*/ MOVT        R0, #40961                  @ 0xa001 
    /*000001d8*/ B           VB_CheckPhyAddr_x18c
    /*000001dc*/ WORD_01dc: .word 0x00000010
FUNC_END VB_CheckPhyAddr


FUNC_BEGIN VB_InquirePool
    /*000001e0*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000001e4*/ LDR         R3, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000001e8*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000001ec*/ SUB         SP, SP, #20 
    /*000001f0*/ LDR         R2, [R3, #8] 
    /*000001f4*/ CMP         R2, #0 
    /*000001f8*/ BEQ         VB_InquirePool_x2a0
    /*000001fc*/ CMP         R1, #0 
    /*00000200*/ BEQ         VB_InquirePool_x274
    /*00000204*/ LDR         R3, [R3, #16] 
    /*00000208*/ CMP         R3, R0 
    /*0000020c*/ BLS         VB_InquirePool_x240
    /*00000210*/ LDR         R3, [R2, R0, LSL #2] 
    /*00000214*/ CMP         R3, #0 
    /*00000218*/ BEQ         VB_InquirePool_x2c0
    /*0000021c*/ LDR         R2, [R3, #8] 
    /*00000220*/ MOV         R0, #0 
    /*00000224*/ STR         R2, [R1] 
    /*00000228*/ LDR         R2, [R3, #12] 
    /*0000022c*/ STR         R2, [R1, #4] 
    /*00000230*/ LDR         R3, [R3, #68]               @ 0x44 
    /*00000234*/ STR         R3, [R1, #8] 
    VB_InquirePool_x238:
    /*00000238*/ ADD         SP, SP, #20 
    /*0000023c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VB_InquirePool_x240:
    /*00000240*/ STR         R3, [SP, #8] 
    /*00000244*/ @ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00000248*/ MOVW        R3, #2094                   @ 0x82e 
    /*0000024c*/ STR         R0, [SP, #4] 
    /*00000250*/ STR         R3, [SP] 
    /*00000254*/ ADDRL       R2, =_RODATA_STR1_4_0138    @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138]
    /*00000258*/ LDR         R3, WORD_02ec               @ LDR         R3, [PC, #140]              @ 0x00000000000002ec 
    /*0000025c*/ MOV         R1, #1 
    /*00000260*/ MOV         R0, #4 
    /*00000264*/ BL          HI_LOG
    /*00000268*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000026c*/ MOVT        R0, #40961                  @ 0xa001 
    /*00000270*/ B           VB_InquirePool_x238
    VB_InquirePool_x274:
    /*00000274*/ MOVW        R3, #2087                   @ 0x827 
    /*00000278*/ @ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*0000027c*/ STR         R3, [SP] 
    /*00000280*/ ADDRL       R2, =_RODATA_STR1_4_0110    @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110]
    /*00000284*/ LDR         R3, WORD_02ec               @ LDR         R3, [PC, #96]               @ 0x00000000000002ec 
    /*00000288*/ MOV         R1, #1 
    /*0000028c*/ MOV         R0, #4 
    /*00000290*/ BL          HI_LOG
    /*00000294*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000298*/ MOVT        R0, #40961                  @ 0xa001 
    /*0000029c*/ B           VB_InquirePool_x238
    VB_InquirePool_x2a0:
    /*000002a0*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*000002a4*/ MOV         R1, #1 
    /*000002a8*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000002ac*/ MOV         R0, #4 
    /*000002b0*/ BL          HI_LOG
    /*000002b4*/ MOVW        R0, #32784                  @ 0x8010 
    /*000002b8*/ MOVT        R0, #40961                  @ 0xa001 
    /*000002bc*/ B           VB_InquirePool_x238
    VB_InquirePool_x2c0:
    /*000002c0*/ STR         R0, [SP, #4] 
    /*000002c4*/ MOVW        R3, #2101                   @ 0x835 
    /*000002c8*/ @ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x184] 
    /*000002cc*/ STR         R3, [SP] 
    /*000002d0*/ ADDRL       R2, =_RODATA_STR1_4_0184    @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x184]
    /*000002d4*/ LDR         R3, WORD_02ec               @ LDR         R3, [PC, #16]               @ 0x00000000000002ec 
    /*000002d8*/ MOV         R1, #1 
    /*000002dc*/ MOV         R0, #4 
    /*000002e0*/ BL          HI_LOG
    /*000002e4*/ MVN         R0, #0 
    /*000002e8*/ B           VB_InquirePool_x238
    /*000002ec*/ WORD_02ec: .word 0x00000020
FUNC_END VB_InquirePool


FUNC_BEGIN VB_Handle2Phys
    /*000002f0*/ PUSH        {R4, R5, R6, LR} 
    /*000002f4*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000002f8*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000002fc*/ LDR         R3, [R4, #8] 
    /*00000300*/ CMP         R3, #0 
    /*00000304*/ BEQ         VB_Handle2Phys_x3b8
    /*00000308*/ LSR         R6, R0, #16 
    /*0000030c*/ UXTH        R5, R0 
    /*00000310*/ LDR         R3, [R3, R6, LSL #2] 
    /*00000314*/ CMP         R3, #0 
    /*00000318*/ BEQ         VB_Handle2Phys_x398
    VB_Handle2Phys_x31c:
    /*0000031c*/ LDR         R3, [R4, #536]              @ 0x218 
    /*00000320*/ LDR         R3, [R3, R6, LSL #2] 
    /*00000324*/ CMP         R3, #0 
    /*00000328*/ BEQ         VB_Handle2Phys_x378
    VB_Handle2Phys_x32c:
    /*0000032c*/ LDR         R3, [R4, #8] 
    /*00000330*/ LDR         R3, [R3, R6, LSL #2] 
    /*00000334*/ LDR         R3, [R3, #12] 
    /*00000338*/ CMP         R5, R3 
    /*0000033c*/ BCS         VB_Handle2Phys_x358
    VB_Handle2Phys_x340:
    /*00000340*/ LDR         R2, [R4, #536]              @ 0x218 
    /*00000344*/ MOV         R3, #200                    @ 0xc8 
    /*00000348*/ LDR         R0, [R2, R6, LSL #2] 
    /*0000034c*/ MLA         R0, R3, R5, R0 
    /*00000350*/ LDRD        R0, [R0, #16] 
    /*00000354*/ POP         {R4, R5, R6, PC} 
    VB_Handle2Phys_x358:
    /*00000358*/ @ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258] 
    /*0000035c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000360*/ ADDRL       R3, =_RODATA_STR1_4_0258    @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258]
    /*00000364*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000368*/ MOVW        R2, #1384                   @ 0x568 
    /*0000036c*/ LDR         R1, WORD_03d8               @ LDR         R1, [PC, #100]              @ 0x00000000000003d8 
    /*00000370*/ BL          osal_panic
    /*00000374*/ B           VB_Handle2Phys_x340
    VB_Handle2Phys_x378:
    /*00000378*/ @ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22c] 
    /*0000037c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000380*/ ADDRL       R3, =_RODATA_STR1_4_022c    @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22c]
    /*00000384*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000388*/ MOVW        R2, #1384                   @ 0x568 
    /*0000038c*/ LDR         R1, WORD_03d8               @ LDR         R1, [PC, #68]               @ 0x00000000000003d8 
    /*00000390*/ BL          osal_panic
    /*00000394*/ B           VB_Handle2Phys_x32c
    VB_Handle2Phys_x398:
    /*00000398*/ @ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c0] 
    /*0000039c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000003a0*/ ADDRL       R3, =_RODATA_STR1_4_01c0    @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c0]
    /*000003a4*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000003a8*/ MOVW        R2, #1384                   @ 0x568 
    /*000003ac*/ LDR         R1, WORD_03d8               @ LDR         R1, [PC, #36]               @ 0x00000000000003d8 
    /*000003b0*/ BL          osal_panic
    /*000003b4*/ B           VB_Handle2Phys_x31c
    VB_Handle2Phys_x3b8:
    /*000003b8*/ MOV         R1, #1 
    /*000003bc*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*000003c0*/ MOV         R0, #4 
    /*000003c4*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000003c8*/ BL          HI_LOG
    /*000003cc*/ MOV         R0, #0 
    /*000003d0*/ MOV         R1, #0 
    /*000003d4*/ POP         {R4, R5, R6, PC} 
    /*000003d8*/ WORD_03d8: .word 0x00000030
FUNC_END VB_Handle2Phys


FUNC_BEGIN VB_Handle2Kern
    /*000003dc*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000003e0*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000003e4*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000003e8*/ LDR         R5, [R4, #8] 
    /*000003ec*/ CMP         R5, #0 
    /*000003f0*/ BEQ         VB_Handle2Kern_x4a8
    /*000003f4*/ LSR         R7, R0, #16 
    /*000003f8*/ UXTH        R6, R0 
    /*000003fc*/ LDR         R3, [R5, R7, LSL #2] 
    /*00000400*/ CMP         R3, #0 
    /*00000404*/ BEQ         VB_Handle2Kern_x488
    VB_Handle2Kern_x408:
    /*00000408*/ LDR         R3, [R4, #536]              @ 0x218 
    /*0000040c*/ LDR         R3, [R3, R7, LSL #2] 
    /*00000410*/ CMP         R3, #0 
    /*00000414*/ BEQ         VB_Handle2Kern_x468
    VB_Handle2Kern_x418:
    /*00000418*/ LDR         R3, [R4, #8] 
    /*0000041c*/ LDR         R3, [R3, R7, LSL #2] 
    /*00000420*/ LDR         R3, [R3, #12] 
    /*00000424*/ CMP         R6, R3 
    /*00000428*/ BCS         VB_Handle2Kern_x448
    VB_Handle2Kern_x42c:
    /*0000042c*/ LDR         R2, [R4, #536]              @ 0x218 
    /*00000430*/ MOV         R3, #200                    @ 0xc8 
    /*00000434*/ LDR         R0, [R2, R7, LSL #2] 
    /*00000438*/ MLA         R0, R3, R6, R0 
    /*0000043c*/ LDR         R0, [R0, #24] 
    VB_Handle2Kern_x440:
    /*00000440*/ MOV         R1, #0 
    /*00000444*/ POP         {R4, R5, R6, R7, R8, PC} 
    VB_Handle2Kern_x448:
    /*00000448*/ @ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258] 
    /*0000044c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000450*/ ADDRL       R3, =_RODATA_STR1_4_0258    @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258]
    /*00000454*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000458*/ MOVW        R2, #1407                   @ 0x57f 
    /*0000045c*/ LDR         R1, WORD_04c4               @ LDR         R1, [PC, #96]               @ 0x00000000000004c4 
    /*00000460*/ BL          osal_panic
    /*00000464*/ B           VB_Handle2Kern_x42c
    VB_Handle2Kern_x468:
    /*00000468*/ @ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22c] 
    /*0000046c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000470*/ ADDRL       R3, =_RODATA_STR1_4_022c    @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22c]
    /*00000474*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000478*/ MOVW        R2, #1407                   @ 0x57f 
    /*0000047c*/ LDR         R1, WORD_04c4               @ LDR         R1, [PC, #64]               @ 0x00000000000004c4 
    /*00000480*/ BL          osal_panic
    /*00000484*/ B           VB_Handle2Kern_x418
    VB_Handle2Kern_x488:
    /*00000488*/ @ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c0] 
    /*0000048c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000490*/ ADDRL       R3, =_RODATA_STR1_4_01c0    @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c0]
    /*00000494*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000498*/ MOVW        R2, #1407                   @ 0x57f 
    /*0000049c*/ LDR         R1, WORD_04c4               @ LDR         R1, [PC, #32]               @ 0x00000000000004c4 
    /*000004a0*/ BL          osal_panic
    /*000004a4*/ B           VB_Handle2Kern_x408
    VB_Handle2Kern_x4a8:
    /*000004a8*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*000004ac*/ MOV         R1, #1 
    /*000004b0*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000004b4*/ MOV         R0, #4 
    /*000004b8*/ BL          HI_LOG
    /*000004bc*/ MOV         R0, R5 
    /*000004c0*/ B           VB_Handle2Kern_x440
    /*000004c4*/ WORD_04c4: .word 0x00000040
FUNC_END VB_Handle2Kern


FUNC_BEGIN VB_Handle2BlkSize
    /*000004c8*/ PUSH        {R4, R5, R6, LR} 
    /*000004cc*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000004d0*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000004d4*/ LDR         R3, [R4, #8] 
    /*000004d8*/ CMP         R3, #0 
    /*000004dc*/ BEQ         VB_Handle2BlkSize_x590
    /*000004e0*/ LSR         R6, R0, #16 
    /*000004e4*/ UXTH        R5, R0 
    /*000004e8*/ LDR         R3, [R3, R6, LSL #2] 
    /*000004ec*/ CMP         R3, #0 
    /*000004f0*/ BEQ         VB_Handle2BlkSize_x570
    VB_Handle2BlkSize_x4f4:
    /*000004f4*/ LDR         R3, [R4, #536]              @ 0x218 
    /*000004f8*/ LDR         R3, [R3, R6, LSL #2] 
    /*000004fc*/ CMP         R3, #0 
    /*00000500*/ BEQ         VB_Handle2BlkSize_x550
    VB_Handle2BlkSize_x504:
    /*00000504*/ LDR         R3, [R4, #8] 
    /*00000508*/ LDR         R3, [R3, R6, LSL #2] 
    /*0000050c*/ LDR         R3, [R3, #12] 
    /*00000510*/ CMP         R5, R3 
    /*00000514*/ BCS         VB_Handle2BlkSize_x530
    VB_Handle2BlkSize_x518:
    /*00000518*/ LDR         R2, [R4, #536]              @ 0x218 
    /*0000051c*/ MOV         R3, #200                    @ 0xc8 
    /*00000520*/ LDR         R0, [R2, R6, LSL #2] 
    /*00000524*/ MLA         R0, R3, R5, R0 
    /*00000528*/ LDRD        R0, [R0, #128]              @ 0x80 
    /*0000052c*/ POP         {R4, R5, R6, PC} 
    VB_Handle2BlkSize_x530:
    /*00000530*/ @ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258] 
    /*00000534*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000538*/ ADDRL       R3, =_RODATA_STR1_4_0258    @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258]
    /*0000053c*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000540*/ MOVW        R2, #1430                   @ 0x596 
    /*00000544*/ LDR         R1, WORD_05b0               @ LDR         R1, [PC, #100]              @ 0x00000000000005b0 
    /*00000548*/ BL          osal_panic
    /*0000054c*/ B           VB_Handle2BlkSize_x518
    VB_Handle2BlkSize_x550:
    /*00000550*/ @ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22c] 
    /*00000554*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000558*/ ADDRL       R3, =_RODATA_STR1_4_022c    @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22c]
    /*0000055c*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000560*/ MOVW        R2, #1430                   @ 0x596 
    /*00000564*/ LDR         R1, WORD_05b0               @ LDR         R1, [PC, #68]               @ 0x00000000000005b0 
    /*00000568*/ BL          osal_panic
    /*0000056c*/ B           VB_Handle2BlkSize_x504
    VB_Handle2BlkSize_x570:
    /*00000570*/ @ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c0] 
    /*00000574*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000578*/ ADDRL       R3, =_RODATA_STR1_4_01c0    @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c0]
    /*0000057c*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000580*/ MOVW        R2, #1430                   @ 0x596 
    /*00000584*/ LDR         R1, WORD_05b0               @ LDR         R1, [PC, #36]               @ 0x00000000000005b0 
    /*00000588*/ BL          osal_panic
    /*0000058c*/ B           VB_Handle2BlkSize_x4f4
    VB_Handle2BlkSize_x590:
    /*00000590*/ MOV         R1, #1 
    /*00000594*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00000598*/ MOV         R0, #4 
    /*0000059c*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000005a0*/ BL          HI_LOG
    /*000005a4*/ MOV         R0, #0 
    /*000005a8*/ MOV         R1, #0 
    /*000005ac*/ POP         {R4, R5, R6, PC} 
    /*000005b0*/ WORD_05b0: .word 0x00000050
FUNC_END VB_Handle2BlkSize


FUNC_BEGIN VB_InquireUserCnt
    /*000005b4*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000005b8*/ @ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000005bc*/ LDR         R5, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000005c0*/ LDR         R4, [R5, #8] 
    /*000005c4*/ CMP         R4, #0 
    /*000005c8*/ BEQ         VB_InquireUserCnt_x69c
    /*000005cc*/ LSR         R7, R0, #16 
    /*000005d0*/ MOV         R6, R0 
    /*000005d4*/ LDR         R3, [R4, R7, LSL #2] 
    /*000005d8*/ CMP         R3, #0 
    /*000005dc*/ BEQ         VB_InquireUserCnt_x67c
    VB_InquireUserCnt_x5e0:
    /*000005e0*/ LDR         R3, [R5, #536]              @ 0x218 
    /*000005e4*/ LDR         R3, [R3, R7, LSL #2] 
    /*000005e8*/ CMP         R3, #0 
    /*000005ec*/ BEQ         VB_InquireUserCnt_x65c
    VB_InquireUserCnt_x5f0:
    /*000005f0*/ LDR         R3, [R5, #8] 
    /*000005f4*/ UXTH        R6, R6 
    /*000005f8*/ LDR         R3, [R3, R7, LSL #2] 
    /*000005fc*/ LDR         R3, [R3, #12] 
    /*00000600*/ CMP         R6, R3 
    /*00000604*/ BCS         VB_InquireUserCnt_x63c
    VB_InquireUserCnt_x608:
    /*00000608*/ LDR         R2, [R5, #536]              @ 0x218 
    /*0000060c*/ MOV         R3, #200                    @ 0xc8 
    /*00000610*/ MOV         R0, #0 
    /*00000614*/ LDR         R1, [R2, R7, LSL #2] 
    /*00000618*/ MLA         R1, R3, R6, R1 
    /*0000061c*/ ADD         R3, R1, #28 
    /*00000620*/ ADD         R1, R1, #120                @ 0x78 
    VB_InquireUserCnt_x624:
    /*00000624*/ LDR         R2, [R3, #4]! 
    /*00000628*/ CMP         R2, #0 
    /*0000062c*/ ADDNE       R0, R0, #1 
    /*00000630*/ CMP         R1, R3 
    /*00000634*/ BNE         VB_InquireUserCnt_x624
    /*00000638*/ POP         {R4, R5, R6, R7, R8, PC} 
    VB_InquireUserCnt_x63c:
    /*0000063c*/ @ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258] 
    /*00000640*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000644*/ ADDRL       R3, =_RODATA_STR1_4_0258    @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258]
    /*00000648*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*0000064c*/ MOV         R2, #1936                   @ 0x790 
    /*00000650*/ LDR         R1, WORD_06b8               @ LDR         R1, [PC, #96]               @ 0x00000000000006b8 
    /*00000654*/ BL          osal_panic
    /*00000658*/ B           VB_InquireUserCnt_x608
    VB_InquireUserCnt_x65c:
    /*0000065c*/ @ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22c] 
    /*00000660*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000664*/ ADDRL       R3, =_RODATA_STR1_4_022c    @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22c]
    /*00000668*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*0000066c*/ MOV         R2, #1936                   @ 0x790 
    /*00000670*/ LDR         R1, WORD_06b8               @ LDR         R1, [PC, #64]               @ 0x00000000000006b8 
    /*00000674*/ BL          osal_panic
    /*00000678*/ B           VB_InquireUserCnt_x5f0
    VB_InquireUserCnt_x67c:
    /*0000067c*/ @ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c0] 
    /*00000680*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000684*/ ADDRL       R3, =_RODATA_STR1_4_01c0    @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c0]
    /*00000688*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*0000068c*/ MOV         R2, #1936                   @ 0x790 
    /*00000690*/ LDR         R1, WORD_06b8               @ LDR         R1, [PC, #32]               @ 0x00000000000006b8 
    /*00000694*/ BL          osal_panic
    /*00000698*/ B           VB_InquireUserCnt_x5e0
    VB_InquireUserCnt_x69c:
    /*0000069c*/ MOV         R1, #1 
    /*000006a0*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*000006a4*/ MOV         R0, #4 
    /*000006a8*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000006ac*/ BL          HI_LOG
    /*000006b0*/ MOV         R0, R4 
    /*000006b4*/ POP         {R4, R5, R6, R7, R8, PC} 
    /*000006b8*/ WORD_06b8: .word 0x00000064
FUNC_END VB_InquireUserCnt


FUNC_BEGIN VB_InquireOneUserCnt
    /*000006bc*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000006c0*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000006c4*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000006c8*/ LDR         R5, [R4, #8] 
    /*000006cc*/ CMP         R5, #0 
    /*000006d0*/ BEQ         VB_InquireOneUserCnt_x7e4
    /*000006d4*/ CMP         R1, #22 
    /*000006d8*/ MOV         R6, R0 
    /*000006dc*/ MOV         R7, R1 
    /*000006e0*/ LSR         R8, R0, #16 
    /*000006e4*/ BHI         VB_InquireOneUserCnt_x738
    VB_InquireOneUserCnt_x6e8:
    /*000006e8*/ LDR         R3, [R5, R8, LSL #2] 
    /*000006ec*/ CMP         R3, #0 
    /*000006f0*/ BEQ         VB_InquireOneUserCnt_x7a4
    VB_InquireOneUserCnt_x6f4:
    /*000006f4*/ LDR         R3, [R4, #536]              @ 0x218 
    /*000006f8*/ LDR         R3, [R3, R8, LSL #2] 
    /*000006fc*/ CMP         R3, #0 
    /*00000700*/ BEQ         VB_InquireOneUserCnt_x7c4
    VB_InquireOneUserCnt_x704:
    /*00000704*/ LDR         R3, [R4, #8] 
    /*00000708*/ UXTH        R6, R6 
    /*0000070c*/ LDR         R3, [R3, R8, LSL #2] 
    /*00000710*/ LDR         R3, [R3, #12] 
    /*00000714*/ CMP         R6, R3 
    /*00000718*/ BCS         VB_InquireOneUserCnt_x784
    VB_InquireOneUserCnt_x71c:
    /*0000071c*/ LDR         R2, [R4, #536]              @ 0x218 
    /*00000720*/ MOV         R3, #200                    @ 0xc8 
    /*00000724*/ ADD         R1, R7, #8 
    /*00000728*/ LDR         R0, [R2, R8, LSL #2] 
    /*0000072c*/ MLA         R0, R3, R6, R0 
    /*00000730*/ LDR         R0, [R0, R1, LSL #2] 
    /*00000734*/ POP         {R4, R5, R6, R7, R8, PC} 
    VB_InquireOneUserCnt_x738:
    /*00000738*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*0000073c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000740*/ ADDRL       R3, =_RODATA_STR1_4_029c    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c]
    /*00000744*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000748*/ MOVW        R2, #1976                   @ 0x7b8 
    /*0000074c*/ LDR         R1, WORD_0800               @ LDR         R1, [PC, #172]              @ 0x0000000000000800 
    /*00000750*/ BL          osal_panic
    /*00000754*/ LDR         R5, [R4, #8] 
    /*00000758*/ CMP         R5, #0 
    /*0000075c*/ BNE         VB_InquireOneUserCnt_x6e8
    /*00000760*/ @ MOVW        R3, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b4] 
    /*00000764*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000768*/ ADDRL       R3, =_RODATA_STR1_4_02b4    @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b4]
    /*0000076c*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000770*/ MOVW        R2, #1977                   @ 0x7b9 
    /*00000774*/ LDR         R1, WORD_0800               @ LDR         R1, [PC, #132]              @ 0x0000000000000800 
    /*00000778*/ BL          osal_panic
    /*0000077c*/ LDR         R5, [R4, #8] 
    /*00000780*/ B           VB_InquireOneUserCnt_x6e8
    VB_InquireOneUserCnt_x784:
    /*00000784*/ @ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258] 
    /*00000788*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*0000078c*/ ADDRL       R3, =_RODATA_STR1_4_0258    @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258]
    /*00000790*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000794*/ MOVW        R2, #1977                   @ 0x7b9 
    /*00000798*/ LDR         R1, WORD_0800               @ LDR         R1, [PC, #96]               @ 0x0000000000000800 
    /*0000079c*/ BL          osal_panic
    /*000007a0*/ B           VB_InquireOneUserCnt_x71c
    VB_InquireOneUserCnt_x7a4:
    /*000007a4*/ @ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c0] 
    /*000007a8*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000007ac*/ ADDRL       R3, =_RODATA_STR1_4_01c0    @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c0]
    /*000007b0*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000007b4*/ MOVW        R2, #1977                   @ 0x7b9 
    /*000007b8*/ LDR         R1, WORD_0800               @ LDR         R1, [PC, #64]               @ 0x0000000000000800 
    /*000007bc*/ BL          osal_panic
    /*000007c0*/ B           VB_InquireOneUserCnt_x6f4
    VB_InquireOneUserCnt_x7c4:
    /*000007c4*/ @ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22c] 
    /*000007c8*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000007cc*/ ADDRL       R3, =_RODATA_STR1_4_022c    @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22c]
    /*000007d0*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000007d4*/ MOVW        R2, #1977                   @ 0x7b9 
    /*000007d8*/ LDR         R1, WORD_0800               @ LDR         R1, [PC, #32]               @ 0x0000000000000800 
    /*000007dc*/ BL          osal_panic
    /*000007e0*/ B           VB_InquireOneUserCnt_x704
    VB_InquireOneUserCnt_x7e4:
    /*000007e4*/ MOV         R1, #1 
    /*000007e8*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*000007ec*/ MOV         R0, #4 
    /*000007f0*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000007f4*/ BL          HI_LOG
    /*000007f8*/ MOV         R0, R5 
    /*000007fc*/ POP         {R4, R5, R6, R7, R8, PC} 
    /*00000800*/ WORD_0800: .word 0x00000078
FUNC_END VB_InquireOneUserCnt


FUNC_BEGIN VB_InquireTotalUserCnt
    /*00000804*/ PUSH        {R4, R5, R6, LR} 
    /*00000808*/ MOV         R5, #0 
    /*0000080c*/ MOV         R6, R0 
    /*00000810*/ MOV         R4, R5 
    VB_InquireTotalUserCnt_x814:
    /*00000814*/ MOV         R1, R4 
    /*00000818*/ MOV         R0, R6 
    /*0000081c*/ BL          VB_InquireOneUserCnt
    /*00000820*/ ADD         R4, R4, #1 
    /*00000824*/ CMP         R4, #23 
    /*00000828*/ ADD         R5, R5, R0 
    /*0000082c*/ BNE         VB_InquireTotalUserCnt_x814
    /*00000830*/ MOV         R0, R5 
    /*00000834*/ POP         {R4, R5, R6, PC} 
FUNC_END VB_InquireTotalUserCnt


FUNC_BEGIN VB_InquireBlkCnt
    /*00000838*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000083c*/ @ MOVW        R8, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000840*/ LDR         R8, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00000844*/ SUB         SP, SP, #12 
    /*00000848*/ LDR         R5, [R8, #8] 
    /*0000084c*/ STR         R1, [SP] 
    /*00000850*/ CMP         R5, #0 
    /*00000854*/ BEQ         VB_InquireBlkCnt_x968
    /*00000858*/ CMP         R0, #22 
    /*0000085c*/ MOV         R9, R0 
    /*00000860*/ BHI         VB_InquireBlkCnt_x940
    VB_InquireBlkCnt_x864:
    /*00000864*/ LDR         R7, [R8, #16] 
    /*00000868*/ CMP         R7, #0 
    /*0000086c*/ BEQ         VB_InquireBlkCnt_x960
    /*00000870*/ MOV         R5, #0 
    /*00000874*/ @ MOVW        FP, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cc] 
    /*00000878*/ LSL         R9, R9, #2 
    /*0000087c*/ ADDRL       FP, =_RODATA_STR1_4_02cc    @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cc]
    /*00000880*/ MOV         R7, R5 
    /*00000884*/ MOV         R6, R5 
    /*00000888*/ @ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*0000088c*/ ADDRL       R3, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000890*/ STR         R3, [SP, #4] 
    VB_InquireBlkCnt_x894:
    /*00000894*/ LDR         R3, [R8, #536]              @ 0x218 
    /*00000898*/ LSL         SL, R6, #2 
    /*0000089c*/ LDR         R4, [R3, R6, LSL #2] 
    /*000008a0*/ CMP         R4, #0 
    /*000008a4*/ BEQ         VB_InquireBlkCnt_x8f8
    /*000008a8*/ LDR         R3, [R8, #8] 
    /*000008ac*/ LDR         IP, [R3, R6, LSL #2] 
    /*000008b0*/ CMP         IP, #0 
    /*000008b4*/ BEQ         VB_InquireBlkCnt_x920
    VB_InquireBlkCnt_x8b8:
    /*000008b8*/ LDR         R0, [IP, #12] 
    /*000008bc*/ CMP         R0, #0 
    /*000008c0*/ BEQ         VB_InquireBlkCnt_x8f8
    /*000008c4*/ ADD         R3, R4, R9 
    /*000008c8*/ MOV         R2, #0 
    VB_InquireBlkCnt_x8cc:
    /*000008cc*/ LDR         R1, [R3, #32] 
    /*000008d0*/ ADD         R2, R2, #1 
    /*000008d4*/ ADD         R3, R3, #200                @ 0xc8 
    /*000008d8*/ CMP         R1, #0 
    /*000008dc*/ BEQ         VB_InquireBlkCnt_x8f0
    /*000008e0*/ LDR         R1, [IP, #8] 
    /*000008e4*/ ADD         R5, R5, #1 
    /*000008e8*/ CMP         R1, #0 
    /*000008ec*/ ADDNE       R7, R7, #1 
    VB_InquireBlkCnt_x8f0:
    /*000008f0*/ CMP         R2, R0 
    /*000008f4*/ BNE         VB_InquireBlkCnt_x8cc
    VB_InquireBlkCnt_x8f8:
    /*000008f8*/ LDR         R3, [R8, #16] 
    /*000008fc*/ ADD         R6, R6, #1 
    /*00000900*/ CMP         R3, R6 
    /*00000904*/ BHI         VB_InquireBlkCnt_x894
    VB_InquireBlkCnt_x908:
    /*00000908*/ LDR         R3, [SP] 
    /*0000090c*/ CMP         R3, #0 
    /*00000910*/ MOVNE       R5, R7 
    VB_InquireBlkCnt_x914:
    /*00000914*/ MOV         R0, R5 
    /*00000918*/ ADD         SP, SP, #12 
    /*0000091c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    VB_InquireBlkCnt_x920:
    /*00000920*/ MOV         R3, FP 
    /*00000924*/ MOVW        R2, #2041                   @ 0x7f9 
    /*00000928*/ LDR         R1, WORD_0980               @ LDR         R1, [PC, #80]               @ 0x0000000000000980 
    /*0000092c*/ LDR         R0, [SP, #4] 
    /*00000930*/ BL          osal_panic
    /*00000934*/ LDR         R3, [R8, #8] 
    /*00000938*/ LDR         IP, [R3, SL] 
    /*0000093c*/ B           VB_InquireBlkCnt_x8b8
    VB_InquireBlkCnt_x940:
    /*00000940*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*00000944*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000948*/ ADDRL       R3, =_RODATA_STR1_4_029c    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c]
    /*0000094c*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000950*/ MOVW        R2, #2031                   @ 0x7ef 
    /*00000954*/ LDR         R1, WORD_0980               @ LDR         R1, [PC, #36]               @ 0x0000000000000980 
    /*00000958*/ BL          osal_panic
    /*0000095c*/ B           VB_InquireBlkCnt_x864
    VB_InquireBlkCnt_x960:
    /*00000960*/ MOV         R5, R7 
    /*00000964*/ B           VB_InquireBlkCnt_x908
    VB_InquireBlkCnt_x968:
    /*00000968*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*0000096c*/ MOV         R1, #1 
    /*00000970*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00000974*/ MOV         R0, #4 
    /*00000978*/ BL          HI_LOG
    /*0000097c*/ B           VB_InquireBlkCnt_x914
    /*00000980*/ WORD_0980: .word 0x00000090
FUNC_END VB_InquireBlkCnt


FUNC_BEGIN VB_InquirePoolUserCnt
    /*00000984*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00000988*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000098c*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00000990*/ SUB         SP, SP, #20 
    /*00000994*/ LDR         R3, [R4, #8] 
    /*00000998*/ CMP         R3, #0 
    /*0000099c*/ BEQ         VB_InquirePoolUserCnt_xaa8
    /*000009a0*/ CMP         R1, #22 
    /*000009a4*/ MOV         R6, R0 
    /*000009a8*/ MOV         R7, R1 
    /*000009ac*/ MOV         R5, R2 
    /*000009b0*/ BHI         VB_InquirePoolUserCnt_xa24
    VB_InquirePoolUserCnt_x9b4:
    /*000009b4*/ LDR         R3, [R4, #16] 
    /*000009b8*/ CMP         R3, R6 
    /*000009bc*/ BLS         VB_InquirePoolUserCnt_xa44
    /*000009c0*/ LDR         R3, [R4, #8] 
    /*000009c4*/ LDR         R3, [R3, R6, LSL #2] 
    /*000009c8*/ CMP         R3, #0 
    /*000009cc*/ BEQ         VB_InquirePoolUserCnt_xa78
    /*000009d0*/ LDR         R2, [R4, #536]              @ 0x218 
    /*000009d4*/ LDR         R1, [R2, R6, LSL #2] 
    /*000009d8*/ CMP         R1, #0 
    /*000009dc*/ BEQ         VB_InquirePoolUserCnt_xac8
    /*000009e0*/ LDR         IP, [R3, #12] 
    /*000009e4*/ CMP         IP, #0 
    /*000009e8*/ MOVEQ       R2, IP 
    /*000009ec*/ BEQ         VB_InquirePoolUserCnt_xa14
    /*000009f0*/ MOV         R2, #0 
    /*000009f4*/ ADD         R1, R1, R7, LSL #2 
    /*000009f8*/ MOV         R3, R2 
    VB_InquirePoolUserCnt_x9fc:
    /*000009fc*/ ADD         R3, R3, #1 
    /*00000a00*/ LDR         R0, [R1, #32] 
    /*00000a04*/ CMP         R3, IP 
    /*00000a08*/ ADD         R1, R1, #200                @ 0xc8 
    /*00000a0c*/ ADD         R2, R2, R0 
    /*00000a10*/ BNE         VB_InquirePoolUserCnt_x9fc
    VB_InquirePoolUserCnt_xa14:
    /*00000a14*/ MOV         R0, #0 
    /*00000a18*/ STR         R2, [R5] 
    VB_InquirePoolUserCnt_xa1c:
    /*00000a1c*/ ADD         SP, SP, #20 
    /*00000a20*/ POP         {R4, R5, R6, R7, PC} 
    VB_InquirePoolUserCnt_xa24:
    /*00000a24*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*00000a28*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000a2c*/ ADDRL       R3, =_RODATA_STR1_4_029c    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c]
    /*00000a30*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000a34*/ MOVW        R2, #2133                   @ 0x855 
    /*00000a38*/ LDR         R1, WORD_0ae0               @ LDR         R1, [PC, #160]              @ 0x0000000000000ae0 
    /*00000a3c*/ BL          osal_panic
    /*00000a40*/ B           VB_InquirePoolUserCnt_x9b4
    VB_InquirePoolUserCnt_xa44:
    /*00000a44*/ STR         R3, [SP, #8] 
    /*00000a48*/ @ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00000a4c*/ MOVW        R3, #2139                   @ 0x85b 
    /*00000a50*/ STR         R6, [SP, #4] 
    /*00000a54*/ STR         R3, [SP] 
    /*00000a58*/ ADDRL       R2, =_RODATA_STR1_4_0138    @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138]
    /*00000a5c*/ LDR         R3, WORD_0ae0               @ LDR         R3, [PC, #124]              @ 0x0000000000000ae0 
    /*00000a60*/ MOV         R1, #1 
    /*00000a64*/ MOV         R0, #4 
    /*00000a68*/ BL          HI_LOG
    /*00000a6c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000a70*/ MOVT        R0, #40961                  @ 0xa001 
    /*00000a74*/ B           VB_InquirePoolUserCnt_xa1c
    VB_InquirePoolUserCnt_xa78:
    /*00000a78*/ @ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x184] 
    /*00000a7c*/ ADDRL       R2, =_RODATA_STR1_4_0184    @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x184]
    /*00000a80*/ MOVW        R3, #2146                   @ 0x862 
    /*00000a84*/ STR         R6, [SP, #4] 
    /*00000a88*/ STR         R3, [SP] 
    VB_InquirePoolUserCnt_xa8c:
    /*00000a8c*/ LDR         R3, WORD_0ae0               @ LDR         R3, [PC, #76]               @ 0x0000000000000ae0 
    /*00000a90*/ MOV         R1, #1 
    /*00000a94*/ MOV         R0, #4 
    /*00000a98*/ BL          HI_LOG
    /*00000a9c*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000aa0*/ MOVT        R0, #40961                  @ 0xa001 
    /*00000aa4*/ B           VB_InquirePoolUserCnt_xa1c
    VB_InquirePoolUserCnt_xaa8:
    /*00000aa8*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00000aac*/ MOV         R1, #1 
    /*00000ab0*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00000ab4*/ MOV         R0, #4 
    /*00000ab8*/ BL          HI_LOG
    /*00000abc*/ MOVW        R0, #32784                  @ 0x8010 
    /*00000ac0*/ MOVT        R0, #40961                  @ 0xa001 
    /*00000ac4*/ B           VB_InquirePoolUserCnt_xa1c
    VB_InquirePoolUserCnt_xac8:
    /*00000ac8*/ MOVW        R3, #2154                   @ 0x86a 
    /*00000acc*/ @ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2e8] 
    /*00000ad0*/ STR         R6, [SP, #4] 
    /*00000ad4*/ ADDRL       R2, =_RODATA_STR1_4_02e8    @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2e8]
    /*00000ad8*/ STR         R3, [SP] 
    /*00000adc*/ B           VB_InquirePoolUserCnt_xa8c
    /*00000ae0*/ WORD_0ae0: .word 0x000000a4
FUNC_END VB_InquirePoolUserCnt


FUNC_BEGIN VB_Handle2Supplement
    /*00000ae4*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000ae8*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000aec*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00000af0*/ LDR         R7, [R4, #8] 
    /*00000af4*/ CMP         R7, #0 
    /*00000af8*/ BEQ         VB_Handle2Supplement_xbac
    /*00000afc*/ LSR         R6, R0, #16 
    /*00000b00*/ UXTH        R5, R0 
    /*00000b04*/ LDR         R3, [R7, R6, LSL #2] 
    /*00000b08*/ CMP         R3, #0 
    /*00000b0c*/ BEQ         VB_Handle2Supplement_xb8c
    VB_Handle2Supplement_xb10:
    /*00000b10*/ LDR         R3, [R4, #536]              @ 0x218 
    /*00000b14*/ LDR         R3, [R3, R6, LSL #2] 
    /*00000b18*/ CMP         R3, #0 
    /*00000b1c*/ BEQ         VB_Handle2Supplement_xb6c
    VB_Handle2Supplement_xb20:
    /*00000b20*/ LDR         R3, [R4, #8] 
    /*00000b24*/ LDR         R3, [R3, R6, LSL #2] 
    /*00000b28*/ LDR         R3, [R3, #12] 
    /*00000b2c*/ CMP         R5, R3 
    /*00000b30*/ BCS         VB_Handle2Supplement_xb4c
    VB_Handle2Supplement_xb34:
    /*00000b34*/ LDR         R2, [R4, #536]              @ 0x218 
    /*00000b38*/ MOV         R3, #200                    @ 0xc8 
    /*00000b3c*/ LDR         R0, [R2, R6, LSL #2] 
    /*00000b40*/ MLA         R0, R3, R5, R0 
    /*00000b44*/ ADD         R0, R0, #136                @ 0x88 
    /*00000b48*/ POP         {R4, R5, R6, R7, R8, PC} 
    VB_Handle2Supplement_xb4c:
    /*00000b4c*/ @ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258] 
    /*00000b50*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000b54*/ ADDRL       R3, =_RODATA_STR1_4_0258    @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258]
    /*00000b58*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000b5c*/ MOVW        R2, #2899                   @ 0xb53 
    /*00000b60*/ LDR         R1, WORD_0bc8               @ LDR         R1, [PC, #96]               @ 0x0000000000000bc8 
    /*00000b64*/ BL          osal_panic
    /*00000b68*/ B           VB_Handle2Supplement_xb34
    VB_Handle2Supplement_xb6c:
    /*00000b6c*/ @ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22c] 
    /*00000b70*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000b74*/ ADDRL       R3, =_RODATA_STR1_4_022c    @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22c]
    /*00000b78*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000b7c*/ MOVW        R2, #2899                   @ 0xb53 
    /*00000b80*/ LDR         R1, WORD_0bc8               @ LDR         R1, [PC, #64]               @ 0x0000000000000bc8 
    /*00000b84*/ BL          osal_panic
    /*00000b88*/ B           VB_Handle2Supplement_xb20
    VB_Handle2Supplement_xb8c:
    /*00000b8c*/ @ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c0] 
    /*00000b90*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000b94*/ ADDRL       R3, =_RODATA_STR1_4_01c0    @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c0]
    /*00000b98*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000b9c*/ MOVW        R2, #2899                   @ 0xb53 
    /*00000ba0*/ LDR         R1, WORD_0bc8               @ LDR         R1, [PC, #32]               @ 0x0000000000000bc8 
    /*00000ba4*/ BL          osal_panic
    /*00000ba8*/ B           VB_Handle2Supplement_xb10
    VB_Handle2Supplement_xbac:
    /*00000bac*/ MOV         R1, #1 
    /*00000bb0*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00000bb4*/ MOV         R0, #4 
    /*00000bb8*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00000bbc*/ BL          HI_LOG
    /*00000bc0*/ MOV         R0, R7 
    /*00000bc4*/ POP         {R4, R5, R6, R7, R8, PC} 
    /*00000bc8*/ WORD_0bc8: .word 0x000000bc
FUNC_END VB_Handle2Supplement


FUNC_BEGIN VB_ModExit
    /*00000bcc*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000bd0*/ LDR         R3, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00000bd4*/ MOV         IP, #0 
    /*00000bd8*/ MOV         R2, #4 
    /*00000bdc*/ MOV         R0, R3 
    /*00000be0*/ MOV         R1, IP 
    /*00000be4*/ STR         IP, [R3, #4] 
    VB_ModExit_xbe8:
    /*00000be8*/ B           VB_ModExit_xbe8             @ osal_memset [R_ARM_JUMP24: +0x0] 
FUNC_END VB_ModExit


FUNC_BEGIN VbSearchCommPoolBySize
    /*00000bec*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000bf0*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000bf4*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00000bf8*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00000bfc*/ STRD        R2, [SP, #16] 
    /*00000c00*/ LDR         R3, [R4, #8] 
    /*00000c04*/ STR         R0, [SP, #32] 
    /*00000c08*/ CMP         R3, #0 
    /*00000c0c*/ LDR         R6, [SP, #84]               @ 0x54 
    /*00000c10*/ BEQ         VbSearchCommPoolBySize_xd64
    VbSearchCommPoolBySize_xc14:
    /*00000c14*/ LDR         R3, [SP, #80]               @ 0x50 
    /*00000c18*/ LDR         R7, WORD_0e00               @ LDR         R7, [PC, #480]              @ 0x0000000000000e00 
    /*00000c1c*/ CMP         R3, #0 
    /*00000c20*/ LDR         R4, [R4, #540]              @ 0x21c 
    /*00000c24*/ LDR         R5, WORD_0e04               @ LDR         R5, [PC, #472]              @ 0x0000000000000e04 
    /*00000c28*/ MOVEQ       R3, R7 
    /*00000c2c*/ STR         R3, [SP, #36]               @ 0x24 
    /*00000c30*/ CMP         R4, R5 
    /*00000c34*/ BEQ         VbSearchCommPoolBySize_xd84
    /*00000c38*/ MVN         R2, #0 
    /*00000c3c*/ MVN         R3, #0 
    /*00000c40*/ MOV         SL, #0 
    /*00000c44*/ @ MOVW        R9, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x394] 
    /*00000c48*/ @ MOVW        R8, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000c4c*/ STR         SL, [SP, #28] 
    /*00000c50*/ STR         SL, [SP, #24] 
    /*00000c54*/ ADD         R7, R7, #4 
    /*00000c58*/ ADDRL       R9, =_RODATA_STR1_4_0394    @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x394]
    /*00000c5c*/ ADDRL       R8, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000c60*/ MOV         SL, R2 
    /*00000c64*/ MOV         FP, R3 
    /*00000c68*/ B           VbSearchCommPoolBySize_xc78
    VbSearchCommPoolBySize_xc6c:
    /*00000c6c*/ LDR         R4, [R4] 
    /*00000c70*/ CMP         R4, R5 
    /*00000c74*/ BEQ         VbSearchCommPoolBySize_xd10
    VbSearchCommPoolBySize_xc78:
    /*00000c78*/ LDR         R3, [R4, #-52]              @ 0xffffffcc 
    /*00000c7c*/ CMP         R3, #1 
    /*00000c80*/ BEQ         VbSearchCommPoolBySize_xc98
    /*00000c84*/ MOV         R3, R9 
    /*00000c88*/ MOVW        R2, #954                    @ 0x3ba 
    /*00000c8c*/ MOV         R1, R7 
    /*00000c90*/ MOV         R0, R8 
    /*00000c94*/ BL          osal_panic
    VbSearchCommPoolBySize_xc98:
    /*00000c98*/ LDR         R3, [R4, #132]              @ 0x84 
    /*00000c9c*/ CMP         R3, R6 
    /*00000ca0*/ BNE         VbSearchCommPoolBySize_xc6c
    /*00000ca4*/ LDRD        R2, [SP, #16] 
    /*00000ca8*/ LDRD        R0, [R4, #-44]              @ 0xffffffd4 
    /*00000cac*/ CMP         R1, R3 
    /*00000cb0*/ CMPEQ       R0, R2 
    /*00000cb4*/ MOVCC       R3, #1 
    /*00000cb8*/ MOVCS       R3, #0 
    /*00000cbc*/ CMP         R1, FP 
    /*00000cc0*/ CMPEQ       R0, SL 
    /*00000cc4*/ ORRHI       R3, R3, #1 
    /*00000cc8*/ CMP         R3, #0 
    /*00000ccc*/ BNE         VbSearchCommPoolBySize_xc6c
    /*00000cd0*/ LDR         R3, [SP, #80]               @ 0x50 
    /*00000cd4*/ CMP         R3, #0 
    /*00000cd8*/ BEQ         VbSearchCommPoolBySize_xcf0
    /*00000cdc*/ LDR         R1, [SP, #36]               @ 0x24 
    /*00000ce0*/ ADD         R0, R4, #116                @ 0x74 
    /*00000ce4*/ BL          osal_strcmp
    /*00000ce8*/ CMP         R0, #0 
    /*00000cec*/ BNE         VbSearchCommPoolBySize_xc6c
    VbSearchCommPoolBySize_xcf0:
    /*00000cf0*/ LDR         R3, [R4, #8] 
    /*00000cf4*/ CMP         R3, #0 
    /*00000cf8*/ BNE         VbSearchCommPoolBySize_xd40
    VbSearchCommPoolBySize_xcfc:
    /*00000cfc*/ LDR         R4, [R4] 
    /*00000d00*/ MOV         R3, #1 
    /*00000d04*/ STR         R3, [SP, #24] 
    /*00000d08*/ CMP         R4, R5 
    /*00000d0c*/ BNE         VbSearchCommPoolBySize_xc78
    VbSearchCommPoolBySize_xd10:
    /*00000d10*/ LDR         SL, [SP, #24] 
    /*00000d14*/ LDR         FP, [SP, #28] 
    /*00000d18*/ CMP         SL, #0 
    /*00000d1c*/ BEQ         VbSearchCommPoolBySize_xd84
    VbSearchCommPoolBySize_xd20:
    /*00000d20*/ CMP         FP, #0 
    /*00000d24*/ BEQ         VbSearchCommPoolBySize_xdc8
    /*00000d28*/ LDR         R3, [FP] 
    /*00000d2c*/ MOV         R0, #0 
    /*00000d30*/ LDR         R2, [SP, #32] 
    /*00000d34*/ STR         R3, [R2] 
    VbSearchCommPoolBySize_xd38:
    /*00000d38*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000d3c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    VbSearchCommPoolBySize_xd40:
    /*00000d40*/ LDRD        SL, [R4, #-44]              @ 0xffffffd4 
    /*00000d44*/ SUB         R3, R4, #60                 @ 0x3c 
    /*00000d48*/ STR         R3, [SP, #28] 
    /*00000d4c*/ LDRD        R2, [SP, #16] 
    /*00000d50*/ CMP         R3, FP 
    /*00000d54*/ CMPEQ       R2, SL 
    /*00000d58*/ BNE         VbSearchCommPoolBySize_xcfc
    /*00000d5c*/ LDR         FP, [SP, #28] 
    /*00000d60*/ B           VbSearchCommPoolBySize_xd20
    VbSearchCommPoolBySize_xd64:
    /*00000d64*/ @ MOVW        R3, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b4] 
    /*00000d68*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00000d6c*/ ADDRL       R3, =_RODATA_STR1_4_02b4    @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b4]
    /*00000d70*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00000d74*/ MOV         R2, #940                    @ 0x3ac 
    /*00000d78*/ LDR         R1, WORD_0e08               @ LDR         R1, [PC, #136]              @ 0x0000000000000e08 
    /*00000d7c*/ BL          osal_panic
    /*00000d80*/ B           VbSearchCommPoolBySize_xc14
    VbSearchCommPoolBySize_xd84:
    /*00000d84*/ LDRD        R2, [SP, #16] 
    /*00000d88*/ MOV         R0, #3 
    /*00000d8c*/ LDR         R1, [SP, #32] 
    /*00000d90*/ STR         R6, [SP, #4] 
    /*00000d94*/ STRD        R2, [SP, #8] 
    /*00000d98*/ MVN         R2, #0 
    /*00000d9c*/ MOVW        R3, #985                    @ 0x3d9 
    /*00000da0*/ STR         R2, [R1] 
    /*00000da4*/ STR         R3, [SP] 
    /*00000da8*/ @ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x324] 
    /*00000dac*/ LDR         R3, WORD_0e08               @ LDR         R3, [PC, #84]               @ 0x0000000000000e08 
    /*00000db0*/ ADDRL       R2, =_RODATA_STR1_4_0324    @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x324]
    /*00000db4*/ MOV         R1, #1 
    /*00000db8*/ BL          HI_LOG
    /*00000dbc*/ MOVW        R0, #32787                  @ 0x8013 
    /*00000dc0*/ MOVT        R0, #40961                  @ 0xa001 
    /*00000dc4*/ B           VbSearchCommPoolBySize_xd38
    VbSearchCommPoolBySize_xdc8:
    /*00000dc8*/ MOV         R3, #996                    @ 0x3e4 
    /*00000dcc*/ @ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b4] 
    /*00000dd0*/ STR         R3, [SP] 
    /*00000dd4*/ ADDRL       R2, =_RODATA_STR1_4_03b4    @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b4]
    /*00000dd8*/ LDR         R3, WORD_0e08               @ LDR         R3, [PC, #40]               @ 0x0000000000000e08 
    /*00000ddc*/ MOV         R1, #1 
    /*00000de0*/ MOV         R0, #7 
    /*00000de4*/ BL          HI_LOG
    /*00000de8*/ LDR         R2, [SP, #32] 
    /*00000dec*/ MOVW        R0, #32781                  @ 0x800d 
    /*00000df0*/ MVN         R3, #0 
    /*00000df4*/ MOVT        R0, #40961                  @ 0xa001 
    /*00000df8*/ STR         R3, [R2] 
    /*00000dfc*/ B           VbSearchCommPoolBySize_xd38
    /*00000e00*/ WORD_0e00: .word 0x000000d4
    /*00000e04*/ WORD_0e04: .word 0x0000021c
    /*00000e08*/ WORD_0e08: .word 0x000000d8
FUNC_END VbSearchCommPoolBySize


FUNC_BEGIN VbCreatePool
    /*00000e0c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000e10*/ SUB         SP, SP, #124                @ 0x7c 
    /*00000e14*/ MOV         R6, R0 
    /*00000e18*/ MOV         R0, #0 
    /*00000e1c*/ LDR         R1, [SP, #160]              @ 0xa0 
    /*00000e20*/ STR         R0, [SP, #92]               @ 0x5c 
    /*00000e24*/ CMP         R1, #1 
    /*00000e28*/ STR         R0, [SP, #88]               @ 0x58 
    /*00000e2c*/ STR         R0, [SP, #96]               @ 0x60 
    /*00000e30*/ MOVWHI      R3, #317                    @ 0x13d 
    /*00000e34*/ @ MOVWHI      R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e4] 
    /*00000e38*/ STR         R0, [SP, #100]              @ 0x64 
    /*00000e3c*/ ADDRL       R2, =_RODATA_STR1_4_03e4    @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e4]
    /*00000e40*/ STR         R0, [SP, #104]              @ 0x68 
    /*00000e44*/ STR         R0, [SP, #108]              @ 0x6c 
    /*00000e48*/ STR         R0, [SP, #112]              @ 0x70 
    /*00000e4c*/ STR         R0, [SP, #116]              @ 0x74 
    /*00000e50*/ LDR         R9, [SP, #184]              @ 0xb8 
    /*00000e54*/ STRHI       R3, [SP] 
    /*00000e58*/ BHI         VbCreatePool_x1634
    /*00000e5c*/ MOV         FP, R3 
    /*00000e60*/ LDR         R3, [SP, #164]              @ 0xa4 
    /*00000e64*/ CMP         R3, #0 
    /*00000e68*/ BEQ         VbCreatePool_x1620
    /*00000e6c*/ CMP         R9, #2 
    /*00000e70*/ BHI         VbCreatePool_x1678
    /*00000e74*/ LDR         R3, [SP, #180]              @ 0xb4 
    /*00000e78*/ CMP         R3, #0 
    /*00000e7c*/ MOVWEQ      R3, #335                    @ 0x14f 
    /*00000e80*/ @ MOVWEQ      R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x484] 
    /*00000e84*/ STREQ       R3, [SP] 
    /*00000e88*/ ADDRL       R2, =_RODATA_STR1_4_0484    @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x484]
    /*00000e8c*/ BEQ         VbCreatePool_x165c
    /*00000e90*/ LDR         R3, [SP, #172]              @ 0xac 
    /*00000e94*/ MOV         SL, R2 
    /*00000e98*/ CMP         R3, #0 
    /*00000e9c*/ BEQ         VbCreatePool_xec4
    /*00000ea0*/ STR         R3, [SP, #4] 
    /*00000ea4*/ @ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4b0] 
    /*00000ea8*/ MOVW        R3, #341                    @ 0x155 
    /*00000eac*/ ADDRL       R2, =_RODATA_STR1_4_04b0    @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4b0]
    /*00000eb0*/ STR         R3, [SP] 
    /*00000eb4*/ MOV         R1, #1 
    /*00000eb8*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #2020]             @ 0x00000000000016a4 
    /*00000ebc*/ MOV         R0, #6 
    /*00000ec0*/ BL          HI_LOG
    VbCreatePool_xec4:
    /*00000ec4*/ CMP         R6, #10240                  @ 0x2800 
    /*00000ec8*/ BHI         VbCreatePool_x164c
    /*00000ecc*/ MOV         R1, #0 
    /*00000ed0*/ MOV         R0, #232                    @ 0xe8 
    /*00000ed4*/ BL          osal_kmalloc
    /*00000ed8*/ MOV         R4, #200                    @ 0xc8 
    /*00000edc*/ MUL         R4, R4, R6 
    /*00000ee0*/ MOV         R1, #0 
    /*00000ee4*/ MOV         R5, R0 
    /*00000ee8*/ MOV         R0, R4 
    /*00000eec*/ BL          osal_kmalloc
    /*00000ef0*/ CMP         R0, #0 
    /*00000ef4*/ CMPNE       R5, #0 
    /*00000ef8*/ MOV         R8, R0 
    /*00000efc*/ MOVEQ       R7, #1 
    /*00000f00*/ MOVNE       R7, #0 
    /*00000f04*/ BNE         VbCreatePool_xf64
    /*00000f08*/ MOVW        R3, #369                    @ 0x171 
    /*00000f0c*/ STR         R0, [SP, #8] 
    /*00000f10*/ STR         R3, [SP] 
    /*00000f14*/ @ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x528] 
    /*00000f18*/ STRD        SL, [SP, #16] 
    /*00000f1c*/ ADDRL       R2, =_RODATA_STR1_4_0528    @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x528]
    /*00000f20*/ STR         R6, [SP, #12] 
    /*00000f24*/ MOV         R1, #1 
    /*00000f28*/ STR         R5, [SP, #4] 
    /*00000f2c*/ MOV         R0, #0 
    /*00000f30*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #1900]             @ 0x00000000000016a4 
    /*00000f34*/ BL          HI_LOG
    /*00000f38*/ CMP         R5, #0 
    /*00000f3c*/ MOVWEQ      R4, #32780                  @ 0x800c 
    /*00000f40*/ MOVTEQ      R4, #40961                  @ 0xa001 
    /*00000f44*/ BNE         VbCreatePool_x1304
    VbCreatePool_xf48:
    /*00000f48*/ CMP         R8, #0 
    /*00000f4c*/ BEQ         VbCreatePool_xf58
    /*00000f50*/ MOV         R0, R8 
    /*00000f54*/ BL          osal_kfree
    VbCreatePool_xf58:
    /*00000f58*/ MOV         R0, R4 
    /*00000f5c*/ ADD         SP, SP, #124                @ 0x7c 
    /*00000f60*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    VbCreatePool_xf64:
    /*00000f64*/ MOV         R2, #232                    @ 0xe8 
    /*00000f68*/ MOV         R1, R7 
    /*00000f6c*/ MOV         R0, R5 
    /*00000f70*/ BL          osal_memset
    /*00000f74*/ MOV         R2, R4 
    /*00000f78*/ MOV         R1, R7 
    /*00000f7c*/ MOV         R0, R8 
    /*00000f80*/ BL          osal_memset
    /*00000f84*/ MOV         R2, #15 
    /*00000f88*/ LDR         R1, [SP, #180]              @ 0xb4 
    /*00000f8c*/ ADD         R0, SP, #88                 @ 0x58 
    /*00000f90*/ BL          osal_strncpy
    /*00000f94*/ LDR         R3, [SP, #172]              @ 0xac 
    /*00000f98*/ CMP         R3, #0 
    /*00000f9c*/ BEQ         VbCreatePool_x12c4
    /*00000fa0*/ MOV         R1, R3 
    /*00000fa4*/ LDR         R0, WORD_16a8               @ LDR         R0, [PC, #1788]             @ 0x00000000000016a8 
    /*00000fa8*/ BL          osal_strcmp
    /*00000fac*/ CMP         R0, #0 
    /*00000fb0*/ BEQ         VbCreatePool_x12c4
    VbCreatePool_xfb4:
    /*00000fb4*/ ADDS        SL, SL, #255                @ 0xff 
    /*00000fb8*/ MOV         R3, #0 
    /*00000fbc*/ ADC         FP, FP, #0 
    /*00000fc0*/ BIC         R0, SL, #255                @ 0xff 
    /*00000fc4*/ MVN         R2, #0 
    /*00000fc8*/ MOV         R1, FP 
    /*00000fcc*/ STRD        R0, [SP, #24] 
    /*00000fd0*/ UMULL       SL, FP, R6, R0 
    /*00000fd4*/ MLA         R1, R6, R1, FP 
    /*00000fd8*/ STRD        SL, [SP, #32] 
    /*00000fdc*/ STR         R1, [SP, #36]               @ 0x24 
    /*00000fe0*/ LDRD        R0, [SP, #32] 
    /*00000fe4*/ CMP         R1, R3 
    /*00000fe8*/ CMPEQ       R0, R2 
    /*00000fec*/ BHI         VbCreatePool_x12d0
    /*00000ff0*/ CMP         R9, #0 
    /*00000ff4*/ LDR         R3, [SP, #24] 
    /*00000ff8*/ BEQ         VbCreatePool_x1474
    /*00000ffc*/ MUL         IP, R3, R6 
    /*00001000*/ CMP         R9, #1 
    /*00001004*/ ADD         R3, R5, #40                 @ 0x28 
    /*00001008*/ ADD         R2, R5, #32 
    /*0000100c*/ ADD         R1, SP, #88                 @ 0x58 
    /*00001010*/ LDR         R0, [SP, #172]              @ 0xac 
    /*00001014*/ STR         IP, [SP] 
    /*00001018*/ BEQ         VbCreatePool_x1318
    /*0000101c*/ BL          CMPI_MmzMallocCached
    /*00001020*/ CMP         R0, #0 
    /*00001024*/ BNE         VbCreatePool_x1304
    VbCreatePool_x1028:
    /*00001028*/ LDR         R3, [SP, #168]              @ 0xa8 
    /*0000102c*/ LDR         R2, [SP, #176]              @ 0xb0 
    /*00001030*/ CMN         R2, #1 
    /*00001034*/ CMPNE       R3, #1 
    /*00001038*/ MOVEQ       R0, #1 
    /*0000103c*/ MOVNE       R0, #0 
    /*00001040*/ BEQ         VbCreatePool_x14d4
    /*00001044*/ STR         R0, [SP, #76]               @ 0x4c 
    /*00001048*/ STR         R0, [SP, #56]               @ 0x38 
    /*0000104c*/ STR         R0, [SP, #52]               @ 0x34 
    VbCreatePool_x1050:
    /*00001050*/ MOV         R3, #0 
    /*00001054*/ MOV         R0, #0 
    /*00001058*/ MOV         R1, #0 
    /*0000105c*/ STR         R3, [SP, #72]               @ 0x48 
    /*00001060*/ STR         R3, [R5, #228]              @ 0xe4 
    /*00001064*/ STRD        R0, [R5, #216]              @ 0xd8 
    VbCreatePool_x1068:
    /*00001068*/ LDR         R3, [SP, #52]               @ 0x34 
    /*0000106c*/ CMP         R3, #0 
    /*00001070*/ BNE         VbCreatePool_x15a4
    /*00001074*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00001078*/ MOV         R2, #0 
    /*0000107c*/ STR         R3, [R5, #224]              @ 0xe0 
    /*00001080*/ MOV         R3, #0 
    /*00001084*/ STRD        R2, [R5, #208]              @ 0xd0 
    VbCreatePool_x1088:
    /*00001088*/ LDRD        SL, [SP, #24] 
    /*0000108c*/ MOV         R2, #92                     @ 0x5c 
    /*00001090*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00001094*/ MOV         R1, #0 
    /*00001098*/ STR         R8, [R5, #4] 
    /*0000109c*/ ADD         R0, R5, #76                 @ 0x4c 
    /*000010a0*/ STRD        SL, [R5, #16] 
    /*000010a4*/ ADD         R4, R5, #44                 @ 0x2c 
    /*000010a8*/ STR         R3, [R5, #196]              @ 0xc4 
    /*000010ac*/ LDRD        SL, [SP, #32] 
    /*000010b0*/ LDR         R3, [SP, #72]               @ 0x48 
    /*000010b4*/ STR         R6, [R5, #12] 
    /*000010b8*/ STRD        SL, [R5, #24] 
    /*000010bc*/ STR         R3, [R5, #200]              @ 0xc8 
    /*000010c0*/ STR         R6, [R5, #68]               @ 0x44 
    /*000010c4*/ STR         R6, [R5, #72]               @ 0x48 
    /*000010c8*/ STR         R4, [SP, #84]               @ 0x54 
    /*000010cc*/ BL          osal_memset
    /*000010d0*/ LDR         R2, [SP, #160]              @ 0xa0 
    /*000010d4*/ CMP         R6, #0 
    /*000010d8*/ LDR         R3, [SP, #168]              @ 0xa8 
    /*000010dc*/ STR         R4, [R5, #44]               @ 0x2c 
    /*000010e0*/ STR         R2, [R5, #8] 
    /*000010e4*/ LDR         R2, [SP, #176]              @ 0xb0 
    /*000010e8*/ STR         R3, [R5, #172]              @ 0xac 
    /*000010ec*/ ADD         R3, R5, #52                 @ 0x34 
    /*000010f0*/ STR         R4, [R5, #48]               @ 0x30 
    /*000010f4*/ STR         R3, [R5, #52]               @ 0x34 
    /*000010f8*/ STR         R3, [R5, #56]               @ 0x38 
    /*000010fc*/ MOV         R3, R4 
    /*00001100*/ STR         R2, [R5, #192]              @ 0xc0 
    /*00001104*/ @ MOVWEQ      R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001108*/ LDR         R3, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*0000110c*/ STREQ       R3, [SP, #80]               @ 0x50 
    /*00001110*/ BEQ         VbCreatePool_x132c
    /*00001114*/ MOV         R2, #0 
    /*00001118*/ MOV         R3, #0 
    /*0000111c*/ MOV         SL, R2 
    /*00001120*/ STRD        R2, [SP, #40]               @ 0x28 
    /*00001124*/ STRD        R2, [SP, #32] 
    /*00001128*/ MOV         FP, R3 
    /*0000112c*/ LDR         R2, [SP, #56]               @ 0x38 
    /*00001130*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001134*/ MOV         R1, R3 
    /*00001138*/ MOV         R3, #0 
    /*0000113c*/ MOV         R7, #0 
    /*00001140*/ ADD         R4, R8, #136                @ 0x88 
    /*00001144*/ STRD        R2, [SP, #64]               @ 0x40 
    /*00001148*/ MOV         R3, R1 
    /*0000114c*/ LDR         R2, [SP, #76]               @ 0x4c 
    /*00001150*/ LDR         R3, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00001154*/ STR         R3, [SP, #80]               @ 0x50 
    /*00001158*/ MOV         R3, #0 
    /*0000115c*/ STR         R9, [SP, #184]              @ 0xb8 
    /*00001160*/ STRD        R2, [SP, #56]               @ 0x38 
    /*00001164*/ LDR         R2, [SP, #84]               @ 0x54 
    /*00001168*/ STR         R8, [SP, #84]               @ 0x54 
    /*0000116c*/ MOV         R3, R2 
    /*00001170*/ STR         R2, [SP, #76]               @ 0x4c 
    /*00001174*/ B           VbCreatePool_x11d0
    VbCreatePool_x1178:
    /*00001178*/ LDR         R3, [SP, #52]               @ 0x34 
    /*0000117c*/ CMP         R3, #0 
    /*00001180*/ BNE         VbCreatePool_x1290
    VbCreatePool_x1184:
    /*00001184*/ LDRD        R2, [SP, #24] 
    /*00001188*/ ADD         R4, R4, #200                @ 0xc8 
    /*0000118c*/ LDRD        R0, [SP, #32] 
    /*00001190*/ ADDS        R2, R2, SL 
    /*00001194*/ ADC         R3, R3, FP 
    /*00001198*/ MOV         SL, R2 
    /*0000119c*/ MOV         FP, R3 
    /*000011a0*/ LDRD        R2, [SP, #64]               @ 0x40 
    /*000011a4*/ ADDS        R2, R2, R0 
    /*000011a8*/ ADC         R3, R3, R1 
    /*000011ac*/ LDRD        R0, [SP, #40]               @ 0x28 
    /*000011b0*/ STRD        R2, [SP, #32] 
    /*000011b4*/ LDRD        R2, [SP, #56]               @ 0x38 
    /*000011b8*/ ADDS        R2, R2, R0 
    /*000011bc*/ ADC         R3, R3, R1 
    /*000011c0*/ CMP         R6, R7 
    /*000011c4*/ STRD        R2, [SP, #40]               @ 0x28 
    /*000011c8*/ BEQ         VbCreatePool_x1328
    /*000011cc*/ LDR         R3, [R5, #48]               @ 0x30 
    VbCreatePool_x11d0:
    /*000011d0*/ LDR         R1, [SP, #76]               @ 0x4c 
    /*000011d4*/ SUB         R2, R4, #136                @ 0x88 
    /*000011d8*/ STR         R2, [R5, #48]               @ 0x30 
    /*000011dc*/ SUB         R0, R4, #104                @ 0x68 
    /*000011e0*/ STR         R3, [R4, #-132]             @ 0xffffff7c 
    /*000011e4*/ STR         R1, [R4, #-136]             @ 0xffffff78 
    /*000011e8*/ STR         R2, [R3] 
    /*000011ec*/ STR         R7, [R4, #-128]             @ 0xffffff80 
    /*000011f0*/ ADD         R7, R7, #1 
    /*000011f4*/ LDRD        R2, [R5, #32] 
    /*000011f8*/ LDR         R1, [SP, #184]              @ 0xb8 
    /*000011fc*/ ADDS        R2, R2, SL 
    /*00001200*/ ADC         R3, R3, FP 
    /*00001204*/ CMP         R1, #0 
    /*00001208*/ STRD        R2, [R4, #-120]             @ 0xffffff88 
    /*0000120c*/ MOV         R2, #92                     @ 0x5c 
    /*00001210*/ LDRNE       R3, [R5, #40]               @ 0x28 
    /*00001214*/ MOVEQ       R3, R1 
    /*00001218*/ ADDNE       R3, R3, SL 
    /*0000121c*/ STR         R3, [R4, #-112]             @ 0xffffff90 
    /*00001220*/ MOV         R3, #0 
    /*00001224*/ MOV         R1, R3 
    /*00001228*/ STR         R3, [R4, #-108]             @ 0xffffff94 
    /*0000122c*/ BL          osal_memset
    /*00001230*/ LDRD        R2, [SP, #24] 
    /*00001234*/ MOV         R1, #0 
    /*00001238*/ MOV         R0, R4 
    /*0000123c*/ STRD        R2, [R4, #-8] 
    /*00001240*/ MOV         R2, #64                     @ 0x40 
    /*00001244*/ BL          osal_memset
    /*00001248*/ LDR         R3, [SP, #72]               @ 0x48 
    /*0000124c*/ CMP         R3, #0 
    /*00001250*/ BEQ         VbCreatePool_x1178
    /*00001254*/ LDRD        R0, [R5, #216]              @ 0xd8 
    /*00001258*/ MOV         R3, R4 
    /*0000125c*/ LDRD        R8, [SP, #40]               @ 0x28 
    /*00001260*/ LDR         R2, [R5, #228]              @ 0xe4 
    /*00001264*/ LDR         IP, [SP, #80]               @ 0x50 
    /*00001268*/ ADD         R2, R2, R8 
    /*0000126c*/ ADDS        R8, R8, R0 
    /*00001270*/ ADC         R9, R9, R1 
    /*00001274*/ STR         IP, [SP] 
    /*00001278*/ MOV         R0, R8 
    /*0000127c*/ MOV         R1, R9 
    /*00001280*/ BL          VB_ValueSupplementNoCacheAddr
    /*00001284*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00001288*/ CMP         R3, #0 
    /*0000128c*/ BEQ         VbCreatePool_x1184
    VbCreatePool_x1290:
    /*00001290*/ LDRD        R0, [R5, #208]              @ 0xd0 
    /*00001294*/ MOV         R3, R4 
    /*00001298*/ LDRD        R8, [SP, #32] 
    /*0000129c*/ LDR         R2, [R5, #224]              @ 0xe0 
    /*000012a0*/ LDR         IP, [SP, #80]               @ 0x50 
    /*000012a4*/ ADD         R2, R2, R8 
    /*000012a8*/ ADDS        R8, R8, R0 
    /*000012ac*/ ADC         R9, R9, R1 
    /*000012b0*/ STR         IP, [SP] 
    /*000012b4*/ MOV         R0, R8 
    /*000012b8*/ MOV         R1, R9 
    /*000012bc*/ BL          VB_ValueSupplementCachedAddr
    /*000012c0*/ B           VbCreatePool_x1184
    VbCreatePool_x12c4:
    /*000012c4*/ MOV         R3, #0 
    /*000012c8*/ STR         R3, [SP, #172]              @ 0xac 
    /*000012cc*/ B           VbCreatePool_xfb4
    VbCreatePool_x12d0:
    /*000012d0*/ LDRD        R0, [SP, #24] 
    /*000012d4*/ @ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x58c] 
    /*000012d8*/ LDR         R3, [SP, #172]              @ 0xac 
    /*000012dc*/ ADDRL       R2, =_RODATA_STR1_4_058c    @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x58c]
    /*000012e0*/ STR         R6, [SP, #16] 
    /*000012e4*/ STRD        R0, [SP, #8] 
    /*000012e8*/ MOV         R1, #1 
    /*000012ec*/ STR         R3, [SP, #20] 
    /*000012f0*/ MOV         R0, #0 
    /*000012f4*/ MOVW        R3, #402                    @ 0x192 
    /*000012f8*/ STR         R3, [SP] 
    /*000012fc*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #928]              @ 0x00000000000016a4 
    /*00001300*/ BL          HI_LOG
    VbCreatePool_x1304:
    /*00001304*/ MOVW        R4, #32780                  @ 0x800c 
    /*00001308*/ MOVT        R4, #40961                  @ 0xa001 
    VbCreatePool_x130c:
    /*0000130c*/ MOV         R0, R5 
    /*00001310*/ BL          osal_kfree
    /*00001314*/ B           VbCreatePool_xf48
    VbCreatePool_x1318:
    /*00001318*/ BL          CMPI_MmzMallocNocache
    /*0000131c*/ CMP         R0, #0 
    /*00001320*/ BNE         VbCreatePool_x1304
    /*00001324*/ B           VbCreatePool_x1028
    VbCreatePool_x1328:
    /*00001328*/ LDR         R8, [SP, #84]               @ 0x54 
    VbCreatePool_x132c:
    /*0000132c*/ LDR         R3, [SP, #172]              @ 0xac 
    /*00001330*/ CMP         R3, #0 
    /*00001334*/ BEQ         VbCreatePool_x1348
    /*00001338*/ LDR         R1, [SP, #172]              @ 0xac 
    /*0000133c*/ MOV         R2, #15 
    /*00001340*/ ADD         R0, R5, #176                @ 0xb0 
    /*00001344*/ BL          osal_strncpy
    VbCreatePool_x1348:
    /*00001348*/ LDR         R2, [SP, #80]               @ 0x50 
    /*0000134c*/ MOV         R3, #16 
    /*00001350*/ STR         R3, [R5, #168]              @ 0xa8 
    /*00001354*/ LDR         LR, [R2, #16] 
    /*00001358*/ CMP         LR, #0 
    /*0000135c*/ BEQ         VbCreatePool_x13a4
    /*00001360*/ LDR         R1, [R2, #8] 
    /*00001364*/ MOV         R3, R2 
    /*00001368*/ LDR         IP, [R1] 
    /*0000136c*/ CMP         IP, #0 
    /*00001370*/ ADDNE       R2, R1, #4 
    /*00001374*/ MOVNE       R3, #0 
    /*00001378*/ BNE         VbCreatePool_x138c
    /*0000137c*/ B           VbCreatePool_x13f4
    VbCreatePool_x1380:
    /*00001380*/ LDR         R0, [R1] 
    /*00001384*/ CMP         R0, #0 
    /*00001388*/ BEQ         VbCreatePool_x13f8
    VbCreatePool_x138c:
    /*0000138c*/ ADD         R3, R3, #1 
    /*00001390*/ MOV         R1, R2 
    /*00001394*/ CMP         R3, LR 
    /*00001398*/ ADD         R2, R2, #4 
    /*0000139c*/ LSL         IP, R3, #2 
    /*000013a0*/ BNE         VbCreatePool_x1380
    VbCreatePool_x13a4:
    /*000013a4*/ MOVW        R3, #573                    @ 0x23d 
    /*000013a8*/ MOV         R1, #1 
    /*000013ac*/ STR         R3, [SP] 
    /*000013b0*/ @ MOVW        R2, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6fc] 
    /*000013b4*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #744]              @ 0x00000000000016a4 
    /*000013b8*/ ADDRL       R2, =_RODATA_STR1_4_06fc    @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6fc]
    /*000013bc*/ MOV         R0, #3 
    /*000013c0*/ MOVW        R4, #32780                  @ 0x800c 
    /*000013c4*/ BL          HI_LOG
    /*000013c8*/ LDR         R2, [R5, #224]              @ 0xe0 
    /*000013cc*/ LDRD        R0, [R5, #208]              @ 0xd0 
    /*000013d0*/ MOVT        R4, #40961                  @ 0xa001 
    /*000013d4*/ BL          CMPI_MmzFree
    VbCreatePool_x13d8:
    /*000013d8*/ LDR         R2, [R5, #228]              @ 0xe4 
    /*000013dc*/ LDRD        R0, [R5, #216]              @ 0xd8 
    /*000013e0*/ BL          CMPI_MmzFree
    VbCreatePool_x13e4:
    /*000013e4*/ LDR         R2, [R5, #40]               @ 0x28 
    /*000013e8*/ LDRD        R0, [R5, #32] 
    /*000013ec*/ BL          CMPI_MmzFree
    /*000013f0*/ B           VbCreatePool_x130c
    VbCreatePool_x13f4:
    /*000013f4*/ MOV         R3, IP 
    VbCreatePool_x13f8:
    /*000013f8*/ LDR         R2, [SP, #160]              @ 0xa0 
    /*000013fc*/ CMP         R2, #1 
    /*00001400*/ BNE         VbCreatePool_x1424
    /*00001404*/ LDR         R4, [SP, #80]               @ 0x50 
    /*00001408*/ ADD         R2, R5, #60                 @ 0x3c 
    /*0000140c*/ LDR         LR, WORD_16ac               @ LDR         LR, [PC, #664]              @ 0x00000000000016ac 
    /*00001410*/ LDR         R0, [R4, #544]              @ 0x220 
    /*00001414*/ STR         LR, [R5, #60]               @ 0x3c 
    /*00001418*/ STR         R2, [R4, #544]              @ 0x220 
    /*0000141c*/ STR         R0, [R5, #64]               @ 0x40 
    /*00001420*/ STR         R2, [R0] 
    VbCreatePool_x1424:
    /*00001424*/ LDR         R2, [SP, #80]               @ 0x50 
    /*00001428*/ MOVW        LR, #593                    @ 0x251 
    /*0000142c*/ MOV         R4, #0 
    /*00001430*/ LDR         R0, [R2, #536]              @ 0x218 
    /*00001434*/ LDR         R2, [SP, #164]              @ 0xa4 
    /*00001438*/ STR         R3, [R2] 
    /*0000143c*/ @ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x728] 
    /*00001440*/ STR         R3, [R5] 
    /*00001444*/ ADDRL       R2, =_RODATA_STR1_4_0728    @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x728]
    /*00001448*/ STR         R5, [R1] 
    /*0000144c*/ MOV         R1, #1 
    /*00001450*/ STR         R8, [R0, IP] 
    /*00001454*/ MOV         R0, #7 
    /*00001458*/ STR         R3, [SP, #4] 
    /*0000145c*/ STR         LR, [SP] 
    /*00001460*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #572]              @ 0x00000000000016a4 
    /*00001464*/ BL          HI_LOG
    /*00001468*/ MOV         R0, R4 
    /*0000146c*/ ADD         SP, SP, #124                @ 0x7c 
    /*00001470*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    VbCreatePool_x1474:
    /*00001474*/ MUL         R2, R6, R3 
    /*00001478*/ ADD         R1, SP, #88                 @ 0x58 
    /*0000147c*/ LDR         R0, [SP, #172]              @ 0xac 
    /*00001480*/ BL          CMPI_MmzMalloc
    /*00001484*/ ORRS        R3, R0, R1 
    /*00001488*/ STRD        R0, [R5, #32] 
    /*0000148c*/ STRNE       R9, [R5, #40]               @ 0x28 
    /*00001490*/ BNE         VbCreatePool_x1028
    /*00001494*/ LDR         R3, [SP, #172]              @ 0xac 
    /*00001498*/ MOV         R0, R9 
    /*0000149c*/ STR         R6, [SP, #16] 
    /*000014a0*/ @ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5f0] 
    /*000014a4*/ LDRD        R6, [SP, #24] 
    /*000014a8*/ ADDRL       R2, =_RODATA_STR1_4_05f0    @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5f0]
    /*000014ac*/ STR         R3, [SP, #20] 
    /*000014b0*/ MOV         R1, #1 
    /*000014b4*/ MOVW        R3, #413                    @ 0x19d 
    /*000014b8*/ STR         R3, [SP] 
    /*000014bc*/ STRD        R6, [SP, #8] 
    /*000014c0*/ MOVW        R4, #32780                  @ 0x800c 
    /*000014c4*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #472]              @ 0x00000000000016a4 
    /*000014c8*/ MOVT        R4, #40961                  @ 0xa001 
    /*000014cc*/ BL          HI_LOG
    /*000014d0*/ B           VbCreatePool_x130c
    VbCreatePool_x14d4:
    /*000014d4*/ MOV         R1, #1 
    /*000014d8*/ @ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000014dc*/ LDR         R0, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000014e0*/ BL          VB_GetSupplementSize
    /*000014e4*/ MOV         R1, #0 
    /*000014e8*/ ADD         R3, R0, #255                @ 0xff 
    /*000014ec*/ @ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000014f0*/ BIC         R3, R3, #255                @ 0xff 
    /*000014f4*/ LDR         R0, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000014f8*/ STR         R3, [SP, #56]               @ 0x38 
    /*000014fc*/ MOV         R4, R3 
    /*00001500*/ BL          VB_GetSupplementSize
    /*00001504*/ MUL         R3, R4, R6 
    /*00001508*/ STR         R3, [SP, #52]               @ 0x34 
    /*0000150c*/ ADD         R0, R0, #255                @ 0xff 
    /*00001510*/ BIC         R3, R0, #255                @ 0xff 
    /*00001514*/ STR         R3, [SP, #76]               @ 0x4c 
    /*00001518*/ MUL         R3, R3, R6 
    /*0000151c*/ CMP         R3, #0 
    /*00001520*/ MOV         R4, R3 
    /*00001524*/ STR         R3, [SP, #72]               @ 0x48 
    /*00001528*/ BEQ         VbCreatePool_x1050
    /*0000152c*/ MOV         R1, #16 
    /*00001530*/ @ MOVW        R3, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x644] 
    /*00001534*/ @ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x63c] 
    /*00001538*/ ADDRL       R3, =_RODATA_STR1_4_0644    @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x644]
    /*0000153c*/ ADDRL       R2, =_RODATA_STR1_4_063c    @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x63c]
    /*00001540*/ STR         R3, [SP] 
    /*00001544*/ ADD         R0, SP, #104                @ 0x68 
    /*00001548*/ ADD         R3, SP, #88                 @ 0x58 
    /*0000154c*/ BL          osal_snprintf
    /*00001550*/ STR         R4, [SP] 
    /*00001554*/ ADD         R3, R5, #228                @ 0xe4 
    /*00001558*/ ADD         R2, R5, #216                @ 0xd8 
    /*0000155c*/ ADD         R1, SP, #104                @ 0x68 
    /*00001560*/ LDR         R0, [SP, #172]              @ 0xac 
    /*00001564*/ BL          CMPI_MmzMallocNocache
    /*00001568*/ SUBS        R4, R0, #0 
    /*0000156c*/ BEQ         VbCreatePool_x1068
    /*00001570*/ LDR         R3, [SP, #172]              @ 0xac 
    /*00001574*/ MOV         R1, #1 
    /*00001578*/ LDR         R2, [SP, #72]               @ 0x48 
    /*0000157c*/ MOV         R0, #0 
    /*00001580*/ STR         R3, [SP, #8] 
    /*00001584*/ MOVW        R3, #466                    @ 0x1d2 
    /*00001588*/ STR         R2, [SP, #4] 
    /*0000158c*/ @ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x64c] 
    /*00001590*/ STR         R3, [SP] 
    /*00001594*/ ADDRL       R2, =_RODATA_STR1_4_064c    @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x64c]
    /*00001598*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #260]              @ 0x00000000000016a4 
    /*0000159c*/ BL          HI_LOG
    /*000015a0*/ B           VbCreatePool_x13e4
    VbCreatePool_x15a4:
    /*000015a4*/ ADD         R3, SP, #88                 @ 0x58 
    /*000015a8*/ @ MOVW        R1, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6a0] 
    /*000015ac*/ @ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x63c] 
    /*000015b0*/ ADDRL       R1, =_RODATA_STR1_4_06a0    @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6a0]
    /*000015b4*/ ADDRL       R2, =_RODATA_STR1_4_063c    @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x63c]
    /*000015b8*/ STR         R1, [SP] 
    /*000015bc*/ ADD         R0, SP, #104                @ 0x68 
    /*000015c0*/ MOV         R1, #16 
    /*000015c4*/ BL          osal_snprintf
    /*000015c8*/ LDR         R3, [SP, #52]               @ 0x34 
    /*000015cc*/ ADD         R1, SP, #104                @ 0x68 
    /*000015d0*/ ADD         R2, R5, #208                @ 0xd0 
    /*000015d4*/ LDR         R0, [SP, #172]              @ 0xac 
    /*000015d8*/ STR         R3, [SP] 
    /*000015dc*/ ADD         R3, R5, #224                @ 0xe0 
    /*000015e0*/ BL          CMPI_MmzMallocCached
    /*000015e4*/ SUBS        R4, R0, #0 
    /*000015e8*/ BEQ         VbCreatePool_x1088
    /*000015ec*/ LDR         R3, [SP, #172]              @ 0xac 
    /*000015f0*/ MOV         R1, #1 
    /*000015f4*/ LDR         R2, [SP, #52]               @ 0x34 
    /*000015f8*/ MOV         R0, #0 
    /*000015fc*/ STR         R3, [SP, #8] 
    /*00001600*/ MOV         R3, #484                    @ 0x1e4 
    /*00001604*/ STR         R2, [SP, #4] 
    /*00001608*/ @ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6a8] 
    /*0000160c*/ STR         R3, [SP] 
    /*00001610*/ ADDRL       R2, =_RODATA_STR1_4_06a8    @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6a8]
    /*00001614*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #136]              @ 0x00000000000016a4 
    /*00001618*/ BL          HI_LOG
    /*0000161c*/ B           VbCreatePool_x13d8
    VbCreatePool_x1620:
    /*00001620*/ @ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x418] 
    /*00001624*/ ADDRL       R2, =_RODATA_STR1_4_0418    @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x418]
    /*00001628*/ MOV         R0, R3 
    /*0000162c*/ MOVW        R3, #323                    @ 0x143 
    /*00001630*/ STR         R3, [SP] 
    VbCreatePool_x1634:
    /*00001634*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #104]              @ 0x00000000000016a4 
    /*00001638*/ MOV         R1, #1 
    /*0000163c*/ MOVW        R4, #32771                  @ 0x8003 
    /*00001640*/ BL          HI_LOG
    /*00001644*/ MOVT        R4, #40961                  @ 0xa001 
    /*00001648*/ B           VbCreatePool_xf58
    VbCreatePool_x164c:
    /*0000164c*/ @ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4d8] 
    /*00001650*/ ADDRL       R2, =_RODATA_STR1_4_04d8    @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4d8]
    /*00001654*/ MOV         R3, #356                    @ 0x164 
    /*00001658*/ STR         R3, [SP] 
    VbCreatePool_x165c:
    /*0000165c*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #64]               @ 0x00000000000016a4 
    /*00001660*/ MOV         R1, #1 
    /*00001664*/ MOV         R0, #0 
    /*00001668*/ MOVW        R4, #32771                  @ 0x8003 
    /*0000166c*/ BL          HI_LOG
    /*00001670*/ MOVT        R4, #40961                  @ 0xa001 
    /*00001674*/ B           VbCreatePool_xf58
    VbCreatePool_x1678:
    /*00001678*/ MOVW        R3, #329                    @ 0x149 
    /*0000167c*/ STR         R9, [SP, #4] 
    /*00001680*/ STR         R3, [SP] 
    /*00001684*/ @ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x448] 
    /*00001688*/ LDR         R3, WORD_16a4               @ LDR         R3, [PC, #20]               @ 0x00000000000016a4 
    /*0000168c*/ ADDRL       R2, =_RODATA_STR1_4_0448    @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x448]
    /*00001690*/ MOV         R1, #1 
    /*00001694*/ MOVW        R4, #32771                  @ 0x8003 
    /*00001698*/ BL          HI_LOG
    /*0000169c*/ MOVT        R4, #40961                  @ 0xa001 
    /*000016a0*/ B           VbCreatePool_xf58
    /*000016a4*/ WORD_16a4: .word 0x000000f0
    /*000016a8*/ WORD_16a8: .word 0x000000d4
    /*000016ac*/ WORD_16ac: .word 0x0000021c
FUNC_END VbCreatePool


FUNC_BEGIN VB_GetBlkByPoolId
    /*000016b0*/ CMP         R1, #22 
    /*000016b4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000016b8*/ MOV         R7, R1 
    /*000016bc*/ SUB         SP, SP, #28 
    /*000016c0*/ MOV         R6, R0 
    /*000016c4*/ BHI         VB_GetBlkByPoolId_x1808
    VB_GetBlkByPoolId_x16c8:
    /*000016c8*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000016cc*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000016d0*/ LDR         R3, [R4, #8] 
    /*000016d4*/ CMP         R3, #0 
    /*000016d8*/ BEQ         VB_GetBlkByPoolId_x18ac
    /*000016dc*/ LDR         R3, [R4, #16] 
    /*000016e0*/ CMP         R3, R6 
    /*000016e4*/ BLS         VB_GetBlkByPoolId_x1828
    VB_GetBlkByPoolId_x16e8:
    /*000016e8*/ LDR         R0, WORD_191c               @ LDR         R0, [PC, #556]              @ 0x000000000000191c 
    /*000016ec*/ ADD         R1, SP, #20 
    /*000016f0*/ BL          osal_spin_lock_irqsave
    /*000016f4*/ LDR         R3, [R4, #8] 
    /*000016f8*/ LDR         R5, [R3, R6, LSL #2] 
    /*000016fc*/ CMP         R5, #0 
    /*00001700*/ BEQ         VB_GetBlkByPoolId_x18cc
    /*00001704*/ LDR         R3, [R5, #168]              @ 0xa8 
    /*00001708*/ CMP         R3, #18 
    /*0000170c*/ BEQ         VB_GetBlkByPoolId_x1870
    /*00001710*/ MOV         R3, R5 
    /*00001714*/ LDR         R4, [R3, #44]!              @ 0x2c 
    /*00001718*/ CMP         R4, R3 
    /*0000171c*/ BEQ         VB_GetBlkByPoolId_x18e4
    /*00001720*/ LDR         R3, [R5, #68]               @ 0x44 
    /*00001724*/ CMP         R3, #0 
    /*00001728*/ BEQ         VB_GetBlkByPoolId_x1848
    VB_GetBlkByPoolId_x172c:
    /*0000172c*/ LDR         R2, [R5, #72]               @ 0x48 
    /*00001730*/ SUB         R3, R3, #1 
    /*00001734*/ STR         R3, [R5, #68]               @ 0x44 
    /*00001738*/ CMP         R3, R2 
    /*0000173c*/ STRCC       R3, [R5, #72]               @ 0x48 
    /*00001740*/ LDR         R1, [R4, #4] 
    /*00001744*/ MOV         R2, #256                    @ 0x100 
    /*00001748*/ LDR         R0, [R4] 
    /*0000174c*/ MOVT        R2, #16 
    /*00001750*/ MOV         R3, #512                    @ 0x200 
    /*00001754*/ MOVT        R3, #32 
    /*00001758*/ STR         R1, [R0, #4] 
    /*0000175c*/ STR         R0, [R1] 
    /*00001760*/ ADD         R1, R5, #52                 @ 0x34 
    /*00001764*/ STM         R4, {R2, R3} 
    /*00001768*/ LDR         R3, [R5, #56]               @ 0x38 
    /*0000176c*/ STR         R4, [R5, #56]               @ 0x38 
    /*00001770*/ STM         R4, {R1, R3} 
    /*00001774*/ STR         R4, [R3] 
    /*00001778*/ LDR         R3, [R4, #184]              @ 0xb8 
    /*0000177c*/ CMP         R3, #0 
    /*00001780*/ BEQ         VB_GetBlkByPoolId_x17a4
    /*00001784*/ MOV         R2, #0 
    /*00001788*/ STRB        R2, [R3, #65]               @ 0x41 
    /*0000178c*/ STRB        R2, [R3] 
    /*00001790*/ STRB        R2, [R3, #1] 
    /*00001794*/ STRB        R2, [R3, #32] 
    /*00001798*/ STRB        R2, [R3, #33]               @ 0x21 
    /*0000179c*/ STRB        R2, [R3, #66]               @ 0x42 
    /*000017a0*/ STRB        R2, [R3, #64]               @ 0x40 
    VB_GetBlkByPoolId_x17a4:
    /*000017a4*/ ADD         R7, R4, R7, LSL #2 
    /*000017a8*/ MOV         R1, #1 
    /*000017ac*/ MOVW        LR, #905                    @ 0x389 
    /*000017b0*/ @ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x804] 
    /*000017b4*/ LDR         R3, [R7, #32] 
    /*000017b8*/ ADDRL       R2, =_RODATA_STR1_4_0804    @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x804]
    /*000017bc*/ MOV         R0, #7 
    /*000017c0*/ ADD         R3, R3, R1 
    /*000017c4*/ STR         R3, [R7, #32] 
    /*000017c8*/ LDR         R3, [R4, #28] 
    /*000017cc*/ ADD         R3, R3, R1 
    /*000017d0*/ STR         R3, [R4, #28] 
    /*000017d4*/ STR         R6, [SP, #8] 
    /*000017d8*/ LDR         IP, [R4, #8] 
    /*000017dc*/ LDR         R3, WORD_1920               @ LDR         R3, [PC, #316]              @ 0x0000000000001920 
    /*000017e0*/ STR         LR, [SP] 
    /*000017e4*/ STR         IP, [SP, #4] 
    /*000017e8*/ BL          HI_LOG
    /*000017ec*/ LDR         R0, WORD_191c               @ LDR         R0, [PC, #296]              @ 0x000000000000191c 
    /*000017f0*/ ADD         R1, SP, #20 
    /*000017f4*/ BL          osal_spin_unlock_irqrestore
    /*000017f8*/ LDRH        R0, [R4, #8] 
    /*000017fc*/ ORR         R0, R0, R6, LSL #16 
    VB_GetBlkByPoolId_x1800:
    /*00001800*/ ADD         SP, SP, #28 
    /*00001804*/ POP         {R4, R5, R6, R7, PC} 
    VB_GetBlkByPoolId_x1808:
    /*00001808*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*0000180c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00001810*/ ADDRL       R3, =_RODATA_STR1_4_029c    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c]
    /*00001814*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00001818*/ MOVW        R2, #849                    @ 0x351 
    /*0000181c*/ LDR         R1, WORD_1920               @ LDR         R1, [PC, #252]              @ 0x0000000000001920 
    /*00001820*/ BL          osal_panic
    /*00001824*/ B           VB_GetBlkByPoolId_x16c8
    VB_GetBlkByPoolId_x1828:
    /*00001828*/ @ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x758] 
    /*0000182c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00001830*/ ADDRL       R3, =_RODATA_STR1_4_0758    @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x758]
    /*00001834*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00001838*/ MOV         R2, #852                    @ 0x354 
    /*0000183c*/ LDR         R1, WORD_1920               @ LDR         R1, [PC, #220]              @ 0x0000000000001920 
    /*00001840*/ BL          osal_panic
    /*00001844*/ B           VB_GetBlkByPoolId_x16e8
    VB_GetBlkByPoolId_x1848:
    /*00001848*/ @ MOVW        R3, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7e8] 
    /*0000184c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00001850*/ ADDRL       R3, =_RODATA_STR1_4_07e8    @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7e8]
    /*00001854*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00001858*/ MOVW        R2, #885                    @ 0x375 
    /*0000185c*/ LDR         R1, WORD_1920               @ LDR         R1, [PC, #188]              @ 0x0000000000001920 
    /*00001860*/ BL          osal_panic
    /*00001864*/ LDR         R3, [R5, #68]               @ 0x44 
    /*00001868*/ LDR         R4, [R5, #44]               @ 0x2c 
    /*0000186c*/ B           VB_GetBlkByPoolId_x172c
    VB_GetBlkByPoolId_x1870:
    /*00001870*/ @ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x780] 
    /*00001874*/ ADDRL       R2, =_RODATA_STR1_4_0780    @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x780]
    /*00001878*/ MOVW        R3, #865                    @ 0x361 
    /*0000187c*/ STR         R6, [SP, #4] 
    /*00001880*/ STR         R3, [SP] 
    VB_GetBlkByPoolId_x1884:
    /*00001884*/ LDR         R3, WORD_1920               @ LDR         R3, [PC, #148]              @ 0x0000000000001920 
    /*00001888*/ MOV         R1, #1 
    /*0000188c*/ MOV         R0, #3 
    /*00001890*/ BL          HI_LOG
    /*00001894*/ ADD         R1, SP, #20 
    /*00001898*/ LDR         R0, WORD_191c               @ LDR         R0, [PC, #124]              @ 0x000000000000191c 
    /*0000189c*/ BL          osal_spin_unlock_irqrestore
    /*000018a0*/ MVN         R0, #0 
    /*000018a4*/ ADD         SP, SP, #28 
    /*000018a8*/ POP         {R4, R5, R6, R7, PC} 
    VB_GetBlkByPoolId_x18ac:
    /*000018ac*/ MOV         R1, #1 
    /*000018b0*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*000018b4*/ MOV         R0, #4 
    /*000018b8*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000018bc*/ BL          HI_LOG
    /*000018c0*/ MVN         R0, #0 
    /*000018c4*/ ADD         SP, SP, #28 
    /*000018c8*/ POP         {R4, R5, R6, R7, PC} 
    VB_GetBlkByPoolId_x18cc:
    /*000018cc*/ MOVW        R3, #859                    @ 0x35b 
    /*000018d0*/ @ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c] 
    /*000018d4*/ STR         R6, [SP, #4] 
    /*000018d8*/ ADDRL       R2, =_RODATA_STR1_4_007c    @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c]
    /*000018dc*/ STR         R3, [SP] 
    /*000018e0*/ B           VB_GetBlkByPoolId_x1884
    VB_GetBlkByPoolId_x18e4:
    /*000018e4*/ ADD         R1, SP, #20 
    /*000018e8*/ LDR         R0, WORD_191c               @ LDR         R0, [PC, #44]               @ 0x000000000000191c 
    /*000018ec*/ BL          osal_spin_unlock_irqrestore
    /*000018f0*/ MOVW        R3, #882                    @ 0x372 
    /*000018f4*/ STR         R6, [SP, #4] 
    /*000018f8*/ @ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b0] 
    /*000018fc*/ STR         R3, [SP] 
    /*00001900*/ ADDRL       R2, =_RODATA_STR1_4_07b0    @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b0]
    /*00001904*/ LDR         R3, WORD_1920               @ LDR         R3, [PC, #20]               @ 0x0000000000001920 
    /*00001908*/ MOV         R1, #1 
    /*0000190c*/ MOV         R0, #4 
    /*00001910*/ BL          HI_LOG
    /*00001914*/ MVN         R0, #0 
    /*00001918*/ B           VB_GetBlkByPoolId_x1800
    /*0000191c*/ WORD_191c: .word 0x00000224
    /*00001920*/ WORD_1920: .word 0x00000100
FUNC_END VB_GetBlkByPoolId


FUNC_BEGIN VB_GetBlkBySizeAndModule
    /*00001924*/ @ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001928*/ LDR         IP, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*0000192c*/ LDR         IP, [IP, #8] 
    /*00001930*/ CMP         IP, #0 
    /*00001934*/ BEQ         VB_GetBlkBySizeAndModule_x19c8
    /*00001938*/ CMP         R2, #22 
    /*0000193c*/ MOV         IP, R3 
    /*00001940*/ PUSH        {R4, R6, R7, LR} 
    /*00001944*/ MOV         R4, R2 
    /*00001948*/ SUB         SP, SP, #16 
    /*0000194c*/ MOV         R6, R0 
    /*00001950*/ MOV         R7, R1 
    /*00001954*/ BHI         VB_GetBlkBySizeAndModule_x19d0
    /*00001958*/ CMP         R2, #12 
    /*0000195c*/ BEQ         VB_GetBlkBySizeAndModule_x1990
    VB_GetBlkBySizeAndModule_x1960:
    /*00001960*/ MOV         R1, #0 
    /*00001964*/ STR         R4, [SP, #4] 
    /*00001968*/ MOV         R2, R6 
    /*0000196c*/ MOV         R3, R7 
    /*00001970*/ ADD         R0, SP, #12 
    /*00001974*/ STR         R1, [SP] 
    /*00001978*/ BL          VbSearchCommPoolBySize      @ BL          0x0000000000000BEC 
    /*0000197c*/ CMP         R0, #0 
    /*00001980*/ MVNNE       R0, #0 
    /*00001984*/ BEQ         VB_GetBlkBySizeAndModule_x19b4
    VB_GetBlkBySizeAndModule_x1988:
    /*00001988*/ ADD         SP, SP, #16 
    /*0000198c*/ POP         {R4, R6, R7, PC} 
    VB_GetBlkBySizeAndModule_x1990:
    /*00001990*/ STR         R2, [SP, #4] 
    /*00001994*/ MOV         R3, R1 
    /*00001998*/ MOV         R2, R0 
    /*0000199c*/ STR         IP, [SP] 
    /*000019a0*/ ADD         R0, SP, #12 
    /*000019a4*/ BL          VbSearchCommPoolBySize      @ BL          0x0000000000000BEC 
    /*000019a8*/ CMP         R0, #0 
    /*000019ac*/ MVNNE       R0, #0 
    /*000019b0*/ BNE         VB_GetBlkBySizeAndModule_x1988
    VB_GetBlkBySizeAndModule_x19b4:
    /*000019b4*/ MOV         R1, R4 
    /*000019b8*/ LDR         R0, [SP, #12] 
    /*000019bc*/ BL          VB_GetBlkByPoolId
    /*000019c0*/ ADD         SP, SP, #16 
    /*000019c4*/ POP         {R4, R6, R7, PC} 
    VB_GetBlkBySizeAndModule_x19c8:
    /*000019c8*/ MVN         R0, #0 
    /*000019cc*/ BX          LR 
    VB_GetBlkBySizeAndModule_x19d0:
    /*000019d0*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*000019d4*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000019d8*/ ADDRL       R3, =_RODATA_STR1_4_029c    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c]
    /*000019dc*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000019e0*/ MOVW        R2, #2364                   @ 0x93c 
    /*000019e4*/ LDR         R1, WORD_19f0               @ LDR         R1, [PC, #4]                @ 0x00000000000019f0 
    /*000019e8*/ BL          osal_panic
    /*000019ec*/ B           VB_GetBlkBySizeAndModule_x1960
    /*000019f0*/ WORD_19f0: .word 0x00000114
FUNC_END VB_GetBlkBySizeAndModule


FUNC_BEGIN VbDestroyPool
    /*000019f4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000019f8*/ SUBS        R4, R0, #0 
    /*000019fc*/ SUB         SP, SP, #20 
    /*00001a00*/ BEQ         VbDestroyPool_x1b78
    VbDestroyPool_x1a04:
    /*00001a04*/ @ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001a08*/ LDR         R5, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00001a0c*/ ADD         R0, R5, #548                @ 0x224 
    /*00001a10*/ ADD         R1, SP, #12 
    /*00001a14*/ LDR         R6, [R4] 
    /*00001a18*/ BL          osal_spin_lock_irqsave
    /*00001a1c*/ LDR         R2, [R4, #12] 
    /*00001a20*/ LDR         R3, [R4, #68]               @ 0x44 
    /*00001a24*/ CMP         R2, R3 
    /*00001a28*/ BEQ         VbDestroyPool_x1a5c
    /*00001a2c*/ MOVW        R3, #703                    @ 0x2bf 
    /*00001a30*/ STR         R6, [SP, #4] 
    /*00001a34*/ STR         R3, [SP] 
    /*00001a38*/ @ MOVW        R2, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x858] 
    /*00001a3c*/ LDR         R3, WORD_1bc4               @ LDR         R3, [PC, #384]              @ 0x0000000000001bc4 
    /*00001a40*/ ADDRL       R2, =_RODATA_STR1_4_0858    @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x858]
    /*00001a44*/ MOV         R1, #1 
    /*00001a48*/ MOV         R0, #3 
    /*00001a4c*/ BL          HI_LOG
    /*00001a50*/ LDR         R3, [R5, #552]              @ 0x228 
    /*00001a54*/ CMP         R3, #0 
    /*00001a58*/ BEQ         VbDestroyPool_x1b98
    VbDestroyPool_x1a5c:
    /*00001a5c*/ ADD         R1, SP, #12 
    /*00001a60*/ LDR         R0, WORD_1bc8               @ LDR         R0, [PC, #352]              @ 0x0000000000001bc8 
    /*00001a64*/ BL          osal_spin_unlock_irqrestore
    /*00001a68*/ ADD         R1, SP, #12 
    /*00001a6c*/ LDR         R0, WORD_1bc8               @ LDR         R0, [PC, #340]              @ 0x0000000000001bc8 
    /*00001a70*/ BL          osal_spin_lock_irqsave
    /*00001a74*/ LDR         R3, [R4, #168]              @ 0xa8 
    /*00001a78*/ CMP         R3, #16 
    /*00001a7c*/ MOVEQ       R3, #18 
    /*00001a80*/ STREQ       R3, [R4, #168]              @ 0xa8 
    /*00001a84*/ BEQ         VbDestroyPool_x1a90
    /*00001a88*/ CMP         R3, #18 
    /*00001a8c*/ BEQ         VbDestroyPool_x1bb0
    VbDestroyPool_x1a90:
    /*00001a90*/ ADD         R1, SP, #12 
    /*00001a94*/ LDR         R0, WORD_1bc8               @ LDR         R0, [PC, #300]              @ 0x0000000000001bc8 
    /*00001a98*/ BL          osal_spin_unlock_irqrestore
    /*00001a9c*/ ADD         R1, SP, #12 
    /*00001aa0*/ LDR         R0, WORD_1bc8               @ LDR         R0, [PC, #288]              @ 0x0000000000001bc8 
    /*00001aa4*/ LDR         R7, [R4, #4] 
    /*00001aa8*/ BL          osal_spin_lock_irqsave
    /*00001aac*/ LDR         IP, [R5, #536]              @ 0x218 
    /*00001ab0*/ MOV         R3, #0 
    /*00001ab4*/ LDR         R2, [R5, #8] 
    /*00001ab8*/ ADD         R1, SP, #12 
    /*00001abc*/ LDR         R0, WORD_1bc8               @ LDR         R0, [PC, #260]              @ 0x0000000000001bc8 
    /*00001ac0*/ STR         R3, [IP, R6, LSL #2] 
    /*00001ac4*/ STR         R3, [R2, R6, LSL #2] 
    /*00001ac8*/ BL          osal_spin_unlock_irqrestore
    /*00001acc*/ LDR         R3, [R4, #8] 
    /*00001ad0*/ CMP         R3, #0 
    /*00001ad4*/ BNE         VbDestroyPool_x1b4c
    VbDestroyPool_x1ad8:
    /*00001ad8*/ LDR         R2, [R4, #196]              @ 0xc4 
    /*00001adc*/ LDR         R1, [R4, #208]              @ 0xd0 
    /*00001ae0*/ LDR         R0, [R4, #224]              @ 0xe0 
    /*00001ae4*/ BL          osal_flush_dcache_area
    /*00001ae8*/ LDR         R2, [R4, #224]              @ 0xe0 
    /*00001aec*/ LDRD        R0, [R4, #208]              @ 0xd0 
    /*00001af0*/ BL          CMPI_MmzFree
    /*00001af4*/ LDR         R2, [R4, #228]              @ 0xe4 
    /*00001af8*/ LDRD        R0, [R4, #216]              @ 0xd8 
    /*00001afc*/ BL          CMPI_MmzFree
    /*00001b00*/ LDRD        R0, [R4, #32] 
    /*00001b04*/ LDR         R2, [R4, #40]               @ 0x28 
    /*00001b08*/ BL          CMPI_MmzFree
    /*00001b0c*/ MOV         R0, R7 
    /*00001b10*/ BL          osal_kfree
    /*00001b14*/ MOV         R0, R4 
    /*00001b18*/ BL          osal_kfree
    /*00001b1c*/ MOV         R3, #760                    @ 0x2f8 
    /*00001b20*/ STR         R6, [SP, #4] 
    /*00001b24*/ @ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8b4] 
    /*00001b28*/ STR         R3, [SP] 
    /*00001b2c*/ ADDRL       R2, =_RODATA_STR1_4_08b4    @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8b4]
    /*00001b30*/ LDR         R3, WORD_1bc4               @ LDR         R3, [PC, #140]              @ 0x0000000000001bc4 
    /*00001b34*/ MOV         R1, #1 
    /*00001b38*/ MOV         R0, #7 
    /*00001b3c*/ BL          HI_LOG
    /*00001b40*/ MOV         R0, #0 
    VbDestroyPool_x1b44:
    /*00001b44*/ ADD         SP, SP, #20 
    /*00001b48*/ POP         {R4, R5, R6, R7, PC} 
    VbDestroyPool_x1b4c:
    /*00001b4c*/ LDR         R1, [R4, #64]               @ 0x40 
    /*00001b50*/ MOV         R2, #256                    @ 0x100 
    /*00001b54*/ LDR         R0, [R4, #60]               @ 0x3c 
    /*00001b58*/ MOVT        R2, #16 
    /*00001b5c*/ MOV         R3, #512                    @ 0x200 
    /*00001b60*/ MOVT        R3, #32 
    /*00001b64*/ STR         R1, [R0, #4] 
    /*00001b68*/ STR         R0, [R1] 
    /*00001b6c*/ STR         R2, [R4, #60]               @ 0x3c 
    /*00001b70*/ STR         R3, [R4, #64]               @ 0x40 
    /*00001b74*/ B           VbDestroyPool_x1ad8
    VbDestroyPool_x1b78:
    /*00001b78*/ @ MOVW        R3, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x844] 
    /*00001b7c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00001b80*/ ADDRL       R3, =_RODATA_STR1_4_0844    @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x844]
    /*00001b84*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00001b88*/ MOVW        R2, #693                    @ 0x2b5 
    /*00001b8c*/ LDR         R1, WORD_1bc4               @ LDR         R1, [PC, #48]               @ 0x0000000000001bc4 
    /*00001b90*/ BL          osal_panic
    /*00001b94*/ B           VbDestroyPool_x1a04
    VbDestroyPool_x1b98:
    /*00001b98*/ ADD         R0, R5, #548                @ 0x224 
    /*00001b9c*/ ADD         R1, SP, #12 
    /*00001ba0*/ BL          osal_spin_unlock_irqrestore
    /*00001ba4*/ MOVW        R0, #32786                  @ 0x8012 
    /*00001ba8*/ MOVT        R0, #40961                  @ 0xa001 
    /*00001bac*/ B           VbDestroyPool_x1b44
    VbDestroyPool_x1bb0:
    /*00001bb0*/ LDR         R0, WORD_1bc8               @ LDR         R0, [PC, #16]               @ 0x0000000000001bc8 
    /*00001bb4*/ ADD         R1, SP, #12 
    /*00001bb8*/ BL          osal_spin_unlock_irqrestore
    /*00001bbc*/ MVN         R0, #0 
    /*00001bc0*/ B           VbDestroyPool_x1b44
    /*00001bc4*/ WORD_1bc4: .word 0x00000130
    /*00001bc8*/ WORD_1bc8: .word 0x00000224
FUNC_END VbDestroyPool


FUNC_BEGIN VbDoVbExit
    /*00001bcc*/ PUSH        {R4, R5, R6, LR} 
    /*00001bd0*/ @ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001bd4*/ LDR         R5, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00001bd8*/ SUB         SP, SP, #8 
    /*00001bdc*/ ADD         R0, R5, #556                @ 0x22c 
    /*00001be0*/ BL          osal_down
    /*00001be4*/ SUBS        R6, R0, #0 
    /*00001be8*/ BNE         VbDoVbExit_x1d78
    /*00001bec*/ LDR         R1, [R5, #8] 
    /*00001bf0*/ CMP         R1, #0 
    /*00001bf4*/ ADDNE       R3, R5, #556                @ 0x22c 
    /*00001bf8*/ MOVNE       R4, R6 
    /*00001bfc*/ BNE         VbDoVbExit_x1c10
    /*00001c00*/ B           VbDoVbExit_x1d4c
    VbDoVbExit_x1c04:
    /*00001c04*/ ADD         R4, R4, #1 
    /*00001c08*/ CMP         R4, #23 
    /*00001c0c*/ BEQ         VbDoVbExit_x1c5c
    VbDoVbExit_x1c10:
    /*00001c10*/ LDR         R2, [R3, #4]! 
    /*00001c14*/ CMP         R2, #0 
    /*00001c18*/ BEQ         VbDoVbExit_x1c04
    /*00001c1c*/ LDR         R0, WORD_1dd8               @ LDR         R0, [PC, #436]              @ 0x0000000000001dd8 
    /*00001c20*/ MOVW        R6, #32777                  @ 0x8009 
    /*00001c24*/ BL          osal_up
    /*00001c28*/ MOVW        R3, #3061                   @ 0xbf5 
    /*00001c2c*/ @ MOVW        R2, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x944] 
    /*00001c30*/ STR         R3, [SP] 
    /*00001c34*/ STR         R4, [SP, #4] 
    /*00001c38*/ ADDRL       R2, =_RODATA_STR1_4_0944    @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x944]
    /*00001c3c*/ LDR         R3, WORD_1ddc               @ LDR         R3, [PC, #408]              @ 0x0000000000001ddc 
    /*00001c40*/ MOV         R1, #1 
    /*00001c44*/ MOV         R0, #3 
    /*00001c48*/ MOVT        R6, #40961                  @ 0xa001 
    /*00001c4c*/ BL          HI_LOG
    VbDoVbExit_x1c50:
    /*00001c50*/ MOV         R0, R6 
    /*00001c54*/ ADD         SP, SP, #8 
    /*00001c58*/ POP         {R4, R5, R6, PC} 
    VbDoVbExit_x1c5c:
    /*00001c5c*/ LDR         R4, [R5, #4] 
    /*00001c60*/ CMP         R4, #0 
    /*00001c64*/ BNE         VbDoVbExit_x1da4
    /*00001c68*/ LDR         R3, [R5, #16] 
    /*00001c6c*/ CMP         R3, #0 
    /*00001c70*/ BNE         VbDoVbExit_x1c7c
    /*00001c74*/ B           VbDoVbExit_x1ca4
    VbDoVbExit_x1c78:
    /*00001c78*/ LDR         R1, [R5, #8] 
    VbDoVbExit_x1c7c:
    /*00001c7c*/ LDR         R0, [R1, R4, LSL #2] 
    /*00001c80*/ ADD         R4, R4, #1 
    /*00001c84*/ CMP         R0, #0 
    /*00001c88*/ BEQ         VbDoVbExit_x1c98
    /*00001c8c*/ BL          VbDestroyPool               @ BL          0x00000000000019F4 
    /*00001c90*/ SUBS        R3, R0, #0 
    /*00001c94*/ BNE         VbDoVbExit_x1d34
    VbDoVbExit_x1c98:
    /*00001c98*/ LDR         R3, [R5, #16] 
    /*00001c9c*/ CMP         R3, R4 
    /*00001ca0*/ BHI         VbDoVbExit_x1c78
    VbDoVbExit_x1ca4:
    /*00001ca4*/ LDR         R3, WORD_1de0               @ LDR         R3, [PC, #308]              @ 0x0000000000001de0 
    /*00001ca8*/ LDR         R2, [R5, #540]              @ 0x21c 
    /*00001cac*/ CMP         R2, R3 
    /*00001cb0*/ BEQ         VbDoVbExit_x1cd0
    /*00001cb4*/ @ MOVW        R3, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9b8] 
    /*00001cb8*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00001cbc*/ ADDRL       R3, =_RODATA_STR1_4_09b8    @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9b8]
    /*00001cc0*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00001cc4*/ MOV         R2, #3088                   @ 0xc10 
    /*00001cc8*/ LDR         R1, WORD_1ddc               @ LDR         R1, [PC, #268]              @ 0x0000000000001ddc 
    /*00001ccc*/ BL          osal_panic
    VbDoVbExit_x1cd0:
    /*00001cd0*/ LDR         R0, [R5, #8] 
    /*00001cd4*/ BL          osal_kfree
    /*00001cd8*/ LDR         R0, [R5, #536]              @ 0x218 
    /*00001cdc*/ BL          osal_kfree
    /*00001ce0*/ MOV         R3, #0 
    /*00001ce4*/ MOV         R1, R3 
    /*00001ce8*/ MOV         R2, #520                    @ 0x208 
    /*00001cec*/ LDR         R0, WORD_1de4               @ LDR         R0, [PC, #240]              @ 0x0000000000001de4 
    /*00001cf0*/ STR         R3, [R5, #536]              @ 0x218 
    /*00001cf4*/ STR         R3, [R5, #8] 
    /*00001cf8*/ STR         R3, [R5, #652]              @ 0x28c 
    /*00001cfc*/ BL          osal_memset
    /*00001d00*/ LDR         R0, WORD_1dd8               @ LDR         R0, [PC, #208]              @ 0x0000000000001dd8 
    /*00001d04*/ BL          osal_up
    /*00001d08*/ MOVW        R3, #3097                   @ 0xc19 
    /*00001d0c*/ MOV         R1, #1 
    /*00001d10*/ STR         R3, [SP] 
    /*00001d14*/ @ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9cc] 
    /*00001d18*/ LDR         R3, WORD_1ddc               @ LDR         R3, [PC, #188]              @ 0x0000000000001ddc 
    /*00001d1c*/ ADDRL       R2, =_RODATA_STR1_4_09cc    @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9cc]
    /*00001d20*/ MOV         R0, #7 
    /*00001d24*/ BL          HI_LOG
    /*00001d28*/ MOV         R0, R6 
    /*00001d2c*/ ADD         SP, SP, #8 
    /*00001d30*/ POP         {R4, R5, R6, PC} 
    VbDoVbExit_x1d34:
    /*00001d34*/ LDR         R0, WORD_1dd8               @ LDR         R0, [PC, #156]              @ 0x0000000000001dd8 
    /*00001d38*/ MOV         R6, R3 
    /*00001d3c*/ BL          osal_up
    /*00001d40*/ MOV         R0, R6 
    /*00001d44*/ ADD         SP, SP, #8 
    /*00001d48*/ POP         {R4, R5, R6, PC} 
    VbDoVbExit_x1d4c:
    /*00001d4c*/ ADD         R0, R5, #556                @ 0x22c 
    /*00001d50*/ BL          osal_up
    /*00001d54*/ MOVW        R3, #3051                   @ 0xbeb 
    /*00001d58*/ @ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x914] 
    /*00001d5c*/ STR         R3, [SP] 
    /*00001d60*/ ADDRL       R2, =_RODATA_STR1_4_0914    @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x914]
    /*00001d64*/ LDR         R3, WORD_1ddc               @ LDR         R3, [PC, #112]              @ 0x0000000000001ddc 
    /*00001d68*/ MOV         R1, #1 
    /*00001d6c*/ MOV         R0, #6 
    /*00001d70*/ BL          HI_LOG
    /*00001d74*/ B           VbDoVbExit_x1c50
    VbDoVbExit_x1d78:
    /*00001d78*/ MOVW        R3, #3044                   @ 0xbe4 
    /*00001d7c*/ @ MOVW        R2, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8e4] 
    /*00001d80*/ STR         R3, [SP] 
    /*00001d84*/ ADDRL       R2, =_RODATA_STR1_4_08e4    @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8e4]
    /*00001d88*/ LDR         R3, WORD_1ddc               @ LDR         R3, [PC, #76]               @ 0x0000000000001ddc 
    /*00001d8c*/ MOV         R1, #1 
    /*00001d90*/ MOV         R0, #3 
    /*00001d94*/ MOV         R6, #65024                  @ 0xfe00 
    /*00001d98*/ BL          HI_LOG
    /*00001d9c*/ MOVT        R6, #65535                  @ 0xffff 
    /*00001da0*/ B           VbDoVbExit_x1c50
    VbDoVbExit_x1da4:
    /*00001da4*/ LDR         R0, WORD_1dd8               @ LDR         R0, [PC, #44]               @ 0x0000000000001dd8 
    /*00001da8*/ MOVW        R6, #32777                  @ 0x8009 
    /*00001dac*/ BL          osal_up
    /*00001db0*/ MOVW        R3, #3069                   @ 0xbfd 
    /*00001db4*/ @ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x984] 
    /*00001db8*/ STR         R3, [SP] 
    /*00001dbc*/ ADDRL       R2, =_RODATA_STR1_4_0984    @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x984]
    /*00001dc0*/ LDR         R3, WORD_1ddc               @ LDR         R3, [PC, #20]               @ 0x0000000000001ddc 
    /*00001dc4*/ MOV         R1, #1 
    /*00001dc8*/ MOV         R0, #3 
    /*00001dcc*/ BL          HI_LOG
    /*00001dd0*/ MOVT        R6, #40961                  @ 0xa001 
    /*00001dd4*/ B           VbDoVbExit_x1c50
    /*00001dd8*/ WORD_1dd8: .word 0x0000022c
    /*00001ddc*/ WORD_1ddc: .word 0x00000140
    /*00001de0*/ WORD_1de0: .word 0x0000021c
    /*00001de4*/ WORD_1de4: .word 0x00000010
FUNC_END VbDoVbExit


FUNC_BEGIN VB_PutBlk
    /*00001de8*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00001dec*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001df0*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00001df4*/ MOV         R7, R3 
    /*00001df8*/ SUB         SP, SP, #16 
    /*00001dfc*/ LDR         R3, [R4, #8] 
    /*00001e00*/ CMP         R3, #0 
    /*00001e04*/ BEQ         VB_PutBlk_x1f88
    /*00001e08*/ LDR         R3, [R4, #16] 
    /*00001e0c*/ MOV         R5, R0 
    /*00001e10*/ MOV         R6, R2 
    /*00001e14*/ CMP         R3, R0 
    /*00001e18*/ BLS         VB_PutBlk_x1f04
    VB_PutBlk_x1e1c:
    /*00001e1c*/ LDR         R0, WORD_1fa4               @ LDR         R0, [PC, #384]              @ 0x0000000000001fa4 
    /*00001e20*/ ADD         R1, SP, #12 
    /*00001e24*/ BL          osal_spin_lock_irqsave
    /*00001e28*/ LDR         R3, [R4, #8] 
    /*00001e2c*/ LDR         R4, [R3, R5, LSL #2] 
    /*00001e30*/ CMP         R4, #0 
    /*00001e34*/ BEQ         VB_PutBlk_x1f74
    /*00001e38*/ LDR         R3, [R4, #168]              @ 0xa8 
    /*00001e3c*/ CMP         R3, #18 
    /*00001e40*/ BEQ         VB_PutBlk_x1f74
    /*00001e44*/ MOV         R2, R6 
    /*00001e48*/ MOV         R3, R7 
    /*00001e4c*/ MOV         R0, R4 
    /*00001e50*/ BL          VB_CheckPhyAddr             @ BL          0x0000000000000158 
    /*00001e54*/ SUBS        R8, R0, #0 
    /*00001e58*/ BNE         VB_PutBlk_x1f24
    /*00001e5c*/ LDRD        R0, [R4, #32] 
    /*00001e60*/ LDRD        R2, [R4, #16] 
    /*00001e64*/ SUBS        R0, R6, R0 
    /*00001e68*/ MOV         R6, #200                    @ 0xc8 
    /*00001e6c*/ SBC         R1, R7, R1 
    /*00001e70*/ LDR         R7, [R4, #4] 
    /*00001e74*/ BL          osal_div64_u64
    /*00001e78*/ MUL         R6, R6, R0 
    /*00001e7c*/ ADD         R5, R7, R6 
    /*00001e80*/ LDR         R3, [R5, #28] 
    /*00001e84*/ CMP         R3, #0 
    /*00001e88*/ BEQ         VB_PutBlk_x1f3c
    /*00001e8c*/ MOV         R2, #92                     @ 0x5c 
    /*00001e90*/ MOV         R1, R8 
    /*00001e94*/ ADD         R0, R5, #32 
    /*00001e98*/ BL          osal_memset
    /*00001e9c*/ STR         R8, [R5, #28] 
    /*00001ea0*/ LDR         R3, [R4, #68]               @ 0x44 
    /*00001ea4*/ MOV         R2, #256                    @ 0x100 
    /*00001ea8*/ MOVT        R2, #16 
    /*00001eac*/ ADD         LR, R4, #44                 @ 0x2c 
    /*00001eb0*/ ADD         R3, R3, #1 
    /*00001eb4*/ STR         R3, [R4, #68]               @ 0x44 
    /*00001eb8*/ LDR         R0, [R5, #4] 
    /*00001ebc*/ MOV         R3, #512                    @ 0x200 
    /*00001ec0*/ LDR         IP, [R7, R6] 
    /*00001ec4*/ MOVT        R3, #32 
    /*00001ec8*/ ADD         R1, SP, #12 
    /*00001ecc*/ STR         R0, [IP, #4] 
    /*00001ed0*/ STR         IP, [R0] 
    /*00001ed4*/ STR         R2, [R7, R6] 
    /*00001ed8*/ STR         R3, [R5, #4] 
    /*00001edc*/ LDR         R3, [R4, #48]               @ 0x30 
    /*00001ee0*/ LDR         R0, WORD_1fa4               @ LDR         R0, [PC, #188]              @ 0x0000000000001fa4 
    /*00001ee4*/ STR         R5, [R4, #48]               @ 0x30 
    /*00001ee8*/ STR         LR, [R7, R6] 
    /*00001eec*/ STR         R3, [R5, #4] 
    /*00001ef0*/ STR         R5, [R3] 
    /*00001ef4*/ BL          osal_spin_unlock_irqrestore
    VB_PutBlk_x1ef8:
    /*00001ef8*/ MOV         R0, R8 
    /*00001efc*/ ADD         SP, SP, #16 
    /*00001f00*/ POP         {R4, R5, R6, R7, R8, PC} 
    VB_PutBlk_x1f04:
    /*00001f04*/ @ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x758] 
    /*00001f08*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00001f0c*/ ADDRL       R3, =_RODATA_STR1_4_0758    @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x758]
    /*00001f10*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00001f14*/ MOVW        R2, #1175                   @ 0x497 
    /*00001f18*/ LDR         R1, WORD_1fa8               @ LDR         R1, [PC, #136]              @ 0x0000000000001fa8 
    /*00001f1c*/ BL          osal_panic
    /*00001f20*/ B           VB_PutBlk_x1e1c
    VB_PutBlk_x1f24:
    /*00001f24*/ ADD         R1, SP, #12 
    /*00001f28*/ LDR         R0, WORD_1fa4               @ LDR         R0, [PC, #116]              @ 0x0000000000001fa4 
    /*00001f2c*/ MOVW        R8, #32771                  @ 0x8003 
    /*00001f30*/ BL          osal_spin_unlock_irqrestore
    /*00001f34*/ MOVT        R8, #40961                  @ 0xa001 
    /*00001f38*/ B           VB_PutBlk_x1ef8
    VB_PutBlk_x1f3c:
    /*00001f3c*/ ADD         R1, SP, #12 
    /*00001f40*/ LDR         R0, WORD_1fa4               @ LDR         R0, [PC, #92]               @ 0x0000000000001fa4 
    /*00001f44*/ BL          osal_spin_unlock_irqrestore
    /*00001f48*/ MOVW        R3, #1252                   @ 0x4e4 
    /*00001f4c*/ @ MOVW        R2, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9f4] 
    /*00001f50*/ STR         R3, [SP] 
    /*00001f54*/ ADDRL       R2, =_RODATA_STR1_4_09f4    @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9f4]
    /*00001f58*/ LDR         R3, WORD_1fa8               @ LDR         R3, [PC, #72]               @ 0x0000000000001fa8 
    /*00001f5c*/ MOV         R1, #1 
    /*00001f60*/ MOV         R0, #4 
    /*00001f64*/ MOVW        R8, #32771                  @ 0x8003 
    /*00001f68*/ BL          HI_LOG
    /*00001f6c*/ MOVT        R8, #40961                  @ 0xa001 
    /*00001f70*/ B           VB_PutBlk_x1ef8
    VB_PutBlk_x1f74:
    /*00001f74*/ ADD         R1, SP, #12 
    /*00001f78*/ LDR         R0, WORD_1fa4               @ LDR         R0, [PC, #36]               @ 0x0000000000001fa4 
    /*00001f7c*/ BL          osal_spin_unlock_irqrestore
    /*00001f80*/ MVN         R8, #0 
    /*00001f84*/ B           VB_PutBlk_x1ef8
    VB_PutBlk_x1f88:
    /*00001f88*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00001f8c*/ MOV         R1, #1 
    /*00001f90*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00001f94*/ MOV         R0, #4 
    /*00001f98*/ BL          HI_LOG
    /*00001f9c*/ MVN         R8, #0 
    /*00001fa0*/ B           VB_PutBlk_x1ef8
    /*00001fa4*/ WORD_1fa4: .word 0x00000224
    /*00001fa8*/ WORD_1fa8: .word 0x0000014c
FUNC_END VB_PutBlk


FUNC_BEGIN VB_Phy2Handle
    /*00001fac*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00001fb0*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001fb4*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00001fb8*/ SUB         SP, SP, #12 
    /*00001fbc*/ LDR         R3, [R4, #8] 
    /*00001fc0*/ CMP         R3, #0 
    /*00001fc4*/ BEQ         VB_Phy2Handle_x204c
    /*00001fc8*/ MOV         R6, R0 
    /*00001fcc*/ MOV         R7, R1 
    /*00001fd0*/ ADD         R0, R4, #548                @ 0x224 
    /*00001fd4*/ ADD         R1, SP, #4 
    /*00001fd8*/ BL          osal_spin_lock_irqsave
    /*00001fdc*/ LDR         LR, [R4, #16] 
    /*00001fe0*/ CMP         LR, #0 
    /*00001fe4*/ BEQ         VB_Phy2Handle_x2034
    /*00001fe8*/ LDR         IP, [R4, #8] 
    /*00001fec*/ MOV         R5, #0 
    /*00001ff0*/ SUB         IP, IP, #4 
    VB_Phy2Handle_x1ff4:
    /*00001ff4*/ LDR         R4, [IP, #4]! 
    /*00001ff8*/ CMP         R4, #0 
    /*00001ffc*/ BEQ         VB_Phy2Handle_x2028
    /*00002000*/ LDRD        R0, [R4, #32] 
    /*00002004*/ CMP         R1, R7 
    /*00002008*/ CMPEQ       R0, R6 
    /*0000200c*/ BHI         VB_Phy2Handle_x2028
    /*00002010*/ LDRD        R2, [R4, #24] 
    /*00002014*/ ADDS        R2, R2, R0 
    /*00002018*/ ADC         R3, R3, R1 
    /*0000201c*/ CMP         R7, R3 
    /*00002020*/ CMPEQ       R6, R2 
    /*00002024*/ BCC         VB_Phy2Handle_x206c
    VB_Phy2Handle_x2028:
    /*00002028*/ ADD         R5, R5, #1 
    /*0000202c*/ CMP         R5, LR 
    /*00002030*/ BNE         VB_Phy2Handle_x1ff4
    VB_Phy2Handle_x2034:
    /*00002034*/ ADD         R1, SP, #4 
    /*00002038*/ LDR         R0, WORD_20c8               @ LDR         R0, [PC, #136]              @ 0x00000000000020c8 
    /*0000203c*/ BL          osal_spin_unlock_irqrestore
    /*00002040*/ MVN         R0, #0 
    /*00002044*/ ADD         SP, SP, #12 
    /*00002048*/ POP         {R4, R5, R6, R7, PC} 
    VB_Phy2Handle_x204c:
    /*0000204c*/ MOV         R1, #1 
    /*00002050*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00002054*/ MOV         R0, #4 
    /*00002058*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*0000205c*/ BL          HI_LOG
    /*00002060*/ MVN         R0, #0 
    /*00002064*/ ADD         SP, SP, #12 
    /*00002068*/ POP         {R4, R5, R6, R7, PC} 
    VB_Phy2Handle_x206c:
    /*0000206c*/ SUBS        R0, R6, R0 
    /*00002070*/ LDRD        R2, [R4, #16] 
    /*00002074*/ SBC         R1, R7, R1 
    /*00002078*/ BL          osal_div64_u64
    /*0000207c*/ LDR         R3, [R4, #12] 
    /*00002080*/ CMP         R0, R3 
    /*00002084*/ MOV         R6, R0 
    /*00002088*/ BCS         VB_Phy2Handle_x20a8
    VB_Phy2Handle_x208c:
    /*0000208c*/ ADD         R1, SP, #4 
    /*00002090*/ LDR         R0, WORD_20c8               @ LDR         R0, [PC, #48]               @ 0x00000000000020c8 
    /*00002094*/ BL          osal_spin_unlock_irqrestore
    /*00002098*/ UXTH        R0, R6 
    /*0000209c*/ ORR         R0, R0, R5, LSL #16 
    /*000020a0*/ ADD         SP, SP, #12 
    /*000020a4*/ POP         {R4, R5, R6, R7, PC} 
    VB_Phy2Handle_x20a8:
    /*000020a8*/ @ MOVW        R3, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa30] 
    /*000020ac*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000020b0*/ ADDRL       R3, =_RODATA_STR1_4_0a30    @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa30]
    /*000020b4*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000020b8*/ MOVW        R2, #1307                   @ 0x51b 
    /*000020bc*/ LDR         R1, WORD_20cc               @ LDR         R1, [PC, #8]                @ 0x00000000000020cc 
    /*000020c0*/ BL          osal_panic
    /*000020c4*/ B           VB_Phy2Handle_x208c
    /*000020c8*/ WORD_20c8: .word 0x00000224
    /*000020cc*/ WORD_20cc: .word 0x00000158
FUNC_END VB_Phy2Handle


FUNC_BEGIN VB_UserAdd
    /*000020d0*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000020d4*/ SUB         SP, SP, #16 
    /*000020d8*/ MOV         R8, R0 
    /*000020dc*/ MOV         R6, R2 
    /*000020e0*/ LDR         R5, [SP, #40]               @ 0x28 
    /*000020e4*/ MOV         R7, R3 
    /*000020e8*/ CMP         R5, #22 
    /*000020ec*/ BHI         VB_UserAdd_x21b4
    VB_UserAdd_x20f0:
    /*000020f0*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000020f4*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000020f8*/ LDR         R3, [R4, #8] 
    /*000020fc*/ CMP         R3, #0 
    /*00002100*/ BEQ         VB_UserAdd_x22a0
    /*00002104*/ LDR         R3, [R4, #16] 
    /*00002108*/ CMP         R3, R8 
    /*0000210c*/ BLS         VB_UserAdd_x21d4
    VB_UserAdd_x2110:
    /*00002110*/ LDR         R0, WORD_22fc               @ LDR         R0, [PC, #484]              @ 0x00000000000022fc 
    /*00002114*/ ADD         R1, SP, #12 
    /*00002118*/ BL          osal_spin_lock_irqsave
    /*0000211c*/ LDR         R3, [R4, #8] 
    /*00002120*/ LDR         R4, [R3, R8, LSL #2] 
    /*00002124*/ CMP         R4, #0 
    /*00002128*/ BEQ         VB_UserAdd_x22c0
    /*0000212c*/ LDR         R3, [R4, #168]              @ 0xa8 
    /*00002130*/ CMP         R3, #18 
    /*00002134*/ BEQ         VB_UserAdd_x2264
    /*00002138*/ CMP         R5, #22 
    /*0000213c*/ BHI         VB_UserAdd_x21f4
    VB_UserAdd_x2140:
    /*00002140*/ MOV         R2, R6 
    /*00002144*/ MOV         R3, R7 
    /*00002148*/ MOV         R0, R4 
    /*0000214c*/ BL          VB_CheckPhyAddr             @ BL          0x0000000000000158 
    /*00002150*/ SUBS        R8, R0, #0 
    /*00002154*/ BNE         VB_UserAdd_x2214
    /*00002158*/ LDRD        R0, [R4, #32] 
    /*0000215c*/ LDRD        R2, [R4, #16] 
    /*00002160*/ SUBS        R0, R6, R0 
    /*00002164*/ LDR         R4, [R4, #4] 
    /*00002168*/ SBC         R1, R7, R1 
    /*0000216c*/ BL          osal_div64_u64
    /*00002170*/ MOV         R3, #200                    @ 0xc8 
    /*00002174*/ MLA         R3, R3, R0, R4 
    /*00002178*/ LDR         R2, [R3, #28] 
    /*0000217c*/ CMP         R2, #0 
    /*00002180*/ BLE         VB_UserAdd_x222c
    /*00002184*/ ADD         R5, R3, R5, LSL #2 
    /*00002188*/ ADD         R2, R2, #1 
    /*0000218c*/ ADD         R1, SP, #12 
    /*00002190*/ LDR         R0, WORD_22fc               @ LDR         R0, [PC, #356]              @ 0x00000000000022fc 
    /*00002194*/ LDR         IP, [R5, #32] 
    /*00002198*/ ADD         IP, IP, #1 
    /*0000219c*/ STR         IP, [R5, #32] 
    /*000021a0*/ STR         R2, [R3, #28] 
    /*000021a4*/ BL          osal_spin_unlock_irqrestore
    VB_UserAdd_x21a8:
    /*000021a8*/ MOV         R0, R8 
    /*000021ac*/ ADD         SP, SP, #16 
    /*000021b0*/ POP         {R4, R5, R6, R7, R8, PC} 
    VB_UserAdd_x21b4:
    /*000021b4*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*000021b8*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000021bc*/ ADDRL       R3, =_RODATA_STR1_4_029c    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c]
    /*000021c0*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000021c4*/ MOVW        R2, #1459                   @ 0x5b3 
    /*000021c8*/ LDR         R1, WORD_2300               @ LDR         R1, [PC, #304]              @ 0x0000000000002300 
    /*000021cc*/ BL          osal_panic
    /*000021d0*/ B           VB_UserAdd_x20f0
    VB_UserAdd_x21d4:
    /*000021d4*/ @ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x758] 
    /*000021d8*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000021dc*/ ADDRL       R3, =_RODATA_STR1_4_0758    @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x758]
    /*000021e0*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000021e4*/ MOVW        R2, #1463                   @ 0x5b7 
    /*000021e8*/ LDR         R1, WORD_2300               @ LDR         R1, [PC, #272]              @ 0x0000000000002300 
    /*000021ec*/ BL          osal_panic
    /*000021f0*/ B           VB_UserAdd_x2110
    VB_UserAdd_x21f4:
    /*000021f4*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*000021f8*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000021fc*/ ADDRL       R3, =_RODATA_STR1_4_029c    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c]
    /*00002200*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00002204*/ MOVW        R2, #1480                   @ 0x5c8 
    /*00002208*/ LDR         R1, WORD_2300               @ LDR         R1, [PC, #240]              @ 0x0000000000002300 
    /*0000220c*/ BL          osal_panic
    /*00002210*/ B           VB_UserAdd_x2140
    VB_UserAdd_x2214:
    /*00002214*/ ADD         R1, SP, #12 
    /*00002218*/ LDR         R0, WORD_22fc               @ LDR         R0, [PC, #220]              @ 0x00000000000022fc 
    /*0000221c*/ MOVW        R8, #32771                  @ 0x8003 
    /*00002220*/ BL          osal_spin_unlock_irqrestore
    /*00002224*/ MOVT        R8, #40961                  @ 0xa001 
    /*00002228*/ B           VB_UserAdd_x21a8
    VB_UserAdd_x222c:
    /*0000222c*/ ADD         R1, SP, #12 
    /*00002230*/ LDR         R0, WORD_22fc               @ LDR         R0, [PC, #196]              @ 0x00000000000022fc 
    /*00002234*/ BL          osal_spin_unlock_irqrestore
    /*00002238*/ MOVW        R3, #1524                   @ 0x5f4 
    /*0000223c*/ @ MOVW        R2, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa50] 
    /*00002240*/ STR         R3, [SP] 
    /*00002244*/ ADDRL       R2, =_RODATA_STR1_4_0a50    @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa50]
    /*00002248*/ LDR         R3, WORD_2300               @ LDR         R3, [PC, #176]              @ 0x0000000000002300 
    /*0000224c*/ MOV         R1, #1 
    /*00002250*/ MOV         R0, #4 
    /*00002254*/ MOVW        R8, #32777                  @ 0x8009 
    /*00002258*/ BL          HI_LOG
    /*0000225c*/ MOVT        R8, #40961                  @ 0xa001 
    /*00002260*/ B           VB_UserAdd_x21a8
    VB_UserAdd_x2264:
    /*00002264*/ STR         R8, [SP, #4] 
    /*00002268*/ MOVW        R3, #1475                   @ 0x5c3 
    /*0000226c*/ MOV         R1, #1 
    /*00002270*/ STR         R3, [SP] 
    /*00002274*/ @ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x780] 
    /*00002278*/ LDR         R3, WORD_2300               @ LDR         R3, [PC, #128]              @ 0x0000000000002300 
    /*0000227c*/ ADDRL       R2, =_RODATA_STR1_4_0780    @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x780]
    /*00002280*/ MOV         R0, #4 
    /*00002284*/ BL          HI_LOG
    /*00002288*/ ADD         R1, SP, #12 
    /*0000228c*/ LDR         R0, WORD_22fc               @ LDR         R0, [PC, #104]              @ 0x00000000000022fc 
    /*00002290*/ MOVW        R8, #32773                  @ 0x8005 
    /*00002294*/ BL          osal_spin_unlock_irqrestore
    /*00002298*/ MOVT        R8, #40961                  @ 0xa001 
    /*0000229c*/ B           VB_UserAdd_x21a8
    VB_UserAdd_x22a0:
    /*000022a0*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*000022a4*/ MOV         R1, #1 
    /*000022a8*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000022ac*/ MOV         R0, #4 
    /*000022b0*/ MOVW        R8, #32774                  @ 0x8006 
    /*000022b4*/ BL          HI_LOG
    /*000022b8*/ MOVT        R8, #40961                  @ 0xa001 
    /*000022bc*/ B           VB_UserAdd_x21a8
    VB_UserAdd_x22c0:
    /*000022c0*/ STR         R8, [SP, #4] 
    /*000022c4*/ MOVW        R3, #1469                   @ 0x5bd 
    /*000022c8*/ MOV         R1, #1 
    /*000022cc*/ STR         R3, [SP] 
    /*000022d0*/ @ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c] 
    /*000022d4*/ LDR         R3, WORD_2300               @ LDR         R3, [PC, #36]               @ 0x0000000000002300 
    /*000022d8*/ ADDRL       R2, =_RODATA_STR1_4_007c    @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c]
    /*000022dc*/ MOV         R0, #4 
    /*000022e0*/ BL          HI_LOG
    /*000022e4*/ ADD         R1, SP, #12 
    /*000022e8*/ LDR         R0, WORD_22fc               @ LDR         R0, [PC, #12]               @ 0x00000000000022fc 
    /*000022ec*/ MOVW        R8, #32774                  @ 0x8006 
    /*000022f0*/ BL          osal_spin_unlock_irqrestore
    /*000022f4*/ MOVT        R8, #40961                  @ 0xa001 
    /*000022f8*/ B           VB_UserAdd_x21a8
    /*000022fc*/ WORD_22fc: .word 0x00000224
    /*00002300*/ WORD_2300: .word 0x00000168
FUNC_END VB_UserAdd


FUNC_BEGIN VB_UserSub
    /*00002304*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00002308*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000230c*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00002310*/ MOV         R9, R3 
    /*00002314*/ SUB         SP, SP, #24 
    /*00002318*/ LDR         R3, [R4, #8] 
    /*0000231c*/ LDR         R6, [SP, #56]               @ 0x38 
    /*00002320*/ CMP         R3, #0 
    /*00002324*/ BEQ         VB_UserSub_x255c
    /*00002328*/ LDR         R3, [R4, #16] 
    /*0000232c*/ MOV         R7, R0 
    /*00002330*/ MOV         R8, R2 
    /*00002334*/ CMP         R3, R0 
    /*00002338*/ BLS         VB_UserSub_x2490
    VB_UserSub_x233c:
    /*0000233c*/ LDR         R0, WORD_25b8               @ LDR         R0, [PC, #628]              @ 0x00000000000025b8 
    /*00002340*/ ADD         R1, SP, #20 
    /*00002344*/ BL          osal_spin_lock_irqsave
    /*00002348*/ LDR         R3, [R4, #8] 
    /*0000234c*/ LDR         R4, [R3, R7, LSL #2] 
    /*00002350*/ CMP         R4, #0 
    /*00002354*/ BEQ         VB_UserSub_x257c
    /*00002358*/ LDR         R3, [R4, #168]              @ 0xa8 
    /*0000235c*/ CMP         R3, #18 
    /*00002360*/ BEQ         VB_UserSub_x2520
    /*00002364*/ CMP         R6, #22 
    /*00002368*/ BHI         VB_UserSub_x24b0
    VB_UserSub_x236c:
    /*0000236c*/ MOV         R2, R8 
    /*00002370*/ MOV         R3, R9 
    /*00002374*/ MOV         R0, R4 
    /*00002378*/ BL          VB_CheckPhyAddr             @ BL          0x0000000000000158 
    /*0000237c*/ SUBS        SL, R0, #0 
    /*00002380*/ BNE         VB_UserSub_x2508
    /*00002384*/ LDRD        R0, [R4, #32] 
    /*00002388*/ LDRD        R2, [R4, #16] 
    /*0000238c*/ SUBS        R0, R8, R0 
    /*00002390*/ MOV         R8, #200                    @ 0xc8 
    /*00002394*/ SBC         R1, R9, R1 
    /*00002398*/ LDR         R9, [R4, #4] 
    /*0000239c*/ BL          osal_div64_u64
    /*000023a0*/ MUL         R8, R8, R0 
    /*000023a4*/ ADD         R5, R9, R8 
    /*000023a8*/ LDR         R3, [R5, #28] 
    /*000023ac*/ CMP         R3, #0 
    /*000023b0*/ BLE         VB_UserSub_x24d0
    /*000023b4*/ ADD         R6, R5, R6, LSL #2 
    /*000023b8*/ LDR         R3, [R6, #32] 
    /*000023bc*/ CMP         R3, #0 
    /*000023c0*/ BEQ         VB_UserSub_x24d0
    /*000023c4*/ STR         R7, [SP, #8] 
    /*000023c8*/ MOVW        R3, #1659                   @ 0x67b 
    /*000023cc*/ LDR         R0, [R5, #8] 
    /*000023d0*/ @ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xad8] 
    /*000023d4*/ STR         R3, [SP] 
    /*000023d8*/ ADDRL       R2, =_RODATA_STR1_4_0ad8    @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0xad8]
    /*000023dc*/ LDR         R3, WORD_25bc               @ LDR         R3, [PC, #472]              @ 0x00000000000025bc 
    /*000023e0*/ MOV         R1, #1 
    /*000023e4*/ STR         R0, [SP, #4] 
    /*000023e8*/ MOV         R0, #7 
    /*000023ec*/ BL          HI_LOG
    /*000023f0*/ LDR         R3, [R6, #32] 
    /*000023f4*/ SUB         R3, R3, #1 
    /*000023f8*/ STR         R3, [R6, #32] 
    /*000023fc*/ LDR         R6, [R5, #28] 
    /*00002400*/ SUB         R6, R6, #1 
    /*00002404*/ STR         R6, [R5, #28] 
    /*00002408*/ CMP         R6, #0 
    /*0000240c*/ BNE         VB_UserSub_x2474
    /*00002410*/ LDR         R0, [R5, #4] 
    /*00002414*/ MOV         R2, #256                    @ 0x100 
    /*00002418*/ LDR         IP, [R9, R8] 
    /*0000241c*/ MOVT        R2, #16 
    /*00002420*/ MOV         R3, #512                    @ 0x200 
    /*00002424*/ MOVT        R3, #32 
    /*00002428*/ ADD         R1, SP, #20 
    /*0000242c*/ STR         R0, [IP, #4] 
    /*00002430*/ STR         IP, [R0] 
    /*00002434*/ ADD         R0, R4, #44                 @ 0x2c 
    /*00002438*/ STR         R2, [R9, R8] 
    /*0000243c*/ STR         R3, [R5, #4] 
    /*00002440*/ LDR         R3, [R4, #48]               @ 0x30 
    /*00002444*/ STR         R5, [R4, #48]               @ 0x30 
    /*00002448*/ STR         R0, [R9, R8] 
    /*0000244c*/ STR         R3, [R5, #4] 
    /*00002450*/ STR         R5, [R3] 
    /*00002454*/ LDR         R3, [R4, #68]               @ 0x44 
    /*00002458*/ LDR         R0, WORD_25b8               @ LDR         R0, [PC, #344]              @ 0x00000000000025b8 
    /*0000245c*/ ADD         R3, R3, #1 
    /*00002460*/ STR         R3, [R4, #68]               @ 0x44 
    /*00002464*/ BL          osal_spin_unlock_irqrestore
    VB_UserSub_x2468:
    /*00002468*/ MOV         R0, R6 
    /*0000246c*/ ADD         SP, SP, #24 
    /*00002470*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    VB_UserSub_x2474:
    /*00002474*/ ADD         R1, SP, #20 
    /*00002478*/ LDR         R0, WORD_25b8               @ LDR         R0, [PC, #312]              @ 0x00000000000025b8 
    /*0000247c*/ BL          osal_spin_unlock_irqrestore
    /*00002480*/ MOV         R6, SL 
    /*00002484*/ MOV         R0, R6 
    /*00002488*/ ADD         SP, SP, #24 
    /*0000248c*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    VB_UserSub_x2490:
    /*00002490*/ @ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x758] 
    /*00002494*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00002498*/ ADDRL       R3, =_RODATA_STR1_4_0758    @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x758]
    /*0000249c*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000024a0*/ MOVW        R2, #1559                   @ 0x617 
    /*000024a4*/ LDR         R1, WORD_25bc               @ LDR         R1, [PC, #272]              @ 0x00000000000025bc 
    /*000024a8*/ BL          osal_panic
    /*000024ac*/ B           VB_UserSub_x233c
    VB_UserSub_x24b0:
    /*000024b0*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*000024b4*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000024b8*/ ADDRL       R3, =_RODATA_STR1_4_029c    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c]
    /*000024bc*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000024c0*/ MOVW        R2, #1576                   @ 0x628 
    /*000024c4*/ LDR         R1, WORD_25bc               @ LDR         R1, [PC, #240]              @ 0x00000000000025bc 
    /*000024c8*/ BL          osal_panic
    /*000024cc*/ B           VB_UserSub_x236c
    VB_UserSub_x24d0:
    /*000024d0*/ ADD         R1, SP, #20 
    /*000024d4*/ LDR         R0, WORD_25b8               @ LDR         R0, [PC, #220]              @ 0x00000000000025b8 
    /*000024d8*/ BL          osal_spin_unlock_irqrestore
    /*000024dc*/ MOVW        R3, #1654                   @ 0x676 
    /*000024e0*/ @ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa98] 
    /*000024e4*/ STR         R3, [SP] 
    /*000024e8*/ ADDRL       R2, =_RODATA_STR1_4_0a98    @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa98]
    /*000024ec*/ LDR         R3, WORD_25bc               @ LDR         R3, [PC, #200]              @ 0x00000000000025bc 
    /*000024f0*/ MOV         R1, #1 
    /*000024f4*/ MOV         R0, #4 
    /*000024f8*/ MOVW        R6, #32771                  @ 0x8003 
    /*000024fc*/ BL          HI_LOG
    /*00002500*/ MOVT        R6, #40961                  @ 0xa001 
    /*00002504*/ B           VB_UserSub_x2468
    VB_UserSub_x2508:
    /*00002508*/ ADD         R1, SP, #20 
    /*0000250c*/ LDR         R0, WORD_25b8               @ LDR         R0, [PC, #164]              @ 0x00000000000025b8 
    /*00002510*/ MOVW        R6, #32771                  @ 0x8003 
    /*00002514*/ BL          osal_spin_unlock_irqrestore
    /*00002518*/ MOVT        R6, #40961                  @ 0xa001 
    /*0000251c*/ B           VB_UserSub_x2468
    VB_UserSub_x2520:
    /*00002520*/ MOVW        R3, #1571                   @ 0x623 
    /*00002524*/ MOV         R1, #1 
    /*00002528*/ STR         R3, [SP] 
    /*0000252c*/ @ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x780] 
    /*00002530*/ LDR         R3, WORD_25bc               @ LDR         R3, [PC, #132]              @ 0x00000000000025bc 
    /*00002534*/ ADDRL       R2, =_RODATA_STR1_4_0780    @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x780]
    /*00002538*/ STR         R7, [SP, #4] 
    /*0000253c*/ MOV         R0, #4 
    /*00002540*/ BL          HI_LOG
    /*00002544*/ ADD         R1, SP, #20 
    /*00002548*/ LDR         R0, WORD_25b8               @ LDR         R0, [PC, #104]              @ 0x00000000000025b8 
    /*0000254c*/ MOVW        R6, #32773                  @ 0x8005 
    /*00002550*/ BL          osal_spin_unlock_irqrestore
    /*00002554*/ MOVT        R6, #40961                  @ 0xa001 
    /*00002558*/ B           VB_UserSub_x2468
    VB_UserSub_x255c:
    /*0000255c*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00002560*/ MOV         R1, #1 
    /*00002564*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00002568*/ MOV         R0, #4 
    /*0000256c*/ MOVW        R6, #32774                  @ 0x8006 
    /*00002570*/ BL          HI_LOG
    /*00002574*/ MOVT        R6, #40961                  @ 0xa001 
    /*00002578*/ B           VB_UserSub_x2468
    VB_UserSub_x257c:
    /*0000257c*/ MOVW        R3, #1565                   @ 0x61d 
    /*00002580*/ MOV         R1, #1 
    /*00002584*/ STR         R3, [SP] 
    /*00002588*/ @ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c] 
    /*0000258c*/ LDR         R3, WORD_25bc               @ LDR         R3, [PC, #40]               @ 0x00000000000025bc 
    /*00002590*/ ADDRL       R2, =_RODATA_STR1_4_007c    @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c]
    /*00002594*/ STR         R7, [SP, #4] 
    /*00002598*/ MOV         R0, #4 
    /*0000259c*/ BL          HI_LOG
    /*000025a0*/ ADD         R1, SP, #20 
    /*000025a4*/ LDR         R0, WORD_25b8               @ LDR         R0, [PC, #12]               @ 0x00000000000025b8 
    /*000025a8*/ MOVW        R6, #32774                  @ 0x8006 
    /*000025ac*/ BL          osal_spin_unlock_irqrestore
    /*000025b0*/ MOVT        R6, #40961                  @ 0xa001 
    /*000025b4*/ B           VB_UserSub_x2468
    /*000025b8*/ WORD_25b8: .word 0x00000224
    /*000025bc*/ WORD_25bc: .word 0x00000174
FUNC_END VB_UserSub


FUNC_BEGIN VB_CopySupplement
    /*000025c0*/ PUSH        {R4, R5, R6, R8, R9, LR} 
    /*000025c4*/ MOV         R4, R0 
    /*000025c8*/ SUB         SP, SP, #16 
    /*000025cc*/ MOV         R6, R1 
    /*000025d0*/ LDRD        R0, [R0, #120]              @ 0x78 
    /*000025d4*/ BL          VB_Phy2Handle
    /*000025d8*/ CMN         R0, #1 
    /*000025dc*/ BEQ         VB_CopySupplement_x26a8
    /*000025e0*/ BL          VB_Handle2Supplement
    /*000025e4*/ MOV         R5, R0 
    /*000025e8*/ LDRD        R0, [R6, #120]              @ 0x78 
    /*000025ec*/ BL          VB_Phy2Handle
    /*000025f0*/ CMN         R0, #1 
    /*000025f4*/ MOV         R3, R0 
    /*000025f8*/ BEQ         VB_CopySupplement_x26e0
    /*000025fc*/ BL          VB_Handle2Supplement
    /*00002600*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002604*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00002608*/ LDR         R3, [R4] 
    /*0000260c*/ TST         R3, #1 
    /*00002610*/ MOV         R6, R0 
    /*00002614*/ BEQ         VB_CopySupplement_x263c
    /*00002618*/ LDR         R0, [R5, #40]               @ 0x28 
    /*0000261c*/ CMP         R0, #0 
    /*00002620*/ BEQ         VB_CopySupplement_x2728
    /*00002624*/ LDR         R1, [R6, #40]               @ 0x28 
    /*00002628*/ CMP         R1, #0 
    /*0000262c*/ BEQ         VB_CopySupplement_x2728
    /*00002630*/ MOV         R2, #204                    @ 0xcc 
    /*00002634*/ BL          osal_memcpy
    /*00002638*/ LDR         R3, [R4] 
    VB_CopySupplement_x263c:
    /*0000263c*/ TST         R3, #2 
    /*00002640*/ BNE         VB_CopySupplement_x2674
    /*00002644*/ TST         R3, #8 
    /*00002648*/ BEQ         VB_CopySupplement_x26a0
    VB_CopySupplement_x264c:
    /*0000264c*/ LDR         R0, [R5, #56]               @ 0x38 
    /*00002650*/ CMP         R0, #0 
    /*00002654*/ BEQ         VB_CopySupplement_x274c
    /*00002658*/ LDR         R1, [R6, #56]               @ 0x38 
    /*0000265c*/ CMP         R1, #0 
    /*00002660*/ BEQ         VB_CopySupplement_x274c
    /*00002664*/ MOV         R2, #88                     @ 0x58 
    /*00002668*/ ADD         SP, SP, #16 
    /*0000266c*/ POP         {R4, R5, R6, R8, R9, LR} 
    VB_CopySupplement_x2670:
    /*00002670*/ B           VB_CopySupplement_x2670     @ osal_memcpy [R_ARM_JUMP24: +0x0] 
    VB_CopySupplement_x2674:
    /*00002674*/ LDR         R0, [R5, #44]               @ 0x2c 
    /*00002678*/ CMP         R0, #0 
    /*0000267c*/ BEQ         VB_CopySupplement_x2770
    /*00002680*/ LDR         R1, [R6, #44]               @ 0x2c 
    /*00002684*/ CMP         R1, #0 
    /*00002688*/ BEQ         VB_CopySupplement_x2770
    /*0000268c*/ MOV         R2, #56                     @ 0x38 
    /*00002690*/ BL          osal_memcpy
    /*00002694*/ LDR         R3, [R4] 
    /*00002698*/ TST         R3, #8 
    /*0000269c*/ BNE         VB_CopySupplement_x264c
    VB_CopySupplement_x26a0:
    /*000026a0*/ ADD         SP, SP, #16 
    /*000026a4*/ POP         {R4, R5, R6, R8, R9, PC} 
    VB_CopySupplement_x26a8:
    /*000026a8*/ MOV         R3, R0 
    /*000026ac*/ LDRD        R0, [R4, #120]              @ 0x78 
    /*000026b0*/ MOVW        IP, #2845                   @ 0xb1d 
    /*000026b4*/ STR         R3, [SP, #4] 
    /*000026b8*/ STR         IP, [SP] 
    /*000026bc*/ @ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb14] 
    /*000026c0*/ STRD        R0, [SP, #8] 
    /*000026c4*/ ADDRL       R2, =_RODATA_STR1_4_0b14    @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb14]
    /*000026c8*/ MOV         R1, #1 
    /*000026cc*/ LDR         R3, WORD_2794               @ LDR         R3, [PC, #192]              @ 0x0000000000002794 
    /*000026d0*/ MOV         R0, #6 
    /*000026d4*/ BL          HI_LOG
    /*000026d8*/ ADD         SP, SP, #16 
    /*000026dc*/ POP         {R4, R5, R6, R8, R9, PC} 
    VB_CopySupplement_x26e0:
    /*000026e0*/ MOV         R2, #0 
    /*000026e4*/ MOV         R8, #0 
    /*000026e8*/ STR         R2, [R5, #40]               @ 0x28 
    /*000026ec*/ MOV         R9, #0 
    /*000026f0*/ STRD        R8, [R5] 
    /*000026f4*/ MOVW        R0, #2860                   @ 0xb2c 
    /*000026f8*/ LDRD        R4, [R6, #120]              @ 0x78 
    /*000026fc*/ MOV         R1, #1 
    /*00002700*/ STR         R3, [SP, #4] 
    /*00002704*/ @ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb58] 
    /*00002708*/ STR         R0, [SP] 
    /*0000270c*/ ADDRL       R2, =_RODATA_STR1_4_0b58    @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb58]
    /*00002710*/ STRD        R4, [SP, #8] 
    /*00002714*/ MOV         R0, #6 
    /*00002718*/ LDR         R3, WORD_2794               @ LDR         R3, [PC, #116]              @ 0x0000000000002794 
    /*0000271c*/ BL          HI_LOG
    /*00002720*/ ADD         SP, SP, #16 
    /*00002724*/ POP         {R4, R5, R6, R8, R9, PC} 
    VB_CopySupplement_x2728:
    /*00002728*/ MOVW        R3, #2868                   @ 0xb34 
    /*0000272c*/ @ MOVW        R2, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb9c] 
    /*00002730*/ STR         R3, [SP] 
    /*00002734*/ ADDRL       R2, =_RODATA_STR1_4_0b9c    @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb9c]
    /*00002738*/ LDR         R3, WORD_2794               @ LDR         R3, [PC, #84]               @ 0x0000000000002794 
    /*0000273c*/ MOV         R1, #1 
    /*00002740*/ MOV         R0, #4 
    /*00002744*/ BL          HI_LOG
    /*00002748*/ B           VB_CopySupplement_x26a0
    VB_CopySupplement_x274c:
    /*0000274c*/ MOVW        R3, #2888                   @ 0xb48 
    /*00002750*/ @ MOVW        R2, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc0c] 
    /*00002754*/ STR         R3, [SP] 
    /*00002758*/ ADDRL       R2, =_RODATA_STR1_4_0c0c    @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc0c]
    /*0000275c*/ LDR         R3, WORD_2794               @ LDR         R3, [PC, #48]               @ 0x0000000000002794 
    /*00002760*/ MOV         R1, #1 
    /*00002764*/ MOV         R0, #4 
    /*00002768*/ BL          HI_LOG
    /*0000276c*/ B           VB_CopySupplement_x26a0
    VB_CopySupplement_x2770:
    /*00002770*/ MOVW        R3, #2878                   @ 0xb3e 
    /*00002774*/ @ MOVW        R2, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbd4] 
    /*00002778*/ STR         R3, [SP] 
    /*0000277c*/ ADDRL       R2, =_RODATA_STR1_4_0bd4    @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbd4]
    /*00002780*/ LDR         R3, WORD_2794               @ LDR         R3, [PC, #12]               @ 0x0000000000002794 
    /*00002784*/ MOV         R1, #1 
    /*00002788*/ MOV         R0, #4 
    /*0000278c*/ BL          HI_LOG
    /*00002790*/ B           VB_CopySupplement_x26a0
    /*00002794*/ WORD_2794: .word 0x00000180
FUNC_END VB_CopySupplement


FUNC_BEGIN VbProcShow
    /*00002798*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000279c*/ SUB         SP, SP, #172                @ 0xac 
    /*000027a0*/ MOV         FP, R0 
    /*000027a4*/ MOV         R2, #92                     @ 0x5c 
    /*000027a8*/ MOV         R1, #0 
    /*000027ac*/ ADD         R0, SP, #76                 @ 0x4c 
    /*000027b0*/ BL          memset
    /*000027b4*/ MOV         R0, FP 
    /*000027b8*/ @ MOVW        R1, #0                      @ .LC82 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcf4] 
    /*000027bc*/ ADDRL       R1, =_RODATA_STR1_4_0cf4    @ .LC82 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcf4]
    /*000027c0*/ @ MOVW        R2, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc44] 
    /*000027c4*/ @ MOVW        R3, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc4c] 
    /*000027c8*/ ADDRL       R2, =_RODATA_STR1_4_0c44    @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc44]
    /*000027cc*/ ADDRL       R3, =_RODATA_STR1_4_0c4c    @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc4c]
    /*000027d0*/ STR         R2, [SP, #76]               @ 0x4c 
    /*000027d4*/ @ MOVW        R2, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc54] 
    /*000027d8*/ STR         R3, [SP, #80]               @ 0x50 
    /*000027dc*/ ADDRL       R2, =_RODATA_STR1_4_0c54    @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc54]
    /*000027e0*/ @ MOVW        R3, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc5c] 
    /*000027e4*/ STR         R2, [SP, #84]               @ 0x54 
    /*000027e8*/ ADDRL       R3, =_RODATA_STR1_4_0c5c    @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc5c]
    /*000027ec*/ @ MOVW        R2, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc64] 
    /*000027f0*/ STR         R3, [SP, #88]               @ 0x58 
    /*000027f4*/ ADDRL       R2, =_RODATA_STR1_4_0c64    @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc64]
    /*000027f8*/ @ MOVW        R3, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc6c] 
    /*000027fc*/ STR         R2, [SP, #92]               @ 0x5c 
    /*00002800*/ ADDRL       R3, =_RODATA_STR1_4_0c6c    @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc6c]
    /*00002804*/ @ MOVW        R2, #0                      @ .LC66 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc74] 
    /*00002808*/ STR         R3, [SP, #96]               @ 0x60 
    /*0000280c*/ ADDRL       R2, =_RODATA_STR1_4_0c74    @ .LC66 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc74]
    /*00002810*/ @ MOVW        R3, #0                      @ .LC67 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc7c] 
    /*00002814*/ STR         R2, [SP, #100]              @ 0x64 
    /*00002818*/ ADDRL       R3, =_RODATA_STR1_4_0c7c    @ .LC67 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc7c]
    /*0000281c*/ @ MOVW        R2, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc84] 
    /*00002820*/ STR         R3, [SP, #104]              @ 0x68 
    /*00002824*/ ADDRL       R2, =_RODATA_STR1_4_0c84    @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc84]
    /*00002828*/ @ MOVW        R3, #0                      @ .LC69 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc8c] 
    /*0000282c*/ STR         R2, [SP, #108]              @ 0x6c 
    /*00002830*/ ADDRL       R3, =_RODATA_STR1_4_0c8c    @ .LC69 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc8c]
    /*00002834*/ @ MOVW        R2, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc94] 
    /*00002838*/ STR         R3, [SP, #112]              @ 0x70 
    /*0000283c*/ ADDRL       R2, =_RODATA_STR1_4_0c94    @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc94]
    /*00002840*/ @ MOVW        R3, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc9c] 
    /*00002844*/ STR         R2, [SP, #116]              @ 0x74 
    /*00002848*/ ADDRL       R3, =_RODATA_STR1_4_0c9c    @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc9c]
    /*0000284c*/ @ MOVW        R2, #0                      @ .LC72 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xca4] 
    /*00002850*/ STR         R3, [SP, #120]              @ 0x78 
    /*00002854*/ ADDRL       R2, =_RODATA_STR1_4_0ca4    @ .LC72 [R_ARM_MOVT_ABS: .rodata.str1.4+0xca4]
    /*00002858*/ @ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcac] 
    /*0000285c*/ STR         R2, [SP, #124]              @ 0x7c 
    /*00002860*/ ADDRL       R3, =_RODATA_STR1_4_0cac    @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcac]
    /*00002864*/ @ MOVW        R2, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcb4] 
    /*00002868*/ STR         R3, [SP, #128]              @ 0x80 
    /*0000286c*/ ADDRL       R2, =_RODATA_STR1_4_0cb4    @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcb4]
    /*00002870*/ @ MOVW        R3, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcbc] 
    /*00002874*/ STR         R2, [SP, #132]              @ 0x84 
    /*00002878*/ ADDRL       R3, =_RODATA_STR1_4_0cbc    @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcbc]
    /*0000287c*/ @ MOVW        R2, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcc4] 
    /*00002880*/ STR         R3, [SP, #136]              @ 0x88 
    /*00002884*/ ADDRL       R2, =_RODATA_STR1_4_0cc4    @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcc4]
    /*00002888*/ @ MOVW        R3, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xccc] 
    /*0000288c*/ STR         R2, [SP, #140]              @ 0x8c 
    /*00002890*/ ADDRL       R3, =_RODATA_STR1_4_0ccc    @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0xccc]
    /*00002894*/ @ MOVW        R2, #0                      @ .LC78 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcd4] 
    /*00002898*/ STR         R3, [SP, #144]              @ 0x90 
    /*0000289c*/ ADDRL       R2, =_RODATA_STR1_4_0cd4    @ .LC78 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcd4]
    /*000028a0*/ @ MOVW        R3, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcdc] 
    /*000028a4*/ STR         R2, [SP, #148]              @ 0x94 
    /*000028a8*/ ADDRL       R3, =_RODATA_STR1_4_0cdc    @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcdc]
    /*000028ac*/ @ MOVW        R2, #0                      @ .LC80 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xce4] 
    /*000028b0*/ STR         R3, [SP, #152]              @ 0x98 
    /*000028b4*/ ADDRL       R2, =_RODATA_STR1_4_0ce4    @ .LC80 [R_ARM_MOVT_ABS: .rodata.str1.4+0xce4]
    /*000028b8*/ @ MOVW        R3, #0                      @ .LC81 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcec] 
    /*000028bc*/ STR         R2, [SP, #156]              @ 0x9c 
    /*000028c0*/ ADDRL       R3, =_RODATA_STR1_4_0cec    @ .LC81 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcec]
    /*000028c4*/ STR         R3, [SP, #160]              @ 0xa0 
    /*000028c8*/ BL          osal_seq_printf
    /*000028cc*/ MOV         R0, FP 
    /*000028d0*/ @ MOVW        R1, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd50] 
    /*000028d4*/ ADDRL       R1, =_RODATA_STR1_4_0d50    @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd50]
    /*000028d8*/ BL          osal_seq_printf
    /*000028dc*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000028e0*/ LDR         R3, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000028e4*/ MOV         R0, FP 
    /*000028e8*/ @ MOVW        R1, #0                      @ .LC84 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd54] 
    /*000028ec*/ ADDRL       R1, =_RODATA_STR1_4_0d54    @ .LC84 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd54]
    /*000028f0*/ MOV         R5, R3 
    /*000028f4*/ STR         R3, [SP, #56]               @ 0x38 
    /*000028f8*/ BL          osal_seq_printf
    /*000028fc*/ MOV         R0, FP 
    /*00002900*/ @ MOVW        R1, #0                      @ .LC85 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xda8] 
    /*00002904*/ ADDRL       R1, =_RODATA_STR1_4_0da8    @ .LC85 [R_ARM_MOVT_ABS: .rodata.str1.4+0xda8]
    /*00002908*/ BL          osal_seq_printf
    /*0000290c*/ LDR         R2, [R5, #16] 
    /*00002910*/ MOV         R0, FP 
    /*00002914*/ @ MOVW        R1, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdb4] 
    /*00002918*/ ADDRL       R1, =_RODATA_STR1_4_0db4    @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdb4]
    /*0000291c*/ BL          osal_seq_printf
    /*00002920*/ MOV         R0, FP 
    /*00002924*/ @ MOVW        R1, #0                      @ .LC87 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdbc] 
    /*00002928*/ ADDRL       R1, =_RODATA_STR1_4_0dbc    @ .LC87 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdbc]
    /*0000292c*/ BL          osal_seq_printf
    /*00002930*/ @ MOVW        R2, #0                      @ .LC91 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe2c] 
    /*00002934*/ MOV         R0, FP 
    /*00002938*/ ADDRL       R2, =_RODATA_STR1_4_0e2c    @ .LC91 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe2c]
    /*0000293c*/ @ MOVW        R3, #0                      @ .LC88 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe10] 
    /*00002940*/ STR         R2, [SP] 
    /*00002944*/ ADDRL       R3, =_RODATA_STR1_4_0e10    @ .LC88 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe10]
    /*00002948*/ @ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe18] 
    /*0000294c*/ @ MOVW        R1, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe20] 
    /*00002950*/ ADDRL       R2, =_RODATA_STR1_4_0e18    @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe18]
    /*00002954*/ ADDRL       R1, =_RODATA_STR1_4_0e20    @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe20]
    /*00002958*/ BL          osal_seq_printf
    /*0000295c*/ MOV         R0, R5 
    /*00002960*/ MOV         R1, #1 
    /*00002964*/ LDR         R4, [R5] 
    /*00002968*/ BL          VB_GetSupplementSize
    /*0000296c*/ @ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe34] 
    /*00002970*/ ADDRL       R1, =_RODATA_STR1_4_0e34    @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe34]
    /*00002974*/ MOV         R2, R4 
    /*00002978*/ MOV         R4, R5 
    /*0000297c*/ ADD         R3, R0, #255                @ 0xff 
    /*00002980*/ MOV         R0, FP 
    /*00002984*/ BIC         R3, R3, #255                @ 0xff 
    /*00002988*/ BL          osal_seq_printf
    /*0000298c*/ ADD         R0, R5, #556                @ 0x22c 
    /*00002990*/ BL          osal_down_interruptible
    /*00002994*/ SUBS        R5, R0, #0 
    /*00002998*/ BNE         VbProcShow_x2f08
    /*0000299c*/ LDR         R4, [R4, #8] 
    /*000029a0*/ CMP         R4, #0 
    /*000029a4*/ BEQ         VbProcShow_x2edc
    /*000029a8*/ LDR         R4, [SP, #56]               @ 0x38 
    /*000029ac*/ ADD         R1, SP, #72                 @ 0x48 
    /*000029b0*/ ADD         R0, R4, #548                @ 0x224 
    /*000029b4*/ BL          osal_spin_lock_irqsave
    /*000029b8*/ MOV         R3, R4 
    /*000029bc*/ LDR         R4, [R4, #16] 
    /*000029c0*/ CMP         R4, #0 
    /*000029c4*/ BEQ         VbProcShow_x29ec
    /*000029c8*/ LDR         R3, [R3, #8] 
    /*000029cc*/ ADD         R0, R3, R4, LSL #2 
    /*000029d0*/ MOV         R4, R5 
    VbProcShow_x29d4:
    /*000029d4*/ LDR         R2, [R3], #4 
    /*000029d8*/ CMP         R2, #0 
    /*000029dc*/ LDRNE       R2, [R2, #12] 
    /*000029e0*/ ADDNE       R4, R4, R2 
    /*000029e4*/ CMP         R3, R0 
    /*000029e8*/ BNE         VbProcShow_x29d4
    VbProcShow_x29ec:
    /*000029ec*/ ADD         R1, SP, #72                 @ 0x48 
    /*000029f0*/ LDR         R0, WORD_2f18               @ LDR         R0, [PC, #1312]             @ 0x0000000000002f18 
    /*000029f4*/ BL          osal_spin_unlock_irqrestore
    /*000029f8*/ MOV         R2, R4 
    /*000029fc*/ @ MOVW        R1, #0                      @ .LC93 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe3c] 
    /*00002a00*/ MOV         R0, FP 
    /*00002a04*/ ADDRL       R1, =_RODATA_STR1_4_0e3c    @ .LC93 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe3c]
    /*00002a08*/ @ MOVW        R5, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xea0] 
    /*00002a0c*/ BL          osal_seq_printf
    /*00002a10*/ @ MOVW        R1, #0                      @ .LC94 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe44] 
    /*00002a14*/ MOV         R0, FP 
    /*00002a18*/ ADDRL       R1, =_RODATA_STR1_4_0e44    @ .LC94 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe44]
    /*00002a1c*/ BL          osal_seq_printf
    /*00002a20*/ @ MOVW        R1, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe98] 
    /*00002a24*/ MOV         R0, FP 
    /*00002a28*/ ADDRL       R1, =_RODATA_STR1_4_0e98    @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe98]
    /*00002a2c*/ BL          osal_seq_printf
    /*00002a30*/ ADDRL       R5, =_RODATA_STR1_4_0ea0    @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0xea0]
    /*00002a34*/ MOV         R4, #0 
    VbProcShow_x2a38:
    /*00002a38*/ MOV         R2, R4 
    /*00002a3c*/ MOV         R1, R5 
    /*00002a40*/ ADD         R4, R4, #1 
    /*00002a44*/ MOV         R0, FP 
    /*00002a48*/ BL          osal_seq_printf
    /*00002a4c*/ CMP         R4, #16 
    /*00002a50*/ BNE         VbProcShow_x2a38
    /*00002a54*/ @ MOVW        R1, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xea8] 
    /*00002a58*/ MOV         R0, FP 
    /*00002a5c*/ ADDRL       R1, =_RODATA_STR1_4_0ea8    @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0xea8]
    /*00002a60*/ LDR         R5, WORD_2f1c               @ LDR         R5, [PC, #1204]             @ 0x0000000000002f1c 
    /*00002a64*/ BL          osal_seq_printf
    /*00002a68*/ @ MOVW        R6, #0                      @ .LC98 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xeb0] 
    /*00002a6c*/ ADD         R7, R5, #512                @ 0x200 
    /*00002a70*/ ADDRL       R6, =_RODATA_STR1_4_0eb0    @ .LC98 [R_ARM_MOVT_ABS: .rodata.str1.4+0xeb0]
    /*00002a74*/ MOV         R4, R5 
    VbProcShow_x2a78:
    /*00002a78*/ LDRD        R2, [R4, #8] 
    /*00002a7c*/ MOV         R1, R6 
    /*00002a80*/ ADD         R4, R4, #32 
    /*00002a84*/ MOV         R0, FP 
    /*00002a88*/ BL          osal_seq_printf
    /*00002a8c*/ CMP         R4, R7 
    /*00002a90*/ BNE         VbProcShow_x2a78
    /*00002a94*/ @ MOVW        R1, #0                      @ .LC99 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xeb8] 
    /*00002a98*/ MOV         R0, FP 
    /*00002a9c*/ ADDRL       R1, =_RODATA_STR1_4_0eb8    @ .LC99 [R_ARM_MOVT_ABS: .rodata.str1.4+0xeb8]
    /*00002aa0*/ @ MOVW        R6, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xea0] 
    /*00002aa4*/ BL          osal_seq_printf
    /*00002aa8*/ ADDRL       R6, =_RODATA_STR1_4_0ea0    @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0xea0]
    VbProcShow_x2aac:
    /*00002aac*/ LDR         R2, [R5, #16] 
    /*00002ab0*/ MOV         R1, R6 
    /*00002ab4*/ ADD         R5, R5, #32 
    /*00002ab8*/ MOV         R0, FP 
    /*00002abc*/ BL          osal_seq_printf
    /*00002ac0*/ CMP         R5, R4 
    /*00002ac4*/ BNE         VbProcShow_x2aac
    /*00002ac8*/ LDR         R6, WORD_2f20               @ LDR         R6, [PC, #1104]             @ 0x0000000000002f20 
    /*00002acc*/ @ MOVW        R1, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd50] 
    /*00002ad0*/ MOV         R0, FP 
    /*00002ad4*/ ADDRL       R1, =_RODATA_STR1_4_0d50    @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd50]
    /*00002ad8*/ BL          osal_seq_printf
    /*00002adc*/ ADD         R9, R6, #100                @ 0x64 
    /*00002ae0*/ @ MOVW        R3, #0                      @ .LC100 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xec0] 
    /*00002ae4*/ ADD         R5, R6, #612                @ 0x264 
    /*00002ae8*/ ADDRL       R3, =_RODATA_STR1_4_0ec0    @ .LC100 [R_ARM_MOVT_ABS: .rodata.str1.4+0xec0]
    /*00002aec*/ MOV         R7, #0 
    /*00002af0*/ STR         R3, [SP, #48]               @ 0x30 
    /*00002af4*/ @ MOVW        R3, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe98] 
    /*00002af8*/ ADDRL       R3, =_RODATA_STR1_4_0e98    @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe98]
    /*00002afc*/ STR         R3, [SP, #52]               @ 0x34 
    /*00002b00*/ B           VbProcShow_x2b1c
    VbProcShow_x2b04:
    /*00002b04*/ LDR         R3, WORD_2f24               @ LDR         R3, [PC, #1048]             @ 0x0000000000002f24 
    /*00002b08*/ ADD         R7, R7, #1 
    /*00002b0c*/ ADD         R9, R9, #520                @ 0x208 
    /*00002b10*/ ADD         R5, R5, #520                @ 0x208 
    /*00002b14*/ CMP         R6, R3 
    /*00002b18*/ BEQ         VbProcShow_x2c00
    VbProcShow_x2b1c:
    /*00002b1c*/ LDR         R3, [R6, #4]! 
    /*00002b20*/ CMP         R3, #0 
    /*00002b24*/ BEQ         VbProcShow_x2b04
    /*00002b28*/ LDR         R3, WORD_2f28               @ LDR         R3, [PC, #1016]             @ 0x0000000000002f28 
    /*00002b2c*/ LDR         R3, [R3, R7, LSL #2] 
    /*00002b30*/ CMP         R3, #0 
    /*00002b34*/ BEQ         VbProcShow_x2b04
    /*00002b38*/ MOV         R2, R7 
    /*00002b3c*/ LDR         R1, [SP, #48]               @ 0x30 
    /*00002b40*/ MOV         R0, FP 
    /*00002b44*/ @ MOVW        R4, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xea0] 
    /*00002b48*/ BL          osal_seq_printf
    /*00002b4c*/ LDR         R1, [SP, #52]               @ 0x34 
    /*00002b50*/ MOV         R0, FP 
    /*00002b54*/ ADDRL       R4, =_RODATA_STR1_4_0ea0    @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0xea0]
    /*00002b58*/ BL          osal_seq_printf
    /*00002b5c*/ MOV         R8, #0 
    VbProcShow_x2b60:
    /*00002b60*/ MOV         R2, R8 
    /*00002b64*/ MOV         R1, R4 
    /*00002b68*/ ADD         R8, R8, #1 
    /*00002b6c*/ MOV         R0, FP 
    /*00002b70*/ BL          osal_seq_printf
    /*00002b74*/ CMP         R8, #16 
    /*00002b78*/ BNE         VbProcShow_x2b60
    /*00002b7c*/ @ MOVW        R1, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xea8] 
    /*00002b80*/ MOV         R0, FP 
    /*00002b84*/ ADDRL       R1, =_RODATA_STR1_4_0ea8    @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0xea8]
    /*00002b88*/ @ MOVW        R8, #0                      @ .LC98 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xeb0] 
    /*00002b8c*/ BL          osal_seq_printf
    /*00002b90*/ ADDRL       R8, =_RODATA_STR1_4_0eb0    @ .LC98 [R_ARM_MOVT_ABS: .rodata.str1.4+0xeb0]
    /*00002b94*/ MOV         R4, R9 
    VbProcShow_x2b98:
    /*00002b98*/ LDRD        R2, [R4, #8] 
    /*00002b9c*/ MOV         R1, R8 
    /*00002ba0*/ ADD         R4, R4, #32 
    /*00002ba4*/ MOV         R0, FP 
    /*00002ba8*/ BL          osal_seq_printf
    /*00002bac*/ CMP         R4, R5 
    /*00002bb0*/ BNE         VbProcShow_x2b98
    /*00002bb4*/ @ MOVW        R1, #0                      @ .LC99 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xeb8] 
    /*00002bb8*/ MOV         R0, FP 
    /*00002bbc*/ ADDRL       R1, =_RODATA_STR1_4_0eb8    @ .LC99 [R_ARM_MOVT_ABS: .rodata.str1.4+0xeb8]
    /*00002bc0*/ @ MOVW        R8, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xea0] 
    /*00002bc4*/ BL          osal_seq_printf
    /*00002bc8*/ ADDRL       R8, =_RODATA_STR1_4_0ea0    @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0xea0]
    /*00002bcc*/ MOV         SL, R9 
    VbProcShow_x2bd0:
    /*00002bd0*/ LDR         R2, [SL, #16] 
    /*00002bd4*/ MOV         R1, R8 
    /*00002bd8*/ ADD         SL, SL, #32 
    /*00002bdc*/ MOV         R0, FP 
    /*00002be0*/ BL          osal_seq_printf
    /*00002be4*/ CMP         SL, R4 
    /*00002be8*/ BNE         VbProcShow_x2bd0
    /*00002bec*/ @ MOVW        R1, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd50] 
    /*00002bf0*/ MOV         R0, FP 
    /*00002bf4*/ ADDRL       R1, =_RODATA_STR1_4_0d50    @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd50]
    /*00002bf8*/ BL          osal_seq_printf
    /*00002bfc*/ B           VbProcShow_x2b04
    VbProcShow_x2c00:
    /*00002c00*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00002c04*/ LDR         R3, [R3, #16] 
    /*00002c08*/ CMP         R3, #0 
    /*00002c0c*/ BEQ         VbProcShow_x2eb8
    /*00002c10*/ @ MOVW        R3, #0                      @ .LC101 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf14] 
    /*00002c14*/ @ MOVW        R2, #0                      @ .LC104 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfa4] 
    /*00002c18*/ ADDRL       R3, =_RODATA_STR1_4_0f14    @ .LC101 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf14]
    /*00002c1c*/ ADDRL       R2, =_RODATA_STR1_4_0fa4    @ .LC104 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfa4]
    /*00002c20*/ STR         R3, [SP, #60]               @ 0x3c 
    /*00002c24*/ MOV         R3, #0 
    /*00002c28*/ STR         R2, [SP, #64]               @ 0x40 
    /*00002c2c*/ STR         R3, [SP, #52]               @ 0x34 
    VbProcShow_x2c30:
    /*00002c30*/ ADD         R1, SP, #72                 @ 0x48 
    /*00002c34*/ LDR         R0, WORD_2f18               @ LDR         R0, [PC, #732]              @ 0x0000000000002f18 
    /*00002c38*/ BL          osal_spin_lock_irqsave
    /*00002c3c*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00002c40*/ LDR         R2, [SP, #52]               @ 0x34 
    /*00002c44*/ LDR         R3, [R3, #8] 
    /*00002c48*/ LDR         R8, [R3, R2, LSL #2] 
    /*00002c4c*/ CMP         R8, #0 
    /*00002c50*/ BEQ         VbProcShow_x2e90
    /*00002c54*/ MOV         R2, R8 
    /*00002c58*/ MOV         R4, #0 
    /*00002c5c*/ LDR         R3, [R2, #44]!              @ 0x2c 
    /*00002c60*/ CMP         R3, R2 
    /*00002c64*/ BEQ         VbProcShow_x2c78
    VbProcShow_x2c68:
    /*00002c68*/ LDR         R3, [R3] 
    /*00002c6c*/ ADD         R4, R4, #1 
    /*00002c70*/ CMP         R3, R2 
    /*00002c74*/ BNE         VbProcShow_x2c68
    VbProcShow_x2c78:
    /*00002c78*/ ADD         R2, R8, #176                @ 0xb0 
    /*00002c7c*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*00002c80*/ MOV         R0, FP 
    /*00002c84*/ BL          osal_seq_printf
    /*00002c88*/ LDR         R3, [SP, #64]               @ 0x40 
    /*00002c8c*/ @ MOVW        R2, #0                      @ .LC106 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfb4] 
    /*00002c90*/ @ MOVW        R1, #0                      @ .LC103 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf78] 
    /*00002c94*/ ADDRL       R2, =_RODATA_STR1_4_0fb4    @ .LC106 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfb4]
    /*00002c98*/ ADDRL       R1, =_RODATA_STR1_4_0f78    @ .LC103 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf78]
    /*00002c9c*/ STR         R3, [SP, #24] 
    /*00002ca0*/ MOV         R0, FP 
    /*00002ca4*/ STR         R2, [SP, #16] 
    /*00002ca8*/ @ MOVW        R3, #0                      @ .LC105 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfac] 
    /*00002cac*/ @ MOVW        R2, #0                      @ .LC108 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc4] 
    /*00002cb0*/ ADDRL       R3, =_RODATA_STR1_4_0fac    @ .LC105 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfac]
    /*00002cb4*/ ADDRL       R2, =_RODATA_STR1_4_0fc4    @ .LC108 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc4]
    /*00002cb8*/ STR         R3, [SP, #20] 
    /*00002cbc*/ STR         R2, [SP, #8] 
    /*00002cc0*/ @ MOVW        R3, #0                      @ .LC107 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfbc] 
    /*00002cc4*/ @ MOVW        R2, #0                      @ .LC110 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfd4] 
    /*00002cc8*/ ADDRL       R3, =_RODATA_STR1_4_0fbc    @ .LC107 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfbc]
    /*00002ccc*/ ADDRL       R2, =_RODATA_STR1_4_0fd4    @ .LC110 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfd4]
    /*00002cd0*/ STR         R3, [SP, #12] 
    /*00002cd4*/ STR         R2, [SP] 
    /*00002cd8*/ @ MOVW        R3, #0                      @ .LC109 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfcc] 
    /*00002cdc*/ @ MOVW        R2, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe98] 
    /*00002ce0*/ ADDRL       R3, =_RODATA_STR1_4_0fcc    @ .LC109 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfcc]
    /*00002ce4*/ ADDRL       R2, =_RODATA_STR1_4_0e98    @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe98]
    /*00002ce8*/ STR         R3, [SP, #4] 
    /*00002cec*/ @ MOVW        R3, #0                      @ .LC102 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf6c] 
    /*00002cf0*/ ADDRL       R3, =_RODATA_STR1_4_0f6c    @ .LC102 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf6c]
    /*00002cf4*/ BL          osal_seq_printf
    /*00002cf8*/ LDR         R3, [R8, #72]               @ 0x48 
    /*00002cfc*/ LDR         R2, [R8] 
    /*00002d00*/ @ MOVW        R1, #0                      @ .LC111 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfe0] 
    /*00002d04*/ MOV         R0, FP 
    /*00002d08*/ ADDRL       R1, =_RODATA_STR1_4_0fe0    @ .LC111 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfe0]
    /*00002d0c*/ STR         R3, [SP, #40]               @ 0x28 
    /*00002d10*/ LDR         R3, [R8, #68]               @ 0x44 
    /*00002d14*/ STR         R3, [SP, #36]               @ 0x24 
    /*00002d18*/ LDR         R3, [R8, #12] 
    /*00002d1c*/ STR         R3, [SP, #32] 
    /*00002d20*/ LDRD        R6, [R8, #16] 
    /*00002d24*/ STRD        R6, [SP, #24] 
    /*00002d28*/ LDR         R3, [R8, #192]              @ 0xc0 
    /*00002d2c*/ STR         R3, [SP, #16] 
    /*00002d30*/ LDR         R3, [R8, #8] 
    /*00002d34*/ STR         R3, [SP, #12] 
    /*00002d38*/ LDR         R3, [R8, #40]               @ 0x28 
    /*00002d3c*/ STR         R3, [SP, #8] 
    /*00002d40*/ LDRD        R6, [R8, #32] 
    /*00002d44*/ STRD        R6, [SP] 
    /*00002d48*/ BL          osal_seq_printf
    /*00002d4c*/ LDR         R3, [R8, #12] 
    /*00002d50*/ CMP         R3, R4 
    /*00002d54*/ BEQ         VbProcShow_x2e90
    /*00002d58*/ @ MOVW        R1, #0                      @ .LC112 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1014] 
    /*00002d5c*/ MOV         R0, FP 
    /*00002d60*/ ADDRL       R1, =_RODATA_STR1_4_1014    @ .LC112 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1014]
    /*00002d64*/ @ MOVW        R4, #0                      @ .LC113 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x101c] 
    /*00002d68*/ BL          osal_seq_printf
    /*00002d6c*/ ADDRL       R4, =_RODATA_STR1_4_101c    @ .LC113 [R_ARM_MOVT_ABS: .rodata.str1.4+0x101c]
    /*00002d70*/ ADD         R5, SP, #164                @ 0xa4 
    /*00002d74*/ ADD         R6, SP, #72                 @ 0x48 
    VbProcShow_x2d78:
    /*00002d78*/ LDR         R2, [R6, #4]! 
    /*00002d7c*/ MOV         R1, R4 
    /*00002d80*/ MOV         R0, FP 
    /*00002d84*/ CMP         R2, #0 
    /*00002d88*/ BEQ         VbProcShow_x2d90
    /*00002d8c*/ BL          osal_seq_printf
    VbProcShow_x2d90:
    /*00002d90*/ CMP         R5, R6 
    /*00002d94*/ BNE         VbProcShow_x2d78
    /*00002d98*/ MOV         R9, R8 
    /*00002d9c*/ @ MOVW        R1, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd50] 
    /*00002da0*/ MOV         R0, FP 
    /*00002da4*/ ADDRL       R1, =_RODATA_STR1_4_0d50    @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd50]
    /*00002da8*/ BL          osal_seq_printf
    /*00002dac*/ LDR         R7, [R9, #52]!              @ 0x34 
    /*00002db0*/ CMP         R7, R9 
    /*00002db4*/ BEQ         VbProcShow_x2e40
    /*00002db8*/ @ MOVW        R3, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd50] 
    /*00002dbc*/ ADDRL       R3, =_RODATA_STR1_4_0d50    @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd50]
    /*00002dc0*/ @ MOVW        R5, #0                      @ .LC115 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1028] 
    /*00002dc4*/ ADDRL       R5, =_RODATA_STR1_4_1028    @ .LC115 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1028]
    /*00002dc8*/ MOV         SL, R3 
    /*00002dcc*/ ADD         R2, R8, #76                 @ 0x4c 
    /*00002dd0*/ STR         R8, [SP, #68]               @ 0x44 
    /*00002dd4*/ STR         R2, [SP, #48]               @ 0x30 
    VbProcShow_x2dd8:
    /*00002dd8*/ LDR         R2, [R7, #8] 
    /*00002ddc*/ MOV         R1, R5 
    /*00002de0*/ MOV         R0, FP 
    /*00002de4*/ ADD         R4, R7, #28 
    /*00002de8*/ BL          osal_seq_printf
    /*00002dec*/ LDR         R8, [SP, #48]               @ 0x30 
    /*00002df0*/ ADD         R6, R7, #120                @ 0x78 
    VbProcShow_x2df4:
    /*00002df4*/ LDR         R2, [R4, #4] 
    /*00002df8*/ MOV         R1, R5 
    /*00002dfc*/ MOV         R0, FP 
    /*00002e00*/ ADD         R8, R8, #4 
    /*00002e04*/ BL          osal_seq_printf
    /*00002e08*/ LDR         R3, [R4, #4]! 
    /*00002e0c*/ CMP         R3, #0 
    /*00002e10*/ LDRGT       R3, [R8, #-4] 
    /*00002e14*/ ADDGT       R3, R3, #1 
    /*00002e18*/ STRGT       R3, [R8, #-4] 
    /*00002e1c*/ CMP         R4, R6 
    /*00002e20*/ BNE         VbProcShow_x2df4
    /*00002e24*/ MOV         R1, SL 
    /*00002e28*/ MOV         R0, FP 
    /*00002e2c*/ BL          osal_seq_printf
    /*00002e30*/ LDR         R7, [R7] 
    /*00002e34*/ CMP         R7, R9 
    /*00002e38*/ BNE         VbProcShow_x2dd8
    /*00002e3c*/ LDR         R8, [SP, #68]               @ 0x44 
    VbProcShow_x2e40:
    /*00002e40*/ @ MOVW        R1, #0                      @ .LC114 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1020] 
    /*00002e44*/ MOV         R0, FP 
    /*00002e48*/ ADDRL       R1, =_RODATA_STR1_4_1020    @ .LC114 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1020]
    /*00002e4c*/ @ MOVW        R4, #0                      @ .LC115 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1028] 
    /*00002e50*/ BL          osal_seq_printf
    /*00002e54*/ ADD         R6, R8, #72                 @ 0x48 
    /*00002e58*/ ADDRL       R4, =_RODATA_STR1_4_1028    @ .LC115 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1028]
    /*00002e5c*/ ADD         R8, R8, #164                @ 0xa4 
    /*00002e60*/ MOV         R5, #0 
    VbProcShow_x2e64:
    /*00002e64*/ LDR         R2, [R6, #4] 
    /*00002e68*/ MOV         R1, R4 
    /*00002e6c*/ MOV         R0, FP 
    /*00002e70*/ BL          osal_seq_printf
    /*00002e74*/ STR         R5, [R6, #4]! 
    /*00002e78*/ CMP         R8, R6 
    /*00002e7c*/ BNE         VbProcShow_x2e64
    /*00002e80*/ @ MOVW        R1, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd50] 
    /*00002e84*/ MOV         R0, FP 
    /*00002e88*/ ADDRL       R1, =_RODATA_STR1_4_0d50    @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd50]
    /*00002e8c*/ BL          osal_seq_printf
    VbProcShow_x2e90:
    /*00002e90*/ ADD         R1, SP, #72                 @ 0x48 
    /*00002e94*/ LDR         R0, WORD_2f18               @ LDR         R0, [PC, #124]              @ 0x0000000000002f18 
    /*00002e98*/ BL          osal_spin_unlock_irqrestore
    /*00002e9c*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00002ea0*/ LDR         R2, [SP, #52]               @ 0x34 
    /*00002ea4*/ LDR         R3, [R3, #16] 
    /*00002ea8*/ ADD         R2, R2, #1 
    /*00002eac*/ STR         R2, [SP, #52]               @ 0x34 
    /*00002eb0*/ CMP         R3, R2 
    /*00002eb4*/ BHI         VbProcShow_x2c30
    VbProcShow_x2eb8:
    /*00002eb8*/ LDR         R0, WORD_2f2c               @ LDR         R0, [PC, #108]              @ 0x0000000000002f2c 
    /*00002ebc*/ BL          osal_up
    /*00002ec0*/ MOV         R0, FP 
    /*00002ec4*/ @ MOVW        R1, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd50] 
    /*00002ec8*/ ADDRL       R1, =_RODATA_STR1_4_0d50    @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd50]
    /*00002ecc*/ BL          osal_seq_printf
    /*00002ed0*/ MOV         R0, #0 
    /*00002ed4*/ ADD         SP, SP, #172                @ 0xac 
    /*00002ed8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    VbProcShow_x2edc:
    /*00002edc*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00002ee0*/ ADD         R0, R3, #556                @ 0x22c 
    /*00002ee4*/ BL          osal_up
    /*00002ee8*/ MOV         R2, R4 
    /*00002eec*/ MOV         R0, FP 
    /*00002ef0*/ @ MOVW        R1, #0                      @ .LC93 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe3c] 
    /*00002ef4*/ ADDRL       R1, =_RODATA_STR1_4_0e3c    @ .LC93 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe3c]
    /*00002ef8*/ BL          osal_seq_printf
    /*00002efc*/ MOV         R0, R4 
    /*00002f00*/ ADD         SP, SP, #172                @ 0xac 
    /*00002f04*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    VbProcShow_x2f08:
    /*00002f08*/ MOV         R0, #65024                  @ 0xfe00 
    /*00002f0c*/ MOVT        R0, #65535                  @ 0xffff 
    /*00002f10*/ ADD         SP, SP, #172                @ 0xac 
    /*00002f14*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    /*00002f18*/ WORD_2f18: .word 0x00000224
    /*00002f1c*/ WORD_2f1c: .word 0x00000010
    /*00002f20*/ WORD_2f20: .word 0x0000028c
    /*00002f24*/ WORD_2f24: .word 0x000002e8
    /*00002f28*/ WORD_2f28: .word 0x00000230
    /*00002f2c*/ WORD_2f2c: .word 0x0000022c
FUNC_END VbProcShow


FUNC_BEGIN VB_Handle2BlkId
    /*00002f30*/ PUSH        {R4, R5, R6, LR} 
    /*00002f34*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002f38*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00002f3c*/ LDR         R3, [R4, #8] 
    /*00002f40*/ CMP         R3, #0 
    /*00002f44*/ BEQ         VB_Handle2BlkId_x2fa8
    /*00002f48*/ LSR         R6, R0, #16 
    /*00002f4c*/ MOV         R5, R0 
    /*00002f50*/ LDR         R3, [R3, R6, LSL #2] 
    /*00002f54*/ CMP         R3, #0 
    /*00002f58*/ BEQ         VB_Handle2BlkId_x2fc8
    VB_Handle2BlkId_x2f5c:
    /*00002f5c*/ LDR         R3, [R4, #536]              @ 0x218 
    /*00002f60*/ LDR         R3, [R3, R6, LSL #2] 
    /*00002f64*/ CMP         R3, #0 
    /*00002f68*/ BEQ         VB_Handle2BlkId_x2fe8
    VB_Handle2BlkId_x2f6c:
    /*00002f6c*/ LDR         R3, [R4, #8] 
    /*00002f70*/ UXTH        R5, R5 
    /*00002f74*/ LDR         R3, [R3, R6, LSL #2] 
    /*00002f78*/ LDR         R3, [R3, #12] 
    /*00002f7c*/ CMP         R5, R3 
    /*00002f80*/ BCC         VB_Handle2BlkId_x2fa0
    /*00002f84*/ @ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258] 
    /*00002f88*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00002f8c*/ ADDRL       R3, =_RODATA_STR1_4_0258    @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258]
    /*00002f90*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00002f94*/ MOVW        R2, #1361                   @ 0x551 
    /*00002f98*/ LDR         R1, WORD_3008               @ LDR         R1, [PC, #104]              @ 0x0000000000003008 
    /*00002f9c*/ BL          osal_panic
    VB_Handle2BlkId_x2fa0:
    /*00002fa0*/ MOV         R0, R5 
    /*00002fa4*/ POP         {R4, R5, R6, PC} 
    VB_Handle2BlkId_x2fa8:
    /*00002fa8*/ MOV         R1, #1 
    /*00002fac*/ MVN         R5, #0 
    /*00002fb0*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00002fb4*/ MOV         R0, #4 
    /*00002fb8*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00002fbc*/ BL          HI_LOG
    /*00002fc0*/ MOV         R0, R5 
    /*00002fc4*/ POP         {R4, R5, R6, PC} 
    VB_Handle2BlkId_x2fc8:
    /*00002fc8*/ @ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c0] 
    /*00002fcc*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00002fd0*/ ADDRL       R3, =_RODATA_STR1_4_01c0    @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c0]
    /*00002fd4*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00002fd8*/ MOVW        R2, #1361                   @ 0x551 
    /*00002fdc*/ LDR         R1, WORD_3008               @ LDR         R1, [PC, #36]               @ 0x0000000000003008 
    /*00002fe0*/ BL          osal_panic
    /*00002fe4*/ B           VB_Handle2BlkId_x2f5c
    VB_Handle2BlkId_x2fe8:
    /*00002fe8*/ @ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22c] 
    /*00002fec*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00002ff0*/ ADDRL       R3, =_RODATA_STR1_4_022c    @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22c]
    /*00002ff4*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00002ff8*/ MOVW        R2, #1361                   @ 0x551 
    /*00002ffc*/ LDR         R1, WORD_3008               @ LDR         R1, [PC, #4]                @ 0x0000000000003008 
    /*00003000*/ BL          osal_panic
    /*00003004*/ B           VB_Handle2BlkId_x2f6c
    /*00003008*/ WORD_3008: .word 0x00000194
FUNC_END VB_Handle2BlkId


FUNC_BEGIN VB_GetPoolId
    /*0000300c*/ @ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003010*/ LDR         IP, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00003014*/ PUSH        {R4, R5, LR} 
    /*00003018*/ MOV         R5, R1 
    /*0000301c*/ MOV         R1, R2 
    /*00003020*/ LDR         R2, [IP, #8] 
    /*00003024*/ SUB         SP, SP, #12 
    /*00003028*/ MOV         R4, R0 
    /*0000302c*/ CMP         R2, #0 
    /*00003030*/ MOV         R0, R3 
    /*00003034*/ LDR         R3, [SP, #24] 
    /*00003038*/ BEQ         VB_GetPoolId_x3074
    /*0000303c*/ CMP         R1, #0 
    /*00003040*/ BEQ         VB_GetPoolId_x3094
    /*00003044*/ CMP         R3, #12 
    /*00003048*/ STR         R3, [SP, #4] 
    /*0000304c*/ STREQ       R0, [SP] 
    /*00003050*/ MOV         R2, R4 
    /*00003054*/ MOVNE       R0, R1 
    /*00003058*/ MOV         R3, R5 
    /*0000305c*/ MOVNE       R1, #0 
    /*00003060*/ MOVEQ       R0, R1 
    /*00003064*/ STRNE       R1, [SP] 
    /*00003068*/ BL          VbSearchCommPoolBySize      @ BL          0x0000000000000BEC 
    VB_GetPoolId_x306c:
    /*0000306c*/ ADD         SP, SP, #12 
    /*00003070*/ POP         {R4, R5, PC} 
    VB_GetPoolId_x3074:
    /*00003074*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00003078*/ MOV         R1, #1 
    /*0000307c*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00003080*/ MOV         R0, #4 
    /*00003084*/ BL          HI_LOG
    /*00003088*/ MOVW        R0, #32784                  @ 0x8010 
    /*0000308c*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003090*/ B           VB_GetPoolId_x306c
    VB_GetPoolId_x3094:
    /*00003094*/ MOVW        R3, #1028                   @ 0x404 
    /*00003098*/ @ MOVW        R2, #0                      @ .LC116 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1030] 
    /*0000309c*/ STR         R3, [SP] 
    /*000030a0*/ ADDRL       R2, =_RODATA_STR1_4_1030    @ .LC116 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1030]
    /*000030a4*/ LDR         R3, WORD_30c0               @ LDR         R3, [PC, #20]               @ 0x00000000000030c0 
    /*000030a8*/ MOV         R1, #1 
    /*000030ac*/ MOV         R0, #4 
    /*000030b0*/ BL          HI_LOG
    /*000030b4*/ MOVW        R0, #32774                  @ 0x8006 
    /*000030b8*/ MOVT        R0, #40961                  @ 0xa001 
    /*000030bc*/ B           VB_GetPoolId_x306c
    /*000030c0*/ WORD_30c0: .word 0x000001a4
FUNC_END VB_GetPoolId


FUNC_BEGIN VB_CreatePool
    /*000030c4*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000030c8*/ SUB         SP, SP, #32 
    /*000030cc*/ LDR         R4, [SP, #64]               @ 0x40 
    /*000030d0*/ CMP         R4, #2 
    /*000030d4*/ BHI         VB_CreatePool_x318c
    /*000030d8*/ MOV         R7, R3 
    /*000030dc*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000030e0*/ LDR         R3, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000030e4*/ LDR         R3, [R3, #8] 
    /*000030e8*/ CMP         R3, #0 
    /*000030ec*/ BEQ         VB_CreatePool_x31bc
    /*000030f0*/ CMP         R0, #0 
    /*000030f4*/ MOV         R5, R0 
    /*000030f8*/ MOV         R8, R1 
    /*000030fc*/ MOV         R6, R2 
    /*00003100*/ BEQ         VB_CreatePool_x316c
    VB_CreatePool_x3104:
    /*00003104*/ LDR         R0, WORD_31c8               @ LDR         R0, [PC, #188]              @ 0x00000000000031c8 
    /*00003108*/ BL          osal_down
    /*0000310c*/ SUBS        R1, R0, #0 
    /*00003110*/ BNE         VB_CreatePool_x3160
    /*00003114*/ LDR         R0, [SP, #60]               @ 0x3c 
    /*00003118*/ MOV         R2, R6 
    /*0000311c*/ LDR         IP, [SP, #56]               @ 0x38 
    /*00003120*/ MOV         R3, R7 
    /*00003124*/ STR         R4, [SP, #24] 
    /*00003128*/ STR         R0, [SP, #20] 
    /*0000312c*/ MOV         R0, R8 
    /*00003130*/ STR         IP, [SP, #12] 
    /*00003134*/ MVN         IP, #1 
    /*00003138*/ STR         R1, [SP, #8] 
    /*0000313c*/ STM         SP, {R1, R5} 
    /*00003140*/ STR         IP, [SP, #16] 
    /*00003144*/ BL          VbCreatePool                @ BL          0x0000000000000E0C 
    /*00003148*/ MOV         R4, R0 
    /*0000314c*/ LDR         R0, WORD_31c8               @ LDR         R0, [PC, #116]              @ 0x00000000000031c8 
    /*00003150*/ BL          osal_up
    VB_CreatePool_x3154:
    /*00003154*/ MOV         R0, R4 
    /*00003158*/ ADD         SP, SP, #32 
    /*0000315c*/ POP         {R4, R5, R6, R7, R8, PC} 
    VB_CreatePool_x3160:
    /*00003160*/ MOV         R4, #65024                  @ 0xfe00 
    /*00003164*/ MOVT        R4, #65535                  @ 0xffff 
    /*00003168*/ B           VB_CreatePool_x3154
    VB_CreatePool_x316c:
    /*0000316c*/ @ MOVW        R3, #0                      @ .LC118 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1098] 
    /*00003170*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00003174*/ ADDRL       R3, =_RODATA_STR1_4_1098    @ .LC118 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1098]
    /*00003178*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*0000317c*/ MOV         R2, #648                    @ 0x288 
    /*00003180*/ LDR         R1, WORD_31cc               @ LDR         R1, [PC, #68]               @ 0x00000000000031cc 
    /*00003184*/ BL          osal_panic
    /*00003188*/ B           VB_CreatePool_x3104
    VB_CreatePool_x318c:
    /*0000318c*/ STR         R4, [SP, #4] 
    /*00003190*/ MOVW        R3, #642                    @ 0x282 
    /*00003194*/ @ MOVW        R2, #0                      @ .LC117 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x105c] 
    /*00003198*/ STR         R3, [SP] 
    /*0000319c*/ ADDRL       R2, =_RODATA_STR1_4_105c    @ .LC117 [R_ARM_MOVT_ABS: .rodata.str1.4+0x105c]
    /*000031a0*/ LDR         R3, WORD_31cc               @ LDR         R3, [PC, #36]               @ 0x00000000000031cc 
    /*000031a4*/ MOV         R1, #1 
    /*000031a8*/ MOV         R0, #3 
    /*000031ac*/ MOVW        R4, #32771                  @ 0x8003 
    /*000031b0*/ BL          HI_LOG
    /*000031b4*/ MOVT        R4, #40961                  @ 0xa001 
    /*000031b8*/ B           VB_CreatePool_x3154
    VB_CreatePool_x31bc:
    /*000031bc*/ MOVW        R4, #32784                  @ 0x8010 
    /*000031c0*/ MOVT        R4, #40961                  @ 0xa001 
    /*000031c4*/ B           VB_CreatePool_x3154
    /*000031c8*/ WORD_31c8: .word 0x0000022c
    /*000031cc*/ WORD_31cc: .word 0x000001b4
FUNC_END VB_CreatePool


FUNC_BEGIN VB_GetBlkBySize
    /*000031d0*/ @ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000031d4*/ LDR         IP, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000031d8*/ PUSH        {R4, R6, R7, LR} 
    /*000031dc*/ MOV         R4, R2 
    /*000031e0*/ LDR         R2, [IP, #8] 
    /*000031e4*/ SUB         SP, SP, #16 
    /*000031e8*/ CMP         R2, #0 
    /*000031ec*/ BEQ         VB_GetBlkBySize_x3294
    /*000031f0*/ CMP         R4, #22 
    /*000031f4*/ MOV         R6, R0 
    /*000031f8*/ MOV         R7, R1 
    /*000031fc*/ BHI         VB_GetBlkBySize_x3274
    /*00003200*/ CMP         R4, #12 
    /*00003204*/ BEQ         VB_GetBlkBySize_x3238
    VB_GetBlkBySize_x3208:
    /*00003208*/ MOV         R0, #0 
    /*0000320c*/ MVN         R1, #0 
    /*00003210*/ MOV         R2, R6 
    /*00003214*/ STM         SP, {R0, R1} 
    /*00003218*/ MOV         R3, R7 
    /*0000321c*/ ADD         R0, SP, #12 
    /*00003220*/ BL          VbSearchCommPoolBySize      @ BL          0x0000000000000BEC 
    /*00003224*/ CMP         R0, #0 
    /*00003228*/ MVNNE       R0, #0 
    /*0000322c*/ BEQ         VB_GetBlkBySize_x3260
    VB_GetBlkBySize_x3230:
    /*00003230*/ ADD         SP, SP, #16 
    /*00003234*/ POP         {R4, R6, R7, PC} 
    VB_GetBlkBySize_x3238:
    /*00003238*/ STR         R3, [SP] 
    /*0000323c*/ MOV         R2, R0 
    /*00003240*/ MOV         R3, R1 
    /*00003244*/ ADD         R0, SP, #12 
    /*00003248*/ MVN         R1, #0 
    /*0000324c*/ STR         R1, [SP, #4] 
    /*00003250*/ BL          VbSearchCommPoolBySize      @ BL          0x0000000000000BEC 
    /*00003254*/ CMP         R0, #0 
    /*00003258*/ MVNNE       R0, #0 
    /*0000325c*/ BNE         VB_GetBlkBySize_x3230
    VB_GetBlkBySize_x3260:
    /*00003260*/ MOV         R1, R4 
    /*00003264*/ LDR         R0, [SP, #12] 
    /*00003268*/ BL          VB_GetBlkByPoolId
    /*0000326c*/ ADD         SP, SP, #16 
    /*00003270*/ POP         {R4, R6, R7, PC} 
    VB_GetBlkBySize_x3274:
    /*00003274*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*00003278*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*0000327c*/ ADDRL       R3, =_RODATA_STR1_4_029c    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c]
    /*00003280*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00003284*/ MOVW        R2, #1080                   @ 0x438 
    /*00003288*/ LDR         R1, WORD_32b4               @ LDR         R1, [PC, #36]               @ 0x00000000000032b4 
    /*0000328c*/ BL          osal_panic
    /*00003290*/ B           VB_GetBlkBySize_x3208
    VB_GetBlkBySize_x3294:
    /*00003294*/ MOV         R1, #1 
    /*00003298*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*0000329c*/ MOV         R0, #4 
    /*000032a0*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000032a4*/ BL          HI_LOG
    /*000032a8*/ MVN         R0, #0 
    /*000032ac*/ ADD         SP, SP, #16 
    /*000032b0*/ POP         {R4, R6, R7, PC} 
    /*000032b4*/ WORD_32b4: .word 0x000001c4
FUNC_END VB_GetBlkBySize


FUNC_BEGIN VB_IsBlkValid
    /*000032b8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000032bc*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000032c0*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000032c4*/ SUB         SP, SP, #40                 @ 0x28 
    /*000032c8*/ LDR         SL, [R4, #8] 
    /*000032cc*/ CMP         SL, #0 
    /*000032d0*/ BEQ         VB_IsBlkValid_x33c0
    /*000032d4*/ MOV         R7, R3 
    /*000032d8*/ LDR         R3, [R4, #16] 
    /*000032dc*/ MOV         R5, R0 
    /*000032e0*/ CMP         R3, R0 
    /*000032e4*/ BLS         VB_IsBlkValid_x3378
    /*000032e8*/ ADD         R0, R4, #548                @ 0x224 
    /*000032ec*/ ADD         R1, SP, #36                 @ 0x24 
    /*000032f0*/ MOV         R6, R2 
    /*000032f4*/ BL          osal_spin_lock_irqsave
    /*000032f8*/ LDR         R3, [R4, #8] 
    /*000032fc*/ LDR         SL, [R3, R5, LSL #2] 
    /*00003300*/ CMP         SL, #0 
    /*00003304*/ BEQ         VB_IsBlkValid_x33e8
    /*00003308*/ LDRD        R8, [SL, #32] 
    /*0000330c*/ LDRD        R2, [SL, #24] 
    /*00003310*/ CMP         R7, R9 
    /*00003314*/ CMPEQ       R6, R8 
    /*00003318*/ BCC         VB_IsBlkValid_x3330
    /*0000331c*/ ADDS        R0, R8, R2 
    /*00003320*/ ADC         R1, R9, R3 
    /*00003324*/ CMP         R7, R1 
    /*00003328*/ CMPEQ       R6, R0 
    /*0000332c*/ BCC         VB_IsBlkValid_x33a8
    VB_IsBlkValid_x3330:
    /*00003330*/ STRD        R2, [SP, #24] 
    /*00003334*/ MOV         R1, #1 
    /*00003338*/ MOVW        R3, #2291                   @ 0x8f3 
    /*0000333c*/ STRD        R8, [SP, #16] 
    /*00003340*/ STR         R3, [SP] 
    /*00003344*/ @ MOVW        R2, #0                      @ .LC121 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x111c] 
    /*00003348*/ LDR         R3, WORD_3420               @ LDR         R3, [PC, #208]              @ 0x0000000000003420 
    /*0000334c*/ ADDRL       R2, =_RODATA_STR1_4_111c    @ .LC121 [R_ARM_MOVT_ABS: .rodata.str1.4+0x111c]
    /*00003350*/ STRD        R6, [SP, #8] 
    /*00003354*/ MOV         R0, #4 
    /*00003358*/ STR         R5, [SP, #4] 
    /*0000335c*/ BL          HI_LOG
    /*00003360*/ LDR         R0, WORD_3424               @ LDR         R0, [PC, #188]              @ 0x0000000000003424 
    /*00003364*/ ADD         R1, SP, #36                 @ 0x24 
    /*00003368*/ BL          osal_spin_unlock_irqrestore
    /*0000336c*/ MOV         R0, #0 
    VB_IsBlkValid_x3370:
    /*00003370*/ ADD         SP, SP, #40                 @ 0x28 
    /*00003374*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    VB_IsBlkValid_x3378:
    /*00003378*/ STR         R0, [SP, #4] 
    /*0000337c*/ MOVW        R3, #2245                   @ 0x8c5 
    /*00003380*/ MOV         R1, #1 
    /*00003384*/ STR         R3, [SP] 
    /*00003388*/ @ MOVW        R2, #0                      @ .LC120 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10e4] 
    /*0000338c*/ LDR         R3, WORD_3420               @ LDR         R3, [PC, #140]              @ 0x0000000000003420 
    /*00003390*/ ADDRL       R2, =_RODATA_STR1_4_10e4    @ .LC120 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10e4]
    /*00003394*/ MOV         R0, #4 
    /*00003398*/ BL          HI_LOG
    /*0000339c*/ MOV         R0, #0 
    /*000033a0*/ ADD         SP, SP, #40                 @ 0x28 
    /*000033a4*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    VB_IsBlkValid_x33a8:
    /*000033a8*/ ADD         R0, R4, #548                @ 0x224 
    /*000033ac*/ ADD         R1, SP, #36                 @ 0x24 
    /*000033b0*/ BL          osal_spin_unlock_irqrestore
    /*000033b4*/ MOV         R0, #1 
    /*000033b8*/ ADD         SP, SP, #40                 @ 0x28 
    /*000033bc*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    VB_IsBlkValid_x33c0:
    /*000033c0*/ MOVW        R3, #2239                   @ 0x8bf 
    /*000033c4*/ @ MOVW        R2, #0                      @ .LC119 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10b0] 
    /*000033c8*/ STR         R3, [SP] 
    /*000033cc*/ ADDRL       R2, =_RODATA_STR1_4_10b0    @ .LC119 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10b0]
    /*000033d0*/ LDR         R3, WORD_3420               @ LDR         R3, [PC, #72]               @ 0x0000000000003420 
    /*000033d4*/ MOV         R1, #1 
    /*000033d8*/ MOV         R0, #4 
    /*000033dc*/ BL          HI_LOG
    /*000033e0*/ MOV         R0, SL 
    /*000033e4*/ B           VB_IsBlkValid_x3370
    VB_IsBlkValid_x33e8:
    /*000033e8*/ MOVW        R3, #2254                   @ 0x8ce 
    /*000033ec*/ MOV         R1, #1 
    /*000033f0*/ STR         R3, [SP] 
    /*000033f4*/ @ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x184] 
    /*000033f8*/ LDR         R3, WORD_3420               @ LDR         R3, [PC, #32]               @ 0x0000000000003420 
    /*000033fc*/ ADDRL       R2, =_RODATA_STR1_4_0184    @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x184]
    /*00003400*/ STR         R5, [SP, #4] 
    /*00003404*/ MOV         R0, #4 
    /*00003408*/ BL          HI_LOG
    /*0000340c*/ ADD         R0, R4, #548                @ 0x224 
    /*00003410*/ ADD         R1, SP, #36                 @ 0x24 
    /*00003414*/ BL          osal_spin_unlock_irqrestore
    /*00003418*/ MOV         R0, SL 
    /*0000341c*/ B           VB_IsBlkValid_x3370
    /*00003420*/ WORD_3420: .word 0x000001d4
    /*00003424*/ WORD_3424: .word 0x00000224
FUNC_END VB_IsBlkValid


FUNC_BEGIN VB_IsPoolIdValid
    /*00003428*/ PUSH        {R4, R5, R6, LR} 
    /*0000342c*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003430*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00003434*/ SUB         SP, SP, #16 
    /*00003438*/ LDR         R5, [R4, #8] 
    /*0000343c*/ CMP         R5, #0 
    /*00003440*/ BEQ         VB_IsPoolIdValid_x34f0
    /*00003444*/ LDR         R3, [R4, #16] 
    /*00003448*/ MOV         R6, R0 
    /*0000344c*/ CMP         R3, R0 
    /*00003450*/ BLS         VB_IsPoolIdValid_x3488
    /*00003454*/ ADD         R0, R4, #548                @ 0x224 
    /*00003458*/ ADD         R1, SP, #12 
    /*0000345c*/ BL          osal_spin_lock_irqsave
    /*00003460*/ LDR         R3, [R4, #8] 
    /*00003464*/ LDR         R5, [R3, R6, LSL #2] 
    /*00003468*/ CMP         R5, #0 
    /*0000346c*/ BEQ         VB_IsPoolIdValid_x34b8
    /*00003470*/ ADD         R0, R4, #548                @ 0x224 
    /*00003474*/ ADD         R1, SP, #12 
    /*00003478*/ BL          osal_spin_unlock_irqrestore
    /*0000347c*/ MOV         R0, #1 
    VB_IsPoolIdValid_x3480:
    /*00003480*/ ADD         SP, SP, #16 
    /*00003484*/ POP         {R4, R5, R6, PC} 
    VB_IsPoolIdValid_x3488:
    /*00003488*/ STR         R0, [SP, #4] 
    /*0000348c*/ MOVW        R3, #2328                   @ 0x918 
    /*00003490*/ MOV         R1, #1 
    /*00003494*/ STR         R3, [SP] 
    /*00003498*/ @ MOVW        R2, #0                      @ .LC120 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10e4] 
    /*0000349c*/ LDR         R3, WORD_3518               @ LDR         R3, [PC, #116]              @ 0x0000000000003518 
    /*000034a0*/ ADDRL       R2, =_RODATA_STR1_4_10e4    @ .LC120 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10e4]
    /*000034a4*/ MOV         R0, #4 
    /*000034a8*/ BL          HI_LOG
    /*000034ac*/ MOV         R0, #0 
    /*000034b0*/ ADD         SP, SP, #16 
    /*000034b4*/ POP         {R4, R5, R6, PC} 
    VB_IsPoolIdValid_x34b8:
    /*000034b8*/ MOVW        R3, #2337                   @ 0x921 
    /*000034bc*/ MOV         R1, #1 
    /*000034c0*/ STR         R3, [SP] 
    /*000034c4*/ @ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x184] 
    /*000034c8*/ LDR         R3, WORD_3518               @ LDR         R3, [PC, #72]               @ 0x0000000000003518 
    /*000034cc*/ ADDRL       R2, =_RODATA_STR1_4_0184    @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x184]
    /*000034d0*/ STR         R6, [SP, #4] 
    /*000034d4*/ MOV         R0, #4 
    /*000034d8*/ BL          HI_LOG
    /*000034dc*/ ADD         R0, R4, #548                @ 0x224 
    /*000034e0*/ ADD         R1, SP, #12 
    /*000034e4*/ BL          osal_spin_unlock_irqrestore
    /*000034e8*/ MOV         R0, R5 
    /*000034ec*/ B           VB_IsPoolIdValid_x3480
    VB_IsPoolIdValid_x34f0:
    /*000034f0*/ MOVW        R3, #2321                   @ 0x911 
    /*000034f4*/ @ MOVW        R2, #0                      @ .LC119 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10b0] 
    /*000034f8*/ STR         R3, [SP] 
    /*000034fc*/ ADDRL       R2, =_RODATA_STR1_4_10b0    @ .LC119 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10b0]
    /*00003500*/ LDR         R3, WORD_3518               @ LDR         R3, [PC, #16]               @ 0x0000000000003518 
    /*00003504*/ MOV         R1, #1 
    /*00003508*/ MOV         R0, #4 
    /*0000350c*/ BL          HI_LOG
    /*00003510*/ MOV         R0, R5 
    /*00003514*/ B           VB_IsPoolIdValid_x3480
    /*00003518*/ WORD_3518: .word 0x000001e4
FUNC_END VB_IsPoolIdValid


FUNC_BEGIN VB_GetBlkBySizeAndPoolId
    /*0000351c*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00003520*/ SUB         SP, SP, #36                 @ 0x24 
    /*00003524*/ MOV         R5, R0 
    /*00003528*/ MOV         R8, R2 
    /*0000352c*/ LDR         R6, [SP, #64]               @ 0x40 
    /*00003530*/ MOV         R9, R3 
    /*00003534*/ CMP         R6, #22 
    /*00003538*/ BHI         VB_GetBlkBySizeAndPoolId_x3698
    VB_GetBlkBySizeAndPoolId_x353c:
    /*0000353c*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003540*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00003544*/ LDR         R3, [R4, #8] 
    /*00003548*/ CMP         R3, #0 
    /*0000354c*/ BEQ         VB_GetBlkBySizeAndPoolId_x3730
    VB_GetBlkBySizeAndPoolId_x3550:
    /*00003550*/ LDR         R3, [R4, #16] 
    /*00003554*/ CMP         R3, R5 
    /*00003558*/ BHI         VB_GetBlkBySizeAndPoolId_x358c
    /*0000355c*/ MOVW        R3, #2418                   @ 0x972 
    /*00003560*/ STR         R5, [SP, #4] 
    /*00003564*/ STR         R3, [SP] 
    /*00003568*/ @ MOVW        R2, #0                      @ .LC122 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1178] 
    /*0000356c*/ LDR         R3, WORD_37c8               @ LDR         R3, [PC, #596]              @ 0x00000000000037c8 
    /*00003570*/ ADDRL       R2, =_RODATA_STR1_4_1178    @ .LC122 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1178]
    /*00003574*/ MOV         R1, #1 
    /*00003578*/ MOV         R0, #3 
    /*0000357c*/ BL          HI_LOG
    /*00003580*/ MVN         R0, #0 
    VB_GetBlkBySizeAndPoolId_x3584:
    /*00003584*/ ADD         SP, SP, #36                 @ 0x24 
    /*00003588*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    VB_GetBlkBySizeAndPoolId_x358c:
    /*0000358c*/ LDR         R0, WORD_37cc               @ LDR         R0, [PC, #568]              @ 0x00000000000037cc 
    /*00003590*/ ADD         R1, SP, #28 
    /*00003594*/ BL          osal_spin_lock_irqsave
    /*00003598*/ LDR         R3, [R4, #8] 
    /*0000359c*/ LDR         R4, [R3, R5, LSL #2] 
    /*000035a0*/ CMP         R4, #0 
    /*000035a4*/ BEQ         VB_GetBlkBySizeAndPoolId_x3750
    /*000035a8*/ LDR         R3, [R4, #168]              @ 0xa8 
    /*000035ac*/ CMP         R3, #18 
    /*000035b0*/ BEQ         VB_GetBlkBySizeAndPoolId_x36f4
    /*000035b4*/ LDRD        R2, [R4, #16] 
    /*000035b8*/ CMP         R9, R3 
    /*000035bc*/ CMPEQ       R8, R2 
    /*000035c0*/ BHI         VB_GetBlkBySizeAndPoolId_x36b8
    /*000035c4*/ MOV         R3, R4 
    /*000035c8*/ LDR         R7, [R3, #44]!              @ 0x2c 
    /*000035cc*/ CMP         R7, R3 
    /*000035d0*/ BEQ         VB_GetBlkBySizeAndPoolId_x3790
    /*000035d4*/ LDR         R3, [R4, #68]               @ 0x44 
    /*000035d8*/ CMP         R3, #0 
    /*000035dc*/ BEQ         VB_GetBlkBySizeAndPoolId_x3768
    VB_GetBlkBySizeAndPoolId_x35e0:
    /*000035e0*/ LDR         R2, [R4, #72]               @ 0x48 
    /*000035e4*/ SUB         R3, R3, #1 
    /*000035e8*/ STR         R3, [R4, #68]               @ 0x44 
    /*000035ec*/ ADD         R6, R7, R6, LSL #2 
    /*000035f0*/ CMP         R3, R2 
    /*000035f4*/ STRCC       R3, [R4, #72]               @ 0x48 
    /*000035f8*/ LDR         R1, [R7, #4] 
    /*000035fc*/ MOV         R2, #256                    @ 0x100 
    /*00003600*/ LDR         R0, [R7] 
    /*00003604*/ MOVT        R2, #16 
    /*00003608*/ MOV         R3, #512                    @ 0x200 
    /*0000360c*/ MOVT        R3, #32 
    /*00003610*/ MOVW        IP, #2479                   @ 0x9af 
    /*00003614*/ STR         R1, [R0, #4] 
    /*00003618*/ STR         R0, [R1] 
    /*0000361c*/ ADD         R1, R4, #52                 @ 0x34 
    /*00003620*/ STR         R2, [R7] 
    /*00003624*/ MOV         R0, #7 
    /*00003628*/ STR         R3, [R7, #4] 
    /*0000362c*/ @ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x804] 
    /*00003630*/ LDR         R3, [R4, #56]               @ 0x38 
    /*00003634*/ ADDRL       R2, =_RODATA_STR1_4_0804    @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x804]
    /*00003638*/ STR         R7, [R4, #56]               @ 0x38 
    /*0000363c*/ STR         R1, [R7] 
    /*00003640*/ MOV         R1, #1 
    /*00003644*/ STR         R3, [R7, #4] 
    /*00003648*/ STR         R7, [R3] 
    /*0000364c*/ LDR         R3, [R6, #32] 
    /*00003650*/ ADD         R3, R3, R1 
    /*00003654*/ STR         R3, [R6, #32] 
    /*00003658*/ LDR         R3, [R7, #28] 
    /*0000365c*/ ADD         R3, R3, R1 
    /*00003660*/ STR         R3, [R7, #28] 
    /*00003664*/ STR         R5, [SP, #8] 
    /*00003668*/ LDR         LR, [R7, #8] 
    /*0000366c*/ LDR         R3, WORD_37c8               @ LDR         R3, [PC, #340]              @ 0x00000000000037c8 
    /*00003670*/ STR         IP, [SP] 
    /*00003674*/ STR         LR, [SP, #4] 
    /*00003678*/ BL          HI_LOG
    /*0000367c*/ ADD         R1, SP, #28 
    /*00003680*/ LDR         R0, WORD_37cc               @ LDR         R0, [PC, #324]              @ 0x00000000000037cc 
    /*00003684*/ BL          osal_spin_unlock_irqrestore
    /*00003688*/ LDRH        R0, [R7, #8] 
    /*0000368c*/ ORR         R0, R0, R5, LSL #16 
    /*00003690*/ ADD         SP, SP, #36                 @ 0x24 
    /*00003694*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    VB_GetBlkBySizeAndPoolId_x3698:
    /*00003698*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*0000369c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000036a0*/ ADDRL       R3, =_RODATA_STR1_4_029c    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c]
    /*000036a4*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000036a8*/ MOVW        R2, #2414                   @ 0x96e 
    /*000036ac*/ LDR         R1, WORD_37c8               @ LDR         R1, [PC, #276]              @ 0x00000000000037c8 
    /*000036b0*/ BL          osal_panic
    /*000036b4*/ B           VB_GetBlkBySizeAndPoolId_x353c
    VB_GetBlkBySizeAndPoolId_x36b8:
    /*000036b8*/ STRD        R2, [SP, #16] 
    /*000036bc*/ MOV         R1, #1 
    /*000036c0*/ MOVW        R3, #2440                   @ 0x988 
    /*000036c4*/ STRD        R8, [SP, #8] 
    /*000036c8*/ STR         R3, [SP] 
    /*000036cc*/ @ MOVW        R2, #0                      @ .LC123 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11ac] 
    /*000036d0*/ LDR         R3, WORD_37c8               @ LDR         R3, [PC, #240]              @ 0x00000000000037c8 
    /*000036d4*/ ADDRL       R2, =_RODATA_STR1_4_11ac    @ .LC123 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11ac]
    /*000036d8*/ MOV         R0, #3 
    /*000036dc*/ BL          HI_LOG
    /*000036e0*/ LDR         R0, WORD_37cc               @ LDR         R0, [PC, #228]              @ 0x00000000000037cc 
    /*000036e4*/ ADD         R1, SP, #28 
    /*000036e8*/ BL          osal_spin_unlock_irqrestore
    /*000036ec*/ MVN         R0, #0 
    /*000036f0*/ B           VB_GetBlkBySizeAndPoolId_x3584
    VB_GetBlkBySizeAndPoolId_x36f4:
    /*000036f4*/ @ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x780] 
    /*000036f8*/ ADDRL       R2, =_RODATA_STR1_4_0780    @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x780]
    /*000036fc*/ MOVW        R3, #2433                   @ 0x981 
    /*00003700*/ STR         R5, [SP, #4] 
    /*00003704*/ STR         R3, [SP] 
    VB_GetBlkBySizeAndPoolId_x3708:
    /*00003708*/ LDR         R3, WORD_37c8               @ LDR         R3, [PC, #184]              @ 0x00000000000037c8 
    /*0000370c*/ MOV         R1, #1 
    /*00003710*/ MOV         R0, #3 
    /*00003714*/ BL          HI_LOG
    /*00003718*/ ADD         R1, SP, #28 
    /*0000371c*/ LDR         R0, WORD_37cc               @ LDR         R0, [PC, #168]              @ 0x00000000000037cc 
    /*00003720*/ BL          osal_spin_unlock_irqrestore
    /*00003724*/ MVN         R0, #0 
    /*00003728*/ ADD         SP, SP, #36                 @ 0x24 
    /*0000372c*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    VB_GetBlkBySizeAndPoolId_x3730:
    /*00003730*/ @ MOVW        R3, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b4] 
    /*00003734*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00003738*/ ADDRL       R3, =_RODATA_STR1_4_02b4    @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b4]
    /*0000373c*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00003740*/ MOVW        R2, #2415                   @ 0x96f 
    /*00003744*/ LDR         R1, WORD_37c8               @ LDR         R1, [PC, #124]              @ 0x00000000000037c8 
    /*00003748*/ BL          osal_panic
    /*0000374c*/ B           VB_GetBlkBySizeAndPoolId_x3550
    VB_GetBlkBySizeAndPoolId_x3750:
    /*00003750*/ MOVW        R3, #2427                   @ 0x97b 
    /*00003754*/ @ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c] 
    /*00003758*/ STR         R5, [SP, #4] 
    /*0000375c*/ ADDRL       R2, =_RODATA_STR1_4_007c    @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c]
    /*00003760*/ STR         R3, [SP] 
    /*00003764*/ B           VB_GetBlkBySizeAndPoolId_x3708
    VB_GetBlkBySizeAndPoolId_x3768:
    /*00003768*/ @ MOVW        R3, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7e8] 
    /*0000376c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00003770*/ ADDRL       R3, =_RODATA_STR1_4_07e8    @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7e8]
    /*00003774*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00003778*/ MOVW        R2, #2460                   @ 0x99c 
    /*0000377c*/ LDR         R1, WORD_37c8               @ LDR         R1, [PC, #68]               @ 0x00000000000037c8 
    /*00003780*/ BL          osal_panic
    /*00003784*/ LDR         R3, [R4, #68]               @ 0x44 
    /*00003788*/ LDR         R7, [R4, #44]               @ 0x2c 
    /*0000378c*/ B           VB_GetBlkBySizeAndPoolId_x35e0
    VB_GetBlkBySizeAndPoolId_x3790:
    /*00003790*/ ADD         R1, SP, #28 
    /*00003794*/ LDR         R0, WORD_37cc               @ LDR         R0, [PC, #48]               @ 0x00000000000037cc 
    /*00003798*/ BL          osal_spin_unlock_irqrestore
    /*0000379c*/ MOVW        R3, #2457                   @ 0x999 
    /*000037a0*/ STR         R5, [SP, #4] 
    /*000037a4*/ @ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b0] 
    /*000037a8*/ STR         R3, [SP] 
    /*000037ac*/ ADDRL       R2, =_RODATA_STR1_4_07b0    @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b0]
    /*000037b0*/ LDR         R3, WORD_37c8               @ LDR         R3, [PC, #16]               @ 0x00000000000037c8 
    /*000037b4*/ MOV         R1, #1 
    /*000037b8*/ MOV         R0, #4 
    /*000037bc*/ BL          HI_LOG
    /*000037c0*/ MVN         R0, #0 
    /*000037c4*/ B           VB_GetBlkBySizeAndPoolId_x3584
    /*000037c8*/ WORD_37c8: .word 0x000001f8
    /*000037cc*/ WORD_37cc: .word 0x00000224
FUNC_END VB_GetBlkBySizeAndPoolId


FUNC_BEGIN VB_DestroyPool
    /*000037d0*/ PUSH        {R4, R5, R6, LR} 
    /*000037d4*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000037d8*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000037dc*/ LDR         R3, [R4, #8] 
    /*000037e0*/ CMP         R3, #0 
    /*000037e4*/ BEQ         VB_DestroyPool_x3880
    /*000037e8*/ LDR         R2, [R4, #16] 
    /*000037ec*/ MOV         R5, R0 
    /*000037f0*/ CMP         R2, R0 
    /*000037f4*/ BLS         VB_DestroyPool_x3830
    VB_DestroyPool_x37f8:
    /*000037f8*/ LDR         R4, [R3, R5, LSL #2] 
    /*000037fc*/ CMP         R4, #0 
    /*00003800*/ BEQ         VB_DestroyPool_x3860
    VB_DestroyPool_x3804:
    /*00003804*/ LDR         R0, WORD_389c               @ LDR         R0, [PC, #144]              @ 0x000000000000389c 
    /*00003808*/ BL          osal_down
    /*0000380c*/ CMP         R0, #0 
    /*00003810*/ BNE         VB_DestroyPool_x3854
    /*00003814*/ MOV         R0, R4 
    /*00003818*/ BL          VbDestroyPool               @ BL          0x00000000000019F4 
    /*0000381c*/ MOV         R4, R0 
    /*00003820*/ LDR         R0, WORD_389c               @ LDR         R0, [PC, #116]              @ 0x000000000000389c 
    /*00003824*/ BL          osal_up
    VB_DestroyPool_x3828:
    /*00003828*/ MOV         R0, R4 
    /*0000382c*/ POP         {R4, R5, R6, PC} 
    VB_DestroyPool_x3830:
    /*00003830*/ @ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x758] 
    /*00003834*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00003838*/ ADDRL       R3, =_RODATA_STR1_4_0758    @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x758]
    /*0000383c*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00003840*/ MOVW        R2, #790                    @ 0x316 
    /*00003844*/ LDR         R1, WORD_38a0               @ LDR         R1, [PC, #84]               @ 0x00000000000038a0 
    /*00003848*/ BL          osal_panic
    /*0000384c*/ LDR         R3, [R4, #8] 
    /*00003850*/ B           VB_DestroyPool_x37f8
    VB_DestroyPool_x3854:
    /*00003854*/ MOV         R4, #65024                  @ 0xfe00 
    /*00003858*/ MOVT        R4, #65535                  @ 0xffff 
    /*0000385c*/ B           VB_DestroyPool_x3828
    VB_DestroyPool_x3860:
    /*00003860*/ @ MOVW        R3, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x844] 
    /*00003864*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00003868*/ ADDRL       R3, =_RODATA_STR1_4_0844    @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x844]
    /*0000386c*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00003870*/ MOV         R2, #792                    @ 0x318 
    /*00003874*/ LDR         R1, WORD_38a0               @ LDR         R1, [PC, #36]               @ 0x00000000000038a0 
    /*00003878*/ BL          osal_panic
    /*0000387c*/ B           VB_DestroyPool_x3804
    VB_DestroyPool_x3880:
    /*00003880*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00003884*/ MOV         R1, #1 
    /*00003888*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*0000388c*/ MOV         R0, #4 
    /*00003890*/ BL          HI_LOG
    /*00003894*/ MVN         R4, #0 
    /*00003898*/ B           VB_DestroyPool_x3828
    /*0000389c*/ WORD_389c: .word 0x0000022c
    /*000038a0*/ WORD_38a0: .word 0x00000214
FUNC_END VB_DestroyPool


FUNC_BEGIN Open
    /*000038a4*/ MOV         R0, #0 
    /*000038a8*/ BX          LR 
FUNC_END Open


FUNC_BEGIN VB_Handle2PoolId
    /*000038ac*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000038b0*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000038b4*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000038b8*/ LDR         R3, [R4, #8] 
    /*000038bc*/ CMP         R3, #0 
    /*000038c0*/ BEQ         VB_Handle2PoolId_x3928
    /*000038c4*/ LSR         R6, R0, #16 
    /*000038c8*/ MOV         R5, R0 
    /*000038cc*/ LDR         R3, [R3, R6, LSL #2] 
    /*000038d0*/ LSL         R7, R6, #2 
    /*000038d4*/ CMP         R3, #0 
    /*000038d8*/ BEQ         VB_Handle2PoolId_x3948
    VB_Handle2PoolId_x38dc:
    /*000038dc*/ LDR         R3, [R4, #536]              @ 0x218 
    /*000038e0*/ LDR         R3, [R3, R7] 
    /*000038e4*/ CMP         R3, #0 
    /*000038e8*/ BEQ         VB_Handle2PoolId_x3968
    VB_Handle2PoolId_x38ec:
    /*000038ec*/ LDR         R3, [R4, #8] 
    /*000038f0*/ UXTH        R5, R5 
    /*000038f4*/ LDR         R3, [R3, R7] 
    /*000038f8*/ LDR         R3, [R3, #12] 
    /*000038fc*/ CMP         R5, R3 
    /*00003900*/ BCC         VB_Handle2PoolId_x3920
    /*00003904*/ @ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258] 
    /*00003908*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*0000390c*/ ADDRL       R3, =_RODATA_STR1_4_0258    @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258]
    /*00003910*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00003914*/ MOVW        R2, #1339                   @ 0x53b 
    /*00003918*/ LDR         R1, WORD_3988               @ LDR         R1, [PC, #104]              @ 0x0000000000003988 
    /*0000391c*/ BL          osal_panic
    VB_Handle2PoolId_x3920:
    /*00003920*/ MOV         R0, R6 
    /*00003924*/ POP         {R4, R5, R6, R7, R8, PC} 
    VB_Handle2PoolId_x3928:
    /*00003928*/ MOV         R1, #1 
    /*0000392c*/ MVN         R6, #0 
    /*00003930*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00003934*/ MOV         R0, #4 
    /*00003938*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*0000393c*/ BL          HI_LOG
    /*00003940*/ MOV         R0, R6 
    /*00003944*/ POP         {R4, R5, R6, R7, R8, PC} 
    VB_Handle2PoolId_x3948:
    /*00003948*/ @ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c0] 
    /*0000394c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00003950*/ ADDRL       R3, =_RODATA_STR1_4_01c0    @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c0]
    /*00003954*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00003958*/ MOVW        R2, #1339                   @ 0x53b 
    /*0000395c*/ LDR         R1, WORD_3988               @ LDR         R1, [PC, #36]               @ 0x0000000000003988 
    /*00003960*/ BL          osal_panic
    /*00003964*/ B           VB_Handle2PoolId_x38dc
    VB_Handle2PoolId_x3968:
    /*00003968*/ @ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22c] 
    /*0000396c*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*00003970*/ ADDRL       R3, =_RODATA_STR1_4_022c    @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22c]
    /*00003974*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*00003978*/ MOVW        R2, #1339                   @ 0x53b 
    /*0000397c*/ LDR         R1, WORD_3988               @ LDR         R1, [PC, #4]                @ 0x0000000000003988 
    /*00003980*/ BL          osal_panic
    /*00003984*/ B           VB_Handle2PoolId_x38ec
    /*00003988*/ WORD_3988: .word 0x00000224
FUNC_END VB_Handle2PoolId


FUNC_BEGIN VB_InquirePoolBusyBlkCnt
    /*0000398c*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003990*/ LDR         R3, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00003994*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00003998*/ SUB         SP, SP, #20 
    /*0000399c*/ LDR         R2, [R3, #8] 
    /*000039a0*/ CMP         R2, #0 
    /*000039a4*/ BEQ         VB_InquirePoolBusyBlkCnt_x3a3c
    /*000039a8*/ LDR         R3, [R3, #16] 
    /*000039ac*/ CMP         R3, R0 
    /*000039b0*/ BLS         VB_InquirePoolBusyBlkCnt_x3a08
    /*000039b4*/ LDR         R3, [R2, R0, LSL #2] 
    /*000039b8*/ CMP         R3, #0 
    /*000039bc*/ BEQ         VB_InquirePoolBusyBlkCnt_x3a5c
    /*000039c0*/ LDR         R0, [R3, #68]               @ 0x44 
    /*000039c4*/ LDR         R2, [R3, #12] 
    /*000039c8*/ SUB         R2, R2, R0 
    /*000039cc*/ STR         R2, [R1] 
    /*000039d0*/ LDR         R3, [R3, #12] 
    /*000039d4*/ CMP         R2, R3 
    /*000039d8*/ MOVLS       R0, #0 
    /*000039dc*/ BLS         VB_InquirePoolBusyBlkCnt_x3a00
    /*000039e0*/ @ MOVW        R3, #0                      @ .LC124 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1200] 
    /*000039e4*/ @ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec] 
    /*000039e8*/ ADDRL       R3, =_RODATA_STR1_4_1200    @ .LC124 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1200]
    /*000039ec*/ ADDRL       R0, =_RODATA_STR1_4_01ec    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec]
    /*000039f0*/ MOVW        R2, #2211                   @ 0x8a3 
    /*000039f4*/ LDR         R1, WORD_3a8c               @ LDR         R1, [PC, #144]              @ 0x0000000000003a8c 
    /*000039f8*/ BL          osal_panic
    /*000039fc*/ MOV         R0, #0 
    VB_InquirePoolBusyBlkCnt_x3a00:
    /*00003a00*/ ADD         SP, SP, #20 
    /*00003a04*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VB_InquirePoolBusyBlkCnt_x3a08:
    /*00003a08*/ STR         R3, [SP, #8] 
    /*00003a0c*/ @ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00003a10*/ MOVW        R3, #2199                   @ 0x897 
    /*00003a14*/ STR         R0, [SP, #4] 
    /*00003a18*/ STR         R3, [SP] 
    /*00003a1c*/ ADDRL       R2, =_RODATA_STR1_4_0138    @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138]
    /*00003a20*/ LDR         R3, WORD_3a8c               @ LDR         R3, [PC, #100]              @ 0x0000000000003a8c 
    /*00003a24*/ MOV         R1, #1 
    /*00003a28*/ MOV         R0, #4 
    /*00003a2c*/ BL          HI_LOG
    /*00003a30*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003a34*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003a38*/ B           VB_InquirePoolBusyBlkCnt_x3a00
    VB_InquirePoolBusyBlkCnt_x3a3c:
    /*00003a3c*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00003a40*/ MOV         R1, #1 
    /*00003a44*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00003a48*/ MOV         R0, #4 
    /*00003a4c*/ BL          HI_LOG
    /*00003a50*/ MOVW        R0, #32784                  @ 0x8010 
    /*00003a54*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003a58*/ B           VB_InquirePoolBusyBlkCnt_x3a00
    VB_InquirePoolBusyBlkCnt_x3a5c:
    /*00003a5c*/ STR         R0, [SP, #4] 
    /*00003a60*/ MOVW        R3, #2206                   @ 0x89e 
    /*00003a64*/ @ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x184] 
    /*00003a68*/ STR         R3, [SP] 
    /*00003a6c*/ ADDRL       R2, =_RODATA_STR1_4_0184    @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x184]
    /*00003a70*/ LDR         R3, WORD_3a8c               @ LDR         R3, [PC, #20]               @ 0x0000000000003a8c 
    /*00003a74*/ MOV         R1, #1 
    /*00003a78*/ MOV         R0, #4 
    /*00003a7c*/ BL          HI_LOG
    /*00003a80*/ MOVW        R0, #32773                  @ 0x8005 
    /*00003a84*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003a88*/ B           VB_InquirePoolBusyBlkCnt_x3a00
    /*00003a8c*/ WORD_3a8c: .word 0x00000238
FUNC_END VB_InquirePoolBusyBlkCnt


FUNC_BEGIN VB_GetConfig
    /*00003a90*/ PUSH        {R4, R5, LR} 
    /*00003a94*/ SUBS        R5, R0, #0 
    /*00003a98*/ SUB         SP, SP, #12 
    /*00003a9c*/ BEQ         VB_GetConfig_x3b14
    /*00003aa0*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003aa4*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00003aa8*/ ADD         R0, R4, #556                @ 0x22c 
    /*00003aac*/ BL          osal_down
    /*00003ab0*/ LDR         R3, [R4, #652]              @ 0x28c 
    /*00003ab4*/ CMP         R3, #0 
    /*00003ab8*/ BEQ         VB_GetConfig_x3ae0
    /*00003abc*/ MOV         R2, #520                    @ 0x208 
    /*00003ac0*/ ADD         R1, R4, #16 
    /*00003ac4*/ MOV         R0, R5 
    /*00003ac8*/ BL          osal_memcpy
    /*00003acc*/ ADD         R0, R4, #556                @ 0x22c 
    /*00003ad0*/ BL          osal_up
    /*00003ad4*/ MOV         R0, #0 
    VB_GetConfig_x3ad8:
    /*00003ad8*/ ADD         SP, SP, #12 
    /*00003adc*/ POP         {R4, R5, PC} 
    VB_GetConfig_x3ae0:
    /*00003ae0*/ MOV         R3, #2496                   @ 0x9c0 
    /*00003ae4*/ MOV         R1, #1 
    /*00003ae8*/ STR         R3, [SP] 
    /*00003aec*/ @ MOVW        R2, #0                      @ .LC125 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1220] 
    /*00003af0*/ LDR         R3, WORD_3b20               @ LDR         R3, [PC, #40]               @ 0x0000000000003b20 
    /*00003af4*/ ADDRL       R2, =_RODATA_STR1_4_1220    @ .LC125 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1220]
    /*00003af8*/ MOV         R0, #3 
    /*00003afc*/ BL          HI_LOG
    /*00003b00*/ ADD         R0, R4, #556                @ 0x22c 
    /*00003b04*/ BL          osal_up
    /*00003b08*/ MOVW        R0, #32784                  @ 0x8010 
    /*00003b0c*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003b10*/ B           VB_GetConfig_x3ad8
    VB_GetConfig_x3b14:
    /*00003b14*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003b18*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003b1c*/ B           VB_GetConfig_x3ad8
    /*00003b20*/ WORD_3b20: .word 0x00000254
FUNC_END VB_GetConfig


FUNC_BEGIN VB_SetConfig
    /*00003b24*/ PUSH        {R4, R5, R6, LR} 
    /*00003b28*/ SUBS        R6, R0, #0 
    /*00003b2c*/ SUB         SP, SP, #8 
    /*00003b30*/ BEQ         VB_SetConfig_x3c68
    /*00003b34*/ ADD         R4, R6, #24 
    /*00003b38*/ ADD         R5, R6, #536                @ 0x218 
    /*00003b3c*/ B           VB_SetConfig_x3b4c
    VB_SetConfig_x3b40:
    /*00003b40*/ ADD         R4, R4, #32 
    /*00003b44*/ CMP         R4, R5 
    /*00003b48*/ BEQ         VB_SetConfig_x3bb4
    VB_SetConfig_x3b4c:
    /*00003b4c*/ LDR         R3, [R4, #-8] 
    /*00003b50*/ CMP         R3, #0 
    /*00003b54*/ BEQ         VB_SetConfig_x3b40
    /*00003b58*/ LDRD        R2, [R4, #-16] 
    /*00003b5c*/ ORRS        R3, R2, R3 
    /*00003b60*/ BEQ         VB_SetConfig_x3b40
    /*00003b64*/ MOV         R1, #16 
    /*00003b68*/ MOV         R0, R4 
    /*00003b6c*/ BL          osal_strnlen
    /*00003b70*/ CMP         R0, #15 
    /*00003b74*/ BGT         VB_SetConfig_x3c3c
    /*00003b78*/ LDR         R3, [R4, #-4] 
    /*00003b7c*/ CMP         R3, #2 
    /*00003b80*/ BLS         VB_SetConfig_x3b40
    /*00003b84*/ STR         R3, [SP, #4] 
    /*00003b88*/ @ MOVW        R2, #0                      @ .LC127 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1284] 
    /*00003b8c*/ MOVW        R3, #2533                   @ 0x9e5 
    /*00003b90*/ ADDRL       R2, =_RODATA_STR1_4_1284    @ .LC127 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1284]
    /*00003b94*/ STR         R3, [SP] 
    /*00003b98*/ MOV         R1, #1 
    /*00003b9c*/ LDR         R3, WORD_3c74               @ LDR         R3, [PC, #208]              @ 0x0000000000003c74 
    /*00003ba0*/ MOV         R0, #3 
    /*00003ba4*/ BL          HI_LOG
    /*00003ba8*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003bac*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003bb0*/ B           VB_SetConfig_x3bfc
    VB_SetConfig_x3bb4:
    /*00003bb4*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003bb8*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00003bbc*/ ADD         R0, R4, #556                @ 0x22c 
    /*00003bc0*/ BL          osal_down
    /*00003bc4*/ LDR         R5, [R4, #8] 
    /*00003bc8*/ CMP         R5, #0 
    /*00003bcc*/ BNE         VB_SetConfig_x3c04
    /*00003bd0*/ MOV         R1, R6 
    /*00003bd4*/ MOV         R2, #520                    @ 0x208 
    /*00003bd8*/ ADD         R0, R4, #16 
    /*00003bdc*/ BL          osal_memcpy
    /*00003be0*/ ADD         R0, R4, #556                @ 0x22c 
    /*00003be4*/ MOV         R2, #512                    @ 0x200 
    /*00003be8*/ MOV         R3, #1 
    /*00003bec*/ STR         R2, [R4, #16] 
    /*00003bf0*/ STR         R3, [R4, #652]              @ 0x28c 
    /*00003bf4*/ BL          osal_up
    /*00003bf8*/ MOV         R0, R5 
    VB_SetConfig_x3bfc:
    /*00003bfc*/ ADD         SP, SP, #8 
    /*00003c00*/ POP         {R4, R5, R6, PC} 
    VB_SetConfig_x3c04:
    /*00003c04*/ MOV         R1, #1 
    /*00003c08*/ MOVW        R3, #2542                   @ 0x9ee 
    /*00003c0c*/ @ MOVW        R2, #0                      @ .LC128 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12bc] 
    /*00003c10*/ STR         R3, [SP] 
    /*00003c14*/ ADDRL       R2, =_RODATA_STR1_4_12bc    @ .LC128 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12bc]
    /*00003c18*/ LDR         R3, WORD_3c74               @ LDR         R3, [PC, #84]               @ 0x0000000000003c74 
    /*00003c1c*/ MOV         R0, #3 
    /*00003c20*/ BL          HI_LOG
    /*00003c24*/ ADD         R0, R4, #556                @ 0x22c 
    /*00003c28*/ BL          osal_up
    /*00003c2c*/ MOVW        R0, #32786                  @ 0x8012 
    /*00003c30*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003c34*/ ADD         SP, SP, #8 
    /*00003c38*/ POP         {R4, R5, R6, PC} 
    VB_SetConfig_x3c3c:
    /*00003c3c*/ MOVW        R3, #2527                   @ 0x9df 
    /*00003c40*/ @ MOVW        R2, #0                      @ .LC126 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*00003c44*/ STR         R3, [SP] 
    /*00003c48*/ ADDRL       R2, =_RODATA_STR1_4_124c    @ .LC126 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c]
    /*00003c4c*/ LDR         R3, WORD_3c74               @ LDR         R3, [PC, #32]               @ 0x0000000000003c74 
    /*00003c50*/ MOV         R1, #1 
    /*00003c54*/ MOV         R0, #3 
    /*00003c58*/ BL          HI_LOG
    /*00003c5c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003c60*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003c64*/ B           VB_SetConfig_x3bfc
    VB_SetConfig_x3c68:
    /*00003c68*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003c6c*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003c70*/ B           VB_SetConfig_x3bfc
    /*00003c74*/ WORD_3c74: .word 0x00000264
FUNC_END VB_SetConfig


FUNC_BEGIN VB_GetModPoolConfig
    /*00003c78*/ PUSH        {R4, R5, R6, LR} 
    /*00003c7c*/ SUBS        R5, R0, #0 
    /*00003c80*/ SUB         SP, SP, #16 
    /*00003c84*/ BEQ         VB_GetModPoolConfig_x3d48
    /*00003c88*/ LDR         R6, [R5] 
    /*00003c8c*/ CMP         R6, #22 
    /*00003c90*/ BHI         VB_GetModPoolConfig_x3cdc
    /*00003c94*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003c98*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00003c9c*/ ADD         R0, R4, #556                @ 0x22c 
    /*00003ca0*/ BL          osal_down
    /*00003ca4*/ ADD         R3, R4, R6, LSL #2 
    /*00003ca8*/ LDR         R3, [R3, #656]              @ 0x290 
    /*00003cac*/ CMP         R3, #0 
    /*00003cb0*/ BEQ         VB_GetModPoolConfig_x3d10
    /*00003cb4*/ ADD         R4, R4, #752                @ 0x2f0 
    /*00003cb8*/ MOV         R2, #520                    @ 0x208 
    /*00003cbc*/ ADD         R0, R5, #8 
    /*00003cc0*/ MLA         R1, R2, R6, R4 
    /*00003cc4*/ BL          osal_memcpy
    /*00003cc8*/ SUB         R0, R4, #196                @ 0xc4 
    /*00003ccc*/ BL          osal_up
    /*00003cd0*/ MOV         R0, #0 
    VB_GetModPoolConfig_x3cd4:
    /*00003cd4*/ ADD         SP, SP, #16 
    /*00003cd8*/ POP         {R4, R5, R6, PC} 
    VB_GetModPoolConfig_x3cdc:
    /*00003cdc*/ MOV         R2, #23 
    /*00003ce0*/ MOVW        R3, #2568                   @ 0xa08 
    /*00003ce4*/ STR         R2, [SP, #8] 
    /*00003ce8*/ MOV         R1, #1 
    /*00003cec*/ STR         R3, [SP] 
    /*00003cf0*/ @ MOVW        R2, #0                      @ .LC129 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12ec] 
    /*00003cf4*/ STR         R6, [SP, #4] 
    /*00003cf8*/ ADDRL       R2, =_RODATA_STR1_4_12ec    @ .LC129 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12ec]
    /*00003cfc*/ LDR         R3, WORD_3d54               @ LDR         R3, [PC, #80]               @ 0x0000000000003d54 
    /*00003d00*/ MOV         R0, #3 
    /*00003d04*/ BL          HI_LOG
    /*00003d08*/ MVN         R0, #0 
    /*00003d0c*/ B           VB_GetModPoolConfig_x3cd4
    VB_GetModPoolConfig_x3d10:
    /*00003d10*/ MOVW        R3, #2575                   @ 0xa0f 
    /*00003d14*/ MOV         R1, #1 
    /*00003d18*/ STR         R3, [SP] 
    /*00003d1c*/ @ MOVW        R2, #0                      @ .LC130 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1330] 
    /*00003d20*/ LDR         R3, WORD_3d54               @ LDR         R3, [PC, #44]               @ 0x0000000000003d54 
    /*00003d24*/ ADDRL       R2, =_RODATA_STR1_4_1330    @ .LC130 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1330]
    /*00003d28*/ STR         R6, [SP, #4] 
    /*00003d2c*/ MOV         R0, #3 
    /*00003d30*/ BL          HI_LOG
    /*00003d34*/ ADD         R0, R4, #556                @ 0x22c 
    /*00003d38*/ BL          osal_up
    /*00003d3c*/ MOVW        R0, #32784                  @ 0x8010 
    /*00003d40*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003d44*/ B           VB_GetModPoolConfig_x3cd4
    VB_GetModPoolConfig_x3d48:
    /*00003d48*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003d4c*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003d50*/ B           VB_GetModPoolConfig_x3cd4
    /*00003d54*/ WORD_3d54: .word 0x00000274
FUNC_END VB_GetModPoolConfig


FUNC_BEGIN VB_SetModPoolConfig
    /*00003d58*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00003d5c*/ SUBS        R6, R0, #0 
    /*00003d60*/ SUB         SP, SP, #16 
    /*00003d64*/ BEQ         VB_SetModPoolConfig_x3eb8
    /*00003d68*/ LDR         R7, [R6] 
    /*00003d6c*/ CMP         R7, #22 
    /*00003d70*/ ADDLS       R4, R6, #32 
    /*00003d74*/ MOVLS       R5, #1 
    /*00003d78*/ BLS         VB_SetModPoolConfig_x3d8c
    /*00003d7c*/ B           VB_SetModPoolConfig_x3e84
    VB_SetModPoolConfig_x3d80:
    /*00003d80*/ CMP         R5, #17 
    /*00003d84*/ ADD         R4, R4, #32 
    /*00003d88*/ BEQ         VB_SetModPoolConfig_x3df8
    VB_SetModPoolConfig_x3d8c:
    /*00003d8c*/ LDR         R3, [R4, #-8] 
    /*00003d90*/ ADD         R5, R5, #1 
    /*00003d94*/ CMP         R3, #0 
    /*00003d98*/ BEQ         VB_SetModPoolConfig_x3d80
    /*00003d9c*/ LDRD        R2, [R4, #-16] 
    /*00003da0*/ ORRS        R3, R2, R3 
    /*00003da4*/ BEQ         VB_SetModPoolConfig_x3d80
    /*00003da8*/ MOV         R1, #16 
    /*00003dac*/ MOV         R0, R4 
    /*00003db0*/ BL          osal_strnlen
    /*00003db4*/ CMP         R0, #15 
    /*00003db8*/ BGT         VB_SetModPoolConfig_x3e58
    /*00003dbc*/ LDR         R3, [R4, #-4] 
    /*00003dc0*/ CMP         R3, #2 
    /*00003dc4*/ BLS         VB_SetModPoolConfig_x3d80
    /*00003dc8*/ STR         R3, [SP, #4] 
    /*00003dcc*/ @ MOVW        R2, #0                      @ .LC127 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1284] 
    /*00003dd0*/ MOVW        R3, #2616                   @ 0xa38 
    /*00003dd4*/ ADDRL       R2, =_RODATA_STR1_4_1284    @ .LC127 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1284]
    /*00003dd8*/ STR         R3, [SP] 
    /*00003ddc*/ MOV         R1, #1 
    /*00003de0*/ LDR         R3, WORD_3ef8               @ LDR         R3, [PC, #272]              @ 0x0000000000003ef8 
    /*00003de4*/ MOV         R0, #3 
    /*00003de8*/ BL          HI_LOG
    /*00003dec*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003df0*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003df4*/ B           VB_SetModPoolConfig_x3e50
    VB_SetModPoolConfig_x3df8:
    /*00003df8*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003dfc*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00003e00*/ ADD         R0, R4, #556                @ 0x22c 
    /*00003e04*/ BL          osal_down
    /*00003e08*/ LDR         R3, [R4, #8] 
    /*00003e0c*/ CMP         R3, #0 
    /*00003e10*/ BEQ         VB_SetModPoolConfig_x3ec4
    /*00003e14*/ MOV         R2, #520                    @ 0x208 
    /*00003e18*/ ADD         R8, R4, #752                @ 0x2f0 
    /*00003e1c*/ MUL         R5, R2, R7 
    /*00003e20*/ ADD         R1, R6, #8 
    /*00003e24*/ ADD         R0, R8, R5 
    /*00003e28*/ ADD         R5, R4, R5 
    /*00003e2c*/ ADD         R4, R4, R7, LSL #2 
    /*00003e30*/ BL          osal_memcpy
    /*00003e34*/ SUB         R0, R8, #196                @ 0xc4 
    /*00003e38*/ MOV         R2, #512                    @ 0x200 
    /*00003e3c*/ STR         R2, [R5, #752]              @ 0x2f0 
    /*00003e40*/ MOV         R3, #1 
    /*00003e44*/ STR         R3, [R4, #656]              @ 0x290 
    /*00003e48*/ BL          osal_up
    /*00003e4c*/ MOV         R0, #0 
    VB_SetModPoolConfig_x3e50:
    /*00003e50*/ ADD         SP, SP, #16 
    /*00003e54*/ POP         {R4, R5, R6, R7, R8, PC} 
    VB_SetModPoolConfig_x3e58:
    /*00003e58*/ MOVW        R3, #2610                   @ 0xa32 
    /*00003e5c*/ @ MOVW        R2, #0                      @ .LC126 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*00003e60*/ STR         R3, [SP] 
    /*00003e64*/ ADDRL       R2, =_RODATA_STR1_4_124c    @ .LC126 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c]
    /*00003e68*/ LDR         R3, WORD_3ef8               @ LDR         R3, [PC, #136]              @ 0x0000000000003ef8 
    /*00003e6c*/ MOV         R1, #1 
    /*00003e70*/ MOV         R0, #3 
    /*00003e74*/ BL          HI_LOG
    /*00003e78*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003e7c*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003e80*/ B           VB_SetModPoolConfig_x3e50
    VB_SetModPoolConfig_x3e84:
    /*00003e84*/ MOV         R2, #23 
    /*00003e88*/ MOVW        R3, #2596                   @ 0xa24 
    /*00003e8c*/ STR         R2, [SP, #8] 
    /*00003e90*/ MOV         R1, #1 
    /*00003e94*/ STR         R3, [SP] 
    /*00003e98*/ @ MOVW        R2, #0                      @ .LC129 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12ec] 
    /*00003e9c*/ STR         R7, [SP, #4] 
    /*00003ea0*/ ADDRL       R2, =_RODATA_STR1_4_12ec    @ .LC129 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12ec]
    /*00003ea4*/ LDR         R3, WORD_3ef8               @ LDR         R3, [PC, #76]               @ 0x0000000000003ef8 
    /*00003ea8*/ MOV         R0, #3 
    /*00003eac*/ BL          HI_LOG
    /*00003eb0*/ MVN         R0, #0 
    /*00003eb4*/ B           VB_SetModPoolConfig_x3e50
    VB_SetModPoolConfig_x3eb8:
    /*00003eb8*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003ebc*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003ec0*/ B           VB_SetModPoolConfig_x3e50
    VB_SetModPoolConfig_x3ec4:
    /*00003ec4*/ MOV         R3, #2624                   @ 0xa40 
    /*00003ec8*/ MOV         R1, #1 
    /*00003ecc*/ STR         R3, [SP] 
    /*00003ed0*/ @ MOVW        R2, #0                      @ .LC131 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1364] 
    /*00003ed4*/ LDR         R3, WORD_3ef8               @ LDR         R3, [PC, #28]               @ 0x0000000000003ef8 
    /*00003ed8*/ ADDRL       R2, =_RODATA_STR1_4_1364    @ .LC131 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1364]
    /*00003edc*/ MOV         R0, #3 
    /*00003ee0*/ BL          HI_LOG
    /*00003ee4*/ ADD         R0, R4, #556                @ 0x22c 
    /*00003ee8*/ BL          osal_up
    /*00003eec*/ MOVW        R0, #32786                  @ 0x8012 
    /*00003ef0*/ MOVT        R0, #40961                  @ 0xa001 
    /*00003ef4*/ B           VB_SetModPoolConfig_x3e50
    /*00003ef8*/ WORD_3ef8: .word 0x00000288
FUNC_END VB_SetModPoolConfig


FUNC_BEGIN VbSetSupplementConf
    /*00003efc*/ PUSH        {R4, R5, R6, LR} 
    /*00003f00*/ SUB         SP, SP, #8 
    /*00003f04*/ MOV         R6, R0 
    /*00003f08*/ BL          VB_CheckSupplement
    /*00003f0c*/ SUBS        R4, R0, #0 
    /*00003f10*/ BEQ         VbSetSupplementConf_x3f20
    VbSetSupplementConf_x3f14:
    /*00003f14*/ MOV         R0, R4 
    /*00003f18*/ ADD         SP, SP, #8 
    /*00003f1c*/ POP         {R4, R5, R6, PC} 
    VbSetSupplementConf_x3f20:
    /*00003f20*/ @ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003f24*/ LDR         R5, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00003f28*/ ADD         R0, R5, #556                @ 0x22c 
    /*00003f2c*/ BL          osal_down_interruptible
    /*00003f30*/ SUBS        R4, R0, #0 
    /*00003f34*/ BNE         VbSetSupplementConf_x3f94
    /*00003f38*/ LDR         R3, [R5, #8] 
    /*00003f3c*/ CMP         R3, #0 
    /*00003f40*/ BEQ         VbSetSupplementConf_x3f80
    /*00003f44*/ MOVW        R3, #2813                   @ 0xafd 
    /*00003f48*/ @ MOVW        R2, #0                      @ .LC128 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12bc] 
    /*00003f4c*/ STR         R3, [SP] 
    /*00003f50*/ ADDRL       R2, =_RODATA_STR1_4_12bc    @ .LC128 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12bc]
    /*00003f54*/ LDR         R3, WORD_3fa0               @ LDR         R3, [PC, #68]               @ 0x0000000000003fa0 
    /*00003f58*/ MOV         R1, #1 
    /*00003f5c*/ MOV         R0, #3 
    /*00003f60*/ MOVW        R4, #32786                  @ 0x8012 
    /*00003f64*/ BL          HI_LOG
    /*00003f68*/ MOVT        R4, #40961                  @ 0xa001 
    VbSetSupplementConf_x3f6c:
    /*00003f6c*/ LDR         R0, WORD_3fa4               @ LDR         R0, [PC, #48]               @ 0x0000000000003fa4 
    /*00003f70*/ BL          osal_up
    /*00003f74*/ MOV         R0, R4 
    /*00003f78*/ ADD         SP, SP, #8 
    /*00003f7c*/ POP         {R4, R5, R6, PC} 
    VbSetSupplementConf_x3f80:
    /*00003f80*/ MOV         R1, R6 
    /*00003f84*/ MOV         R0, R5 
    /*00003f88*/ MOV         R2, #4 
    /*00003f8c*/ BL          osal_memcpy
    /*00003f90*/ B           VbSetSupplementConf_x3f6c
    VbSetSupplementConf_x3f94:
    /*00003f94*/ MOV         R4, #65024                  @ 0xfe00 
    /*00003f98*/ MOVT        R4, #65535                  @ 0xffff 
    /*00003f9c*/ B           VbSetSupplementConf_x3f14
    /*00003fa0*/ WORD_3fa0: .word 0x0000029c
    /*00003fa4*/ WORD_3fa4: .word 0x0000022c
FUNC_END VbSetSupplementConf


FUNC_BEGIN BaseIoctl
    /*00003fa8*/ MOVW        R3, #16900                  @ 0x4204 
    /*00003fac*/ MOVT        R3, #49156                  @ 0xc004 
    /*00003fb0*/ CMP         R0, R3 
    /*00003fb4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003fb8*/ MOV         R5, R1 
    /*00003fbc*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00003fc0*/ BEQ         BaseIoctl_x4780
    /*00003fc4*/ BHI         BaseIoctl_x401c
    /*00003fc8*/ MOVW        R3, #16914                  @ 0x4212 
    /*00003fcc*/ MOVT        R3, #16388                  @ 0x4004 
    /*00003fd0*/ CMP         R0, R3 
    /*00003fd4*/ BEQ         BaseIoctl_x4804
    /*00003fd8*/ BLS         BaseIoctl_x4088
    /*00003fdc*/ MOVW        R3, #16915                  @ 0x4213 
    /*00003fe0*/ MOVT        R3, #32772                  @ 0x8004 
    /*00003fe4*/ CMP         R0, R3 
    /*00003fe8*/ BEQ         BaseIoctl_x43b8
    /*00003fec*/ BHI         BaseIoctl_x420c
    /*00003ff0*/ MOVW        R3, #16907                  @ 0x420b 
    /*00003ff4*/ MOVT        R3, #16904                  @ 0x4208 
    /*00003ff8*/ CMP         R0, R3 
    /*00003ffc*/ BEQ         BaseIoctl_x46c4
    /*00004000*/ MOVW        R3, #16913                  @ 0x4211 
    /*00004004*/ MOVT        R3, #16912                  @ 0x4210 
    /*00004008*/ CMP         R0, R3 
    /*0000400c*/ BNE         BaseIoctl_x447c
    /*00004010*/ MOV         R0, R1 
    /*00004014*/ BL          VB_SetModPoolConfig
    /*00004018*/ B           BaseIoctl_x43b0
    BaseIoctl_x401c:
    /*0000401c*/ MOVW        R3, #16902                  @ 0x4206 
    /*00004020*/ MOVT        R3, #49160                  @ 0xc008 
    /*00004024*/ CMP         R0, R3 
    /*00004028*/ BEQ         BaseIoctl_x4810
    /*0000402c*/ BLS         BaseIoctl_x4174
    /*00004030*/ MOVW        R3, #16897                  @ 0x4201 
    /*00004034*/ MOVT        R3, #49200                  @ 0xc030 
    /*00004038*/ CMP         R0, R3 
    /*0000403c*/ BEQ         BaseIoctl_x43d4
    /*00004040*/ BHI         BaseIoctl_x41e0
    /*00004044*/ MOVW        R3, #16916                  @ 0x4214 
    /*00004048*/ MOVT        R3, #49160                  @ 0xc008 
    /*0000404c*/ CMP         R0, R3 
    /*00004050*/ BEQ         BaseIoctl_x46d0
    /*00004054*/ MOVW        R3, #16908                  @ 0x420c 
    /*00004058*/ MOVT        R3, #49192                  @ 0xc028 
    /*0000405c*/ CMP         R0, R3 
    /*00004060*/ BNE         BaseIoctl_x447c
    /*00004064*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #4024]             @ 0x0000000000005024 
    /*00004068*/ BL          osal_down
    /*0000406c*/ MOV         R0, R5 
    /*00004070*/ BL          VB_GetPoolInfo
    /*00004074*/ MOV         R4, R0 
    /*00004078*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #4004]             @ 0x0000000000005024 
    /*0000407c*/ BL          osal_up
    /*00004080*/ MOV         R0, R4 
    /*00004084*/ B           BaseIoctl_x43b0
    BaseIoctl_x4088:
    /*00004088*/ MOVW        R3, #16905                  @ 0x4209 
    /*0000408c*/ CMP         R0, R3 
    /*00004090*/ BEQ         BaseIoctl_x4638
    /*00004094*/ BLS         BaseIoctl_x42c8
    /*00004098*/ MOVW        R3, #16910                  @ 0x420e 
    /*0000409c*/ MOVT        R3, #16388                  @ 0x4004 
    /*000040a0*/ CMP         R0, R3 
    /*000040a4*/ BEQ         BaseIoctl_x450c
    /*000040a8*/ MOVW        R3, #16911                  @ 0x420f 
    /*000040ac*/ MOVT        R3, #16388                  @ 0x4004 
    /*000040b0*/ CMP         R0, R3 
    /*000040b4*/ BNE         BaseIoctl_x447c
    /*000040b8*/ LDR         R9, [R1] 
    /*000040bc*/ CMP         R9, #22 
    /*000040c0*/ BHI         BaseIoctl_x4e88
    /*000040c4*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #3928]             @ 0x0000000000005024 
    /*000040c8*/ BL          osal_down
    /*000040cc*/ SUBS        R4, R0, #0 
    /*000040d0*/ BNE         BaseIoctl_x5078
    /*000040d4*/ @ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000040d8*/ LDR         R6, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000040dc*/ LDR         R7, [R6, #8] 
    /*000040e0*/ CMP         R7, #0 
    /*000040e4*/ BEQ         BaseIoctl_x4ebc
    /*000040e8*/ MOV         R8, #520                    @ 0x208 
    /*000040ec*/ MLA         R8, R8, R9, R6 
    /*000040f0*/ LDR         R3, [R8, #752]              @ 0x2f0 
    /*000040f4*/ CMP         R3, #0 
    /*000040f8*/ BEQ         BaseIoctl_x48f4
    /*000040fc*/ MOV         R5, #0 
    /*00004100*/ B           BaseIoctl_x4118
    BaseIoctl_x4104:
    /*00004104*/ LDR         R3, [R8, #752]              @ 0x2f0 
    /*00004108*/ ADD         R5, R5, #1 
    /*0000410c*/ CMP         R5, R3 
    /*00004110*/ BCS         BaseIoctl_x48f4
    /*00004114*/ LDR         R7, [R6, #8] 
    BaseIoctl_x4118:
    /*00004118*/ LDR         R3, [R7, R5, LSL #2] 
    /*0000411c*/ LSL         R7, R5, #2 
    /*00004120*/ CMP         R3, #0 
    /*00004124*/ BEQ         BaseIoctl_x4104
    /*00004128*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*0000412c*/ CMP         R2, R9 
    /*00004130*/ BNE         BaseIoctl_x4104
    /*00004134*/ LDR         R0, [R3] 
    /*00004138*/ ADD         R1, SP, #36                 @ 0x24 
    /*0000413c*/ BL          VB_InquirePoolBusyBlkCnt
    /*00004140*/ CMP         R0, #0 
    /*00004144*/ BNE         BaseIoctl_x4cec
    /*00004148*/ LDR         R3, [SP, #36]               @ 0x24 
    /*0000414c*/ CMP         R3, #0 
    /*00004150*/ BEQ         BaseIoctl_x4160
    /*00004154*/ LDR         R3, [R6, #552]              @ 0x228 
    /*00004158*/ CMP         R3, #0 
    /*0000415c*/ BEQ         BaseIoctl_x4dd4
    BaseIoctl_x4160:
    /*00004160*/ LDR         R3, [R6, #8] 
    /*00004164*/ LDR         R0, [R3, R7] 
    /*00004168*/ BL          VbDestroyPool               @ BL          0x00000000000019F4 
    /*0000416c*/ MOV         R4, R0 
    /*00004170*/ B           BaseIoctl_x4104
    BaseIoctl_x4174:
    /*00004174*/ MOVW        R3, #16909                  @ 0x420d 
    /*00004178*/ MOVT        R3, #49156                  @ 0xc004 
    /*0000417c*/ CMP         R0, R3 
    /*00004180*/ BEQ         BaseIoctl_x4898
    /*00004184*/ BLS         BaseIoctl_x4334
    /*00004188*/ MOVW        R3, #16917                  @ 0x4215 
    /*0000418c*/ MOVT        R3, #49156                  @ 0xc004 
    /*00004190*/ CMP         R0, R3 
    /*00004194*/ BEQ         BaseIoctl_x44a4
    /*00004198*/ MOVW        R3, #16901                  @ 0x4205 
    /*0000419c*/ MOVT        R3, #49160                  @ 0xc008 
    /*000041a0*/ CMP         R0, R3 
    /*000041a4*/ BNE         BaseIoctl_x447c
    /*000041a8*/ CMP         R1, #0 
    /*000041ac*/ BEQ         BaseIoctl_x4774
    /*000041b0*/ LDRD        R6, [R1] 
    /*000041b4*/ MOV         R4, #0 
    /*000041b8*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #3684]             @ 0x0000000000005024 
    /*000041bc*/ BL          osal_down
    /*000041c0*/ MOV         R1, R7 
    /*000041c4*/ MOV         R0, R6 
    /*000041c8*/ BL          VB_Phy2Handle
    /*000041cc*/ STM         R5, {R0, R4} 
    /*000041d0*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #3660]             @ 0x0000000000005024 
    /*000041d4*/ BL          osal_up
    /*000041d8*/ MOV         R0, R4 
    /*000041dc*/ B           BaseIoctl_x43b0
    BaseIoctl_x41e0:
    /*000041e0*/ MOVW        R3, #16899                  @ 0x4203 
    /*000041e4*/ MOVT        R3, #49200                  @ 0xc030 
    /*000041e8*/ CMP         R0, R3 
    /*000041ec*/ BEQ         BaseIoctl_x464c
    /*000041f0*/ MOVW        R3, #16912                  @ 0x4210 
    /*000041f4*/ MOVT        R3, #49680                  @ 0xc210 
    /*000041f8*/ CMP         R0, R3 
    /*000041fc*/ BNE         BaseIoctl_x447c
    /*00004200*/ MOV         R0, R1 
    /*00004204*/ BL          VB_GetModPoolConfig
    /*00004208*/ B           BaseIoctl_x43b0
    BaseIoctl_x420c:
    /*0000420c*/ MOVW        R3, #16906                  @ 0x420a 
    /*00004210*/ MOVT        R3, #33288                  @ 0x8208 
    /*00004214*/ CMP         R0, R3 
    /*00004218*/ BEQ         BaseIoctl_x4640
    /*0000421c*/ MOVW        R3, #16898                  @ 0x4202 
    /*00004220*/ MOVT        R3, #49156                  @ 0xc004 
    /*00004224*/ CMP         R0, R3 
    /*00004228*/ BNE         BaseIoctl_x447c
    /*0000422c*/ @ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004230*/ LDR         R6, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00004234*/ ADD         R0, R6, #556                @ 0x22c 
    /*00004238*/ MOV         R3, #0 
    /*0000423c*/ LDR         R4, [R1] 
    /*00004240*/ STR         R3, [SP, #36]               @ 0x24 
    /*00004244*/ BL          osal_down
    /*00004248*/ LDR         R3, [R6, #8] 
    /*0000424c*/ CMP         R3, #0 
    /*00004250*/ @ MOVWEQ      R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00004254*/ MOVEQ       R1, #1 
    /*00004258*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*0000425c*/ BEQ         BaseIoctl_x4d84
    /*00004260*/ LDR         R2, [R6, #16] 
    /*00004264*/ CMP         R4, R2 
    /*00004268*/ BCS         BaseIoctl_x4a04
    /*0000426c*/ LDR         R5, [R3, R4, LSL #2] 
    /*00004270*/ CMP         R5, #0 
    /*00004274*/ MOVWEQ      R3, #3588                   @ 0xe04 
    /*00004278*/ @ MOVWEQ      R2, #0                      @ .LC136 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1448] 
    /*0000427c*/ STREQ       R3, [SP] 
    /*00004280*/ ADDRL       R2, =_RODATA_STR1_4_1448    @ .LC136 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1448]
    /*00004284*/ BEQ         BaseIoctl_x4c20
    /*00004288*/ LDR         R3, [R5, #172]              @ 0xac 
    /*0000428c*/ CMP         R3, #0 
    /*00004290*/ BNE         BaseIoctl_x49bc
    /*00004294*/ MOVW        R3, #3594                   @ 0xe0a 
    /*00004298*/ MOV         R1, #1 
    /*0000429c*/ STR         R3, [SP] 
    /*000042a0*/ @ MOVW        R2, #0                      @ .LC137 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1488] 
    /*000042a4*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #3472]             @ 0x000000000000503c 
    /*000042a8*/ ADDRL       R2, =_RODATA_STR1_4_1488    @ .LC137 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1488]
    /*000042ac*/ MOV         R0, #3 
    /*000042b0*/ BL          HI_LOG
    /*000042b4*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #3432]             @ 0x0000000000005024 
    /*000042b8*/ BL          osal_up
    /*000042bc*/ MOVW        R0, #32777                  @ 0x8009 
    /*000042c0*/ MOVT        R0, #40961                  @ 0xa001 
    /*000042c4*/ B           BaseIoctl_x43b0
    BaseIoctl_x42c8:
    /*000042c8*/ MOVW        R3, #16904                  @ 0x4208 
    /*000042cc*/ CMP         R0, R3 
    /*000042d0*/ BNE         BaseIoctl_x447c
    /*000042d4*/ @ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000042d8*/ LDR         R6, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000042dc*/ ADD         R0, R6, #556                @ 0x22c 
    /*000042e0*/ BL          osal_down_interruptible
    /*000042e4*/ CMP         R0, #0 
    /*000042e8*/ BNE         BaseIoctl_x4d64
    /*000042ec*/ LDR         R3, [R6, #652]              @ 0x28c 
    /*000042f0*/ CMP         R3, #0 
    /*000042f4*/ BEQ         BaseIoctl_x5140
    /*000042f8*/ LDR         R3, [R6, #8] 
    /*000042fc*/ CMP         R3, #0 
    /*00004300*/ BEQ         BaseIoctl_x4a94
    /*00004304*/ MOVW        R3, #2949                   @ 0xb85 
    /*00004308*/ MOV         R1, #1 
    /*0000430c*/ STR         R3, [SP] 
    /*00004310*/ @ MOVW        R2, #0                      @ .LC147 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16d8] 
    /*00004314*/ LDR         R3, WORD_5038               @ LDR         R3, [PC, #3356]             @ 0x0000000000005038 
    /*00004318*/ ADDRL       R2, =_RODATA_STR1_4_16d8    @ .LC147 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16d8]
    /*0000431c*/ MOV         R0, #6 
    /*00004320*/ BL          HI_LOG
    /*00004324*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #3320]             @ 0x0000000000005024 
    /*00004328*/ BL          osal_up
    /*0000432c*/ MOV         R0, #0 
    /*00004330*/ B           BaseIoctl_x43b0
    BaseIoctl_x4334:
    /*00004334*/ MOVW        R3, #16903                  @ 0x4207 
    /*00004338*/ MOVT        R3, #49156                  @ 0xc004 
    /*0000433c*/ CMP         R0, R3 
    /*00004340*/ BNE         BaseIoctl_x447c
    /*00004344*/ @ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004348*/ LDR         R6, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*0000434c*/ ADD         R0, R6, #556                @ 0x22c 
    /*00004350*/ LDR         R4, [R1] 
    /*00004354*/ BL          osal_down
    /*00004358*/ LDR         R3, [R6, #8] 
    /*0000435c*/ CMP         R3, #0 
    /*00004360*/ BEQ         BaseIoctl_x4d70
    /*00004364*/ LDR         R1, [R6, #16] 
    /*00004368*/ LSR         R2, R4, #16 
    /*0000436c*/ CMP         R2, R1 
    /*00004370*/ MVNCS       R4, #0 
    /*00004374*/ MOVWCS      R3, #3807                   @ 0xedf 
    /*00004378*/ BCS         BaseIoctl_x4f58
    /*0000437c*/ LDR         R3, [R3, R2, LSL #2] 
    /*00004380*/ CMP         R3, #0 
    /*00004384*/ BEQ         BaseIoctl_x4c94
    /*00004388*/ LDR         R3, [R3, #12] 
    /*0000438c*/ UXTH        R2, R4 
    /*00004390*/ CMP         R2, R3 
    /*00004394*/ BCS         BaseIoctl_x4c94
    /*00004398*/ MOV         R0, R4 
    /*0000439c*/ BL          VB_Handle2PoolId
    /*000043a0*/ STR         R0, [R5] 
    /*000043a4*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #3192]             @ 0x0000000000005024 
    /*000043a8*/ BL          osal_up
    /*000043ac*/ MOV         R0, #0 
    BaseIoctl_x43b0:
    /*000043b0*/ ADD         SP, SP, #44                 @ 0x2c 
    /*000043b4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    BaseIoctl_x43b8:
    /*000043b8*/ MOV         R0, R1 
    /*000043bc*/ MOV         R2, #4 
    /*000043c0*/ @ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000043c4*/ LDR         R1, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000043c8*/ BL          osal_memcpy
    /*000043cc*/ MOV         R0, #0 
    /*000043d0*/ B           BaseIoctl_x43b0
    BaseIoctl_x43d4:
    /*000043d4*/ LDR         R3, [R1, #16] 
    /*000043d8*/ CMP         R3, #0 
    /*000043dc*/ BEQ         BaseIoctl_x4e00
    /*000043e0*/ LDRD        R2, [R1, #8] 
    /*000043e4*/ ORRS        R3, R2, R3 
    /*000043e8*/ BEQ         BaseIoctl_x4e00
    /*000043ec*/ LDR         R3, [R1, #28] 
    /*000043f0*/ CMP         R3, #2 
    /*000043f4*/ BHI         BaseIoctl_x4eec
    /*000043f8*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000043fc*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00004400*/ ADD         R0, R4, #556                @ 0x22c 
    /*00004404*/ BL          osal_down
    /*00004408*/ CMP         R0, #0 
    /*0000440c*/ BNE         BaseIoctl_x4d64
    /*00004410*/ LDR         R3, [R4, #8] 
    /*00004414*/ CMP         R3, #0 
    /*00004418*/ BEQ         BaseIoctl_x51bc
    /*0000441c*/ LDR         R2, [R5, #28] 
    /*00004420*/ MVN         LR, #1 
    /*00004424*/ LDRD        R6, [R5, #8] 
    /*00004428*/ ADD         IP, R5, #32 
    /*0000442c*/ LDR         R0, [R5, #16] 
    /*00004430*/ MOV         R1, #1 
    /*00004434*/ STR         R2, [SP, #24] 
    /*00004438*/ @ MOVW        R3, #0                      @ .LC134 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1408] 
    /*0000443c*/ STR         LR, [SP, #16] 
    /*00004440*/ ADDRL       R3, =_RODATA_STR1_4_1408    @ .LC134 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1408]
    /*00004444*/ STR         IP, [SP, #12] 
    /*00004448*/ MOV         R2, R6 
    /*0000444c*/ STR         R3, [SP, #20] 
    /*00004450*/ MOV         R3, R7 
    /*00004454*/ STR         R5, [SP, #4] 
    /*00004458*/ STR         R1, [SP, #8] 
    /*0000445c*/ LDR         R1, [R5, #24] 
    /*00004460*/ STR         R1, [SP] 
    /*00004464*/ BL          VbCreatePool                @ BL          0x0000000000000E0C 
    /*00004468*/ MOV         R4, R0 
    /*0000446c*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #2992]             @ 0x0000000000005024 
    /*00004470*/ BL          osal_up
    /*00004474*/ MOV         R0, R4 
    /*00004478*/ B           BaseIoctl_x43b0
    BaseIoctl_x447c:
    /*0000447c*/ MOVW        R3, #3998                   @ 0xf9e 
    /*00004480*/ @ MOVW        R2, #0                      @ .LC159 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x191c] 
    /*00004484*/ STR         R3, [SP] 
    /*00004488*/ ADDRL       R2, =_RODATA_STR1_4_191c    @ .LC159 [R_ARM_MOVT_ABS: .rodata.str1.4+0x191c]
    /*0000448c*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #2984]             @ 0x000000000000503c 
    /*00004490*/ MOV         R1, #1 
    /*00004494*/ MOV         R0, #3 
    /*00004498*/ BL          HI_LOG
    /*0000449c*/ MVN         R0, #0 
    /*000044a0*/ B           BaseIoctl_x43b0
    BaseIoctl_x44a4:
    /*000044a4*/ @ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000044a8*/ LDR         R6, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000044ac*/ ADD         R0, R6, #556                @ 0x22c 
    /*000044b0*/ LDR         R4, [R1] 
    /*000044b4*/ BL          osal_down
    /*000044b8*/ LDR         R3, [R6, #8] 
    /*000044bc*/ CMP         R3, #0 
    /*000044c0*/ BEQ         BaseIoctl_x5174
    /*000044c4*/ LDR         R1, [R6, #16] 
    /*000044c8*/ LSR         R2, R4, #16 
    /*000044cc*/ CMP         R2, R1 
    /*000044d0*/ BCS         BaseIoctl_x4f50
    /*000044d4*/ LDR         R3, [R3, R2, LSL #2] 
    /*000044d8*/ CMP         R3, #0 
    /*000044dc*/ BEQ         BaseIoctl_x4ccc
    /*000044e0*/ LDR         R3, [R3, #12] 
    /*000044e4*/ UXTH        R2, R4 
    /*000044e8*/ CMP         R2, R3 
    /*000044ec*/ BCS         BaseIoctl_x4ccc
    /*000044f0*/ MOV         R0, R4 
    /*000044f4*/ BL          VB_InquireUserCnt
    /*000044f8*/ STR         R0, [R5] 
    /*000044fc*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #2848]             @ 0x0000000000005024 
    /*00004500*/ BL          osal_up
    /*00004504*/ MOV         R0, #0 
    /*00004508*/ B           BaseIoctl_x43b0
    BaseIoctl_x450c:
    /*0000450c*/ LDR         R8, [R1] 
    /*00004510*/ CMP         R8, #22 
    /*00004514*/ BHI         BaseIoctl_x4e2c
    /*00004518*/ @ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000451c*/ LDR         R6, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00004520*/ ADD         R0, R6, #556                @ 0x22c 
    /*00004524*/ BL          osal_down
    /*00004528*/ SUBS        R4, R0, #0 
    /*0000452c*/ BNE         BaseIoctl_x50d8
    /*00004530*/ LDR         R3, [R6, #652]              @ 0x28c 
    /*00004534*/ CMP         R3, #0 
    /*00004538*/ BEQ         BaseIoctl_x50a4
    /*0000453c*/ LDR         R3, [R6, #8] 
    /*00004540*/ CMP         R3, #0 
    /*00004544*/ BEQ         BaseIoctl_x4e5c
    /*00004548*/ LSL         R9, R8, #2 
    /*0000454c*/ ADD         R3, R6, R9 
    /*00004550*/ LDR         R2, [R3, #656]              @ 0x290 
    /*00004554*/ CMP         R2, #0 
    /*00004558*/ BEQ         BaseIoctl_x5104
    /*0000455c*/ LDR         R1, [R3, #560]              @ 0x230 
    /*00004560*/ CMP         R1, #1 
    /*00004564*/ BEQ         BaseIoctl_x4d00
    /*00004568*/ MOV         R5, #520                    @ 0x208 
    /*0000456c*/ LDR         R7, WORD_5028               @ LDR         R7, [PC, #2740]             @ 0x0000000000005028 
    /*00004570*/ MUL         R5, R5, R8 
    /*00004574*/ @ MOVW        SL, #0                      @ .LC156 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1898] 
    /*00004578*/ ADD         FP, SP, #36                 @ 0x24 
    /*0000457c*/ ADDRL       SL, =_RODATA_STR1_4_1898    @ .LC156 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1898]
    /*00004580*/ ADD         R3, R5, #512                @ 0x200 
    /*00004584*/ ADD         R5, R7, R5 
    /*00004588*/ ADD         R7, R7, R3 
    /*0000458c*/ B           BaseIoctl_x459c
    BaseIoctl_x4590:
    /*00004590*/ ADD         R5, R5, #32 
    /*00004594*/ CMP         R5, R7 
    /*00004598*/ BEQ         BaseIoctl_x4934
    BaseIoctl_x459c:
    /*0000459c*/ LDR         R0, [R5, #16] 
    /*000045a0*/ CMP         R0, #0 
    /*000045a4*/ BEQ         BaseIoctl_x4590
    /*000045a8*/ LDRD        R2, [R5, #8] 
    /*000045ac*/ ORRS        R1, R2, R3 
    /*000045b0*/ BEQ         BaseIoctl_x4590
    /*000045b4*/ LDR         R1, [R5, #20] 
    /*000045b8*/ ADD         IP, R5, #24 
    /*000045bc*/ MOV         LR, #0 
    /*000045c0*/ STR         SL, [SP, #20] 
    /*000045c4*/ STR         R8, [SP, #16] 
    /*000045c8*/ STR         R1, [SP, #24] 
    /*000045cc*/ MOV         R1, #1 
    /*000045d0*/ STMIB       SP, {FP, LR} 
    /*000045d4*/ STR         IP, [SP, #12] 
    /*000045d8*/ STR         R1, [SP] 
    /*000045dc*/ BL          VbCreatePool                @ BL          0x0000000000000E0C 
    /*000045e0*/ CMP         R0, #0 
    /*000045e4*/ BEQ         BaseIoctl_x4590
    /*000045e8*/ MOV         R5, #520                    @ 0x208 
    /*000045ec*/ MLA         R5, R5, R8, R6 
    /*000045f0*/ LDR         R3, [R5, #752]              @ 0x2f0 
    /*000045f4*/ CMP         R3, #0 
    /*000045f8*/ MOVNE       R4, #0 
    /*000045fc*/ BNE         BaseIoctl_x4610
    /*00004600*/ B           BaseIoctl_x4d2c
    BaseIoctl_x4604:
    /*00004604*/ LDR         R3, [R5, #752]              @ 0x2f0 
    /*00004608*/ CMP         R4, R3 
    /*0000460c*/ BCS         BaseIoctl_x4d2c
    BaseIoctl_x4610:
    /*00004610*/ LDR         R3, [R6, #8] 
    /*00004614*/ LDR         R0, [R3, R4, LSL #2] 
    /*00004618*/ ADD         R4, R4, #1 
    /*0000461c*/ CMP         R0, #0 
    /*00004620*/ BEQ         BaseIoctl_x4604
    /*00004624*/ LDR         R3, [R0, #192]              @ 0xc0 
    /*00004628*/ CMP         R8, R3 
    /*0000462c*/ BNE         BaseIoctl_x4604
    /*00004630*/ BL          VbDestroyPool               @ BL          0x00000000000019F4 
    /*00004634*/ B           BaseIoctl_x4604
    BaseIoctl_x4638:
    /*00004638*/ BL          VbDoVbExit                  @ BL          0x0000000000001BCC 
    /*0000463c*/ B           BaseIoctl_x43b0
    BaseIoctl_x4640:
    /*00004640*/ MOV         R0, R1 
    /*00004644*/ BL          VB_GetConfig
    /*00004648*/ B           BaseIoctl_x43b0
    BaseIoctl_x464c:
    /*0000464c*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004650*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00004654*/ ADD         R0, R4, #556                @ 0x22c 
    /*00004658*/ BL          osal_down
    /*0000465c*/ LDR         R3, [R4, #8] 
    /*00004660*/ CMP         R3, #0 
    /*00004664*/ BEQ         BaseIoctl_x5174
    /*00004668*/ LDR         R0, [R5] 
    /*0000466c*/ CMN         R0, #1 
    /*00004670*/ BEQ         BaseIoctl_x4c44
    /*00004674*/ LDR         R2, [R4, #16] 
    /*00004678*/ CMP         R0, R2 
    /*0000467c*/ BCS         BaseIoctl_x5118
    /*00004680*/ LDR         R3, [R3, R0, LSL #2] 
    /*00004684*/ CMP         R3, #0 
    /*00004688*/ BEQ         BaseIoctl_x5260
    /*0000468c*/ LDRD        R6, [R5, #8] 
    /*00004690*/ LDRD        R2, [R3, #16] 
    /*00004694*/ CMP         R7, R3 
    /*00004698*/ CMPEQ       R6, R2 
    /*0000469c*/ BHI         BaseIoctl_x4f88
    /*000046a0*/ MOV         R1, #12 
    /*000046a4*/ BL          VB_GetBlkByPoolId
    /*000046a8*/ CMN         R0, #1 
    /*000046ac*/ STR         R0, [R5, #20] 
    /*000046b0*/ BEQ         BaseIoctl_x519c
    BaseIoctl_x46b4:
    /*000046b4*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #2408]             @ 0x0000000000005024 
    /*000046b8*/ BL          osal_up
    /*000046bc*/ MOV         R0, #0 
    /*000046c0*/ B           BaseIoctl_x43b0
    BaseIoctl_x46c4:
    /*000046c4*/ MOV         R0, R1 
    /*000046c8*/ BL          VB_SetConfig
    /*000046cc*/ B           BaseIoctl_x43b0
    BaseIoctl_x46d0:
    /*000046d0*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000046d4*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000046d8*/ ADD         R0, R4, #556                @ 0x22c 
    /*000046dc*/ LDR         R6, [R1] 
    /*000046e0*/ BL          osal_down
    /*000046e4*/ LDR         R3, [R4, #8] 
    /*000046e8*/ CMP         R3, #0 
    /*000046ec*/ BEQ         BaseIoctl_x5174
    /*000046f0*/ LDR         R1, [R4, #16] 
    /*000046f4*/ LSR         R2, R6, #16 
    /*000046f8*/ CMP         R2, R1 
    /*000046fc*/ BCS         BaseIoctl_x5044
    /*00004700*/ LDR         R3, [R3, R2, LSL #2] 
    /*00004704*/ CMP         R3, #0 
    /*00004708*/ BEQ         BaseIoctl_x4c10
    /*0000470c*/ LDR         R3, [R3, #12] 
    /*00004710*/ UXTH        R2, R6 
    /*00004714*/ CMP         R2, R3 
    /*00004718*/ BCS         BaseIoctl_x4c10
    /*0000471c*/ MOV         R0, R6 
    /*00004720*/ BL          VB_Handle2Supplement
    /*00004724*/ LDR         R4, [R5, #4] 
    /*00004728*/ MOV         R2, #64                     @ 0x40 
    /*0000472c*/ MOV         R3, #0 
    /*00004730*/ STRB        R3, [SP, #36]               @ 0x24 
    /*00004734*/ MOV         R1, R4 
    /*00004738*/ MOV         R6, R0 
    /*0000473c*/ MOV         R0, #1 
    /*00004740*/ BL          osal_access_ok
    /*00004744*/ CMP         R0, #0 
    /*00004748*/ BNE         BaseIoctl_x496c
    BaseIoctl_x474c:
    /*0000474c*/ @ MOVW        R2, #0                      @ .LC160 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1954] 
    /*00004750*/ ADDRL       R2, =_RODATA_STR1_4_1954    @ .LC160 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1954]
    /*00004754*/ MOVW        R3, #3861                   @ 0xf15 
    /*00004758*/ STR         R3, [SP] 
    BaseIoctl_x475c:
    /*0000475c*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #2264]             @ 0x000000000000503c 
    /*00004760*/ MOV         R1, #1 
    /*00004764*/ MOV         R0, #3 
    /*00004768*/ BL          HI_LOG
    /*0000476c*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #2224]             @ 0x0000000000005024 
    /*00004770*/ BL          osal_up
    BaseIoctl_x4774:
    /*00004774*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004778*/ MOVT        R0, #40961                  @ 0xa001 
    /*0000477c*/ B           BaseIoctl_x43b0
    BaseIoctl_x4780:
    /*00004780*/ LDR         R3, [R1] 
    /*00004784*/ @ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004788*/ LDR         R6, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*0000478c*/ ADD         R0, R6, #556                @ 0x22c 
    /*00004790*/ LSR         R7, R3, #16 
    /*00004794*/ UXTH        R4, R3 
    /*00004798*/ BL          osal_down
    /*0000479c*/ LDR         R3, [R6, #8] 
    /*000047a0*/ CMP         R3, #0 
    /*000047a4*/ BEQ         BaseIoctl_x5174
    /*000047a8*/ LDR         R2, [R6, #16] 
    /*000047ac*/ CMP         R7, R2 
    /*000047b0*/ BCS         BaseIoctl_x4fe8
    /*000047b4*/ LDR         R3, [R3, R7, LSL #2] 
    /*000047b8*/ CMP         R3, #0 
    /*000047bc*/ BEQ         BaseIoctl_x5208
    /*000047c0*/ LDR         R3, [R3, #12] 
    /*000047c4*/ CMP         R4, R3 
    /*000047c8*/ BCS         BaseIoctl_x4f1c
    /*000047cc*/ LDR         R3, [R6, #536]              @ 0x218 
    /*000047d0*/ MOV         R2, #200                    @ 0xc8 
    /*000047d4*/ MOV         R1, #12 
    /*000047d8*/ MOV         R0, R7 
    /*000047dc*/ LDR         R3, [R3, R7, LSL #2] 
    /*000047e0*/ MLA         R3, R2, R4, R3 
    /*000047e4*/ LDRD        R2, [R3, #16] 
    /*000047e8*/ STR         R1, [SP] 
    /*000047ec*/ BL          VB_UserSub
    /*000047f0*/ MOV         R4, R0 
    /*000047f4*/ ADD         R0, R6, #556                @ 0x22c 
    /*000047f8*/ BL          osal_up
    /*000047fc*/ MOV         R0, R4 
    /*00004800*/ B           BaseIoctl_x43b0
    BaseIoctl_x4804:
    /*00004804*/ MOV         R0, R1 
    /*00004808*/ BL          VbSetSupplementConf
    /*0000480c*/ B           BaseIoctl_x43b0
    BaseIoctl_x4810:
    /*00004810*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004814*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00004818*/ ADD         R0, R4, #556                @ 0x22c 
    /*0000481c*/ LDR         R6, [R1] 
    /*00004820*/ BL          osal_down
    /*00004824*/ LDR         R3, [R4, #8] 
    /*00004828*/ CMP         R3, #0 
    /*0000482c*/ BEQ         BaseIoctl_x5240
    /*00004830*/ LDR         R1, [R4, #16] 
    /*00004834*/ LSR         R2, R6, #16 
    /*00004838*/ CMP         R2, R1 
    /*0000483c*/ BCS         BaseIoctl_x4a58
    /*00004840*/ LDR         R3, [R3, R2, LSL #2] 
    /*00004844*/ CMP         R3, #0 
    /*00004848*/ BEQ         BaseIoctl_x485c
    /*0000484c*/ LDR         R3, [R3, #12] 
    /*00004850*/ UXTH        R2, R6 
    /*00004854*/ CMP         R2, R3 
    /*00004858*/ BCC         BaseIoctl_x4a3c
    BaseIoctl_x485c:
    /*0000485c*/ MOV         R0, #0 
    /*00004860*/ MOV         R1, #0 
    /*00004864*/ MOVW        R3, #3778                   @ 0xec2 
    /*00004868*/ STRD        R0, [R5] 
    /*0000486c*/ @ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x164c] 
    /*00004870*/ STR         R3, [SP] 
    /*00004874*/ ADDRL       R2, =_RODATA_STR1_4_164c    @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x164c]
    /*00004878*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #1980]             @ 0x000000000000503c 
    /*0000487c*/ MOV         R1, #1 
    /*00004880*/ MOV         R0, #3 
    /*00004884*/ BL          HI_LOG
    /*00004888*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #1940]             @ 0x0000000000005024 
    /*0000488c*/ BL          osal_up
    /*00004890*/ MOV         R0, #0 
    /*00004894*/ B           BaseIoctl_x43b0
    BaseIoctl_x4898:
    /*00004898*/ @ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000489c*/ LDR         R6, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000048a0*/ ADD         R0, R6, #556                @ 0x22c 
    /*000048a4*/ LDR         R4, [R1] 
    /*000048a8*/ BL          osal_down
    /*000048ac*/ LDR         R3, [R6, #8] 
    /*000048b0*/ CMP         R3, #0 
    /*000048b4*/ BEQ         BaseIoctl_x5174
    /*000048b8*/ LDR         R2, [R6, #16] 
    /*000048bc*/ CMP         R4, R2 
    /*000048c0*/ BCS         BaseIoctl_x4fcc
    /*000048c4*/ LDR         R3, [R3, R4, LSL #2] 
    /*000048c8*/ CMP         R3, #0 
    /*000048cc*/ BEQ         BaseIoctl_x4fcc
    /*000048d0*/ MOV         R2, R5 
    /*000048d4*/ MOV         R0, R4 
    /*000048d8*/ MOV         R1, #12 
    /*000048dc*/ BL          VB_InquirePoolUserCnt
    /*000048e0*/ MOV         R4, R0 
    /*000048e4*/ ADD         R0, R6, #556                @ 0x22c 
    /*000048e8*/ BL          osal_up
    /*000048ec*/ MOV         R0, R4 
    /*000048f0*/ B           BaseIoctl_x43b0
    BaseIoctl_x48f4:
    /*000048f4*/ ADD         R6, R6, R9, LSL #2 
    /*000048f8*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #1828]             @ 0x0000000000005024 
    /*000048fc*/ MOV         R3, #0 
    /*00004900*/ STR         R3, [R6, #560]              @ 0x230 
    /*00004904*/ BL          osal_up
    /*00004908*/ MOVW        R3, #2786                   @ 0xae2 
    /*0000490c*/ @ MOVW        R2, #0                      @ .LC158 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18e0] 
    /*00004910*/ STR         R3, [SP] 
    /*00004914*/ ADDRL       R2, =_RODATA_STR1_4_18e0    @ .LC158 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18e0]
    /*00004918*/ STR         R9, [SP, #4] 
    /*0000491c*/ MOV         R1, #1 
    /*00004920*/ LDR         R3, WORD_5030               @ LDR         R3, [PC, #1800]             @ 0x0000000000005030 
    /*00004924*/ MOV         R0, #7 
    /*00004928*/ BL          HI_LOG
    BaseIoctl_x492c:
    /*0000492c*/ MOV         R0, R4 
    /*00004930*/ B           BaseIoctl_x43b0
    BaseIoctl_x4934:
    /*00004934*/ ADD         R6, R6, R9 
    /*00004938*/ MOV         IP, #1 
    /*0000493c*/ MOV         R1, IP 
    /*00004940*/ MOVW        R3, #2706                   @ 0xa92 
    /*00004944*/ @ MOVW        R2, #0                      @ .LC150 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1734] 
    /*00004948*/ STR         R3, [SP] 
    /*0000494c*/ ADDRL       R2, =_RODATA_STR1_4_1734    @ .LC150 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1734]
    /*00004950*/ LDR         R3, WORD_5034               @ LDR         R3, [PC, #1756]             @ 0x0000000000005034 
    /*00004954*/ STR         IP, [R6, #560]              @ 0x230 
    /*00004958*/ MOV         R0, #7 
    /*0000495c*/ BL          HI_LOG
    /*00004960*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #1724]             @ 0x0000000000005024 
    /*00004964*/ BL          osal_up
    /*00004968*/ B           BaseIoctl_x492c
    BaseIoctl_x496c:
    /*0000496c*/ ADD         R0, R4, #63                 @ 0x3f 
    /*00004970*/ MOV         R2, #1 
    /*00004974*/ ADD         R1, SP, #36                 @ 0x24 
    /*00004978*/ BL          osal_copy_to_user
    /*0000497c*/ CMP         R0, #0 
    /*00004980*/ BNE         BaseIoctl_x474c
    /*00004984*/ MOV         R1, R6 
    /*00004988*/ LDR         R0, [R5, #4] 
    /*0000498c*/ MOV         R2, #64                     @ 0x40 
    /*00004990*/ BL          osal_copy_to_user
    /*00004994*/ CMP         R0, #0 
    /*00004998*/ BEQ         BaseIoctl_x46b4
    /*0000499c*/ @ MOVW        R0, #0                      @ .LC145 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1678] 
    /*000049a0*/ ADDRL       R0, =_RODATA_STR1_4_1678    @ .LC145 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1678]
    /*000049a4*/ BL          osal_printk
    /*000049a8*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #1652]             @ 0x0000000000005024 
    /*000049ac*/ BL          osal_up
    /*000049b0*/ MOVW        R0, #32771                  @ 0x8003 
    /*000049b4*/ MOVT        R0, #40961                  @ 0xa001 
    /*000049b8*/ B           BaseIoctl_x43b0
    BaseIoctl_x49bc:
    /*000049bc*/ MOV         R0, R4 
    /*000049c0*/ ADD         R1, SP, #36                 @ 0x24 
    /*000049c4*/ BL          VB_InquirePoolBusyBlkCnt
    /*000049c8*/ CMP         R0, #0 
    /*000049cc*/ BNE         BaseIoctl_x4cd8
    /*000049d0*/ LDR         R3, [SP, #36]               @ 0x24 
    /*000049d4*/ CMP         R3, #0 
    /*000049d8*/ BEQ         BaseIoctl_x49e8
    /*000049dc*/ LDR         R3, [R6, #552]              @ 0x228 
    /*000049e0*/ CMP         R3, #0 
    /*000049e4*/ BEQ         BaseIoctl_x4da0
    BaseIoctl_x49e8:
    /*000049e8*/ MOV         R0, R5 
    /*000049ec*/ BL          VbDestroyPool               @ BL          0x00000000000019F4 
    /*000049f0*/ MOV         R4, R0 
    /*000049f4*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #1576]             @ 0x0000000000005024 
    /*000049f8*/ BL          osal_up
    /*000049fc*/ MOV         R0, R4 
    /*00004a00*/ B           BaseIoctl_x43b0
    BaseIoctl_x4a04:
    /*00004a04*/ MOVW        R3, #3580                   @ 0xdfc 
    /*00004a08*/ MOV         R1, #1 
    /*00004a0c*/ STR         R3, [SP] 
    /*00004a10*/ @ MOVW        R2, #0                      @ .LC135 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1414] 
    /*00004a14*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #1568]             @ 0x000000000000503c 
    /*00004a18*/ ADDRL       R2, =_RODATA_STR1_4_1414    @ .LC135 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1414]
    /*00004a1c*/ STR         R4, [SP, #4] 
    /*00004a20*/ MOV         R0, #3 
    /*00004a24*/ BL          HI_LOG
    /*00004a28*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #1524]             @ 0x0000000000005024 
    /*00004a2c*/ BL          osal_up
    /*00004a30*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004a34*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004a38*/ B           BaseIoctl_x43b0
    BaseIoctl_x4a3c:
    /*00004a3c*/ MOV         R0, R6 
    /*00004a40*/ BL          VB_Handle2Phys
    /*00004a44*/ STRD        R0, [R5] 
    /*00004a48*/ ADD         R0, R4, #556                @ 0x22c 
    /*00004a4c*/ BL          osal_up
    /*00004a50*/ MOV         R0, #0 
    /*00004a54*/ B           BaseIoctl_x43b0
    BaseIoctl_x4a58:
    /*00004a58*/ MOV         R0, #0 
    /*00004a5c*/ MOV         R1, #0 
    /*00004a60*/ MOVW        R3, #3769                   @ 0xeb9 
    /*00004a64*/ STRD        R0, [R5] 
    /*00004a68*/ @ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x164c] 
    /*00004a6c*/ STR         R3, [SP] 
    /*00004a70*/ ADDRL       R2, =_RODATA_STR1_4_164c    @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x164c]
    /*00004a74*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #1472]             @ 0x000000000000503c 
    /*00004a78*/ MOV         R1, #1 
    /*00004a7c*/ MOV         R0, #3 
    /*00004a80*/ BL          HI_LOG
    /*00004a84*/ ADD         R0, R4, #556                @ 0x22c 
    /*00004a88*/ BL          osal_up
    /*00004a8c*/ MOV         R0, #0 
    /*00004a90*/ B           BaseIoctl_x43b0
    BaseIoctl_x4a94:
    /*00004a94*/ LDR         R0, [R6, #16] 
    /*00004a98*/ MOV         R1, #0 
    /*00004a9c*/ LDR         R5, WORD_502c               @ LDR         R5, [PC, #1416]             @ 0x000000000000502c 
    /*00004aa0*/ LSL         R0, R0, #2 
    /*00004aa4*/ STR         R5, [R6, #540]              @ 0x21c 
    /*00004aa8*/ STR         R5, [R6, #544]              @ 0x220 
    /*00004aac*/ BL          osal_kmalloc
    /*00004ab0*/ LDR         R3, [R6, #16] 
    /*00004ab4*/ MOV         R1, #0 
    /*00004ab8*/ STR         R0, [R6, #8] 
    /*00004abc*/ LSL         R0, R3, #2 
    /*00004ac0*/ BL          osal_kmalloc
    /*00004ac4*/ LDR         R3, [R6, #8] 
    /*00004ac8*/ CMP         R3, #0 
    /*00004acc*/ CMPNE       R0, #0 
    /*00004ad0*/ STR         R0, [R6, #536]              @ 0x218 
    /*00004ad4*/ MOVEQ       R8, #1 
    /*00004ad8*/ MOVNE       R8, #0 
    /*00004adc*/ BEQ         BaseIoctl_x51e4
    /*00004ae0*/ LDR         R2, [R6, #16] 
    /*00004ae4*/ MOV         R1, R8 
    /*00004ae8*/ MOV         R0, R3 
    /*00004aec*/ @ MOVW        R7, #0                      @ .LC149 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x172c] 
    /*00004af0*/ SUB         R4, R5, #500                @ 0x1f4 
    /*00004af4*/ MOV         R9, R8 
    /*00004af8*/ LSL         R2, R2, #2 
    /*00004afc*/ ADDRL       R7, =_RODATA_STR1_4_172c    @ .LC149 [R_ARM_MOVT_ABS: .rodata.str1.4+0x172c]
    /*00004b00*/ BL          osal_memset
    /*00004b04*/ LDR         R2, [R6, #16] 
    /*00004b08*/ MOV         R1, R8 
    /*00004b0c*/ LDR         R0, [R6, #536]              @ 0x218 
    /*00004b10*/ ADD         R5, R5, #12 
    /*00004b14*/ MVN         R8, #0 
    /*00004b18*/ LSL         R2, R2, #2 
    /*00004b1c*/ BL          osal_memset
    /*00004b20*/ B           BaseIoctl_x4b30
    BaseIoctl_x4b24:
    /*00004b24*/ ADD         R4, R4, #32 
    /*00004b28*/ CMP         R5, R4 
    /*00004b2c*/ BEQ         BaseIoctl_x4bec
    BaseIoctl_x4b30:
    /*00004b30*/ LDR         R0, [R4, #-8] 
    /*00004b34*/ CMP         R0, #0 
    /*00004b38*/ BEQ         BaseIoctl_x4b24
    /*00004b3c*/ LDRD        R2, [R4, #-16] 
    /*00004b40*/ ORRS        R1, R2, R3 
    /*00004b44*/ BEQ         BaseIoctl_x4b24
    /*00004b48*/ LDR         R1, [R4, #-4] 
    /*00004b4c*/ ADD         IP, SP, #36                 @ 0x24 
    /*00004b50*/ STR         R7, [SP, #20] 
    /*00004b54*/ STR         R8, [SP, #16] 
    /*00004b58*/ STR         R1, [SP, #24] 
    /*00004b5c*/ MOV         R1, #1 
    /*00004b60*/ STR         R4, [SP, #12] 
    /*00004b64*/ STR         R9, [SP, #8] 
    /*00004b68*/ STM         SP, {R1, IP} 
    /*00004b6c*/ BL          VbCreatePool                @ BL          0x0000000000000E0C 
    /*00004b70*/ CMP         R0, #0 
    /*00004b74*/ BEQ         BaseIoctl_x4b24
    /*00004b78*/ LDR         R3, [R6, #16] 
    /*00004b7c*/ CMP         R3, #0 
    /*00004b80*/ MOVNE       R4, #0 
    /*00004b84*/ BEQ         BaseIoctl_x4bac
    BaseIoctl_x4b88:
    /*00004b88*/ LDR         R3, [R6, #8] 
    /*00004b8c*/ LDR         R0, [R3, R4, LSL #2] 
    /*00004b90*/ ADD         R4, R4, #1 
    /*00004b94*/ CMP         R0, #0 
    /*00004b98*/ BEQ         BaseIoctl_x4ba0
    /*00004b9c*/ BL          VbDestroyPool               @ BL          0x00000000000019F4 
    BaseIoctl_x4ba0:
    /*00004ba0*/ LDR         R3, [R6, #16] 
    /*00004ba4*/ CMP         R4, R3 
    /*00004ba8*/ BCC         BaseIoctl_x4b88
    BaseIoctl_x4bac:
    /*00004bac*/ LDR         R0, [R6, #536]              @ 0x218 
    /*00004bb0*/ CMP         R0, #0 
    /*00004bb4*/ BEQ         BaseIoctl_x4bbc
    /*00004bb8*/ BL          osal_kfree
    BaseIoctl_x4bbc:
    /*00004bbc*/ LDR         R0, [R6, #8] 
    /*00004bc0*/ CMP         R0, #0 
    /*00004bc4*/ BEQ         BaseIoctl_x4bcc
    /*00004bc8*/ BL          osal_kfree
    BaseIoctl_x4bcc:
    /*00004bcc*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #1104]             @ 0x0000000000005024 
    /*00004bd0*/ MOV         R3, #0 
    /*00004bd4*/ STR         R3, [R6, #536]              @ 0x218 
    /*00004bd8*/ STR         R3, [R6, #8] 
    /*00004bdc*/ BL          osal_up
    /*00004be0*/ MOVW        R0, #32780                  @ 0x800c 
    /*00004be4*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004be8*/ B           BaseIoctl_x43b0
    BaseIoctl_x4bec:
    /*00004bec*/ MOVW        R3, #2985                   @ 0xba9 
    /*00004bf0*/ @ MOVW        R2, #0                      @ .LC150 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1734] 
    /*00004bf4*/ STR         R3, [SP] 
    /*00004bf8*/ ADDRL       R2, =_RODATA_STR1_4_1734    @ .LC150 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1734]
    /*00004bfc*/ LDR         R3, WORD_5038               @ LDR         R3, [PC, #1076]             @ 0x0000000000005038 
    /*00004c00*/ MOV         R1, #1 
    /*00004c04*/ MOV         R0, #7 
    /*00004c08*/ BL          HI_LOG
    /*00004c0c*/ B           BaseIoctl_x46b4
    BaseIoctl_x4c10:
    /*00004c10*/ @ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x164c] 
    /*00004c14*/ ADDRL       R2, =_RODATA_STR1_4_164c    @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x164c]
    /*00004c18*/ MOVW        R3, #3852                   @ 0xf0c 
    /*00004c1c*/ STR         R3, [SP] 
    BaseIoctl_x4c20:
    /*00004c20*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #1044]             @ 0x000000000000503c 
    /*00004c24*/ MOV         R1, #1 
    /*00004c28*/ MOV         R0, #3 
    /*00004c2c*/ BL          HI_LOG
    /*00004c30*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #1004]             @ 0x0000000000005024 
    /*00004c34*/ BL          osal_up
    /*00004c38*/ MOVW        R0, #32773                  @ 0x8005 
    /*00004c3c*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004c40*/ B           BaseIoctl_x43b0
    BaseIoctl_x4c44:
    /*00004c44*/ ADD         R3, R5, #32 
    /*00004c48*/ MOV         R2, #12 
    /*00004c4c*/ LDRD        R0, [R5, #8] 
    /*00004c50*/ BL          VB_GetBlkBySize
    /*00004c54*/ CMN         R0, #1 
    /*00004c58*/ STR         R0, [R5, #20] 
    /*00004c5c*/ BNE         BaseIoctl_x46b4
    /*00004c60*/ MOVW        R3, #3643                   @ 0xe3b 
    /*00004c64*/ STR         R3, [SP] 
    /*00004c68*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #972]              @ 0x000000000000503c 
    /*00004c6c*/ @ MOVW        R2, #0                      @ .LC139 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1528] 
    /*00004c70*/ ADDRL       R2, =_RODATA_STR1_4_1528    @ .LC139 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1528]
    /*00004c74*/ MOV         R1, #1 
    /*00004c78*/ MOV         R0, #4 
    BaseIoctl_x4c7c:
    /*00004c7c*/ BL          HI_LOG
    /*00004c80*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #924]              @ 0x0000000000005024 
    /*00004c84*/ BL          osal_up
    /*00004c88*/ MOVW        R0, #32781                  @ 0x800d 
    /*00004c8c*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004c90*/ B           BaseIoctl_x43b0
    BaseIoctl_x4c94:
    /*00004c94*/ MVN         R4, #0 
    /*00004c98*/ MOVW        R3, #3816                   @ 0xee8 
    BaseIoctl_x4c9c:
    /*00004c9c*/ STR         R4, [R5] 
    /*00004ca0*/ MOV         R1, #1 
    /*00004ca4*/ STR         R3, [SP] 
    /*00004ca8*/ @ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x164c] 
    /*00004cac*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #904]              @ 0x000000000000503c 
    /*00004cb0*/ ADDRL       R2, =_RODATA_STR1_4_164c    @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x164c]
    /*00004cb4*/ MOV         R0, #3 
    /*00004cb8*/ BL          HI_LOG
    /*00004cbc*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #864]              @ 0x0000000000005024 
    /*00004cc0*/ BL          osal_up
    /*00004cc4*/ MOV         R0, R4 
    /*00004cc8*/ B           BaseIoctl_x43b0
    BaseIoctl_x4ccc:
    /*00004ccc*/ MVN         R4, #0 
    /*00004cd0*/ MOV         R3, #3904                   @ 0xf40 
    /*00004cd4*/ B           BaseIoctl_x4c9c
    BaseIoctl_x4cd8:
    /*00004cd8*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #836]              @ 0x0000000000005024 
    /*00004cdc*/ BL          osal_up
    /*00004ce0*/ MOVW        R0, #32777                  @ 0x8009 
    /*00004ce4*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004ce8*/ B           BaseIoctl_x43b0
    BaseIoctl_x4cec:
    /*00004cec*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #816]              @ 0x0000000000005024 
    /*00004cf0*/ BL          osal_up
    BaseIoctl_x4cf4:
    /*00004cf4*/ MOVW        R0, #32777                  @ 0x8009 
    /*00004cf8*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004cfc*/ B           BaseIoctl_x43b0
    BaseIoctl_x4d00:
    /*00004d00*/ STR         R8, [SP, #4] 
    /*00004d04*/ MOVW        R3, #2681                   @ 0xa79 
    /*00004d08*/ @ MOVW        R2, #0                      @ .LC155 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1858] 
    /*00004d0c*/ STR         R3, [SP] 
    /*00004d10*/ ADDRL       R2, =_RODATA_STR1_4_1858    @ .LC155 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1858]
    /*00004d14*/ LDR         R3, WORD_5034               @ LDR         R3, [PC, #792]              @ 0x0000000000005034 
    /*00004d18*/ MOV         R0, #6 
    /*00004d1c*/ BL          HI_LOG
    /*00004d20*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #764]              @ 0x0000000000005024 
    /*00004d24*/ BL          osal_up
    /*00004d28*/ B           BaseIoctl_x492c
    BaseIoctl_x4d2c:
    /*00004d2c*/ MOVW        R3, #2723                   @ 0xaa3 
    /*00004d30*/ MOV         R1, #1 
    /*00004d34*/ STR         R3, [SP] 
    /*00004d38*/ @ MOVW        R2, #0                      @ .LC157 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18a0] 
    /*00004d3c*/ LDR         R3, WORD_5034               @ LDR         R3, [PC, #752]              @ 0x0000000000005034 
    /*00004d40*/ ADDRL       R2, =_RODATA_STR1_4_18a0    @ .LC157 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18a0]
    /*00004d44*/ STR         R8, [SP, #4] 
    /*00004d48*/ MOV         R0, #3 
    /*00004d4c*/ BL          HI_LOG
    /*00004d50*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #716]              @ 0x0000000000005024 
    /*00004d54*/ MOVW        R4, #32780                  @ 0x800c 
    /*00004d58*/ BL          osal_up
    /*00004d5c*/ MOVT        R4, #40961                  @ 0xa001 
    /*00004d60*/ B           BaseIoctl_x492c
    BaseIoctl_x4d64:
    /*00004d64*/ MOV         R0, #65024                  @ 0xfe00 
    /*00004d68*/ MOVT        R0, #65535                  @ 0xffff 
    /*00004d6c*/ B           BaseIoctl_x43b0
    BaseIoctl_x4d70:
    /*00004d70*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*00004d74*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00004d78*/ MOV         R1, #1 
    /*00004d7c*/ MVN         R3, #0 
    /*00004d80*/ STR         R3, [R5] 
    BaseIoctl_x4d84:
    /*00004d84*/ MOV         R0, #4 
    /*00004d88*/ BL          HI_LOG
    /*00004d8c*/ ADD         R0, R6, #556                @ 0x22c 
    /*00004d90*/ BL          osal_up
    /*00004d94*/ MOVW        R0, #32784                  @ 0x8010 
    /*00004d98*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004d9c*/ B           BaseIoctl_x43b0
    BaseIoctl_x4da0:
    /*00004da0*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #636]              @ 0x0000000000005024 
    /*00004da4*/ BL          osal_up
    /*00004da8*/ MOVW        R3, #3615                   @ 0xe1f 
    /*00004dac*/ @ MOVW        R2, #0                      @ .LC138 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14c4] 
    /*00004db0*/ STR         R3, [SP] 
    /*00004db4*/ ADDRL       R2, =_RODATA_STR1_4_14c4    @ .LC138 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14c4]
    /*00004db8*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #636]              @ 0x000000000000503c 
    /*00004dbc*/ MOV         R1, #1 
    /*00004dc0*/ MOV         R0, #3 
    /*00004dc4*/ BL          HI_LOG
    /*00004dc8*/ MOVW        R0, #32777                  @ 0x8009 
    /*00004dcc*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004dd0*/ B           BaseIoctl_x43b0
    BaseIoctl_x4dd4:
    /*00004dd4*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #584]              @ 0x0000000000005024 
    /*00004dd8*/ BL          osal_up
    /*00004ddc*/ MOVW        R3, #2774                   @ 0xad6 
    /*00004de0*/ @ MOVW        R2, #0                      @ .LC138 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14c4] 
    /*00004de4*/ STR         R3, [SP] 
    /*00004de8*/ ADDRL       R2, =_RODATA_STR1_4_14c4    @ .LC138 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14c4]
    /*00004dec*/ LDR         R3, WORD_5030               @ LDR         R3, [PC, #572]              @ 0x0000000000005030 
    /*00004df0*/ MOV         R1, #1 
    /*00004df4*/ MOV         R0, #3 
    /*00004df8*/ BL          HI_LOG
    /*00004dfc*/ B           BaseIoctl_x4cf4
    BaseIoctl_x4e00:
    /*00004e00*/ MOVW        R3, #3528                   @ 0xdc8 
    /*00004e04*/ @ MOVW        R2, #0                      @ .LC132 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1398] 
    /*00004e08*/ STR         R3, [SP] 
    /*00004e0c*/ ADDRL       R2, =_RODATA_STR1_4_1398    @ .LC132 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1398]
    /*00004e10*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #548]              @ 0x000000000000503c 
    /*00004e14*/ MOV         R1, #1 
    /*00004e18*/ MOV         R0, #3 
    /*00004e1c*/ BL          HI_LOG
    /*00004e20*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004e24*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004e28*/ B           BaseIoctl_x43b0
    BaseIoctl_x4e2c:
    /*00004e2c*/ MOV         R2, #23 
    /*00004e30*/ MOVW        R3, #2646                   @ 0xa56 
    /*00004e34*/ STR         R2, [SP, #4] 
    /*00004e38*/ MOV         R1, #1 
    /*00004e3c*/ STR         R3, [SP] 
    /*00004e40*/ @ MOVW        R2, #0                      @ .LC151 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x175c] 
    /*00004e44*/ LDR         R3, WORD_5034               @ LDR         R3, [PC, #488]              @ 0x0000000000005034 
    /*00004e48*/ ADDRL       R2, =_RODATA_STR1_4_175c    @ .LC151 [R_ARM_MOVT_ABS: .rodata.str1.4+0x175c]
    /*00004e4c*/ MOV         R0, #3 
    /*00004e50*/ MVN         R4, #0 
    /*00004e54*/ BL          HI_LOG
    /*00004e58*/ B           BaseIoctl_x492c
    BaseIoctl_x4e5c:
    /*00004e5c*/ MOVW        R3, #2667                   @ 0xa6b 
    /*00004e60*/ MOV         R1, #1 
    /*00004e64*/ STR         R3, [SP] 
    /*00004e68*/ @ MOVW        R2, #0                      @ .LC153 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17d8] 
    /*00004e6c*/ LDR         R3, WORD_5034               @ LDR         R3, [PC, #448]              @ 0x0000000000005034 
    /*00004e70*/ ADDRL       R2, =_RODATA_STR1_4_17d8    @ .LC153 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17d8]
    /*00004e74*/ MOV         R0, #6 
    /*00004e78*/ BL          HI_LOG
    /*00004e7c*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #416]              @ 0x0000000000005024 
    /*00004e80*/ BL          osal_up
    /*00004e84*/ B           BaseIoctl_x492c
    BaseIoctl_x4e88:
    /*00004e88*/ MOV         R2, #23 
    /*00004e8c*/ MOVW        R3, #2737                   @ 0xab1 
    /*00004e90*/ STR         R2, [SP, #8] 
    /*00004e94*/ MOV         R1, #1 
    /*00004e98*/ STR         R3, [SP] 
    /*00004e9c*/ @ MOVW        R2, #0                      @ .LC129 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12ec] 
    /*00004ea0*/ STR         R9, [SP, #4] 
    /*00004ea4*/ ADDRL       R2, =_RODATA_STR1_4_12ec    @ .LC129 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12ec]
    /*00004ea8*/ LDR         R3, WORD_5030               @ LDR         R3, [PC, #384]              @ 0x0000000000005030 
    /*00004eac*/ MOV         R0, #3 
    /*00004eb0*/ BL          HI_LOG
    /*00004eb4*/ MVN         R0, #0 
    /*00004eb8*/ B           BaseIoctl_x43b0
    BaseIoctl_x4ebc:
    /*00004ebc*/ ADD         R0, R6, #556                @ 0x22c 
    /*00004ec0*/ BL          osal_up
    /*00004ec4*/ MOV         R3, #2752                   @ 0xac0 
    /*00004ec8*/ @ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x914] 
    /*00004ecc*/ STR         R3, [SP] 
    /*00004ed0*/ ADDRL       R2, =_RODATA_STR1_4_0914    @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x914]
    /*00004ed4*/ LDR         R3, WORD_5030               @ LDR         R3, [PC, #340]              @ 0x0000000000005030 
    /*00004ed8*/ MOV         R1, #1 
    /*00004edc*/ MOV         R0, #6 
    /*00004ee0*/ BL          HI_LOG
    /*00004ee4*/ MOV         R0, R7 
    /*00004ee8*/ B           BaseIoctl_x43b0
    BaseIoctl_x4eec:
    /*00004eec*/ STR         R3, [SP, #4] 
    /*00004ef0*/ @ MOVW        R2, #0                      @ .LC133 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13c8] 
    /*00004ef4*/ MOVW        R3, #3540                   @ 0xdd4 
    /*00004ef8*/ ADDRL       R2, =_RODATA_STR1_4_13c8    @ .LC133 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13c8]
    /*00004efc*/ STR         R3, [SP] 
    /*00004f00*/ MOV         R1, #1 
    /*00004f04*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #304]              @ 0x000000000000503c 
    /*00004f08*/ MOV         R0, #3 
    /*00004f0c*/ BL          HI_LOG
    /*00004f10*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004f14*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004f18*/ B           BaseIoctl_x43b0
    BaseIoctl_x4f1c:
    /*00004f1c*/ MOVW        R3, #3726                   @ 0xe8e 
    /*00004f20*/ MOV         R1, #1 
    /*00004f24*/ STR         R3, [SP] 
    /*00004f28*/ @ MOVW        R2, #0                      @ .LC125 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1220] 
    /*00004f2c*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #264]              @ 0x000000000000503c 
    /*00004f30*/ ADDRL       R2, =_RODATA_STR1_4_1220    @ .LC125 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1220]
    /*00004f34*/ MOV         R0, #3 
    /*00004f38*/ BL          HI_LOG
    /*00004f3c*/ ADD         R0, R6, #556                @ 0x22c 
    /*00004f40*/ BL          osal_up
    /*00004f44*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004f48*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004f4c*/ B           BaseIoctl_x43b0
    BaseIoctl_x4f50:
    /*00004f50*/ MVN         R4, #0 
    /*00004f54*/ MOVW        R3, #3895                   @ 0xf37 
    BaseIoctl_x4f58:
    /*00004f58*/ STR         R4, [R5] 
    /*00004f5c*/ MOV         R1, #1 
    /*00004f60*/ STR         R3, [SP] 
    /*00004f64*/ @ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x164c] 
    /*00004f68*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #204]              @ 0x000000000000503c 
    /*00004f6c*/ ADDRL       R2, =_RODATA_STR1_4_164c    @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x164c]
    /*00004f70*/ MOV         R0, #3 
    /*00004f74*/ BL          HI_LOG
    /*00004f78*/ ADD         R0, R6, #556                @ 0x22c 
    /*00004f7c*/ BL          osal_up
    /*00004f80*/ MOV         R0, R4 
    /*00004f84*/ B           BaseIoctl_x43b0
    BaseIoctl_x4f88:
    /*00004f88*/ STRD        R2, [SP, #16] 
    /*00004f8c*/ MOV         R1, #1 
    /*00004f90*/ MOVW        R3, #3676                   @ 0xe5c 
    /*00004f94*/ @ MOVW        R2, #0                      @ .LC142 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c4] 
    /*00004f98*/ STR         R3, [SP] 
    /*00004f9c*/ ADDRL       R2, =_RODATA_STR1_4_15c4    @ .LC142 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c4]
    /*00004fa0*/ STRD        R6, [SP, #8] 
    /*00004fa4*/ MOV         R0, #3 
    /*00004fa8*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #140]              @ 0x000000000000503c 
    /*00004fac*/ BL          HI_LOG
    BaseIoctl_x4fb0:
    /*00004fb0*/ MVN         R3, #0 
    /*00004fb4*/ LDR         R0, WORD_5024               @ LDR         R0, [PC, #104]              @ 0x0000000000005024 
    /*00004fb8*/ STR         R3, [R5, #20] 
    /*00004fbc*/ BL          osal_up
    /*00004fc0*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004fc4*/ MOVT        R0, #40961                  @ 0xa001 
    /*00004fc8*/ B           BaseIoctl_x43b0
    BaseIoctl_x4fcc:
    /*00004fcc*/ MOV         R2, #0 
    /*00004fd0*/ MOVW        R3, #3958                   @ 0xf76 
    /*00004fd4*/ STR         R2, [R5] 
    /*00004fd8*/ @ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x164c] 
    /*00004fdc*/ STR         R3, [SP] 
    /*00004fe0*/ ADDRL       R2, =_RODATA_STR1_4_164c    @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x164c]
    /*00004fe4*/ B           BaseIoctl_x475c
    BaseIoctl_x4fe8:
    /*00004fe8*/ LDR         R1, [R5] 
    /*00004fec*/ MOV         R3, #3712                   @ 0xe80 
    /*00004ff0*/ @ MOVW        R2, #0                      @ .LC143 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x161c] 
    /*00004ff4*/ STR         R3, [SP] 
    /*00004ff8*/ ADDRL       R2, =_RODATA_STR1_4_161c    @ .LC143 [R_ARM_MOVT_ABS: .rodata.str1.4+0x161c]
    /*00004ffc*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #56]               @ 0x000000000000503c 
    /*00005000*/ STR         R1, [SP, #4] 
    /*00005004*/ MOV         R0, #3 
    /*00005008*/ MOV         R1, #1 
    /*0000500c*/ BL          HI_LOG
    /*00005010*/ ADD         R0, R6, #556                @ 0x22c 
    /*00005014*/ BL          osal_up
    /*00005018*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000501c*/ MOVT        R0, #40961                  @ 0xa001 
    /*00005020*/ B           BaseIoctl_x43b0
    /*00005024*/ WORD_5024: .word 0x0000022c
    /*00005028*/ WORD_5028: .word 0x000002f0
    /*0000502c*/ WORD_502c: .word 0x0000021c
    /*00005030*/ WORD_5030: .word 0x000002d8
    /*00005034*/ WORD_5034: .word 0x000002c8
    /*00005038*/ WORD_5038: .word 0x000002bc
    /*0000503c*/ WORD_503c: .word 0x000002b0
    /*00005040*/ WORD_5040: .word 0x0000022c
    BaseIoctl_x5044:
    /*00005044*/ MOVW        R3, #3844                   @ 0xf04 
    /*00005048*/ MOV         R1, #1 
    /*0000504c*/ STR         R3, [SP] 
    /*00005050*/ @ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x164c] 
    /*00005054*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #-32]              @ 0x000000000000503c 
    /*00005058*/ ADDRL       R2, =_RODATA_STR1_4_164c    @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x164c]
    /*0000505c*/ MOV         R0, #3 
    /*00005060*/ BL          HI_LOG
    /*00005064*/ ADD         R0, R4, #556                @ 0x22c 
    /*00005068*/ BL          osal_up
    /*0000506c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005070*/ MOVT        R0, #40961                  @ 0xa001 
    /*00005074*/ B           BaseIoctl_x43b0
    BaseIoctl_x5078:
    /*00005078*/ MOVW        R3, #2745                   @ 0xab9 
    /*0000507c*/ @ MOVW        R2, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8e4] 
    /*00005080*/ STR         R3, [SP] 
    /*00005084*/ ADDRL       R2, =_RODATA_STR1_4_08e4    @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8e4]
    /*00005088*/ LDR         R3, WORD_5030               @ LDR         R3, [PC, #-96]              @ 0x0000000000005030 
    /*0000508c*/ MOV         R1, #1 
    /*00005090*/ MOV         R0, #3 
    /*00005094*/ BL          HI_LOG
    /*00005098*/ MOV         R0, #65024                  @ 0xfe00 
    /*0000509c*/ MOVT        R0, #65535                  @ 0xffff 
    /*000050a0*/ B           BaseIoctl_x43b0
    BaseIoctl_x50a4:
    /*000050a4*/ @ MOVW        R2, #0                      @ .LC152 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1798] 
    /*000050a8*/ ADDRL       R2, =_RODATA_STR1_4_1798    @ .LC152 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1798]
    /*000050ac*/ MOVW        R3, #2659                   @ 0xa63 
    /*000050b0*/ STR         R3, [SP] 
    BaseIoctl_x50b4:
    /*000050b4*/ LDR         R3, WORD_5034               @ LDR         R3, [PC, #-136]             @ 0x0000000000005034 
    /*000050b8*/ MOV         R1, #1 
    /*000050bc*/ MOV         R0, #3 
    /*000050c0*/ MOVW        R4, #32784                  @ 0x8010 
    /*000050c4*/ BL          HI_LOG
    /*000050c8*/ LDR         R0, WORD_5040               @ LDR         R0, [PC, #-144]             @ 0x0000000000005040 
    /*000050cc*/ BL          osal_up
    /*000050d0*/ MOVT        R4, #40961                  @ 0xa001 
    /*000050d4*/ B           BaseIoctl_x492c
    BaseIoctl_x50d8:
    /*000050d8*/ MOVW        R3, #2653                   @ 0xa5d 
    /*000050dc*/ @ MOVW        R2, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8e4] 
    /*000050e0*/ STR         R3, [SP] 
    /*000050e4*/ ADDRL       R2, =_RODATA_STR1_4_08e4    @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8e4]
    /*000050e8*/ LDR         R3, WORD_5034               @ LDR         R3, [PC, #-188]             @ 0x0000000000005034 
    /*000050ec*/ MOV         R1, #1 
    /*000050f0*/ MOV         R0, #3 
    /*000050f4*/ MOV         R4, #65024                  @ 0xfe00 
    /*000050f8*/ BL          HI_LOG
    /*000050fc*/ MOVT        R4, #65535                  @ 0xffff 
    /*00005100*/ B           BaseIoctl_x492c
    BaseIoctl_x5104:
    /*00005104*/ MOVW        R3, #2674                   @ 0xa72 
    /*00005108*/ @ MOVW        R2, #0                      @ .LC154 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1814] 
    /*0000510c*/ STR         R3, [SP] 
    /*00005110*/ ADDRL       R2, =_RODATA_STR1_4_1814    @ .LC154 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1814]
    /*00005114*/ B           BaseIoctl_x50b4
    BaseIoctl_x5118:
    /*00005118*/ STR         R0, [SP, #4] 
    /*0000511c*/ MOVW        R3, #3657                   @ 0xe49 
    /*00005120*/ @ MOVW        R2, #0                      @ .LC140 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1558] 
    /*00005124*/ STR         R3, [SP] 
    /*00005128*/ ADDRL       R2, =_RODATA_STR1_4_1558    @ .LC140 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1558]
    /*0000512c*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #-248]             @ 0x000000000000503c 
    /*00005130*/ MOV         R1, #1 
    /*00005134*/ MOV         R0, #3 
    /*00005138*/ BL          HI_LOG
    /*0000513c*/ B           BaseIoctl_x4fb0
    BaseIoctl_x5140:
    /*00005140*/ MOVW        R3, #2942                   @ 0xb7e 
    /*00005144*/ MOV         R1, #1 
    /*00005148*/ STR         R3, [SP] 
    /*0000514c*/ @ MOVW        R2, #0                      @ .LC146 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x169c] 
    /*00005150*/ LDR         R3, WORD_5038               @ LDR         R3, [PC, #-288]             @ 0x0000000000005038 
    /*00005154*/ ADDRL       R2, =_RODATA_STR1_4_169c    @ .LC146 [R_ARM_MOVT_ABS: .rodata.str1.4+0x169c]
    /*00005158*/ MOV         R0, #3 
    /*0000515c*/ BL          HI_LOG
    /*00005160*/ ADD         R0, R6, #556                @ 0x22c 
    /*00005164*/ BL          osal_up
    /*00005168*/ MOVW        R0, #32784                  @ 0x8010 
    /*0000516c*/ MOVT        R0, #40961                  @ 0xa001 
    /*00005170*/ B           BaseIoctl_x43b0
    BaseIoctl_x5174:
    /*00005174*/ MOV         R1, #1 
    /*00005178*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*0000517c*/ MOV         R0, #4 
    /*00005180*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00005184*/ BL          HI_LOG
    /*00005188*/ LDR         R0, WORD_5040               @ LDR         R0, [PC, #-336]             @ 0x0000000000005040 
    /*0000518c*/ BL          osal_up
    /*00005190*/ MOVW        R0, #32784                  @ 0x8010 
    /*00005194*/ MOVT        R0, #40961                  @ 0xa001 
    /*00005198*/ B           BaseIoctl_x43b0
    BaseIoctl_x519c:
    /*0000519c*/ MOVW        R3, #3686                   @ 0xe66 
    /*000051a0*/ @ MOVW        R2, #0                      @ .LC139 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1528] 
    /*000051a4*/ STR         R3, [SP] 
    /*000051a8*/ ADDRL       R2, =_RODATA_STR1_4_1528    @ .LC139 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1528]
    /*000051ac*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #-376]             @ 0x000000000000503c 
    /*000051b0*/ MOV         R1, #1 
    /*000051b4*/ MOV         R0, #3 
    /*000051b8*/ B           BaseIoctl_x4c7c
    BaseIoctl_x51bc:
    /*000051bc*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*000051c0*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*000051c4*/ MOV         R1, #1 
    /*000051c8*/ MOV         R0, #3 
    BaseIoctl_x51cc:
    /*000051cc*/ BL          HI_LOG
    /*000051d0*/ ADD         R0, R4, #556                @ 0x22c 
    /*000051d4*/ BL          osal_up
    /*000051d8*/ MOVW        R0, #32784                  @ 0x8010 
    /*000051dc*/ MOVT        R0, #40961                  @ 0xa001 
    /*000051e0*/ B           BaseIoctl_x43b0
    BaseIoctl_x51e4:
    /*000051e4*/ MOV         R3, #2960                   @ 0xb90 
    /*000051e8*/ @ MOVW        R2, #0                      @ .LC148 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1704] 
    /*000051ec*/ STR         R3, [SP] 
    /*000051f0*/ ADDRL       R2, =_RODATA_STR1_4_1704    @ .LC148 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1704]
    /*000051f4*/ LDR         R3, WORD_5038               @ LDR         R3, [PC, #-452]             @ 0x0000000000005038 
    /*000051f8*/ MOV         R1, #1 
    /*000051fc*/ MOV         R0, #0 
    /*00005200*/ BL          HI_LOG
    /*00005204*/ B           BaseIoctl_x4bac
    BaseIoctl_x5208:
    /*00005208*/ MOVW        R3, #3719                   @ 0xe87 
    /*0000520c*/ MOV         R1, #1 
    /*00005210*/ STR         R3, [SP] 
    /*00005214*/ @ MOVW        R2, #0                      @ .LC141 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x158c] 
    /*00005218*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #-484]             @ 0x000000000000503c 
    /*0000521c*/ ADDRL       R2, =_RODATA_STR1_4_158c    @ .LC141 [R_ARM_MOVT_ABS: .rodata.str1.4+0x158c]
    /*00005220*/ STR         R7, [SP, #4] 
    /*00005224*/ MOV         R0, #3 
    /*00005228*/ BL          HI_LOG
    /*0000522c*/ ADD         R0, R6, #556                @ 0x22c 
    /*00005230*/ BL          osal_up
    /*00005234*/ MOVW        R0, #32773                  @ 0x8005 
    /*00005238*/ MOVT        R0, #40961                  @ 0xa001 
    /*0000523c*/ B           BaseIoctl_x43b0
    BaseIoctl_x5240:
    /*00005240*/ MOV         R0, #0 
    /*00005244*/ MOV         R1, #0 
    /*00005248*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*0000524c*/ STRD        R0, [R5] 
    /*00005250*/ ADDRL       R2, =_RODATA_STR1_4_0030    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30]
    /*00005254*/ MOV         R1, #1 
    /*00005258*/ MOV         R0, #4 
    /*0000525c*/ B           BaseIoctl_x51cc
    BaseIoctl_x5260:
    /*00005260*/ STR         R0, [SP, #4] 
    /*00005264*/ MOVW        R3, #3666                   @ 0xe52 
    /*00005268*/ MOV         R1, #1 
    /*0000526c*/ STR         R3, [SP] 
    /*00005270*/ @ MOVW        R2, #0                      @ .LC141 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x158c] 
    /*00005274*/ LDR         R3, WORD_503c               @ LDR         R3, [PC, #-576]             @ 0x000000000000503c 
    /*00005278*/ ADDRL       R2, =_RODATA_STR1_4_158c    @ .LC141 [R_ARM_MOVT_ABS: .rodata.str1.4+0x158c]
    /*0000527c*/ MOV         R0, #3 
    /*00005280*/ BL          HI_LOG
    /*00005284*/ MVN         R3, #0 
    /*00005288*/ LDR         R0, WORD_5040               @ LDR         R0, [PC, #-592]             @ 0x0000000000005040 
    /*0000528c*/ STR         R3, [R5, #20] 
    /*00005290*/ BL          osal_up
    /*00005294*/ MOVW        R0, #32773                  @ 0x8005 
    /*00005298*/ MOVT        R0, #40961                  @ 0xa001 
    /*0000529c*/ B           BaseIoctl_x43b0
FUNC_END BaseIoctl


FUNC_BEGIN VbGetSupplementConf
    /*000052a0*/ PUSH        {R4, LR} 
    /*000052a4*/ MOV         R2, #4 
    /*000052a8*/ @ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000052ac*/ LDR         R1, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*000052b0*/ BL          osal_memcpy
    /*000052b4*/ MOV         R0, #0 
    /*000052b8*/ POP         {R4, PC} 
FUNC_END VbGetSupplementConf


FUNC_BEGIN VB_Init
    /*000052bc*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000052c0*/ @ MOVW        R0, #0                      @ .LC161 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1990] 
    /*000052c4*/ SUB         SP, SP, #12 
    /*000052c8*/ ADDRL       R0, =_RODATA_STR1_4_1990    @ .LC161 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1990]
    /*000052cc*/ BL          osal_createdev
    /*000052d0*/ @ MOVW        R6, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x3ff0] 
    /*000052d4*/ LDR         R6, BSS_ANCHOR4               @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x3ff0]
    /*000052d8*/ CMP         R0, #0 
    /*000052dc*/ STR         R0, [R6, #-3656]            @ 0xfffff1b8 
    /*000052e0*/ BEQ         VB_Init_x5414
    /*000052e4*/ MOV         R2, #9 
    /*000052e8*/ @ MOVW        R4, #0                      @ .LANCHOR3 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000052ec*/ STR         R2, [R0, #52]               @ 0x34 
    /*000052f0*/ LDR         R4, DATA_ANCHOR0            @ .LANCHOR3 [R_ARM_MOVT_ABS: .data+0x0]
    /*000052f4*/ STR         R4, [R0, #56]               @ 0x38 
    /*000052f8*/ BL          osal_registerdevice
    /*000052fc*/ SUBS        R7, R0, #0 
    /*00005300*/ BNE         VB_Init_x539c
    /*00005304*/ ADD         R0, R4, #32 
    /*00005308*/ BL          CMPI_RegisterModule
    /*0000530c*/ SUBS        R5, R0, #0 
    /*00005310*/ BNE         VB_Init_x53e8
    /*00005314*/ @ MOVW        R0, #0                      @ .LC161 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1990] 
    /*00005318*/ MOV         R1, R5 
    /*0000531c*/ ADDRL       R0, =_RODATA_STR1_4_1990    @ .LC161 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1990]
    /*00005320*/ BL          osal_create_proc_entry
    /*00005324*/ SUBS        IP, R0, #0 
    /*00005328*/ BEQ         VB_Init_x5434
    /*0000532c*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005330*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00005334*/ MOV         R2, #520                    @ 0x208 
    /*00005338*/ MOV         R1, R5 
    /*0000533c*/ ADD         R0, R4, #16 
    /*00005340*/ @ MOVW        R3, #0                      @ VbProcShow [R_ARM_MOVW_ABS_NC: .text+0x2798] 
    /*00005344*/ ADDRL       R3, =_TEXT_2798             @ VbProcShow [R_ARM_MOVT_ABS: .text+0x2798]
    /*00005348*/ STR         R3, [IP, #60]               @ 0x3c 
    /*0000534c*/ BL          osal_memset
    /*00005350*/ MOV         R1, #1 
    /*00005354*/ ADD         R0, R4, #556                @ 0x22c 
    /*00005358*/ STR         R5, [R4, #652]              @ 0x28c 
    /*0000535c*/ STR         R5, [R4, #8] 
    /*00005360*/ STR         R5, [R4, #536]              @ 0x218 
    /*00005364*/ BL          osal_sema_init
    /*00005368*/ ADD         R0, R4, #548                @ 0x224 
    /*0000536c*/ BL          osal_spin_lock_init
    /*00005370*/ MOVW        R3, #4407                   @ 0x1137 
    /*00005374*/ @ MOVW        R2, #0                      @ .LC165 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a38] 
    /*00005378*/ STR         R3, [SP] 
    /*0000537c*/ ADDRL       R2, =_RODATA_STR1_4_1a38    @ .LC165 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a38]
    /*00005380*/ LDR         R3, WORD_545c               @ LDR         R3, [PC, #212]              @ 0x000000000000545c 
    /*00005384*/ MOV         R1, #1 
    /*00005388*/ MOV         R0, #6 
    /*0000538c*/ BL          HI_LOG
    VB_Init_x5390:
    /*00005390*/ MOV         R0, R5 
    /*00005394*/ ADD         SP, SP, #12 
    /*00005398*/ POP         {R4, R5, R6, R7, PC} 
    VB_Init_x539c:
    /*0000539c*/ MOVW        R3, #4378                   @ 0x111a 
    /*000053a0*/ @ MOVW        R2, #0                      @ .LC163 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19c8] 
    /*000053a4*/ STR         R3, [SP] 
    /*000053a8*/ ADDRL       R2, =_RODATA_STR1_4_19c8    @ .LC163 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19c8]
    /*000053ac*/ LDR         R3, WORD_545c               @ LDR         R3, [PC, #168]              @ 0x000000000000545c 
    /*000053b0*/ MOV         R1, #1 
    /*000053b4*/ MOV         R0, #0 
    /*000053b8*/ BL          HI_LOG
    VB_Init_x53bc:
    /*000053bc*/ LDR         R0, [R6, #-3656]            @ 0xfffff1b8 
    /*000053c0*/ BL          osal_destroydev
    /*000053c4*/ MOV         R3, #0 
    /*000053c8*/ STR         R3, [R6, #-3656]            @ 0xfffff1b8 
    VB_Init_x53cc:
    /*000053cc*/ @ MOVW        R1, #0                      @ .LC166 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a60] 
    /*000053d0*/ @ MOVW        R0, #0                      @ .LC167 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a6c] 
    /*000053d4*/ ADDRL       R1, =_RODATA_STR1_4_1a60    @ .LC166 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a60]
    /*000053d8*/ ADDRL       R0, =_RODATA_STR1_4_1a6c    @ .LC167 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a6c]
    /*000053dc*/ BL          osal_printk
    /*000053e0*/ MVN         R5, #0 
    /*000053e4*/ B           VB_Init_x5390
    VB_Init_x53e8:
    /*000053e8*/ MOVW        R3, #4383                   @ 0x111f 
    /*000053ec*/ @ MOVW        R2, #0                      @ .LC164 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a00] 
    /*000053f0*/ STR         R3, [SP] 
    /*000053f4*/ MOV         R0, R7 
    /*000053f8*/ ADDRL       R2, =_RODATA_STR1_4_1a00    @ .LC164 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a00]
    /*000053fc*/ LDR         R3, WORD_545c               @ LDR         R3, [PC, #88]               @ 0x000000000000545c 
    /*00005400*/ MOV         R1, #1 
    /*00005404*/ BL          HI_LOG
    VB_Init_x5408:
    /*00005408*/ LDR         R0, [R6, #-3656]            @ 0xfffff1b8 
    /*0000540c*/ BL          osal_deregisterdevice
    /*00005410*/ B           VB_Init_x53bc
    VB_Init_x5414:
    /*00005414*/ MOVW        R3, #4371                   @ 0x1113 
    /*00005418*/ @ MOVW        R2, #0                      @ .LC162 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1994] 
    /*0000541c*/ STR         R3, [SP] 
    /*00005420*/ ADDRL       R2, =_RODATA_STR1_4_1994    @ .LC162 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1994]
    /*00005424*/ LDR         R3, WORD_545c               @ LDR         R3, [PC, #48]               @ 0x000000000000545c 
    /*00005428*/ MOV         R1, #1 
    /*0000542c*/ BL          HI_LOG
    /*00005430*/ B           VB_Init_x53cc
    VB_Init_x5434:
    /*00005434*/ MOVW        R3, #4391                   @ 0x1127 
    /*00005438*/ MOV         R1, #1 
    /*0000543c*/ STR         R3, [SP] 
    /*00005440*/ @ MOVW        R2, #0                      @ .LC164 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a00] 
    /*00005444*/ LDR         R3, WORD_545c               @ LDR         R3, [PC, #16]               @ 0x000000000000545c 
    /*00005448*/ ADDRL       R2, =_RODATA_STR1_4_1a00    @ .LC164 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a00]
    /*0000544c*/ BL          HI_LOG
    /*00005450*/ MOV         R0, #1 
    /*00005454*/ BL          CMPI_UnRegisterModule
    /*00005458*/ B           VB_Init_x5408
    /*0000545c*/ WORD_545c: .word 0x000002e8
FUNC_END VB_Init


FUNC_BEGIN VB_Exit
    /*00005460*/ PUSH        {R4, R5, LR} 
    /*00005464*/ MOV         R0, #1 
    /*00005468*/ SUB         SP, SP, #12 
    /*0000546c*/ @ MOVW        R5, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x3ff0] 
    /*00005470*/ LDR         R5, BSS_ANCHOR4               @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x3ff0]
    /*00005474*/ BL          CMPI_UnRegisterModule
    /*00005478*/ MOV         R1, #0 
    /*0000547c*/ @ MOVW        R0, #0                      @ .LC161 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1990] 
    /*00005480*/ ADDRL       R0, =_RODATA_STR1_4_1990    @ .LC161 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1990]
    /*00005484*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005488*/ BL          osal_remove_proc_entry
    /*0000548c*/ LDR         R0, [R5, #-3656]            @ 0xfffff1b8 
    /*00005490*/ BL          osal_deregisterdevice
    /*00005494*/ LDR         R4, BSS_ANCHOR0               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0]
    /*00005498*/ LDR         R0, [R5, #-3656]            @ 0xfffff1b8 
    /*0000549c*/ BL          osal_destroydev
    /*000054a0*/ BL          VbDoVbExit                  @ BL          0x0000000000001BCC 
    /*000054a4*/ ADD         R0, R4, #556                @ 0x22c 
    /*000054a8*/ BL          osal_sema_destory
    /*000054ac*/ ADD         R0, R4, #548                @ 0x224 
    /*000054b0*/ BL          osal_spin_lock_destory
    /*000054b4*/ MOVW        R3, #4452                   @ 0x1164 
    /*000054b8*/ MOV         R1, #1 
    /*000054bc*/ STR         R3, [SP] 
    /*000054c0*/ @ MOVW        R2, #0                      @ .LC168 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a8c] 
    /*000054c4*/ LDR         R3, WORD_54e0               @ LDR         R3, [PC, #20]               @ 0x00000000000054e0 
    /*000054c8*/ ADDRL       R2, =_RODATA_STR1_4_1a8c    @ .LC168 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a8c]
    /*000054cc*/ MOV         R0, #6 
    /*000054d0*/ BL          HI_LOG
    /*000054d4*/ MOV         R0, #0 
    /*000054d8*/ ADD         SP, SP, #12 
    /*000054dc*/ POP         {R4, R5, PC} 
    /*000054e0*/ WORD_54e0: .word 0x000002f0
FUNC_END VB_Exit


FUNC_BEGIN Open
    /*000054e4*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31ac] 
    /*000054e8*/ LDR         R3, BSS_ANCHOR1               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31ac]
    /*000054ec*/ MOV         R2, #1 
    /*000054f0*/ MOV         R0, #0 
    /*000054f4*/ STR         R2, [R3] 
    /*000054f8*/ BX          LR 
FUNC_END Open


FUNC_BEGIN Close
    /*000054fc*/ @ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31ac] 
    /*00005500*/ LDR         R0, BSS_ANCHOR1               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31ac]
    /*00005504*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*00005508*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*0000550c*/ PUSH        {R4, LR} 
    /*00005510*/ MOV         R2, #4 
    /*00005514*/ STR         R2, [R0], #28 
    /*00005518*/ MOV         R2, #1 
    /*0000551c*/ STR         R2, [R3] 
    /*00005520*/ BL          osal_wakeup
    /*00005524*/ MOV         R0, #0 
    /*00005528*/ POP         {R4, PC} 
FUNC_END Close


FUNC_BEGIN LogProcWrite
    /*0000552c*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00005530*/ SUB         SP, SP, #68                 @ 0x44 
    /*00005534*/ MOV         R6, R2 
    /*00005538*/ MOV         R5, R1 
    /*0000553c*/ MOV         R2, #32 
    /*00005540*/ MOV         R1, #0 
    /*00005544*/ MOV         R0, SP 
    /*00005548*/ MOV         R4, R3 
    /*0000554c*/ BL          memset
    /*00005550*/ MOV         R2, #32 
    /*00005554*/ ADD         R0, SP, #32 
    /*00005558*/ MOV         R1, #0 
    /*0000555c*/ BL          memset
    /*00005560*/ LDRD        R2, [R4] 
    /*00005564*/ CMP         R2, #32 
    /*00005568*/ SBCS        R3, R3, #0 
    /*0000556c*/ MOVGE       R3, #1 
    /*00005570*/ MOVLT       R3, #0 
    /*00005574*/ CMP         R6, #0 
    /*00005578*/ ORRLE       R3, R3, #1 
    /*0000557c*/ CMP         R3, #0 
    /*00005580*/ BNE         LogProcWrite_x5740
    /*00005584*/ CMP         R6, #32 
    /*00005588*/ MOV         R1, R5 
    /*0000558c*/ MOV         R0, SP 
    /*00005590*/ MOVLT       R5, R6 
    /*00005594*/ MOVGE       R5, #32 
    /*00005598*/ MOV         R2, R5 
    /*0000559c*/ BL          osal_copy_from_user
    /*000055a0*/ CMP         R0, #0 
    /*000055a4*/ BNE         LogProcWrite_x5740
    /*000055a8*/ ADD         R3, SP, #64                 @ 0x40 
    /*000055ac*/ ADD         R2, SP, #1 
    /*000055b0*/ ADD         R3, R3, R5 
    /*000055b4*/ ADD         R1, SP, #32 
    /*000055b8*/ STRB        R0, [R3, #-65]              @ 0xffffffbf 
    LogProcWrite_x55bc:
    /*000055bc*/ LDRB        R3, [R2, #-1] 
    /*000055c0*/ ADD         R0, R1, #1 
    /*000055c4*/ CMP         R3, #10 
    /*000055c8*/ BEQ         LogProcWrite_x5630
    /*000055cc*/ CMP         R3, #32 
    /*000055d0*/ ADD         R2, R2, #1 
    /*000055d4*/ BEQ         LogProcWrite_x55bc
    /*000055d8*/ CMP         R3, #0 
    /*000055dc*/ STRB        R3, [R1] 
    /*000055e0*/ MOV         R1, R0 
    /*000055e4*/ BNE         LogProcWrite_x55bc
    LogProcWrite_x55e8:
    /*000055e8*/ ADD         R3, SP, #32 
    /*000055ec*/ B           LogProcWrite_x55fc
    LogProcWrite_x55f0:
    /*000055f0*/ CMP         R2, #61                     @ 0x3d 
    /*000055f4*/ MOV         R4, R3 
    /*000055f8*/ BEQ         LogProcWrite_x5640
    LogProcWrite_x55fc:
    /*000055fc*/ MOV         R4, R3 
    /*00005600*/ LDRB        R2, [R3], #1 
    /*00005604*/ CMP         R2, #0 
    /*00005608*/ BNE         LogProcWrite_x55f0
    /*0000560c*/ LDRB        R3, [R4, #-1] 
    /*00005610*/ CMP         R3, #61                     @ 0x3d 
    /*00005614*/ BEQ         LogProcWrite_x5640
    /*00005618*/ @ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ae4] 
    /*0000561c*/ ADDRL       R0, =_RODATA_STR1_4_1ae4    @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ae4]
    /*00005620*/ BL          osal_printk
    LogProcWrite_x5624:
    /*00005624*/ MOV         R0, R5 
    /*00005628*/ ADD         SP, SP, #68                 @ 0x44 
    /*0000562c*/ POP         {R4, R5, R6, R7, PC} 
    LogProcWrite_x5630:
    /*00005630*/ MOV         R3, #0 
    /*00005634*/ STRB        R3, [R2, #-1] 
    /*00005638*/ STRB        R3, [R1] 
    /*0000563c*/ B           LogProcWrite_x55e8
    LogProcWrite_x5640:
    /*00005640*/ MOV         R3, #0 
    /*00005644*/ MOV         R2, #10 
    /*00005648*/ MOV         R1, R3 
    /*0000564c*/ STRB        R3, [R4, #-1] 
    /*00005650*/ MOV         R0, R4 
    /*00005654*/ BL          osal_strtol
    /*00005658*/ ADD         R1, SP, #32 
    /*0000565c*/ MOV         R7, R0 
    /*00005660*/ @ MOVW        R0, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ab4] 
    /*00005664*/ ADDRL       R0, =_RODATA_STR1_4_1ab4    @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ab4]
    /*00005668*/ BL          osal_strcmp
    /*0000566c*/ CMP         R0, #0 
    /*00005670*/ BEQ         LogProcWrite_x56c4
    /*00005674*/ CMP         R7, #0 
    /*00005678*/ BNE         LogProcWrite_x56dc
    /*0000567c*/ LDRB        R3, [R4] 
    /*00005680*/ CMP         R3, #48                     @ 0x30 
    /*00005684*/ BNE         LogProcWrite_x5748
    LogProcWrite_x5688:
    /*00005688*/ @ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ae0] 
    /*0000568c*/ ADD         R1, SP, #32 
    /*00005690*/ ADDRL       R0, =_RODATA_STR1_4_1ae0    @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ae0]
    /*00005694*/ BL          osal_strcmp
    /*00005698*/ CMP         R0, #0 
    /*0000569c*/ BNE         LogProcWrite_x56f8
    /*000056a0*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*000056a4*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*000056a8*/ ADD         R2, R3, #240                @ 0xf0 
    LogProcWrite_x56ac:
    /*000056ac*/ STR         R7, [R3, #4]! 
    /*000056b0*/ CMP         R2, R3 
    /*000056b4*/ BNE         LogProcWrite_x56ac
    /*000056b8*/ MOV         R0, R5 
    /*000056bc*/ ADD         SP, SP, #68                 @ 0x44 
    /*000056c0*/ POP         {R4, R5, R6, R7, PC} 
    LogProcWrite_x56c4:
    /*000056c4*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*000056c8*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*000056cc*/ MOV         R0, R5 
    /*000056d0*/ STR         R7, [R3] 
    /*000056d4*/ ADD         SP, SP, #68                 @ 0x44 
    /*000056d8*/ POP         {R4, R5, R6, R7, PC} 
    LogProcWrite_x56dc:
    /*000056dc*/ CMP         R7, #7 
    /*000056e0*/ BLE         LogProcWrite_x5688
    /*000056e4*/ @ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1acc] 
    /*000056e8*/ MOV         R1, R7 
    /*000056ec*/ ADDRL       R0, =_RODATA_STR1_4_1acc    @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1acc]
    /*000056f0*/ BL          osal_printk
    /*000056f4*/ B           LogProcWrite_x5688
    LogProcWrite_x56f8:
    /*000056f8*/ MOV         R4, #0 
    /*000056fc*/ B           LogProcWrite_x570c
    LogProcWrite_x5700:
    /*00005700*/ ADD         R4, R4, #1 
    /*00005704*/ CMP         R4, #60                     @ 0x3c 
    /*00005708*/ BEQ         LogProcWrite_x5758
    LogProcWrite_x570c:
    /*0000570c*/ MOV         R0, R4 
    /*00005710*/ BL          CMPI_GetModuleName
    /*00005714*/ ADD         R1, SP, #32 
    /*00005718*/ CMP         R0, #0 
    /*0000571c*/ BEQ         LogProcWrite_x5700
    /*00005720*/ BL          osal_strcmp
    /*00005724*/ CMP         R0, #0 
    /*00005728*/ BNE         LogProcWrite_x5700
    /*0000572c*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*00005730*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*00005734*/ ADD         R3, R3, R4, LSL #2 
    /*00005738*/ STR         R7, [R3, #4] 
    /*0000573c*/ B           LogProcWrite_x5624
    LogProcWrite_x5740:
    /*00005740*/ MVN         R5, #0 
    /*00005744*/ B           LogProcWrite_x5624
    LogProcWrite_x5748:
    /*00005748*/ @ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1abc] 
    /*0000574c*/ ADDRL       R0, =_RODATA_STR1_4_1abc    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1abc]
    /*00005750*/ BL          osal_printk
    /*00005754*/ B           LogProcWrite_x5624
    LogProcWrite_x5758:
    /*00005758*/ ADD         R1, SP, #32 
    /*0000575c*/ @ MOVW        R0, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1af8] 
    /*00005760*/ ADDRL       R0, =_RODATA_STR1_4_1af8    @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1af8]
    /*00005764*/ MOV         R5, R6 
    /*00005768*/ BL          osal_printk
    /*0000576c*/ B           LogProcWrite_x5624
FUNC_END LogProcWrite


FUNC_BEGIN LogProcRead
    /*00005770*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00005774*/ @ MOVW        R1, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b10] 
    /*00005778*/ SUB         SP, SP, #12 
    /*0000577c*/ MOV         R5, R0 
    /*00005780*/ ADDRL       R1, =_RODATA_STR1_4_1b10    @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b10]
    /*00005784*/ LDR         R7, WORD_5834               @ LDR         R7, [PC, #168]              @ 0x0000000000005834 
    /*00005788*/ BL          osal_seq_printf
    /*0000578c*/ MOV         R0, R5 
    /*00005790*/ @ MOVW        R1, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b64] 
    /*00005794*/ ADDRL       R1, =_RODATA_STR1_4_1b64    @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b64]
    /*00005798*/ BL          osal_seq_printf
    /*0000579c*/ @ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31ac] 
    /*000057a0*/ LDR         IP, BSS_ANCHOR1               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31ac]
    /*000057a4*/ MOV         R0, R5 
    /*000057a8*/ @ MOVW        R1, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b88] 
    /*000057ac*/ ADDRL       R1, =_RODATA_STR1_4_1b88    @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b88]
    /*000057b0*/ LDR         R3, [IP, #12] 
    /*000057b4*/ @ MOVW        R6, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bf8] 
    /*000057b8*/ LDR         LR, [IP, #20] 
    /*000057bc*/ ADDRL       R6, =_RODATA_STR1_4_1bf8    @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bf8]
    /*000057c0*/ LDR         R2, [IP, #4] 
    /*000057c4*/ MOV         R4, #0 
    /*000057c8*/ LDR         IP, [IP, #16] 
    /*000057cc*/ STR         LR, [SP, #4] 
    /*000057d0*/ LSR         R2, R2, #10 
    /*000057d4*/ STR         IP, [SP] 
    /*000057d8*/ BL          osal_seq_printf
    /*000057dc*/ MOV         R0, R5 
    /*000057e0*/ @ MOVW        R1, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ba0] 
    /*000057e4*/ ADDRL       R1, =_RODATA_STR1_4_1ba0    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ba0]
    /*000057e8*/ BL          osal_seq_printf
    /*000057ec*/ MOV         R0, R5 
    /*000057f0*/ @ MOVW        R1, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ba4] 
    /*000057f4*/ ADDRL       R1, =_RODATA_STR1_4_1ba4    @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ba4]
    /*000057f8*/ BL          osal_seq_printf
    LogProcRead_x57fc:
    /*000057fc*/ MOV         R0, R4 
    /*00005800*/ BL          CMPI_GetModuleName
    /*00005804*/ MOV         R1, R6 
    /*00005808*/ SUBS        R2, R0, #0 
    /*0000580c*/ MOV         R0, R5 
    /*00005810*/ BEQ         LogProcRead_x581c
    /*00005814*/ LDR         R3, [R7, R4, LSL #2] 
    /*00005818*/ BL          osal_seq_printf
    LogProcRead_x581c:
    /*0000581c*/ ADD         R4, R4, #1 
    /*00005820*/ CMP         R4, #60                     @ 0x3c 
    /*00005824*/ BNE         LogProcRead_x57fc
    /*00005828*/ MOV         R0, #0 
    /*0000582c*/ ADD         SP, SP, #12 
    /*00005830*/ POP         {R4, R5, R6, R7, PC} 
    /*00005834*/ WORD_5834: .word 0x000000d4
FUNC_END LogProcRead


FUNC_BEGIN WaitConditionCallBack
    /*00005838*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31ac] 
    /*0000583c*/ LDR         R3, BSS_ANCHOR1               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31ac]
    /*00005840*/ LDR         R1, [R3, #12] 
    /*00005844*/ LDR         R2, [R3, #16] 
    /*00005848*/ CMP         R1, R2 
    /*0000584c*/ BEQ         WaitConditionCallBack_x5858
    WaitConditionCallBack_x5850:
    /*00005850*/ MOV         R0, #1 
    /*00005854*/ BX          LR 
    WaitConditionCallBack_x5858:
    /*00005858*/ LDR         R3, [R3] 
    /*0000585c*/ CMP         R3, #4 
    /*00005860*/ BEQ         WaitConditionCallBack_x5850
    /*00005864*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*00005868*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*0000586c*/ LDR         R0, [R3] 
    /*00005870*/ CLZ         R0, R0 
    /*00005874*/ LSR         R0, R0, #5 
    /*00005878*/ BX          LR 
FUNC_END WaitConditionCallBack


FUNC_BEGIN Read
    /*0000587c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00005880*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31ac] 
    /*00005884*/ LDR         R4, BSS_ANCHOR1               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31ac]
    /*00005888*/ @ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*0000588c*/ @ MOVW        R6, #0                      @ WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x5838] 
    /*00005890*/ MOV         R3, #2 
    /*00005894*/ SUB         SP, SP, #8 
    /*00005898*/ LDR         R7, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*0000589c*/ ADDRL       R6, =_TEXT_5838             @ WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x5838]
    /*000058a0*/ MOV         R9, R0 
    /*000058a4*/ MOV         R8, R1 
    /*000058a8*/ STR         R3, [R4] 
    /*000058ac*/ B           Read_x58bc
    Read_x58b0:
    /*000058b0*/ BL          osal_wait_timeout_interruptible
    /*000058b4*/ SUBS        R5, R0, #0 
    /*000058b8*/ BLT         Read_x59c4
    Read_x58bc:
    /*000058bc*/ LDR         LR, [R4, #12] 
    /*000058c0*/ MOV         R3, #100                    @ 0x64 
    /*000058c4*/ LDR         IP, [R4, #16] 
    /*000058c8*/ MOV         R2, #0 
    /*000058cc*/ MOV         R1, R6 
    /*000058d0*/ LDR         R0, WORD_59dc               @ LDR         R0, [PC, #260]              @ 0x00000000000059dc 
    /*000058d4*/ CMP         LR, IP 
    /*000058d8*/ BNE         Read_x58f4
    /*000058dc*/ LDR         IP, [R4] 
    /*000058e0*/ CMP         IP, #4 
    /*000058e4*/ BEQ         Read_x58f4
    /*000058e8*/ LDR         IP, [R7] 
    /*000058ec*/ CMP         IP, R2 
    /*000058f0*/ BNE         Read_x58b0
    Read_x58f4:
    /*000058f4*/ LDR         R0, WORD_59e0               @ LDR         R0, [PC, #228]              @ 0x00000000000059e0 
    /*000058f8*/ ADD         R1, SP, #4 
    /*000058fc*/ BL          osal_spin_lock_irqsave
    /*00005900*/ CMP         R8, #0 
    /*00005904*/ BLE         Read_x59d0
    /*00005908*/ LDR         IP, [R4, #16] 
    /*0000590c*/ MOV         R5, #0 
    /*00005910*/ LDR         R2, [R4, #12] 
    /*00005914*/ LDR         R7, WORD_59e0               @ LDR         R7, [PC, #196]              @ 0x00000000000059e0 
    /*00005918*/ CMP         R2, IP 
    /*0000591c*/ BEQ         Read_x59b8
    /*00005920*/ MOV         SL, R5 
    /*00005924*/ B           Read_x598c
    Read_x5928:
    /*00005928*/ LDR         IP, [R4, #8] 
    /*0000592c*/ ADD         R5, R5, #1 
    /*00005930*/ LDRB        R3, [IP, R3] 
    /*00005934*/ STR         R2, [R4, #12] 
    /*00005938*/ STRB        R3, [SP, #3] 
    /*0000593c*/ BL          osal_spin_unlock_irqrestore
    /*00005940*/ MOV         R2, #1 
    /*00005944*/ ADD         R1, SP, #3 
    /*00005948*/ MOV         R0, R6 
    /*0000594c*/ BL          osal_copy_to_user
    /*00005950*/ MOV         R6, R0 
    /*00005954*/ BL          osal_yield
    /*00005958*/ CLZ         R6, R6 
    /*0000595c*/ ADD         R1, SP, #4 
    /*00005960*/ MOV         R0, R7 
    /*00005964*/ BL          osal_spin_lock_irqsave
    /*00005968*/ CMP         R8, R5 
    /*0000596c*/ LSR         R6, R6, #5 
    /*00005970*/ MOVLE       R6, #0 
    /*00005974*/ CMP         R6, #0 
    /*00005978*/ BEQ         Read_x59b8
    /*0000597c*/ LDR         IP, [R4, #16] 
    /*00005980*/ LDR         R2, [R4, #12] 
    /*00005984*/ CMP         IP, R2 
    /*00005988*/ BEQ         Read_x59b8
    Read_x598c:
    /*0000598c*/ LDR         R1, [R4, #20] 
    /*00005990*/ MOV         R3, #0 
    /*00005994*/ ADD         R6, R9, R5 
    /*00005998*/ MOV         R0, R7 
    /*0000599c*/ CMP         R1, R2 
    /*000059a0*/ STRCC       SL, [R4, #12] 
    /*000059a4*/ ADD         R1, SP, #4 
    /*000059a8*/ LDRCS       R3, [R4, #12] 
    /*000059ac*/ CMP         R3, IP 
    /*000059b0*/ ADD         R2, R3, #1 
    /*000059b4*/ BNE         Read_x5928
    Read_x59b8:
    /*000059b8*/ ADD         R1, SP, #4 
    /*000059bc*/ MOV         R0, R7 
    /*000059c0*/ BL          osal_spin_unlock_irqrestore
    Read_x59c4:
    /*000059c4*/ MOV         R0, R5 
    /*000059c8*/ ADD         SP, SP, #8 
    /*000059cc*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    Read_x59d0:
    /*000059d0*/ MOV         R5, #0 
    /*000059d4*/ LDR         R7, WORD_59e0               @ LDR         R7, [PC, #4]                @ 0x00000000000059e0 
    /*000059d8*/ B           Read_x59b8
    /*000059dc*/ WORD_59dc: .word 0x000031c8
    /*000059e0*/ WORD_59e0: .word 0x000031c4
FUNC_END Read


FUNC_BEGIN HI_ChkLogLevel
    /*000059e4*/ CMP         R1, #59                     @ 0x3b 
    /*000059e8*/ BHI         HI_ChkLogLevel_x5a0c
    /*000059ec*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*000059f0*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*000059f4*/ ADD         R3, R3, R1, LSL #2 
    /*000059f8*/ LDR         R3, [R3, #4] 
    /*000059fc*/ CMP         R3, R0 
    /*00005a00*/ MOVLT       R0, #0 
    /*00005a04*/ MOVGE       R0, #1 
    /*00005a08*/ BX          LR 
    HI_ChkLogLevel_x5a0c:
    /*00005a0c*/ MOV         R0, #0 
    /*00005a10*/ BX          LR 
FUNC_END HI_ChkLogLevel


FUNC_BEGIN LogWrite
    /*00005a14*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00005a18*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31ac] 
    /*00005a1c*/ LDR         R4, BSS_ANCHOR1               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31ac]
    /*00005a20*/ SUB         SP, SP, #20 
    /*00005a24*/ LDR         R5, [R4, #8] 
    /*00005a28*/ CMP         R5, #0 
    /*00005a2c*/ BEQ         LogWrite_x5b00
    /*00005a30*/ MOV         R7, R3 
    /*00005a34*/ MOV         R6, R2 
    /*00005a38*/ LDR         R3, [R4, #4] 
    /*00005a3c*/ MOV         R8, R1 
    /*00005a40*/ LDR         R2, [R4, #16] 
    /*00005a44*/ MOV         R5, R0 
    /*00005a48*/ CMP         R2, R3 
    /*00005a4c*/ BHI         LogWrite_x5ba0
    LogWrite_x5a50:
    /*00005a50*/ LDR         R2, [R4, #12] 
    /*00005a54*/ CMP         R2, R3 
    /*00005a58*/ BHI         LogWrite_x5b7c
    LogWrite_x5a5c:
    /*00005a5c*/ LDR         R0, WORD_5bc8               @ LDR         R0, [PC, #356]              @ 0x0000000000005bc8 
    /*00005a60*/ ADD         R1, SP, #12 
    /*00005a64*/ BL          osal_spin_lock_irqsave
    /*00005a68*/ MOV         R0, R8 
    /*00005a6c*/ LDR         R9, [R4, #12] 
    /*00005a70*/ LDR         R8, [R4, #16] 
    /*00005a74*/ BL          CMPI_GetModuleName
    /*00005a78*/ LDR         R3, [R4, #4] 
    /*00005a7c*/ @ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ca4] 
    /*00005a80*/ LDR         FP, [R4, #8] 
    /*00005a84*/ SUB         R3, R3, R8 
    /*00005a88*/ CMP         R3, #512                    @ 0x200 
    /*00005a8c*/ MOV         R3, R5 
    /*00005a90*/ MOV         SL, R0 
    /*00005a94*/ STR         R0, [SP] 
    /*00005a98*/ BHI         LogWrite_x5b0c
    /*00005a9c*/ MOV         R1, #512                    @ 0x200 
    /*00005aa0*/ ADDRL       R2, =_RODATA_STR1_4_1ca4    @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ca4]
    /*00005aa4*/ MOV         R0, FP 
    /*00005aa8*/ STR         R8, [R4, #20] 
    /*00005aac*/ BL          osal_snprintf
    /*00005ab0*/ MOV         R3, R7 
    /*00005ab4*/ MOV         R2, R6 
    /*00005ab8*/ MOV         R5, R0 
    /*00005abc*/ ADD         R0, FP, R0 
    /*00005ac0*/ RSB         R1, R5, #512                @ 0x200 
    /*00005ac4*/ BL          osal_vsnprintf
    /*00005ac8*/ ADD         R5, R5, R0 
    /*00005acc*/ CMP         R5, #512                    @ 0x200 
    /*00005ad0*/ BGE         LogWrite_x5b58
    /*00005ad4*/ CMP         R8, R9 
    /*00005ad8*/ CMPCS       R9, R5 
    /*00005adc*/ MOV         R3, R5 
    /*00005ae0*/ ADDLS       R9, R5, #1 
    LogWrite_x5ae4:
    /*00005ae4*/ LDR         R0, WORD_5bc8               @ LDR         R0, [PC, #220]              @ 0x0000000000005bc8 
    /*00005ae8*/ ADD         R1, SP, #12 
    /*00005aec*/ STR         R3, [R4, #16] 
    /*00005af0*/ STR         R9, [R4, #12] 
    /*00005af4*/ BL          osal_spin_unlock_irqrestore
    /*00005af8*/ LDR         R0, WORD_5bcc               @ LDR         R0, [PC, #204]              @ 0x0000000000005bcc 
    /*00005afc*/ BL          osal_wakeup
    LogWrite_x5b00:
    /*00005b00*/ MOV         R0, R5 
    /*00005b04*/ ADD         SP, SP, #20 
    /*00005b08*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    LogWrite_x5b0c:
    /*00005b0c*/ ADD         FP, FP, R8 
    /*00005b10*/ ADDRL       R2, =_RODATA_STR1_4_1ca4    @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ca4]
    /*00005b14*/ MOV         R1, #512                    @ 0x200 
    /*00005b18*/ MOV         R0, FP 
    /*00005b1c*/ BL          osal_snprintf
    /*00005b20*/ MOV         R3, R7 
    /*00005b24*/ MOV         R2, R6 
    /*00005b28*/ MOV         R5, R0 
    /*00005b2c*/ ADD         R0, FP, R0 
    /*00005b30*/ RSB         R1, R5, #512                @ 0x200 
    /*00005b34*/ BL          osal_vsnprintf
    /*00005b38*/ ADD         R5, R5, R0 
    /*00005b3c*/ CMP         R5, #512                    @ 0x200 
    /*00005b40*/ BGE         LogWrite_x5b58
    /*00005b44*/ ADD         R3, R5, R8 
    /*00005b48*/ CMP         R9, R3 
    /*00005b4c*/ CMPLS       R8, R9 
    /*00005b50*/ ADDCC       R9, R3, #1 
    /*00005b54*/ B           LogWrite_x5ae4
    LogWrite_x5b58:
    /*00005b58*/ MOV         R1, SL 
    /*00005b5c*/ @ MOVW        R0, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1cb0] 
    /*00005b60*/ ADDRL       R0, =_RODATA_STR1_4_1cb0    @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1cb0]
    /*00005b64*/ BL          osal_printk
    /*00005b68*/ MOV         R1, R7 
    /*00005b6c*/ MOV         R0, R6 
    /*00005b70*/ BL          osal_printk
    /*00005b74*/ MOV         R3, R8 
    /*00005b78*/ B           LogWrite_x5ae4
    LogWrite_x5b7c:
    /*00005b7c*/ @ MOVW        R3, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c74] 
    /*00005b80*/ @ MOVW        R1, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x2f8] 
    /*00005b84*/ ADDRL       R3, =_RODATA_STR1_4_1c74    @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c74]
    /*00005b88*/ ADDRL       R1, =_RODATA_02f8           @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x2f8]
    /*00005b8c*/ @ MOVW        R0, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c34] 
    /*00005b90*/ MOV         R2, #152                    @ 0x98 
    /*00005b94*/ ADDRL       R0, =_RODATA_STR1_4_1c34    @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c34]
    /*00005b98*/ BL          osal_panic
    /*00005b9c*/ B           LogWrite_x5a5c
    LogWrite_x5ba0:
    /*00005ba0*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c04] 
    /*00005ba4*/ @ MOVW        R1, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x2f8] 
    /*00005ba8*/ ADDRL       R3, =_RODATA_STR1_4_1c04    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c04]
    /*00005bac*/ ADDRL       R1, =_RODATA_02f8           @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x2f8]
    /*00005bb0*/ @ MOVW        R0, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c34] 
    /*00005bb4*/ MOV         R2, #151                    @ 0x97 
    /*00005bb8*/ ADDRL       R0, =_RODATA_STR1_4_1c34    @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c34]
    /*00005bbc*/ BL          osal_panic
    /*00005bc0*/ LDR         R3, [R4, #4] 
    /*00005bc4*/ B           LogWrite_x5a50
    /*00005bc8*/ WORD_5bc8: .word 0x000031c4
    /*00005bcc*/ WORD_5bcc: .word 0x000031c8
FUNC_END LogWrite


FUNC_BEGIN HI_LOG
    /*00005bd0*/ CMP         R1, #59                     @ 0x3b 
    /*00005bd4*/ BHI         HI_LOG_x5c20
    /*00005bd8*/ PUSH        {R2, R3} 
    /*00005bdc*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*00005be0*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*00005be4*/ ADD         R3, R3, R1, LSL #2 
    /*00005be8*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00005bec*/ SUB         SP, SP, #12 
    /*00005bf0*/ LDR         R3, [R3, #4] 
    /*00005bf4*/ CMP         R0, R3 
    /*00005bf8*/ MOVGT       R0, #0 
    /*00005bfc*/ BGT         HI_LOG_x5c10
    /*00005c00*/ ADD         R3, SP, #20 
    /*00005c04*/ LDR         R2, [SP, #16] 
    /*00005c08*/ STR         R3, [SP, #4] 
    /*00005c0c*/ BL          LogWrite
    HI_LOG_x5c10:
    /*00005c10*/ ADD         SP, SP, #12 
    /*00005c14*/ POP         {LR}                        @ (ldr lr, [sp], #4) 
    /*00005c18*/ ADD         SP, SP, #8 
    /*00005c1c*/ BX          LR 
    HI_LOG_x5c20:
    /*00005c20*/ MOV         R0, #0 
    /*00005c24*/ BX          LR 
FUNC_END HI_LOG


FUNC_BEGIN Ioctl
    /*00005c28*/ MOVW        R3, #19459                  @ 0x4c03 
    /*00005c2c*/ MOVT        R3, #16388                  @ 0x4004 
    /*00005c30*/ CMP         R0, R3 
    /*00005c34*/ PUSH        {R4, R5, LR} 
    /*00005c38*/ MOV         R4, R1 
    /*00005c3c*/ SUB         SP, SP, #12 
    /*00005c40*/ BEQ         Ioctl_x5d40
    /*00005c44*/ BLS         Ioctl_x5d04
    /*00005c48*/ MOV         R3, #19456                  @ 0x4c00 
    /*00005c4c*/ MOVT        R3, #16408                  @ 0x4018 
    /*00005c50*/ CMP         R0, R3 
    /*00005c54*/ BEQ         Ioctl_x5cb8
    /*00005c58*/ MOVW        R3, #19457                  @ 0x4c01 
    /*00005c5c*/ MOVT        R3, #49176                  @ 0xc018 
    /*00005c60*/ CMP         R0, R3 
    /*00005c64*/ BNE         Ioctl_x5cac
    /*00005c68*/ LDR         R0, [R1] 
    /*00005c6c*/ CMP         R0, #59                     @ 0x3b 
    /*00005c70*/ BHI         Ioctl_x5dcc
    /*00005c74*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*00005c78*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*00005c7c*/ ADD         R3, R3, R0, LSL #2 
    /*00005c80*/ LDR         R3, [R3, #4] 
    /*00005c84*/ STR         R3, [R1, #4] 
    /*00005c88*/ BL          CMPI_GetModuleName
    /*00005c8c*/ CMP         R0, #0 
    /*00005c90*/ BEQ         Ioctl_x5de4
    /*00005c94*/ LDR         R0, [R4], #8 
    /*00005c98*/ BL          CMPI_GetModuleName
    /*00005c9c*/ MOV         R2, #15 
    /*00005ca0*/ MOV         R1, R0 
    /*00005ca4*/ MOV         R0, R4 
    /*00005ca8*/ BL          osal_strncpy
    Ioctl_x5cac:
    /*00005cac*/ MOV         R0, #0 
    Ioctl_x5cb0:
    /*00005cb0*/ ADD         SP, SP, #12 
    /*00005cb4*/ POP         {R4, R5, PC} 
    Ioctl_x5cb8:
    /*00005cb8*/ LDR         R3, [R1, #4] 
    /*00005cbc*/ CMP         R3, #7 
    /*00005cc0*/ BGT         Ioctl_x5db0
    /*00005cc4*/ MOV         R5, #0 
    /*00005cc8*/ @ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ae0] 
    /*00005ccc*/ STRB        R5, [R1, #23] 
    /*00005cd0*/ ADDRL       R0, =_RODATA_STR1_4_1ae0    @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ae0]
    /*00005cd4*/ ADD         R1, R1, #8 
    /*00005cd8*/ BL          osal_strcmp
    /*00005cdc*/ CMP         R0, R5 
    /*00005ce0*/ BNE         Ioctl_x5d88
    /*00005ce4*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*00005ce8*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*00005cec*/ ADD         R1, R3, #240                @ 0xf0 
    Ioctl_x5cf0:
    /*00005cf0*/ LDR         R2, [R4, #4] 
    /*00005cf4*/ STR         R2, [R3, #4]! 
    /*00005cf8*/ CMP         R3, R1 
    /*00005cfc*/ BNE         Ioctl_x5cf0
    /*00005d00*/ B           Ioctl_x5cac
    Ioctl_x5d04:
    /*00005d04*/ MOVW        R3, #19458                  @ 0x4c02 
    /*00005d08*/ CMP         R0, R3 
    /*00005d0c*/ BNE         Ioctl_x5cac
    /*00005d10*/ @ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31ac] 
    /*00005d14*/ LDR         R0, BSS_ANCHOR1               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31ac]
    /*00005d18*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*00005d1c*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*00005d20*/ MOV         R2, #4 
    /*00005d24*/ STR         R2, [R0], #28 
    /*00005d28*/ MOV         R2, #1 
    /*00005d2c*/ STR         R2, [R3] 
    /*00005d30*/ BL          osal_wakeup
    /*00005d34*/ MOV         R0, #0 
    /*00005d38*/ ADD         SP, SP, #12 
    /*00005d3c*/ POP         {R4, R5, PC} 
    Ioctl_x5d40:
    /*00005d40*/ LDR         R2, [R1] 
    /*00005d44*/ CMP         R2, #1 
    /*00005d48*/ @ MOVWLS      R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*00005d4c*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*00005d50*/ MOVLS       R0, #0 
    /*00005d54*/ STRLS       R2, [R3] 
    /*00005d58*/ BLS         Ioctl_x5cb0
    /*00005d5c*/ STR         R2, [SP, #4] 
    /*00005d60*/ @ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d14] 
    /*00005d64*/ ADDRL       R2, =_RODATA_STR1_4_1d14    @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d14]
    /*00005d68*/ MOV         R3, #392                    @ 0x188 
    /*00005d6c*/ STR         R3, [SP] 
    Ioctl_x5d70:
    /*00005d70*/ LDR         R3, WORD_5e18               @ LDR         R3, [PC, #160]              @ 0x0000000000005e18 
    /*00005d74*/ MOV         R1, #0 
    /*00005d78*/ MOV         R0, #3 
    /*00005d7c*/ BL          HI_LOG
    /*00005d80*/ MVN         R0, #0 
    /*00005d84*/ B           Ioctl_x5cb0
    Ioctl_x5d88:
    /*00005d88*/ LDR         R2, [R4] 
    /*00005d8c*/ CMP         R2, #59                     @ 0x3b 
    /*00005d90*/ BHI         Ioctl_x5dec
    /*00005d94*/ LDR         R1, [R4, #4] 
    /*00005d98*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0xd0] 
    /*00005d9c*/ LDR         R3, DATA_ANCHOR1            @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0xd0]
    /*00005da0*/ MOV         R0, R5 
    /*00005da4*/ ADD         R3, R3, R2, LSL #2 
    /*00005da8*/ STR         R1, [R3, #4] 
    /*00005dac*/ B           Ioctl_x5cb0
    Ioctl_x5db0:
    /*00005db0*/ @ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1cd4] 
    /*00005db4*/ MOV         R1, #0 
    /*00005db8*/ ADDRL       R2, =_RODATA_STR1_4_1cd4    @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1cd4]
    /*00005dbc*/ MOV         R0, #3 
    /*00005dc0*/ BL          HI_LOG
    /*00005dc4*/ MVN         R0, #0 
    /*00005dc8*/ B           Ioctl_x5cb0
    Ioctl_x5dcc:
    /*00005dcc*/ MOVW        R3, #363                    @ 0x16b 
    /*00005dd0*/ @ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ce8] 
    /*00005dd4*/ STR         R0, [SP, #4] 
    /*00005dd8*/ ADDRL       R2, =_RODATA_STR1_4_1ce8    @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ce8]
    /*00005ddc*/ STR         R3, [SP] 
    /*00005de0*/ B           Ioctl_x5d70
    Ioctl_x5de4:
    /*00005de4*/ MVN         R0, #0 
    /*00005de8*/ B           Ioctl_x5cb0
    Ioctl_x5dec:
    /*00005dec*/ STR         R2, [SP, #4] 
    /*00005df0*/ MOVW        R3, #349                    @ 0x15d 
    /*00005df4*/ MOV         R1, R5 
    /*00005df8*/ STR         R3, [SP] 
    /*00005dfc*/ @ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ce8] 
    /*00005e00*/ LDR         R3, WORD_5e18               @ LDR         R3, [PC, #16]               @ 0x0000000000005e18 
    /*00005e04*/ ADDRL       R2, =_RODATA_STR1_4_1ce8    @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ce8]
    /*00005e08*/ MOV         R0, #3 
    /*00005e0c*/ BL          HI_LOG
    /*00005e10*/ MVN         R0, #0 
    /*00005e14*/ B           Ioctl_x5cb0
    /*00005e18*/ WORD_5e18: .word 0x00000304
FUNC_END Ioctl


FUNC_BEGIN CMPI_LogInit
    /*00005e1c*/ PUSH        {R4, R5, R6, LR} 
    /*00005e20*/ MOV         R5, R0 
    /*00005e24*/ @ MOVW        R0, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d3c] 
    /*00005e28*/ ADDRL       R0, =_RODATA_STR1_4_1d3c    @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d3c]
    /*00005e2c*/ BL          osal_createdev
    /*00005e30*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31ac] 
    /*00005e34*/ LDR         R4, BSS_ANCHOR1               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31ac]
    /*00005e38*/ CMP         R0, #0 
    /*00005e3c*/ STR         R0, [R4, #32] 
    /*00005e40*/ BEQ         CMPI_LogInit_x5f30
    /*00005e44*/ LDR         R2, WORD_5f44               @ LDR         R2, [PC, #248]              @ 0x0000000000005f44 
    /*00005e48*/ MOV         R1, #12 
    /*00005e4c*/ STR         R1, [R0, #52]               @ 0x34 
    /*00005e50*/ STR         R2, [R0, #56]               @ 0x38 
    /*00005e54*/ BL          osal_registerdevice
    /*00005e58*/ SUBS        R1, R0, #0 
    /*00005e5c*/ BNE         CMPI_LogInit_x5ee0
    /*00005e60*/ @ MOVW        R0, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d3c] 
    /*00005e64*/ ADDRL       R0, =_RODATA_STR1_4_1d3c    @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d3c]
    /*00005e68*/ BL          osal_create_proc_entry
    /*00005e6c*/ SUBS        R1, R0, #0 
    /*00005e70*/ BEQ         CMPI_LogInit_x5f24
    /*00005e74*/ @ MOVW        R2, #0                      @ LogProcRead [R_ARM_MOVW_ABS_NC: .text+0x5770] 
    /*00005e78*/ @ MOVW        R3, #0                      @ LogProcWrite [R_ARM_MOVW_ABS_NC: .text+0x552c] 
    /*00005e7c*/ ADDRL       R2, =_TEXT_5770             @ LogProcRead [R_ARM_MOVT_ABS: .text+0x5770]
    /*00005e80*/ ADDRL       R3, =_TEXT_552c             @ LogProcWrite [R_ARM_MOVT_ABS: .text+0x552c]
    /*00005e84*/ STR         R2, [R1, #60]               @ 0x3c 
    /*00005e88*/ ADD         R0, R4, #24 
    /*00005e8c*/ STR         R3, [R1, #64]               @ 0x40 
    /*00005e90*/ BL          osal_spin_lock_init
    /*00005e94*/ ADD         R0, R4, #28 
    /*00005e98*/ BL          osal_wait_init
    /*00005e9c*/ SUB         R3, R5, #2 
    /*00005ea0*/ CMP         R3, #126                    @ 0x7e 
    /*00005ea4*/ LSLLS       R3, R5, #10 
    /*00005ea8*/ MOV         R5, #0 
    /*00005eac*/ MOVHI       R3, #65536                  @ 0x10000 
    /*00005eb0*/ STR         R5, [R4, #16] 
    /*00005eb4*/ MOV         R0, R3 
    /*00005eb8*/ STR         R3, [R4, #4] 
    /*00005ebc*/ STR         R3, [R4, #20] 
    /*00005ec0*/ STR         R5, [R4, #12] 
    /*00005ec4*/ BL          osal_vmalloc
    /*00005ec8*/ CMP         R0, R5 
    /*00005ecc*/ MOV         R6, R0 
    /*00005ed0*/ STR         R0, [R4, #8] 
    /*00005ed4*/ BEQ         CMPI_LogInit_x5f04
    /*00005ed8*/ MOV         R0, R5 
    /*00005edc*/ POP         {R4, R5, R6, PC} 
    CMPI_LogInit_x5ee0:
    /*00005ee0*/ @ MOVW        R0, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d5c] 
    /*00005ee4*/ ADDRL       R0, =_RODATA_STR1_4_1d5c    @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d5c]
    /*00005ee8*/ BL          osal_printk
    CMPI_LogInit_x5eec:
    /*00005eec*/ LDR         R0, [R4, #32] 
    /*00005ef0*/ BL          osal_destroydev
    /*00005ef4*/ MOV         R3, #0 
    /*00005ef8*/ MVN         R0, #0 
    /*00005efc*/ STR         R3, [R4, #32] 
    /*00005f00*/ POP         {R4, R5, R6, PC} 
    CMPI_LogInit_x5f04:
    /*00005f04*/ LDR         R1, [R4, #4] 
    /*00005f08*/ @ MOVW        R0, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d7c] 
    /*00005f0c*/ ADDRL       R0, =_RODATA_STR1_4_1d7c    @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d7c]
    /*00005f10*/ BL          osal_printk
    /*00005f14*/ @ MOVW        R0, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d3c] 
    /*00005f18*/ MOV         R1, R6 
    /*00005f1c*/ ADDRL       R0, =_RODATA_STR1_4_1d3c    @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d3c]
    /*00005f20*/ BL          osal_remove_proc_entry
    CMPI_LogInit_x5f24:
    /*00005f24*/ LDR         R0, [R4, #32] 
    /*00005f28*/ BL          osal_deregisterdevice
    /*00005f2c*/ B           CMPI_LogInit_x5eec
    CMPI_LogInit_x5f30:
    /*00005f30*/ @ MOVW        R0, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d44] 
    /*00005f34*/ ADDRL       R0, =_RODATA_STR1_4_1d44    @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d44]
    /*00005f38*/ BL          osal_printk
    /*00005f3c*/ MVN         R0, #0 
    /*00005f40*/ POP         {R4, R5, R6, PC} 
    /*00005f44*/ WORD_5f44: .word 0x000001c8
FUNC_END CMPI_LogInit


FUNC_BEGIN CMPI_LogExit
    /*00005f48*/ PUSH        {R4, LR} 
    /*00005f4c*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31ac] 
    /*00005f50*/ LDR         R4, BSS_ANCHOR1               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31ac]
    /*00005f54*/ LDR         R0, [R4, #8] 
    /*00005f58*/ CMP         R0, #0 
    /*00005f5c*/ BEQ         CMPI_LogExit_x5f6c
    /*00005f60*/ BL          osal_vfree
    /*00005f64*/ MOV         R3, #0 
    /*00005f68*/ STR         R3, [R4, #8] 
    CMPI_LogExit_x5f6c:
    /*00005f6c*/ LDR         R0, [R4, #32] 
    /*00005f70*/ BL          osal_deregisterdevice
    /*00005f74*/ LDR         R0, [R4, #32] 
    /*00005f78*/ BL          osal_destroydev
    /*00005f7c*/ MOV         R1, #0 
    /*00005f80*/ @ MOVW        R0, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d3c] 
    /*00005f84*/ ADDRL       R0, =_RODATA_STR1_4_1d3c    @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d3c]
    /*00005f88*/ BL          osal_remove_proc_entry
    /*00005f8c*/ LDR         R0, WORD_5fa0               @ LDR         R0, [PC, #12]               @ 0x0000000000005fa0 
    /*00005f90*/ BL          osal_spin_lock_destory
    /*00005f94*/ LDR         R0, WORD_5fa4               @ LDR         R0, [PC, #8]                @ 0x0000000000005fa4 
    /*00005f98*/ POP         {R4, LR} 
    CMPI_LogExit_x5f9c:
    /*00005f9c*/ B           CMPI_LogExit_x5f9c          @ osal_wait_destory [R_ARM_JUMP24: +0x0] 
    /*00005fa0*/ WORD_5fa0: .word 0x000031c4
    /*00005fa4*/ WORD_5fa4: .word 0x000031c8
FUNC_END CMPI_LogExit


FUNC_BEGIN CMPI_MmzMalloc
    /*00005fa8*/ PUSH        {R4, LR} 
    /*00005fac*/ SUB         SP, SP, #8 
    /*00005fb0*/ MOV         IP, R1 
    /*00005fb4*/ MOV         R3, #0 
    /*00005fb8*/ MOV         R1, R2 
    /*00005fbc*/ STR         R0, [SP] 
    /*00005fc0*/ MOV         R2, R3 
    /*00005fc4*/ MOV         R0, IP 
    /*00005fc8*/ BL          hil_mmb_alloc
    /*00005fcc*/ SUBS        R4, R0, #0 
    /*00005fd0*/ LDRNE       R0, [R4, #32] 
    /*00005fd4*/ BEQ         CMPI_MmzMalloc_x5fe4
    CMPI_MmzMalloc_x5fd8:
    /*00005fd8*/ MOV         R1, #0 
    /*00005fdc*/ ADD         SP, SP, #8 
    /*00005fe0*/ POP         {R4, PC} 
    CMPI_MmzMalloc_x5fe4:
    /*00005fe4*/ @ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d9c] 
    /*00005fe8*/ MOV         R1, R4 
    /*00005fec*/ ADDRL       R2, =_RODATA_STR1_4_1d9c    @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d9c]
    /*00005ff0*/ MOV         R0, #3 
    /*00005ff4*/ BL          HI_LOG
    /*00005ff8*/ MOV         R0, R4 
    /*00005ffc*/ B           CMPI_MmzMalloc_x5fd8
FUNC_END CMPI_MmzMalloc


FUNC_BEGIN CMPI_MmzFree
    /*00006000*/ CMP         R2, #0 
    /*00006004*/ PUSH        {R4, R5, R6, LR} 
    /*00006008*/ MOV         R4, R0 
    /*0000600c*/ MOV         R5, R1 
    /*00006010*/ BEQ         CMPI_MmzFree_x6028
    /*00006014*/ MOV         R0, R2 
    /*00006018*/ BL          hil_mmb_getby_kvirt
    /*0000601c*/ CMP         R0, #0 
    /*00006020*/ BEQ         CMPI_MmzFree_x6028
    /*00006024*/ BL          hil_mmb_unmap
    CMPI_MmzFree_x6028:
    /*00006028*/ ORRS        R3, R4, R5 
    /*0000602c*/ POPEQ       {R4, R5, R6, PC} 
    /*00006030*/ MOV         R0, R4 
    /*00006034*/ BL          hil_mmb_getby_phys
    /*00006038*/ POP         {R4, R5, R6, LR} 
    CMPI_MmzFree_x603c:
    /*0000603c*/ B           CMPI_MmzFree_x603c          @ hil_mmb_free [R_ARM_JUMP24: +0x0] 
FUNC_END CMPI_MmzFree


FUNC_BEGIN CMPI_Remap_Cached
    /*00006040*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00006044*/ MOV         R6, #61440                  @ 0xf000 
    /*00006048*/ MOVT        R6, #65535                  @ 0xffff 
    /*0000604c*/ SUB         R2, R2, #1 
    /*00006050*/ BIC         R8, R0, R6 
    /*00006054*/ MVN         R7, #0 
    /*00006058*/ ADD         R2, R2, R8 
    /*0000605c*/ AND         R7, R7, R1 
    /*00006060*/ BIC         R1, R2, #4080               @ 0xff0 
    /*00006064*/ AND         R6, R6, R0 
    /*00006068*/ BIC         R1, R1, #15 
    /*0000606c*/ SUB         SP, SP, #16 
    /*00006070*/ ADD         R4, R1, #4096               @ 0x1000 
    /*00006074*/ MOV         R0, R6 
    /*00006078*/ MOV         R1, R4 
    /*0000607c*/ BL          hil_mmf_map2kern_cache
    /*00006080*/ SUBS        R5, R0, #0 
    /*00006084*/ ADDNE       R0, R5, R8 
    /*00006088*/ BEQ         CMPI_Remap_Cached_x6094
    CMPI_Remap_Cached_x608c:
    /*0000608c*/ ADD         SP, SP, #16 
    /*00006090*/ POP         {R4, R5, R6, R7, R8, PC} 
    CMPI_Remap_Cached_x6094:
    /*00006094*/ STR         R4, [SP, #8] 
    /*00006098*/ @ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1db0] 
    /*0000609c*/ STRD        R6, [SP] 
    /*000060a0*/ ADDRL       R2, =_RODATA_STR1_4_1db0    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1db0]
    /*000060a4*/ MOV         R1, R5 
    /*000060a8*/ MOV         R0, #3 
    /*000060ac*/ BL          HI_LOG
    /*000060b0*/ MOV         R0, R5 
    /*000060b4*/ B           CMPI_Remap_Cached_x608c
FUNC_END CMPI_Remap_Cached


FUNC_BEGIN CMPI_Remap_Nocache
    /*000060b8*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000060bc*/ MOV         R6, #61440                  @ 0xf000 
    /*000060c0*/ MOVT        R6, #65535                  @ 0xffff 
    /*000060c4*/ SUB         R2, R2, #1 
    /*000060c8*/ BIC         R8, R0, R6 
    /*000060cc*/ MVN         R7, #0 
    /*000060d0*/ ADD         R2, R2, R8 
    /*000060d4*/ AND         R7, R7, R1 
    /*000060d8*/ BIC         R1, R2, #4080               @ 0xff0 
    /*000060dc*/ AND         R6, R6, R0 
    /*000060e0*/ BIC         R1, R1, #15 
    /*000060e4*/ SUB         SP, SP, #16 
    /*000060e8*/ ADD         R4, R1, #4096               @ 0x1000 
    /*000060ec*/ MOV         R0, R6 
    /*000060f0*/ MOV         R1, R4 
    /*000060f4*/ BL          hil_mmf_map2kern_nocache
    /*000060f8*/ SUBS        R5, R0, #0 
    /*000060fc*/ ADDNE       R0, R5, R8 
    /*00006100*/ BEQ         CMPI_Remap_Nocache_x610c
    CMPI_Remap_Nocache_x6104:
    /*00006104*/ ADD         SP, SP, #16 
    /*00006108*/ POP         {R4, R5, R6, R7, R8, PC} 
    CMPI_Remap_Nocache_x610c:
    /*0000610c*/ STR         R4, [SP, #8] 
    /*00006110*/ @ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1dd8] 
    /*00006114*/ STRD        R6, [SP] 
    /*00006118*/ ADDRL       R2, =_RODATA_STR1_4_1dd8    @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1dd8]
    /*0000611c*/ MOV         R1, R5 
    /*00006120*/ MOV         R0, #3 
    /*00006124*/ BL          HI_LOG
    /*00006128*/ MOV         R0, R5 
    /*0000612c*/ B           CMPI_Remap_Nocache_x6104
FUNC_END CMPI_Remap_Nocache


FUNC_BEGIN CMPI_Unmap
    CMPI_Unmap_x6130:
    /*00006130*/ B           CMPI_Unmap_x6130            @ hil_mmf_unmap [R_ARM_JUMP24: +0x0] 
FUNC_END CMPI_Unmap


FUNC_BEGIN CMPI_MmzMallocNocache
    /*00006134*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00006138*/ SUB         SP, SP, #12 
    /*0000613c*/ MOV         R7, R3 
    /*00006140*/ MOV         R3, #0 
    /*00006144*/ STR         R0, [SP] 
    /*00006148*/ MOV         R5, R2 
    /*0000614c*/ MOV         R0, R1 
    /*00006150*/ MOV         R2, R3 
    /*00006154*/ LDR         R1, [SP, #32] 
    /*00006158*/ BL          hil_mmb_alloc
    /*0000615c*/ SUBS        R6, R0, #0 
    /*00006160*/ BEQ         CMPI_MmzMallocNocache_x6190
    /*00006164*/ LDR         R3, [R6, #32] 
    /*00006168*/ MOV         R4, #0 
    /*0000616c*/ STM         R5, {R3, R4} 
    /*00006170*/ BL          hil_mmb_map2kern
    /*00006174*/ CMP         R0, R4 
    /*00006178*/ MOV         R5, R0 
    /*0000617c*/ STR         R0, [R7] 
    /*00006180*/ MOVNE       R0, R4 
    /*00006184*/ BEQ         CMPI_MmzMallocNocache_x61ac
    CMPI_MmzMallocNocache_x6188:
    /*00006188*/ ADD         SP, SP, #12 
    /*0000618c*/ POP         {R4, R5, R6, R7, PC} 
    CMPI_MmzMallocNocache_x6190:
    /*00006190*/ MOV         R1, R6 
    /*00006194*/ @ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d9c] 
    /*00006198*/ MOV         R0, #3 
    /*0000619c*/ ADDRL       R2, =_RODATA_STR1_4_1d9c    @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d9c]
    /*000061a0*/ BL          HI_LOG
    /*000061a4*/ MVN         R0, #0 
    /*000061a8*/ B           CMPI_MmzMallocNocache_x6188
    CMPI_MmzMallocNocache_x61ac:
    /*000061ac*/ MOV         R0, R6 
    /*000061b0*/ BL          hil_mmb_free
    /*000061b4*/ MOV         R1, R5 
    /*000061b8*/ @ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e04] 
    /*000061bc*/ MOV         R0, #3 
    /*000061c0*/ ADDRL       R2, =_RODATA_STR1_4_1e04    @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e04]
    /*000061c4*/ BL          HI_LOG
    /*000061c8*/ MVN         R0, #0 
    /*000061cc*/ B           CMPI_MmzMallocNocache_x6188
FUNC_END CMPI_MmzMallocNocache


FUNC_BEGIN CMPI_MmzMallocCached
    /*000061d0*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000061d4*/ SUB         SP, SP, #12 
    /*000061d8*/ MOV         R7, R3 
    /*000061dc*/ MOV         R3, #0 
    /*000061e0*/ STR         R0, [SP] 
    /*000061e4*/ MOV         R5, R2 
    /*000061e8*/ MOV         R0, R1 
    /*000061ec*/ MOV         R2, R3 
    /*000061f0*/ LDR         R1, [SP, #32] 
    /*000061f4*/ BL          hil_mmb_alloc
    /*000061f8*/ SUBS        R6, R0, #0 
    /*000061fc*/ BEQ         CMPI_MmzMallocCached_x622c
    /*00006200*/ LDR         R3, [R6, #32] 
    /*00006204*/ MOV         R4, #0 
    /*00006208*/ STM         R5, {R3, R4} 
    /*0000620c*/ BL          hil_mmb_map2kern_cached
    /*00006210*/ CMP         R0, R4 
    /*00006214*/ MOV         R5, R0 
    /*00006218*/ STR         R0, [R7] 
    /*0000621c*/ MOVNE       R0, R4 
    /*00006220*/ BEQ         CMPI_MmzMallocCached_x6248
    CMPI_MmzMallocCached_x6224:
    /*00006224*/ ADD         SP, SP, #12 
    /*00006228*/ POP         {R4, R5, R6, R7, PC} 
    CMPI_MmzMallocCached_x622c:
    /*0000622c*/ MOV         R1, R6 
    /*00006230*/ @ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d9c] 
    /*00006234*/ MOV         R0, #3 
    /*00006238*/ ADDRL       R2, =_RODATA_STR1_4_1d9c    @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d9c]
    /*0000623c*/ BL          HI_LOG
    /*00006240*/ MVN         R0, #0 
    /*00006244*/ B           CMPI_MmzMallocCached_x6224
    CMPI_MmzMallocCached_x6248:
    /*00006248*/ MOV         R0, R6 
    /*0000624c*/ BL          hil_mmb_free
    /*00006250*/ MOV         R1, R5 
    /*00006254*/ @ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e20] 
    /*00006258*/ MOV         R0, #3 
    /*0000625c*/ ADDRL       R2, =_RODATA_STR1_4_1e20    @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e20]
    /*00006260*/ BL          HI_LOG
    /*00006264*/ MVN         R0, #0 
    /*00006268*/ B           CMPI_MmzMallocCached_x6224
FUNC_END CMPI_MmzMallocCached


FUNC_BEGIN CMPI_GetModuleName
    /*0000626c*/ @ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31d0] 
    /*00006270*/ LDR         R1, BSS_ANCHOR2               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31d0]
    /*00006274*/ LDR         R2, [R1] 
    /*00006278*/ CMP         R2, R1 
    /*0000627c*/ BEQ         CMPI_GetModuleName_x62ac
    /*00006280*/ LDR         R3, [R2, #24] 
    /*00006284*/ CMP         R0, R3 
    /*00006288*/ BNE         CMPI_GetModuleName_x629c
    /*0000628c*/ B           CMPI_GetModuleName_x62b4
    CMPI_GetModuleName_x6290:
    /*00006290*/ LDR         R3, [R3, #24] 
    /*00006294*/ CMP         R3, R0 
    /*00006298*/ BEQ         CMPI_GetModuleName_x62b4
    CMPI_GetModuleName_x629c:
    /*0000629c*/ LDR         R3, [R2] 
    /*000062a0*/ CMP         R3, R1 
    /*000062a4*/ MOV         R2, R3 
    /*000062a8*/ BNE         CMPI_GetModuleName_x6290
    CMPI_GetModuleName_x62ac:
    /*000062ac*/ MOV         R0, #0 
    /*000062b0*/ BX          LR 
    CMPI_GetModuleName_x62b4:
    /*000062b4*/ ADD         R0, R2, #8 
    /*000062b8*/ BX          LR 
FUNC_END CMPI_GetModuleName


FUNC_BEGIN CMPI_GetModuleById
    /*000062bc*/ @ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31d0] 
    /*000062c0*/ LDR         R1, BSS_ANCHOR2               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31d0]
    /*000062c4*/ LDR         R2, [R1] 
    /*000062c8*/ CMP         R2, R1 
    /*000062cc*/ BEQ         CMPI_GetModuleById_x62fc
    /*000062d0*/ LDR         R3, [R2, #24] 
    /*000062d4*/ CMP         R0, R3 
    /*000062d8*/ BNE         CMPI_GetModuleById_x62ec
    /*000062dc*/ B           CMPI_GetModuleById_x6304
    CMPI_GetModuleById_x62e0:
    /*000062e0*/ LDR         R3, [R3, #24] 
    /*000062e4*/ CMP         R3, R0 
    /*000062e8*/ BEQ         CMPI_GetModuleById_x6304
    CMPI_GetModuleById_x62ec:
    /*000062ec*/ LDR         R3, [R2] 
    /*000062f0*/ CMP         R3, R1 
    /*000062f4*/ MOV         R2, R3 
    /*000062f8*/ BNE         CMPI_GetModuleById_x62e0
    CMPI_GetModuleById_x62fc:
    /*000062fc*/ MOV         R0, #0 
    /*00006300*/ BX          LR 
    CMPI_GetModuleById_x6304:
    /*00006304*/ MOV         R0, R2 
    /*00006308*/ BX          LR 
FUNC_END CMPI_GetModuleById


FUNC_BEGIN CMPI_GetModuleFuncById
    /*0000630c*/ @ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31d0] 
    /*00006310*/ LDR         R1, BSS_ANCHOR2               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31d0]
    /*00006314*/ LDR         R2, [R1] 
    /*00006318*/ CMP         R2, R1 
    /*0000631c*/ BEQ         CMPI_GetModuleFuncById_x634c
    /*00006320*/ LDR         R3, [R2, #24] 
    /*00006324*/ CMP         R0, R3 
    /*00006328*/ BNE         CMPI_GetModuleFuncById_x633c
    /*0000632c*/ B           CMPI_GetModuleFuncById_x6354
    CMPI_GetModuleFuncById_x6330:
    /*00006330*/ LDR         R3, [R3, #24] 
    /*00006334*/ CMP         R3, R0 
    /*00006338*/ BEQ         CMPI_GetModuleFuncById_x6354
    CMPI_GetModuleFuncById_x633c:
    /*0000633c*/ LDR         R3, [R2] 
    /*00006340*/ CMP         R3, R1 
    /*00006344*/ MOV         R2, R3 
    /*00006348*/ BNE         CMPI_GetModuleFuncById_x6330
    CMPI_GetModuleFuncById_x634c:
    /*0000634c*/ MOV         R0, #0 
    /*00006350*/ BX          LR 
    CMPI_GetModuleFuncById_x6354:
    /*00006354*/ LDR         R0, [R2, #52]               @ 0x34 
    /*00006358*/ BX          LR 
FUNC_END CMPI_GetModuleFuncById


FUNC_BEGIN CMPI_StopModules
    /*0000635c*/ PUSH        {R4, R5, R6, LR} 
    /*00006360*/ @ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31d0] 
    /*00006364*/ LDR         R5, BSS_ANCHOR2               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31d0]
    /*00006368*/ LDR         R4, [R5, #4] 
    /*0000636c*/ CMP         R4, R5 
    /*00006370*/ POPEQ       {R4, R5, R6, PC} 
    CMPI_StopModules_x6374:
    /*00006374*/ LDR         R3, [R4, #40]               @ 0x28 
    /*00006378*/ MOV         R0, #17 
    /*0000637c*/ CMP         R3, #0 
    /*00006380*/ BEQ         CMPI_StopModules_x6388
    /*00006384*/ BLX         R3 
    CMPI_StopModules_x6388:
    /*00006388*/ LDR         R4, [R4, #4] 
    /*0000638c*/ CMP         R4, R5 
    /*00006390*/ BNE         CMPI_StopModules_x6374
    /*00006394*/ POP         {R4, R5, R6, PC} 
FUNC_END CMPI_StopModules


FUNC_BEGIN CMPI_QueryModules
    /*00006398*/ PUSH        {R4, R5, R6, LR} 
    /*0000639c*/ @ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31d0] 
    /*000063a0*/ LDR         R5, BSS_ANCHOR2               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31d0]
    /*000063a4*/ SUB         SP, SP, #8 
    /*000063a8*/ LDR         R4, [R5, #4] 
    /*000063ac*/ CMP         R4, R5 
    /*000063b0*/ MOVNE       R6, #34                     @ 0x22 
    /*000063b4*/ BEQ         CMPI_QueryModules_x63e8
    CMPI_QueryModules_x63b8:
    /*000063b8*/ LDR         R3, [R4, #36]               @ 0x24 
    /*000063bc*/ ADD         R0, SP, #4 
    /*000063c0*/ CMP         R3, #0 
    /*000063c4*/ BEQ         CMPI_QueryModules_x63dc
    /*000063c8*/ STR         R6, [SP, #4] 
    /*000063cc*/ BLX         R3 
    /*000063d0*/ LDR         R3, [SP, #4] 
    /*000063d4*/ CMP         R3, #17 
    /*000063d8*/ BNE         CMPI_QueryModules_x63f4
    CMPI_QueryModules_x63dc:
    /*000063dc*/ LDR         R4, [R4, #4] 
    /*000063e0*/ CMP         R4, R5 
    /*000063e4*/ BNE         CMPI_QueryModules_x63b8
    CMPI_QueryModules_x63e8:
    /*000063e8*/ MOV         R0, #0 
    /*000063ec*/ ADD         SP, SP, #8 
    /*000063f0*/ POP         {R4, R5, R6, PC} 
    CMPI_QueryModules_x63f4:
    /*000063f4*/ ADD         R3, R4, #8 
    /*000063f8*/ MOV         R1, #0 
    /*000063fc*/ @ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e3c] 
    /*00006400*/ MOV         R0, #6 
    /*00006404*/ ADDRL       R2, =_RODATA_STR1_4_1e3c    @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e3c]
    /*00006408*/ BL          HI_LOG
    /*0000640c*/ MVN         R0, #0 
    /*00006410*/ ADD         SP, SP, #8 
    /*00006414*/ POP         {R4, R5, R6, PC} 
FUNC_END CMPI_QueryModules


FUNC_BEGIN CMPI_ExitModules
    /*00006418*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000641c*/ @ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31d0] 
    /*00006420*/ LDR         R5, BSS_ANCHOR2               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31d0]
    /*00006424*/ LDR         R4, [R5, #4] 
    /*00006428*/ CMP         R4, R5 
    /*0000642c*/ POPEQ       {R4, R5, R6, R7, R8, PC} 
    /*00006430*/ @ MOVW        R7, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e6c] 
    /*00006434*/ @ MOVW        R6, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e50] 
    /*00006438*/ ADDRL       R7, =_RODATA_STR1_4_1e6c    @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e6c]
    /*0000643c*/ ADDRL       R6, =_RODATA_STR1_4_1e50    @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e50]
    /*00006440*/ MOV         R8, #0 
    CMPI_ExitModules_x6444:
    /*00006444*/ LDR         R1, [R4, #48]               @ 0x30 
    /*00006448*/ ADD         R3, R4, #8 
    /*0000644c*/ MOV         R2, R6 
    /*00006450*/ MOV         R0, #3 
    /*00006454*/ CMP         R1, #0 
    /*00006458*/ BEQ         CMPI_ExitModules_x6478
    /*0000645c*/ LDR         R3, [R4, #32] 
    /*00006460*/ BLX         R3 
    /*00006464*/ ADD         R3, R4, #8 
    /*00006468*/ MOV         R2, R7 
    /*0000646c*/ MOV         R1, #0 
    /*00006470*/ MOV         R0, #7 
    /*00006474*/ STR         R8, [R4, #48]               @ 0x30 
    CMPI_ExitModules_x6478:
    /*00006478*/ BL          HI_LOG
    /*0000647c*/ LDR         R4, [R4, #4] 
    /*00006480*/ CMP         R4, R5 
    /*00006484*/ BNE         CMPI_ExitModules_x6444
    /*00006488*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END CMPI_ExitModules


FUNC_BEGIN CMPI_InitModules
    /*0000648c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00006490*/ @ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31d0] 
    /*00006494*/ LDR         R6, BSS_ANCHOR2               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31d0]
    /*00006498*/ LDR         R4, [R6] 
    /*0000649c*/ CMP         R4, R6 
    /*000064a0*/ MOVNE       R7, #1 
    /*000064a4*/ BNE         CMPI_InitModules_x64b8
    /*000064a8*/ B           CMPI_InitModules_x64e8
    CMPI_InitModules_x64ac:
    /*000064ac*/ LDR         R4, [R4] 
    /*000064b0*/ CMP         R4, R6 
    /*000064b4*/ BEQ         CMPI_InitModules_x64e8
    CMPI_InitModules_x64b8:
    /*000064b8*/ LDR         R5, [R4, #48]               @ 0x30 
    /*000064bc*/ CMP         R5, #0 
    /*000064c0*/ BNE         CMPI_InitModules_x64ac
    /*000064c4*/ LDR         R3, [R4, #28] 
    /*000064c8*/ LDR         R0, [R4, #56]               @ 0x38 
    /*000064cc*/ BLX         R3 
    /*000064d0*/ CMP         R0, #0 
    /*000064d4*/ BLT         CMPI_InitModules_x64f0
    /*000064d8*/ STR         R7, [R4, #48]               @ 0x30 
    /*000064dc*/ LDR         R4, [R4] 
    /*000064e0*/ CMP         R4, R6 
    /*000064e4*/ BNE         CMPI_InitModules_x64b8
    CMPI_InitModules_x64e8:
    /*000064e8*/ MOV         R0, #0 
    /*000064ec*/ POP         {R4, R5, R6, R7, R8, PC} 
    CMPI_InitModules_x64f0:
    /*000064f0*/ ADD         R3, R4, #8 
    /*000064f4*/ MOV         R1, R5 
    /*000064f8*/ @ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e80] 
    /*000064fc*/ MOV         R0, #3 
    /*00006500*/ ADDRL       R2, =_RODATA_STR1_4_1e80    @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e80]
    /*00006504*/ BL          HI_LOG
    /*00006508*/ MVN         R0, #0 
    /*0000650c*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END CMPI_InitModules


FUNC_BEGIN CMPI_RegisterModule
    /*00006510*/ PUSH        {R4, R5, R6, LR} 
    /*00006514*/ SUBS        R5, R0, #0 
    /*00006518*/ BEQ         CMPI_RegisterModule_x6688
    CMPI_RegisterModule_x651c:
    /*0000651c*/ LDR         R3, [R5, #24] 
    /*00006520*/ CMP         R3, #59                     @ 0x3b 
    /*00006524*/ BHI         CMPI_RegisterModule_x65fc
    CMPI_RegisterModule_x6528:
    /*00006528*/ LDR         R3, [R5, #28] 
    /*0000652c*/ CMP         R3, #0 
    /*00006530*/ BEQ         CMPI_RegisterModule_x6640
    CMPI_RegisterModule_x6534:
    /*00006534*/ LDR         R3, [R5, #32] 
    /*00006538*/ CMP         R3, #0 
    /*0000653c*/ BEQ         CMPI_RegisterModule_x6664
    CMPI_RegisterModule_x6540:
    /*00006540*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*00006544*/ CMP         R3, #0 
    /*00006548*/ BEQ         CMPI_RegisterModule_x66ac
    CMPI_RegisterModule_x654c:
    /*0000654c*/ @ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31d0] 
    /*00006550*/ LDR         R4, BSS_ANCHOR2               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31d0]
    /*00006554*/ LDR         R2, [R4] 
    /*00006558*/ CMP         R2, R4 
    /*0000655c*/ BEQ         CMPI_RegisterModule_x6590
    /*00006560*/ LDR         R1, [R5, #24] 
    /*00006564*/ LDR         R3, [R2, #24] 
    /*00006568*/ CMP         R3, R1 
    /*0000656c*/ BNE         CMPI_RegisterModule_x6580
    /*00006570*/ B           CMPI_RegisterModule_x65dc
    CMPI_RegisterModule_x6574:
    /*00006574*/ LDR         R3, [R3, #24] 
    /*00006578*/ CMP         R3, R1 
    /*0000657c*/ BEQ         CMPI_RegisterModule_x65dc
    CMPI_RegisterModule_x6580:
    /*00006580*/ LDR         R3, [R2] 
    /*00006584*/ CMP         R3, R4 
    /*00006588*/ MOV         R2, R3 
    /*0000658c*/ BNE         CMPI_RegisterModule_x6574
    CMPI_RegisterModule_x6590:
    /*00006590*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*00006594*/ BLX         R3 
    /*00006598*/ MOVW        R3, #61479                  @ 0xf027 
    /*0000659c*/ MOVT        R3, #307                    @ 0x133 
    /*000065a0*/ CMP         R0, R3 
    /*000065a4*/ BNE         CMPI_RegisterModule_x6620
    /*000065a8*/ LDR         R3, [R5, #28] 
    /*000065ac*/ LDR         R0, [R5, #56]               @ 0x38 
    /*000065b0*/ BLX         R3 
    /*000065b4*/ CMP         R0, #0 
    /*000065b8*/ BLT         CMPI_RegisterModule_x66d0
    /*000065bc*/ LDR         R3, [R4, #4] 
    /*000065c0*/ MOV         R2, #1 
    /*000065c4*/ STR         R5, [R4, #4] 
    /*000065c8*/ STR         R2, [R5, #48]               @ 0x30 
    /*000065cc*/ STR         R3, [R5, #4] 
    /*000065d0*/ STR         R4, [R5] 
    /*000065d4*/ STR         R5, [R3] 
    /*000065d8*/ POP         {R4, R5, R6, PC} 
    CMPI_RegisterModule_x65dc:
    /*000065dc*/ ADD         R3, R5, #8 
    /*000065e0*/ MOV         R1, #0 
    /*000065e4*/ @ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f80] 
    /*000065e8*/ MOV         R0, #3 
    /*000065ec*/ ADDRL       R2, =_RODATA_STR1_4_1f80    @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f80]
    /*000065f0*/ BL          HI_LOG
    /*000065f4*/ MVN         R0, #0 
    /*000065f8*/ POP         {R4, R5, R6, PC} 
    CMPI_RegisterModule_x65fc:
    /*000065fc*/ @ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ef4] 
    /*00006600*/ @ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x30c] 
    /*00006604*/ ADDRL       R3, =_RODATA_STR1_4_1ef4    @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ef4]
    /*00006608*/ ADDRL       R1, =_RODATA_030c           @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x30c]
    /*0000660c*/ @ MOVW        R0, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb4] 
    /*00006610*/ MOVW        R2, #263                    @ 0x107 
    /*00006614*/ ADDRL       R0, =_RODATA_STR1_4_1eb4    @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb4]
    /*00006618*/ BL          osal_panic
    /*0000661c*/ B           CMPI_RegisterModule_x6528
    CMPI_RegisterModule_x6620:
    /*00006620*/ ADD         R3, R5, #8 
    /*00006624*/ MOV         R1, #0 
    /*00006628*/ @ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fa0] 
    /*0000662c*/ MOV         R0, #3 
    /*00006630*/ ADDRL       R2, =_RODATA_STR1_4_1fa0    @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fa0]
    /*00006634*/ BL          HI_LOG
    /*00006638*/ MVN         R0, #0 
    /*0000663c*/ POP         {R4, R5, R6, PC} 
    CMPI_RegisterModule_x6640:
    /*00006640*/ @ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f1c] 
    /*00006644*/ @ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x30c] 
    /*00006648*/ ADDRL       R3, =_RODATA_STR1_4_1f1c    @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f1c]
    /*0000664c*/ ADDRL       R1, =_RODATA_030c           @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x30c]
    /*00006650*/ @ MOVW        R0, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb4] 
    /*00006654*/ MOV         R2, #264                    @ 0x108 
    /*00006658*/ ADDRL       R0, =_RODATA_STR1_4_1eb4    @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb4]
    /*0000665c*/ BL          osal_panic
    /*00006660*/ B           CMPI_RegisterModule_x6534
    CMPI_RegisterModule_x6664:
    /*00006664*/ @ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f3c] 
    /*00006668*/ @ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x30c] 
    /*0000666c*/ ADDRL       R3, =_RODATA_STR1_4_1f3c    @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f3c]
    /*00006670*/ ADDRL       R1, =_RODATA_030c           @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x30c]
    /*00006674*/ @ MOVW        R0, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb4] 
    /*00006678*/ MOVW        R2, #265                    @ 0x109 
    /*0000667c*/ ADDRL       R0, =_RODATA_STR1_4_1eb4    @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb4]
    /*00006680*/ BL          osal_panic
    /*00006684*/ B           CMPI_RegisterModule_x6540
    CMPI_RegisterModule_x6688:
    /*00006688*/ @ MOVW        R3, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e9c] 
    /*0000668c*/ @ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x30c] 
    /*00006690*/ ADDRL       R3, =_RODATA_STR1_4_1e9c    @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e9c]
    /*00006694*/ ADDRL       R1, =_RODATA_030c           @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x30c]
    /*00006698*/ @ MOVW        R0, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb4] 
    /*0000669c*/ MOVW        R2, #262                    @ 0x106 
    /*000066a0*/ ADDRL       R0, =_RODATA_STR1_4_1eb4    @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb4]
    /*000066a4*/ BL          osal_panic
    /*000066a8*/ B           CMPI_RegisterModule_x651c
    CMPI_RegisterModule_x66ac:
    /*000066ac*/ @ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f5c] 
    /*000066b0*/ @ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x30c] 
    /*000066b4*/ ADDRL       R3, =_RODATA_STR1_4_1f5c    @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f5c]
    /*000066b8*/ ADDRL       R1, =_RODATA_030c           @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x30c]
    /*000066bc*/ @ MOVW        R0, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb4] 
    /*000066c0*/ MOVW        R2, #266                    @ 0x10a 
    /*000066c4*/ ADDRL       R0, =_RODATA_STR1_4_1eb4    @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb4]
    /*000066c8*/ BL          osal_panic
    /*000066cc*/ B           CMPI_RegisterModule_x654c
    CMPI_RegisterModule_x66d0:
    /*000066d0*/ ADD         R3, R5, #8 
    /*000066d4*/ MOV         R1, #0 
    /*000066d8*/ @ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e80] 
    /*000066dc*/ MOV         R0, #3 
    /*000066e0*/ ADDRL       R2, =_RODATA_STR1_4_1e80    @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e80]
    /*000066e4*/ BL          HI_LOG
    /*000066e8*/ MVN         R0, #0 
    /*000066ec*/ POP         {R4, R5, R6, PC} 
FUNC_END CMPI_RegisterModule


FUNC_BEGIN CMPI_UnRegisterModule
    /*000066f0*/ CMP         R0, #59                     @ 0x3b 
    /*000066f4*/ PUSH        {R4, R5, R6, LR} 
    /*000066f8*/ MOV         R5, R0 
    /*000066fc*/ BHI         CMPI_UnRegisterModule_x67bc
    CMPI_UnRegisterModule_x6700:
    /*00006700*/ @ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31d0] 
    /*00006704*/ LDR         R1, BSS_ANCHOR2               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31d0]
    /*00006708*/ LDR         R3, [R1] 
    /*0000670c*/ CMP         R3, R1 
    /*00006710*/ LDR         R4, [R3] 
    /*00006714*/ BEQ         CMPI_UnRegisterModule_x6744
    /*00006718*/ LDR         R2, [R3, #24] 
    /*0000671c*/ CMP         R5, R2 
    /*00006720*/ BNE         CMPI_UnRegisterModule_x6738
    /*00006724*/ B           CMPI_UnRegisterModule_x6760
    CMPI_UnRegisterModule_x6728:
    /*00006728*/ LDR         R2, [R4, #24] 
    /*0000672c*/ CMP         R5, R2 
    /*00006730*/ BEQ         CMPI_UnRegisterModule_x6764
    /*00006734*/ MOV         R4, R3 
    CMPI_UnRegisterModule_x6738:
    /*00006738*/ CMP         R4, R1 
    /*0000673c*/ LDR         R3, [R4] 
    /*00006740*/ BNE         CMPI_UnRegisterModule_x6728
    CMPI_UnRegisterModule_x6744:
    /*00006744*/ MOV         R3, R5 
    /*00006748*/ @ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ff4] 
    /*0000674c*/ MOV         R1, #0 
    /*00006750*/ ADDRL       R2, =_RODATA_STR1_4_1ff4    @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ff4]
    /*00006754*/ MOV         R0, #3 
    /*00006758*/ POP         {R4, R5, R6, LR} 
    CMPI_UnRegisterModule_x675c:
    /*0000675c*/ B           CMPI_UnRegisterModule_x675c @ HI_LOG [R_ARM_JUMP24: .text+0x5bd0] 
    CMPI_UnRegisterModule_x6760:
    /*00006760*/ MOV         R4, R3 
    CMPI_UnRegisterModule_x6764:
    /*00006764*/ LDR         R3, [R4, #48]               @ 0x30 
    /*00006768*/ CMP         R3, #0 
    /*0000676c*/ BEQ         CMPI_UnRegisterModule_x6778
    /*00006770*/ LDR         R3, [R4, #32] 
    /*00006774*/ BLX         R3 
    CMPI_UnRegisterModule_x6778:
    /*00006778*/ LDR         R1, [R4, #4] 
    /*0000677c*/ MOV         R0, #256                    @ 0x100 
    /*00006780*/ LDR         LR, [R4] 
    /*00006784*/ MOVT        R0, #16 
    /*00006788*/ MOV         IP, #512                    @ 0x200 
    /*0000678c*/ ADD         R3, R4, #8 
    /*00006790*/ MOVT        IP, #32 
    /*00006794*/ @ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fdc] 
    /*00006798*/ STR         R1, [LR, #4] 
    /*0000679c*/ ADDRL       R2, =_RODATA_STR1_4_1fdc    @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fdc]
    /*000067a0*/ STR         LR, [R1] 
    /*000067a4*/ MOV         R1, #0 
    /*000067a8*/ STR         R0, [R4] 
    /*000067ac*/ MOV         R0, #7 
    /*000067b0*/ STR         IP, [R4, #4] 
    /*000067b4*/ POP         {R4, R5, R6, LR} 
    CMPI_UnRegisterModule_x67b8:
    /*000067b8*/ B           CMPI_UnRegisterModule_x67b8 @ HI_LOG [R_ARM_JUMP24: .text+0x5bd0] 
    CMPI_UnRegisterModule_x67bc:
    /*000067bc*/ @ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc0] 
    /*000067c0*/ @ MOVW        R0, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb4] 
    /*000067c4*/ ADDRL       R3, =_RODATA_STR1_4_1fc0    @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc0]
    /*000067c8*/ ADDRL       R0, =_RODATA_STR1_4_1eb4    @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb4]
    /*000067cc*/ MOV         R2, #308                    @ 0x134 
    /*000067d0*/ LDR         R1, WORD_67dc               @ LDR         R1, [PC, #4]                @ 0x00000000000067dc 
    /*000067d4*/ BL          osal_panic
    /*000067d8*/ B           CMPI_UnRegisterModule_x6700
    /*000067dc*/ WORD_67dc: .word 0x00000320
FUNC_END CMPI_UnRegisterModule


FUNC_BEGIN COMM_Init
    /*000067e0*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x31d0] 
    /*000067e4*/ LDR         R3, BSS_ANCHOR2               @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x31d0]
    /*000067e8*/ PUSH        {R4, R5, R6, LR} 
    /*000067ec*/ MOV         R0, #0 
    /*000067f0*/ STR         R3, [R3] 
    /*000067f4*/ STR         R3, [R3, #4] 
    /*000067f8*/ BL          VB_Init
    /*000067fc*/ SUBS        R5, R0, #0 
    /*00006800*/ BNE         COMM_Init_x682c
    /*00006804*/ BL          CMPI_LogInit
    /*00006808*/ SUBS        R4, R0, #0 
    /*0000680c*/ BNE         COMM_Init_x6848
    /*00006810*/ MOV         R1, R4 
    /*00006814*/ @ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x203c] 
    /*00006818*/ MOV         R0, #7 
    /*0000681c*/ ADDRL       R2, =_RODATA_STR1_4_203c    @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x203c]
    /*00006820*/ BL          HI_LOG
    /*00006824*/ MOV         R0, R4 
    /*00006828*/ POP         {R4, R5, R6, PC} 
    COMM_Init_x682c:
    /*0000682c*/ MOV         R1, #0 
    /*00006830*/ @ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2014] 
    /*00006834*/ MOV         R0, #3 
    /*00006838*/ ADDRL       R2, =_RODATA_STR1_4_2014    @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2014]
    /*0000683c*/ BL          HI_LOG
    /*00006840*/ MVN         R0, #0 
    /*00006844*/ POP         {R4, R5, R6, PC} 
    COMM_Init_x6848:
    /*00006848*/ MOV         R1, R5 
    /*0000684c*/ @ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2028] 
    /*00006850*/ MOV         R0, #3 
    /*00006854*/ ADDRL       R2, =_RODATA_STR1_4_2028    @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2028]
    /*00006858*/ BL          HI_LOG
    /*0000685c*/ BL          VB_Exit
    /*00006860*/ MVN         R0, #0 
    /*00006864*/ POP         {R4, R5, R6, PC} 
FUNC_END COMM_Init


FUNC_BEGIN COMM_Exit
    /*00006868*/ PUSH        {R4, LR} 
    /*0000686c*/ BL          CMPI_LogExit
    /*00006870*/ BL          VB_Exit
    /*00006874*/ @ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2058] 
    /*00006878*/ MOV         R1, #0 
    /*0000687c*/ ADDRL       R2, =_RODATA_STR1_4_2058    @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2058]
    /*00006880*/ MOV         R0, #7 
    /*00006884*/ POP         {R4, LR} 
    COMM_Exit_x6888:
    /*00006888*/ B           COMM_Exit_x6888             @ HI_LOG [R_ARM_JUMP24: .text+0x5bd0] 
FUNC_END COMM_Exit


FUNC_BEGIN VB_CheckSupplement
    /*0000688c*/ LDR         R3, [R0] 
    /*00006890*/ BICS        R0, R3, #15 
    /*00006894*/ BXEQ        LR 
    /*00006898*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000689c*/ SUB         SP, SP, #20 
    /*000068a0*/ MOV         R2, #15 
    /*000068a4*/ MOV         R1, #1 
    /*000068a8*/ STR         R3, [SP, #4] 
    /*000068ac*/ MOV         R0, #3 
    /*000068b0*/ STR         R2, [SP, #8] 
    /*000068b4*/ MOV         R3, #19 
    /*000068b8*/ @ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2074] 
    /*000068bc*/ STR         R3, [SP] 
    /*000068c0*/ ADDRL       R2, =_RODATA_STR1_4_2074    @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2074]
    /*000068c4*/ @ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x338] 
    /*000068c8*/ ADDRL       R3, =_RODATA_0338           @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x338]
    /*000068cc*/ BL          HI_LOG
    /*000068d0*/ MOVW        R0, #32771                  @ 0x8003 
    /*000068d4*/ MOVT        R0, #40961                  @ 0xa001 
    /*000068d8*/ ADD         SP, SP, #20 
    /*000068dc*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END VB_CheckSupplement


FUNC_BEGIN VB_GetSupplementSize
    /*000068e0*/ CMP         R1, #1 
    /*000068e4*/ BEQ         VB_GetSupplementSize_x68f0
    /*000068e8*/ MOV         R0, #80                     @ 0x50 
    /*000068ec*/ BX          LR 
    VB_GetSupplementSize_x68f0:
    /*000068f0*/ LDR         R3, [R0] 
    /*000068f4*/ TST         R3, #1 
    /*000068f8*/ MOVNE       R0, #204                    @ 0xcc 
    /*000068fc*/ MOVEQ       R0, #0 
    /*00006900*/ TST         R3, #2 
    /*00006904*/ ADDNE       R0, R0, #56                 @ 0x38 
    /*00006908*/ TST         R3, #4 
    /*0000690c*/ ADDNE       R0, R0, #240                @ 0xf0 
    /*00006910*/ TST         R3, #8 
    /*00006914*/ ADDNE       R0, R0, #88                 @ 0x58 
    /*00006918*/ BX          LR 
FUNC_END VB_GetSupplementSize


FUNC_BEGIN VB_ValueSupplementCachedAddr
    /*0000691c*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00006920*/ MOV         R5, R1 
    /*00006924*/ LDR         LR, [SP, #20] 
    /*00006928*/ MOV         R4, R0 
    /*0000692c*/ LDR         IP, [LR] 
    /*00006930*/ ANDS        R1, IP, #1 
    /*00006934*/ STRDNE      R4, [R3] 
    /*00006938*/ MOVNE       R1, #204                    @ 0xcc 
    /*0000693c*/ STRNE       R2, [R3, #40]               @ 0x28 
    /*00006940*/ LDRNE       IP, [LR] 
    /*00006944*/ TST         IP, #2 
    /*00006948*/ BEQ         VB_ValueSupplementCachedAddr_x6968
    /*0000694c*/ ADDS        R6, R4, R1 
    /*00006950*/ ADD         R0, R2, R1 
    /*00006954*/ ADC         R7, R5, #0 
    /*00006958*/ STR         R0, [R3, #44]               @ 0x2c 
    /*0000695c*/ STRD        R6, [R3, #8] 
    /*00006960*/ ADD         R1, R1, #56                 @ 0x38 
    /*00006964*/ LDR         IP, [LR] 
    VB_ValueSupplementCachedAddr_x6968:
    /*00006968*/ TST         IP, #4 
    /*0000696c*/ BEQ         VB_ValueSupplementCachedAddr_x698c
    /*00006970*/ ADDS        R6, R4, R1 
    /*00006974*/ ADD         R0, R2, R1 
    /*00006978*/ ADC         R7, R5, #0 
    /*0000697c*/ STR         R0, [R3, #52]               @ 0x34 
    /*00006980*/ STRD        R6, [R3, #24] 
    /*00006984*/ ADD         R1, R1, #240                @ 0xf0 
    /*00006988*/ LDR         IP, [LR] 
    VB_ValueSupplementCachedAddr_x698c:
    /*0000698c*/ TST         IP, #8 
    /*00006990*/ POPEQ       {R4, R5, R6, R7, PC} 
    /*00006994*/ ADDS        R4, R4, R1 
    /*00006998*/ ADD         R1, R2, R1 
    /*0000699c*/ ADC         R5, R5, #0 
    /*000069a0*/ STR         R1, [R3, #56]               @ 0x38 
    /*000069a4*/ STRD        R4, [R3, #32] 
    /*000069a8*/ POP         {R4, R5, R6, R7, PC} 
FUNC_END VB_ValueSupplementCachedAddr


FUNC_BEGIN VB_ValueSupplementNoCacheAddr
    /*000069ac*/ STRD        R0, [R3, #16] 
    /*000069b0*/ STR         R2, [R3, #48]               @ 0x30 
    /*000069b4*/ BX          LR 
FUNC_END VB_ValueSupplementNoCacheAddr


FUNC_BEGIN VB_IsLowDelaySupported
    /*000069b8*/ MOV         R0, #1 
    /*000069bc*/ BX          LR 
FUNC_END VB_IsLowDelaySupported


FUNC_BEGIN COMM_exit_proc_ctrl
    /*000069c0*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x31d8] 
    /*000069c4*/ LDR         R3, BSS_ANCHOR3               @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x31d8]
    /*000069c8*/ LDR         R0, [R3] 
    COMM_exit_proc_ctrl_x69cc:
    /*000069cc*/ B           COMM_exit_proc_ctrl_x69cc   @ unregister_sysctl_table [R_ARM_JUMP24: +0x0] 
FUNC_END COMM_exit_proc_ctrl


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0x4c
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN COMM_init_proc_ctrl
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ @ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x1ec] 
    /*00000008*/ LDR         R0, DATA_ANCHOR2            @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x1ec]
    /*0000000c*/ BL          register_sysctl_table
    /*00000010*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x31d8] 
    /*00000014*/ LDR         R3, BSS_ANCHOR3               @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x31d8]
    /*00000018*/ CMP         R0, #0 
    /*0000001c*/ STR         R0, [R3] 
    /*00000020*/ MVNEQ       R0, #11 
    /*00000024*/ MOVNE       R0, #0 
    /*00000028*/ POP         {R4, PC} 
FUNC_END COMM_init_proc_ctrl


FUNC_BEGIN base_mod_init
    /*0000002c*/ PUSH        {R4, LR} 
    /*00000030*/ BL          COMM_Init
    /*00000034*/ BL          COMM_init_proc_ctrl
    /*00000038*/ CMP         R0, #0 
    /*0000003c*/ POPEQ       {R4, PC} 
    /*00000040*/ BL          COMM_Exit
    /*00000044*/ MVN         R0, #0 
    /*00000048*/ POP         {R4, PC} 
FUNC_END base_mod_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0x1c
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN base_mod_exit
    /*00000000*/ @ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x31d8] 
    /*00000004*/ LDR         R3, BSS_ANCHOR3               @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x31d8]
    /*00000008*/ PUSH        {R4, LR} 
    /*0000000c*/ LDR         R0, [R3] 
    /*00000010*/ BL          unregister_sysctl_table
    /*00000014*/ POP         {R4, LR} 
    base_mod_exit_x18:
    /*00000018*/ B           base_mod_exit_x18           @ COMM_Exit [R_ARM_JUMP24: .text+0x6868] 
FUNC_END base_mod_exit




