/*
* Copyright (c) Hisilicon Technologies Co., Ltd. 2011-2019. All rights reserved.
* Description: internal vb interface
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#ifndef __VB_EXT_H__
#define __VB_EXT_H__

#include "hi_comm_vb_adapt.h"
#include "hi_common_adapt.h"
#include "hi_comm_video_adapt.h"
#include "mod_ext.h"

typedef hi_u32 vb_blk_handle;

typedef struct {
    hi_mpp_chn mod;
    hi_s32 (*call_back)(hi_mpp_chn mod, hi_u64 phy_addr);
} vb_create_ex_info;

typedef struct {
    hi_u32 pool_id;
    hi_u32 blk_cnt;
    hi_u64 blk_size;
    hi_u64 pool_size;
    hi_u64 pool_phy_addr;
    hi_void ATTRIBUTE *pool_vir_addr;
} vb_pool_info;
/*****************************************************************************
 prototype    : vb_create_pool
 Description  : create a private VB pool
 input        : blk_cnt      : count of buffer blocks
                blk_cnt      : size of buffer blocks
 output       : pool_id     : pointer to the ID of pool created
 return value : hi_s32         : 0 -- success; !0 : failure
*****************************************************************************/
extern hi_s32 vb_create_pool(hi_u32 *pool_id, hi_u32 blk_cnt, hi_u64 blk_size,
                            const hi_char *pc_mmz_name, const hi_char *buf_name, hi_vb_remap_mode vb_remap_mode);

/*****************************************************************************
 prototype    : vb_create_pool_ext
 Description  : create a private VB pool,but it's not malloc buffer
 input        : vb_reg_info      : register info
                blk_cnt      : count of buffer blocks
 output       : pool_id     : pointer to the ID of pool created
 return value : hi_s32         : 0 -- success; !0 : failure
*****************************************************************************/
extern hi_s32 vb_create_pool_ext(hi_u32 *pool_id, hi_u32 blk_cnt, vb_create_ex_info *vb_reg_info);

/*****************************************************************************
 prototype    : vb_add_blk_to_pool
 Description  : add a buffer block to a specified pool
 input        : pool_id      : ID of a pool
                phys_addr    : physcial address of the buffer
                uid         : ID of user getting the buffer
 output       :
 return value : vb_blk_handle   :
                success : not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern hi_s32 vb_add_blk_to_pool(hi_u32 pool_id, hi_u64 phys_addr, hi_u64 size, hi_u32 uid);

/*****************************************************************************
 prototype    : vb_add_blk_to_pool
 Description  : add a buffer block to a specified pool
 input        : pool_id      : ID of a pool
                phys_addr    : physcial address of the buffer
                uid         : ID of user getting the buffer
 output       :
 return value : vb_blk_handle   :
                success : not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern hi_s32 vb_get_pool_info(vb_pool_info *info);

extern hi_s32 vb_destroy_pool(hi_u32 pool_id);

/*****************************************************************************
 prototype    : vb_get_pool_id
 Description  : get the first one in common pools in which block size is greater
                than  'blk_size'
 input        :
 output       :
 return value : hi_s32
                success : 0
                failure : not 0
*****************************************************************************/
extern hi_s32 vb_get_pool_id(hi_u64 blk_size, hi_u32 *pool_id, const hi_char *pc_mmz_name, hi_s32 owner);

/*****************************************************************************
 prototype    : vb_get_blk_by_pool_id
 Description  : get a buffer block from a specified pool
 input        : pool_id      : ID of a pool
                uid         : ID of user getting the buffer
 output       :
 return value : vb_blk_handle   :
                success : not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern vb_blk_handle vb_get_blk_by_pool_id(hi_u32 pool_id, hi_u32 uid);

/*****************************************************************************
 prototype    : vb_get_blk_by_size
 Description  : get a block in the first fit common pool
 input        : blk_size : size of buffer block
                uid     : ID of user who will use the buffer
 output       : NONE
 return value : vb_blk_handle
                success : not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern vb_blk_handle vb_get_blk_by_size(hi_u64 blk_size, hi_u32 uid, const hi_char *pc_mmz_name);

/*****************************************************************************
 prototype    : vb_get_blk_by_size_and_module
 Description  : get a block in the module common pool
 input        : blk_size : size of buffer block
                uid     : ID of user who build the buffer
 output       : NONE
 return value : vb_blk_handle
                success : not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern vb_blk_handle vb_get_blk_by_size_and_module(hi_u64 blk_size, hi_u32 uid, const hi_char *pc_mmz_name);

/*****************************************************************************
 prototype    : vb_get_blk_by_size_and_pool_id
 Description  : get a block in the pool with id
 input        : blk_size : size of buffer block
                pool_id  : ID of block
                uid     : ID of user who build the buffer
 output       : NONE
 return value : vb_blk_handle
                success : not VB_INVALID_HANDLE
                failure : VB_INVALID_HANDLE
*****************************************************************************/
extern vb_blk_handle vb_get_blk_by_size_and_pool_id(hi_u32 pool_id, hi_u64 blk_size, hi_u32 uid);

/*****************************************************************************
 prototype    : vb_put_blk
 Description  : forcebly free the buffer even if it is used by more than one
                user. put a block back to free list, reset all couter for all users.
 input        : pool_id      : ID of pool in which the buffer is.
                phys_addr    : physical address of the buffer
 output       :
 return value : hi_s32
                success : 0
                failure : not 0
*****************************************************************************/
extern hi_s32 vb_put_blk(hi_u32 pool_id, hi_u64 phy_addr);

/*****************************************************************************
 prototype    : vb_phy_to_handle
 Description  : convert physical address to handle
 input        :
 output       :
 return value : hi_u32
******************************************************************************/
extern vb_blk_handle vb_phy_to_handle(hi_u64 phy_addr);

/*****************************************************************************
 prototype    : vb_handle_to_phys
 Description  : convert handle to physical address of a buffer
 input        :
 output       :
 return value : hi_u32
******************************************************************************/
extern hi_u64 vb_handle_to_phys(vb_blk_handle handle);

/*****************************************************************************
 prototype    : vb_handle_to_kern
 Description  : convert handle to kernel virtual address of a buffer
 input        : handle     : hanle of a buffer
 output       :
 return value : hi_u32
*****************************************************************************/
extern hi_u64 vb_handle_to_kern(vb_blk_handle handle);

/*****************************************************************************
 prototype    : vb_handle_to_blk_size
 Description  : convert handle to the size a buffer
 input        : handle     : hanle of a buffer
 output       : ?T
 return value : hi_u32
*****************************************************************************/
extern hi_u64 vb_handle_to_blk_size(vb_blk_handle handle);

/*****************************************************************************
 prototype    : vb_handle_to_pool_id
 Description  : convert handle to pool ID
 input        : vb_blk_handle   : handle of the buffer
 output       :
 return value : hi_u32
*****************************************************************************/
extern hi_u32 vb_handle_to_pool_id(vb_blk_handle handle);

/*****************************************************************************
 prototype    : vb_handle_to_blk_id
 Description  : convert handle to pool ID
 input        : vb_blk_handle   : handle of the buffer
 output       :
 return value : hi_u32
*****************************************************************************/
extern hi_u32 vb_handle_to_blk_id(vb_blk_handle handle);

/*****************************************************************************
 prototype    : vb_user_add
 Description  : increase one to reference counter of  a buffer
 input        : pool_id      : ID of pool the buffer is in
                phys_addr    : physcial address of the buffer
                uid         : ID of the user
 output       :
 return value : hi_s32
                success    : 0
                failure    : not 0
*****************************************************************************/
extern hi_s32 vb_user_add(hi_u32 pool_id, hi_u64 phy_addr, hi_u32 uid);

/*****************************************************************************
 prototype    : vb_user_sub
 Description  : decrease one to reference counter of a buffer
 input        : pool_id      : ID of pool the buffer is in
                phys_addr    : physcial address of the buffer
                uid         : ID of the user
 output       :
 return value : hi_s32
*****************************************************************************/
extern hi_s32 vb_user_sub(hi_u32 pool_id, hi_u64 phy_addr, hi_u32 uid);

/*****************************************************************************
 prototype    : vb_inquire_user_cnt
 Description  : inquire how many users are using the buffer
 input        : handle     : hanle of a buffer
 output       :
 return value : hi_u32
******************************************************************************/
extern hi_u32 vb_inquire_user_cnt(vb_blk_handle handle);

extern hi_u32 vb_inquire_one_user_cnt(vb_blk_handle handle, hi_u32 uid);

extern hi_u32 vb_inquire_total_user_cnt(vb_blk_handle handle);
/*****************************************************************************
 prototype    : vb_inquire_blk_cnt
 Description  : inquire how many blocks the user is using
 input        : uid
                is_comm_pool
 output       :
 return value : hi_u32
*****************************************************************************/
extern hi_u32 vb_inquire_blk_cnt(hi_u32 uid, hi_bool is_comm_pool);

/*****************************************************************************
 prototype    : vb_inquire_pool
 Description  : inquire the statistic of a pool
 input        : pool_id     : ID of pool the buffer is in
 output       : pool_status : pool status
 return value : hi_u32
******************************************************************************/
extern hi_s32 vb_inquire_pool(hi_u32 pool_id, hi_vb_pool_status *pool_status);

/*****************************************************************************
 prototype    : vb_inquire_pool_user_cnt
 Description  : inquire how many blocks the user used in this pool
 input        : pool_id     : ID of pool the buffer is in
                 uid          : ID of the user
 output       : cnt          : count of vb used by user
 return value : hi_u32
******************************************************************************/
extern hi_s32 vb_inquire_pool_user_cnt(hi_u32 pool_id, hi_u32 uid, hi_u32 *cnt);

/*****************************************************************************
 prototype    : vb_is_blk_valid
 Description  : check if the address is valid
 input        : pool_id
                phys_addr
 output       :
 return value : hi_bool
*****************************************************************************/
extern hi_bool vb_is_blk_valid(hi_u32 pool_id, hi_u64 phys_addr);

/*****************************************************************************
 prototype    : vb_handle_to_supplement
 Description  : convert handle to video_supplement
 input        :
 output       :
 return value : video_supplement*
******************************************************************************/
extern hi_video_supplement *vb_handle_to_supplement(vb_blk_handle handle);

/*****************************************************************************
 prototype    : vb_is_supplement_support
 Description  : check whether supplement is supported
 input        :
 output       :
 return value : hi_bool
******************************************************************************/
extern hi_bool vb_is_supplement_support(hi_u32 mask);

/*****************************************************************************
 prototype    : vb_init
 Description  : initialize video buffer modules
 input        : arg       : unused now
 output       :
 return value : hi_s32
                success    : 0
                failure    : not 0
*****************************************************************************/
extern hi_s32 vb_init(hi_void *);

/*****************************************************************************
 prototype    : vb_exit
 Description  : cleanup video buffer module
 input        :
 output       :
 return value :hi_s32
               success    : 0
               failure    : not 0 , we never reture failure in fact.
*****************************************************************************/
extern hi_s32 vb_exit(hi_void);

/*****************************************************************************
 prototype    : vb_copy_supplement
 Description  : copy the supplement info form source to destination
 input        : dst_v_frame : source
                src_v_frame : destination
 output       : NONE
 return value : hi_void
*****************************************************************************/
extern hi_void vb_copy_supplement(hi_video_frame *dst_v_frame, const hi_video_frame *src_v_frame);

typedef struct {
    hi_s32 (*pfn_vb_create_pool)(hi_u32 *pool_id, hi_u32 blk_cnt, hi_u64 blk_size,
                                 const hi_char *pc_mmz_name, const hi_char *buf_name,
                                 hi_vb_remap_mode vb_remap_mode);
    hi_s32 (*pfn_vb_destroy_pool)(hi_u32 pool_id);
    vb_blk_handle (*pfn_vb_get_blk_by_pool_id)(hi_u32 pool_id, hi_u32 uid);
    vb_blk_handle (*pfn_vb_get_blk_by_size)(hi_u64 blk_size, hi_u32 uid, const hi_char *pc_mmz_name);
    vb_blk_handle (*pfn_vb_get_blk_by_size_and_module)(hi_u64 blk_size, hi_u32 uid, const hi_char *pc_mmz_name);
    vb_blk_handle (*pfn_vb_get_blk_by_size_and_pool_id)(hi_u32 pool_id, hi_u64 blk_size, hi_u32 uid);
    hi_s32 (*pfn_vb_get_pool_id)(hi_u64 blk_size, hi_u32 *pool_id, const hi_char *pc_mmz_name, hi_s32 owner);
    hi_s32 (*pfn_vb_get_pool_info)(vb_pool_info *info);
    hi_s32 (*pfn_vb_put_blk)(hi_u32 pool_id, hi_u64 phy_addr);
    vb_blk_handle (*pfn_vb_phy_to_handle)(hi_u64 phy_addr);
    hi_u32 (*pfn_vb_handle_to_pool_id)(vb_blk_handle handle);
    hi_u32 (*pfn_vb_handle_to_blk_id)(vb_blk_handle handle);
    hi_u64 (*pfn_vb_handle_to_kern)(vb_blk_handle handle);
    hi_u64 (*pfn_vb_handle_to_phys)(vb_blk_handle handle);
    hi_u64 (*pfn_vb_handle_to_blk_size)(vb_blk_handle handle);
    hi_s32 (*pfn_vb_user_add)(hi_u32 pool_id, hi_u64 phy_addr, hi_u32 uid);
    hi_s32 (*pfn_vb_user_sub)(hi_u32 pool_id, hi_u64 phy_addr, hi_u32 uid);
    hi_u32 (*pfn_vb_inquire_user_cnt)(vb_blk_handle handle);
    hi_u32 (*pfn_vb_inquire_one_user_cnt)(vb_blk_handle handle, hi_u32 uid);
    hi_u32 (*pfn_vb_inquire_total_user_cnt)(vb_blk_handle handle);
    hi_u32 (*pfn_vb_inquire_blk_cnt)(hi_u32 uid, hi_bool is_comm_pool);
    hi_bool (*pfn_vb_is_blk_valid)(hi_u32 pool_id, hi_u64 phy_addr);
    hi_bool (*pfn_vb_is_pool_id_valid)(hi_u32 pool_id);
    hi_s32 (*pfn_vb_inquire_pool)(hi_u32 pool_id, hi_vb_pool_status *pool_status);
    hi_s32 (*pfn_vb_inquire_pool_user_cnt)(hi_u32 pool_id, hi_u32 uid, hi_u32 *cnt);
    hi_video_supplement *(*pfn_vb_handle_to_supplement)(vb_blk_handle handle);
    hi_bool (*pfn_vb_is_supplement_support)(hi_u32 mask);
    hi_void (*pfn_vb_copy_supplement)(hi_video_frame *dst_v_frame, const hi_video_frame *src_v_frame);
} vb_export_func;

#define ckfn_vb_create_pool() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_create_pool != NULL)
#define call_vb_create_pool(pool_id, blk_cnt, blk_size, pc_mmz_name, buf_name, vb_remap_mode) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_create_pool(pool_id, blk_cnt, blk_size, \
                                                            pc_mmz_name, buf_name, vb_remap_mode)

#define ckfn_vb_destroy_pool() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_destroy_pool != NULL)
#define call_vb_destroy_pool(pool_id) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_destroy_pool(pool_id)

#define ckfn_vb_get_blk_by_pool_id() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_blk_by_pool_id != NULL)
#define call_vb_get_blk_by_pool_id(pool_id, uid) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_blk_by_pool_id(pool_id, uid)

#define ckfn_vb_get_blk_by_size() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_blk_by_size != NULL)
#define call_vb_get_blk_by_size(blk_size, uid, pc_mmz_name) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_blk_by_size(blk_size, uid, pc_mmz_name)

#define ckfn_vb_get_blk_by_size_and_module() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_blk_by_size_and_module != NULL)
#define call_vb_get_blk_by_size_and_module(blk_size, uid, pc_mmz_name) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_blk_by_size_and_module(blk_size, uid, pc_mmz_name)

#define ckfn_vb_get_blk_by_size_and_pool_id() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_blk_by_size_and_pool_id != NULL)
#define call_vb_get_blk_by_size_and_pool_id(pool_id, blk_size, uid) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_blk_by_size_and_pool_id(pool_id, blk_size, uid)

#define ckfn_vb_get_pool_id() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_pool_id != NULL)
#define call_vb_get_pool_id(blk_size, pool_id, pc_mmz_name, owner) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_pool_id(blk_size, pool_id, pc_mmz_name, owner)

#define ckfn_vb_get_pool_info() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_pool_info != NULL)
#define call_vb_get_pool_info(info) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_get_pool_info(info)

#define ckfn_vb_put_blk() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_put_blk != NULL)
#define call_vb_put_blk(pool_id, phy_addr) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_put_blk(pool_id, phy_addr)

#define ckfn_vb_phy_to_handle() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_phy_to_handle != NULL)
#define call_vb_phy_to_handle(phy_addr) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_phy_to_handle(phy_addr)

#define ckfn_vb_handle_to_pool_id() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_pool_id != NULL)
#define call_vb_handle_to_pool_id(handle) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_pool_id(handle)

#define ckfn_vb_handle_to_blk_id() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_blk_id != NULL)
#define call_vb_handle_to_blk_id(handle) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_blk_id(handle)

#define ckfn_vb_handle_to_kern() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_kern != NULL)
#define call_vb_handle_to_kern(handle) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_kern(handle)

#define ckfn_vb_handle_to_phys() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_phys != NULL)
#define call_vb_handle_to_phys(handle) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_phys(handle)

#define ckfn_vb_handle_to_blk_size() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_blk_size != NULL)
#define call_vb_handle_to_blk_size(handle) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_blk_size(handle)

#define ckfn_vb_user_add() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_user_add != NULL)
#define call_vb_user_add(pool_id, phy_addr, uid) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_user_add(pool_id, phy_addr, uid)

#define ckfn_vb_user_sub() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_user_sub != NULL)
#define call_vb_user_sub(pool_id, phy_addr, uid) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_user_sub(pool_id, phy_addr, uid)

#define ckfn_vb_inquire_user_cnt() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_user_cnt != NULL)
#define call_vb_inquire_user_cnt(handle) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_user_cnt(handle)

#define ckfn_vb_inquire_one_user_cnt() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_one_user_cnt != NULL)
#define call_vb_inquire_one_user_cnt(handle, uid) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_one_user_cnt(handle, uid)

#define ckfn_vb_inquire_total_user_cnt() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_total_user_cnt != NULL)
#define call_vb_inquire_total_user_cnt(handle) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_total_user_cnt(handle)

#define ckfn_vb_inquire_blk_cnt() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_blk_cnt != NULL)
#define call_vb_inquire_blk_cnt(uid, is_comm_pool) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_blk_cnt(uid, is_comm_pool)

#define ckfn_vb_is_blk_valid() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_is_blk_valid != NULL)
#define call_vb_is_blk_valid(pool_id, phy_addr) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_is_blk_valid(pool_id, phy_addr)

#define ckfn_vb_is_pool_id_valid() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_is_pool_id_valid != NULL)
#define call_vb_is_pool_id_valid(pool_id) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_is_pool_id_valid(pool_id)

#define ckfn_vb_inquire_pool() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_pool != NULL)
#define call_vb_inquire_pool(pool_id, pool_status) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_pool(pool_id, pool_status)

#define ckfn_vb_inquire_pool_user_cnt() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_pool_user_cnt != NULL)
#define call_vb_inquire_pool_user_cnt(pool_id, uid, cnt) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_inquire_pool_user_cnt(pool_id, uid, cnt)

#define ckfn_vb_handle_to_supplement() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_supplement != NULL)
#define call_vb_handle_to_supplement(handle) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_handle_to_supplement(handle)

#define ckfn_vb_is_supplement_support() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_is_supplement_support != NULL)
#define call_vb_is_supplement_support(mask) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_is_supplement_support(mask)

#define ckfn_vb_copy_supplement() \
    (func_entry(vb_export_func, HI_ID_VB)->pfn_vb_copy_supplement != NULL)
#define call_vb_copy_supplement(dst_v_frame, src_v_frame) \
    func_entry(vb_export_func, HI_ID_VB)->pfn_vb_copy_supplement(dst_v_frame, src_v_frame)
#endif /* __VB_EXT_H__ */

