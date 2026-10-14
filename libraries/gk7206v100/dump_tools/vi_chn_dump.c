/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "xmedia_vi.h"
#include "xmedia_mmz.h"
#include "xmedia_vb.h"
#include "xmedia_vgs.h"
#include "xmedia_video_common.h"

#define MAX_FRAME_CNT   100
#define MAX_FRAME_WIDTH 3840

#define MAX_WAIT_TIME   1000 // 获取帧最长等待时间，单位ms

#define YUV_DUMP_DEPTH  2

#define FILE_NAME_LEN   256

#define PIXEL_NAME_LEN  10
#define VIDEO_FRAME_LEN 20

#define r_printf(fmt, ...)                                                                  \
    do {                                                                                    \
        printf("\e[31m\e[1m [%s]-%d: " fmt "\e[0m", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

xmedia_void vi_chn_dump_usage(xmedia_void)
{
    printf("\n"
           "***************************************************************\n"
           "Usage: ./vi_chn_dump [pipe] [chn] [frame_cnt]\n"
           "pipe: \n"
           "    0 ~ 7: Vi pipe 0 ~ 7\n"
           "chn: \n"
           "    0: Vi chn\n"
           "frame_cnt: \n"
           "    The count of frame to be dump\n"
           "e.g: ./vi_chn_dump 0 0 1\n"
           "***************************************************************\n"
           "\n");
    exit(1);
}

xmedia_s32 make_yuv_file_name(xmedia_s32 pipe, xmedia_s32 chn, xmedia_u32 cnt, xmedia_char *file_name)
{
    xmedia_s32 ret;
    xmedia_char pixel_format[PIXEL_NAME_LEN];
    xmedia_char video_format[VIDEO_FRAME_LEN];
    xmedia_video_frame_info frame_info;

    ret = xmedia_vi_acquire_chn_frame(pipe, chn, &frame_info, MAX_WAIT_TIME);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d get chn frame failed with 0x%x\n", pipe, chn, ret);
        return ret;
    }

    switch (frame_info.frame.pixel_fmt) {
        case XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_420:
        case XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420:
            snprintf(pixel_format, PIXEL_NAME_LEN, "p420");
            break;

        case XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_422:
        case XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_422:
            snprintf(pixel_format, PIXEL_NAME_LEN, "p422");
            break;

        case XMEDIA_VIDEO_PIXEL_FMT_YUV_400:
            snprintf(pixel_format, PIXEL_NAME_LEN, "p400");
            break;

        default:
            snprintf(pixel_format, PIXEL_NAME_LEN, "--");
            break;
    }

    switch (frame_info.frame.video_fmt) {
        case XMEDIA_VIDEO_FMT_LINEAR:
            snprintf(video_format, VIDEO_FRAME_LEN, "linear");
            break;

        case XMEDIA_VIDEO_FMT_TILE_32x4:
            snprintf(video_format, VIDEO_FRAME_LEN, "tile_32x4");
            break;

        case XMEDIA_VIDEO_FMT_TILE_64x4:
            snprintf(video_format, VIDEO_FRAME_LEN, "tile_64x4");
            break;

        default:
            snprintf(video_format, VIDEO_FRAME_LEN, "--");
            break;
    }

    snprintf(file_name, FILE_NAME_LEN, "./vi_pipe%d_chn%d_%ux%u_%s_%s_%u.yuv", pipe, chn, frame_info.frame.width,
             frame_info.frame.height, pixel_format, video_format, cnt);

    ret = xmedia_vi_release_chn_frame(pipe, chn, &frame_info);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d release chn frame failed with 0x%x\n", pipe, chn, ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 save_yuv_8bit_to_file(xmedia_video_frame *frame, FILE *fd)
{
    xmedia_u32 width, height, size;
    xmedia_u32 c_height;
    xmedia_u64 phy_addr;
    xmedia_char *usr_page_addr = XMEDIA_NULL;
    xmedia_char *y_virt = XMEDIA_NULL;
    xmedia_char *c_virt = XMEDIA_NULL;
    xmedia_char *mem_offset = XMEDIA_NULL;
    xmedia_char buf_tmp[MAX_FRAME_WIDTH];

    if (frame->pixel_fmt == XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_420 ||
        frame->pixel_fmt == XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420) {
        size = frame->stride.y_stride * frame->height * 3 / 2;
        c_height = frame->height / 2;
    } else if (frame->pixel_fmt == XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_422 ||
               frame->pixel_fmt == XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_422) {
        size = frame->stride.y_stride * frame->height * 2;
        c_height = frame->height;
    } else if (frame->pixel_fmt == XMEDIA_VIDEO_PIXEL_FMT_YUV_400) {
        size = frame->width * frame->height;
        c_height = 0;
    } else {
        r_printf("unknown pixel format %d\n", frame->pixel_fmt);
        return XMEDIA_FAILURE;
    }

    phy_addr = frame->addr.y_phy_addr;

    usr_page_addr = xmedia_mmz_map(phy_addr, size, XMEDIA_FALSE);
    if (usr_page_addr == XMEDIA_NULL) {
        r_printf("mmz map phy 0x%llx size %u failed\n", phy_addr, size);
        return XMEDIA_FAILURE;
    }

    y_virt = usr_page_addr;
    c_virt = y_virt + frame->stride.y_stride * frame->height;

    printf("saving......Y......\n");
    for (height = 0; height < frame->height; height++) {
        mem_offset = y_virt + frame->stride.y_stride * height;
        fwrite(mem_offset, frame->width, 1, fd);
    }

    if (frame->pixel_fmt != XMEDIA_VIDEO_PIXEL_FMT_YUV_400) {
        printf("saving......U......\n");
        for (height = 0; height < c_height; height++) {
            mem_offset = c_virt + frame->stride.c_stride * height;
            mem_offset += 1;

            for (width = 0; width < frame->width / 2; width++) {
                buf_tmp[width] = *mem_offset;
                mem_offset += 2;
            }
            fwrite(buf_tmp, frame->width / 2, 1, fd);
        }

        printf("saving......V......\n");
        for (height = 0; height < c_height; height++) {
            mem_offset = c_virt + frame->stride.c_stride * height;

            for (width = 0; width < frame->width / 2; width++) {
                buf_tmp[width] = *mem_offset;
                mem_offset += 2;
            }
            fwrite(buf_tmp, frame->width / 2, 1, fd);
        }
    }

    printf("done %llu\n", frame->pts);

    xmedia_mmz_unmap(usr_page_addr);

    return XMEDIA_SUCCESS;
}

xmedia_s32 submit_vgs_to_transform(xmedia_s32 pipe, xmedia_s32 chn, xmedia_video_frame_info *in_frame, FILE *fd)
{
    xmedia_s32 ret;
    xmedia_vb_base_info base_info;
    xmedia_vb_cal_cfg cal_cfg;
    xmedia_vb_pool_config pool_config = {0};
    xmedia_u32 pool_id;
    xmedia_handle vb_handle;
    xmedia_u64 phy_addr;
    xmedia_s32 handle;
    xmedia_vgs_frame_info task_info;
    xmedia_video_rotation rot = XMEDIA_VIDEO_ROTATION_0;
    xmedia_video_frame_info out_frame;
    xmedia_vb_config vb_config = {0};

    base_info.align = 0;
    base_info.bit_width = in_frame->frame.bit_width;
    base_info.cmp_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
    base_info.width = in_frame->frame.width;
    base_info.height = in_frame->frame.height;
    base_info.pixel_fmt = in_frame->frame.pixel_fmt;
    base_info.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;

    ret = xmedia_vb_get_buffer_config(&base_info, &cal_cfg);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d get buffer config failed with 0x%x\n", pipe, chn, ret);
        return ret;
    }

    vb_config.max_pool_cnt = 1;
    vb_config.common_pool[0].block_size = cal_cfg.vb_size;
    vb_config.common_pool[0].block_cnt = 1;
    ret = xmedia_vb_init(&vb_config);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("vb init failed\n");
        return ret;
    }

    pool_config.block_size = cal_cfg.vb_size;
    pool_config.block_cnt = 1;
    pool_config.map_mode = XMEDIA_VB_MAP_MODE_NONE;
    pool_id = xmedia_vb_create_pool(&pool_config);
    if (pool_id == VB_INVALID_POOLID) {
        r_printf("pipe %d chn %d create pool failed\n", pipe, chn);
        xmedia_vb_exit();
        return XMEDIA_FAILURE;
    }

    vb_handle = xmedia_vb_get_block(pool_id, cal_cfg.vb_size, XMEDIA_NULL);
    if (vb_handle == VB_INVALID_HANDLE) {
        r_printf("pipe %d chn %d get block failed\n", pipe, chn);
        xmedia_vb_exit();
        return XMEDIA_FAILURE;
    }

    phy_addr = xmedia_vb_handle_to_phy_addr(vb_handle);

    memset(&out_frame, 0, sizeof(xmedia_video_frame_info));
    out_frame.pool_id = pool_id;
    out_frame.mod_id = MOD_ID_VI;
    out_frame.frame.addr.y_phy_addr = phy_addr;
    out_frame.frame.addr.c_phy_addr = phy_addr + cal_cfg.main_y_size;
    out_frame.frame.width = in_frame->frame.width;
    out_frame.frame.height = in_frame->frame.height;
    out_frame.frame.stride.y_stride = cal_cfg.main_stride;
    out_frame.frame.stride.c_stride = cal_cfg.main_stride;
    out_frame.frame.compress_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
    out_frame.frame.pixel_fmt = in_frame->frame.pixel_fmt;
    out_frame.frame.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;
    out_frame.frame.dynamic_range = in_frame->frame.dynamic_range;
    out_frame.frame.pts = in_frame->frame.pts;
    out_frame.frame.index = in_frame->frame.index;

    ret = xmedia_vgs_init();
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d init failed\n", pipe, chn);
        xmedia_vb_release_block(vb_handle);
        xmedia_vb_destroy_pool(pool_id);
        xmedia_vb_exit();
        return ret;
    }

    ret = xmedia_vgs_create_job(&handle);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d create vgs job failed\n", pipe, chn);
        xmedia_vgs_exit();
        xmedia_vb_release_block(vb_handle);
        xmedia_vb_destroy_pool(pool_id);
        xmedia_vb_exit();
        return ret;
    }

    memcpy(&task_info.img_in, in_frame, sizeof(xmedia_video_frame_info));
    memcpy(&task_info.img_out, &out_frame, sizeof(xmedia_video_frame_info));

    ret = xmedia_vgs_add_task_rotation(handle, &task_info, rot);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d add vgs task failed\n", pipe, chn);
        xmedia_vgs_cancel_job(handle);
        xmedia_vgs_exit();
        xmedia_vb_release_block(vb_handle);
        xmedia_vb_destroy_pool(pool_id);
        xmedia_vb_exit();
        return ret;
    }

    ret = xmedia_vgs_submit_job(handle);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d submit vgs job failed\n", pipe, chn);
        xmedia_vgs_cancel_job(handle);
        xmedia_vgs_exit();
        xmedia_vb_release_block(vb_handle);
        xmedia_vb_destroy_pool(pool_id);
        xmedia_vb_exit();
        return ret;
    }

    ret = xmedia_vgs_wait_job(handle, 2000);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d wait vgs job failed\n", pipe, chn);
        xmedia_vgs_exit();
        xmedia_vb_release_block(vb_handle);
        xmedia_vb_destroy_pool(pool_id);
        xmedia_vb_exit();
        return ret;
    }

    save_yuv_8bit_to_file(&task_info.img_out.frame, fd);

    xmedia_vgs_exit();
    xmedia_vb_release_block(vb_handle);
    xmedia_vb_destroy_pool(pool_id);
    xmedia_vb_exit();

    return XMEDIA_SUCCESS;
}

xmedia_void dump_yuv(xmedia_s32 pipe, xmedia_s32 chn, xmedia_u32 cnt)
{
    xmedia_s32 ret;
    FILE *fd = XMEDIA_NULL;
    xmedia_char file_name[FILE_NAME_LEN];
    xmedia_video_frame_info frame_info;

    ret = make_yuv_file_name(pipe, chn, cnt, file_name);
    if (ret != XMEDIA_SUCCESS) {
        return;
    }

    fd = fopen(file_name, "wb");
    if (fd == XMEDIA_NULL) {
        r_printf("opne file failed\n");
        return;
    }

    while (cnt--) {
        ret = xmedia_vi_acquire_chn_frame(pipe, chn, &frame_info, MAX_WAIT_TIME);
        if (ret != XMEDIA_SUCCESS) {
            r_printf("pipe %d chn %d get chn frame %u failed with 0x%x\n", pipe, chn, cnt, ret);
            continue;
        }

        if (frame_info.frame.compress_mode != XMEDIA_VIDEO_COMPRESS_MODE_NONE ||
            frame_info.frame.video_fmt != XMEDIA_VIDEO_FMT_LINEAR) {
            submit_vgs_to_transform(pipe, chn, &frame_info, fd);
        } else {
            save_yuv_8bit_to_file(&frame_info.frame, fd);
        }

        ret = xmedia_vi_release_chn_frame(pipe, chn, &frame_info);
        if (ret != XMEDIA_SUCCESS) {
            r_printf("pipe %d chn %d release chn frame %u failed with 0x%x\n", pipe, chn, cnt, ret);
        }
    }

    fclose(fd);
}

xmedia_s32 main(xmedia_s32 argc, xmedia_char *argv[])
{
    xmedia_s32 ret;
    xmedia_s32 pipe;
    xmedia_s32 chn;
    xmedia_u32 frame_cnt = 0;
    xmedia_u32 origin_depth;
    xmedia_u32 depth;

    printf("\nNOTIC: this tool only can be used for TESTING !!!\n");
    printf("\n To see more usage, please enter: ./vi_chn_dump -h\n\n");

    if (argc > 1) {
        if (strncmp(argv[1], "-h", 2) == 0) {
            vi_chn_dump_usage();
            exit(XMEDIA_SUCCESS);
        }
    }

    if (argc < 4) {
        vi_chn_dump_usage();
        exit(XMEDIA_SUCCESS);
    }

    pipe = atoi(argv[1]);
    if (pipe < 0 || pipe >= VI_MAX_PIPE_NUM) {
        r_printf("pipe %d is illegal!\n", pipe);
        return XMEDIA_FAILURE;
    }

    chn = atoi(argv[2]);
    if (chn < 0 || chn >= VI_MAX_CHN_NUM) {
        r_printf("chn %d is illegal!\n", chn);
        return XMEDIA_FAILURE;
    }

    frame_cnt = atoi(argv[3]);
    if (frame_cnt < 0 || frame_cnt > MAX_FRAME_CNT) {
        r_printf("frame cnt %d is illegal!\n", frame_cnt);
        return XMEDIA_FAILURE;
    }

    ret = xmedia_vi_init();
    if (ret != XMEDIA_SUCCESS) {
        r_printf("vi init failed\n");
        return ret;
    }
    ret = xmedia_vi_get_chn_depth(pipe, chn, &origin_depth);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d get chn depth failed with 0x%x", pipe, chn, ret);
        xmedia_vi_exit();
        return ret;
    }

    depth = YUV_DUMP_DEPTH;
    ret = xmedia_vi_set_chn_depth(pipe, chn, depth);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d set chn depth failed with 0x%x", pipe, chn, ret);
        xmedia_vi_exit();
        return ret;
    }

    dump_yuv(pipe, chn, frame_cnt);

    depth = origin_depth;
    ret = xmedia_vi_set_chn_depth(pipe, chn, depth);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("pipe %d chn %d set chn depth failed with 0x%x", pipe, chn, ret);
        xmedia_vi_exit();
        return ret;
    }

    xmedia_vi_exit();

    return XMEDIA_SUCCESS;
}
