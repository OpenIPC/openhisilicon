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

#include "xmedia_sys.h"
#include "xmedia_mmz.h"
#include "xmedia_vb.h"
#include "xmedia_vpss.h"
#include "xmedia_vgs.h"

#define VALUE_BETWEEN(x,min,max) (((x) >= (min)) && ((x) <= (max)))

#define VPSS_MAX_FRAME_CNT   50

static xmedia_u32 g_vpss_depth_flag = 0;
static volatile sig_atomic_t g_vpss_signal_flag = 0;
static xmedia_u32 g_vpss_continuous_dump = 0;

static xmedia_s32 g_vpss_pipe_id = 0;
static xmedia_s32 g_vpss_ochn_id = 0;
static xmedia_u32 g_vpss_ori_depth = 0;
static xmedia_video_frame_info g_vpss_frame_info;
static xmedia_video_frame_info g_vpss_multi_frame_info[VPSS_MAX_FRAME_CNT];

static FILE* g_vpss_fd = XMEDIA_NULL;

static void vpss_ochn_dump_8bit_yuv(xmedia_video_frame *video_frame, FILE *fd)
{
    unsigned int w, h;
    char* virt_addr_y;
    char* virt_addr_c;
    char* mem_content;
    unsigned char tmp_buff[8192];
    xmedia_u64 phy_addr;
    xmedia_u32 c_height;
    xmedia_u32 size;
    xmedia_char* page_addr[2];
    xmedia_bool uv_invert;
    xmedia_video_pixel_format pixel_fmt = video_frame->pixel_fmt;

    uv_invert = (pixel_fmt == XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_420) ? XMEDIA_TRUE : XMEDIA_FALSE;

    if (video_frame->compress_mode != XMEDIA_VIDEO_COMPRESS_MODE_NONE) {
        video_frame->stride.y_stride = video_frame->width;
        video_frame->stride.c_stride = video_frame->width;
    }

    if ((pixel_fmt == XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420) ||
        (pixel_fmt == XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_420)) {
        size = video_frame->stride.y_stride * video_frame->height * 3 / 2;
        c_height = video_frame->height / 2;
    } else if (pixel_fmt == XMEDIA_VIDEO_PIXEL_FMT_YUV_400) {
        size = video_frame->stride.y_stride * video_frame->height;
        c_height = video_frame->height;
    } else {
        size = video_frame->stride.y_stride * video_frame->height * 3 / 2;
        c_height = video_frame->height / 2;
    }

    phy_addr = video_frame->addr.y_phy_addr;

    page_addr[0] = (xmedia_char*)xmedia_mmz_map(phy_addr, size, XMEDIA_FALSE);
    if (XMEDIA_NULL == page_addr[0]) {
        return;
    }

    virt_addr_y = page_addr[0];
    virt_addr_c = virt_addr_y + (video_frame->stride.y_stride) * (video_frame->height);

    fprintf(stderr, "saving......Y......");
    fflush(stderr);
    for (h = 0; h < video_frame->height; h++) {
        mem_content = virt_addr_y + h * video_frame->stride.y_stride;
        fwrite(mem_content, video_frame->width, 1, fd);
    }

    if (pixel_fmt != XMEDIA_VIDEO_PIXEL_FMT_YUV_400) {
        fflush(fd);
        fprintf(stderr, "U......");
        fflush(stderr);

        for (h = 0; h < c_height; h++) {
            mem_content = virt_addr_c + h * video_frame->stride.c_stride;

            if (!uv_invert) {
                mem_content += 1;
            }

            for (w = 0; w < video_frame->width / 2; w++) {
                tmp_buff[w] = *mem_content;
                mem_content += 2;
            }
            fwrite(tmp_buff, video_frame->width / 2, 1, fd);
        }
        fflush(fd);

        fprintf(stderr, "V......");
        fflush(stderr);
        for (h = 0; h < c_height; h++) {
            mem_content = virt_addr_c + h * video_frame->stride.c_stride;

            if (uv_invert) {
                mem_content += 1;
            }

            for (w = 0; w < video_frame->width / 2; w++) {
                tmp_buff[w] = *mem_content;
                mem_content += 2;
            }
            fwrite(tmp_buff, video_frame->width / 2, 1, fd);
        }
    }
    fflush(fd);

    fprintf(stderr, "done!\n");
    fflush(stderr);

    xmedia_mmz_unmap(page_addr[0]);
    page_addr[0] = XMEDIA_NULL;
}

static xmedia_s32 vpss_ochn_restore(xmedia_s32 pipe_id, xmedia_s32 ochn_id)
{
    xmedia_s32 ret = XMEDIA_FAILURE;
    xmedia_u32 i;

    if (g_vpss_continuous_dump == 0) {
        if (g_vpss_frame_info.pool_id != VB_INVALID_POOLID) {
            ret = xmedia_vpss_release_ochn_frame(pipe_id, ochn_id, &g_vpss_frame_info);
            if (ret != XMEDIA_SUCCESS) {
                printf("pipe(%d) ochn(%d) release frame failed!\n", pipe_id, ochn_id);
            }

            g_vpss_frame_info.pool_id = VB_INVALID_POOLID;
        }
    } else {
        for (i = 0; i < VPSS_MAX_FRAME_CNT; i++) {
            if (g_vpss_multi_frame_info[i].pool_id != VB_INVALID_POOLID) {
                ret = xmedia_vpss_release_ochn_frame(pipe_id, ochn_id, &g_vpss_multi_frame_info[i]);
                if (ret != XMEDIA_SUCCESS) {
                    printf("pipe(%d) ochn(%d) release frame failed!\n", pipe_id, ochn_id);
                }

                g_vpss_multi_frame_info[i].pool_id = VB_INVALID_POOLID;
            }
        }
    }

    if (g_vpss_fd != XMEDIA_NULL) {
        fclose(g_vpss_fd);
        g_vpss_fd = XMEDIA_NULL;
    }

    if (g_vpss_depth_flag != 0) {
        if (ochn_id < VPSS_MAX_PHY_OCHN_NUM) {
            xmedia_vpss_ochn_attr ochn_attr = {0};

            ret = xmedia_vpss_get_ochn_attr(pipe_id, ochn_id, &ochn_attr) ;
            if (ret != XMEDIA_SUCCESS) {
                printf("pipe(%d) ochn(%d) get ochn attr failed!\n", pipe_id, ochn_id);
            }

            ochn_attr.depth = g_vpss_ori_depth;

            ret = xmedia_vpss_set_ochn_attr(pipe_id, ochn_id, &ochn_attr) ;
            if (ret != XMEDIA_SUCCESS) {
                printf("pipe(%d) ochn(%d) set ochn attr failed!\n", pipe_id, ochn_id);
            }
        } else {
            xmedia_vpss_ext_ochn_attr ext_ochn_attr = {0};

            ret = xmedia_vpss_get_ext_ochn_attr(pipe_id, ochn_id, &ext_ochn_attr) ;
            if (ret != XMEDIA_SUCCESS) {
                printf("pipe(%d) ochn(%d) get ext ochn attr failed!\n", pipe_id, ochn_id);
            }

            ext_ochn_attr.depth = g_vpss_ori_depth;

            ret = xmedia_vpss_set_ext_ochn_attr(pipe_id, ochn_id, &ext_ochn_attr) ;
            if (ret != XMEDIA_SUCCESS) {
                printf("pipe(%d) ochn(%d) set ext ochn attr failed!\n", pipe_id, ochn_id);
            }
        }

        g_vpss_depth_flag = 0;
    }

    ret = xmedia_vpss_exit();
    if (ret != XMEDIA_SUCCESS) {
        printf("vpss exit failed!\n");
    }

    return XMEDIA_SUCCESS;
}

void vpss_ochn_dump_handle_signal(xmedia_s32 signal)
{
    if (g_vpss_signal_flag != 0) {
        exit(-1);
    }

    if (signal == SIGINT || signal == SIGTERM) {
        g_vpss_signal_flag++;
        vpss_ochn_restore(g_vpss_pipe_id, g_vpss_ochn_id);
        g_vpss_signal_flag--;
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }

    exit(-1);
}

static xmedia_s32 vpss_ochn_dump_transform_format(xmedia_video_frame_info *frame_in,
    xmedia_video_frame_info *tmp_frame, xmedia_video_frame_info *frame_out)
{
    xmedia_s32 ret;
    xmedia_vgs_frame_info task_info;
    xmedia_s32 job_handle;

    ret = xmedia_vgs_init();
    if (ret != XMEDIA_SUCCESS) {
        printf("vgs init failed\n");
        return ret;
    }

    ret = xmedia_vgs_create_job(&job_handle);
    if (ret != XMEDIA_SUCCESS) {
        printf("create vgs job failed\n");
        xmedia_vgs_exit();
        goto EXIT1;
    }

    memcpy(&task_info.img_in, tmp_frame, sizeof(xmedia_video_frame_info));
    memcpy(&task_info.img_out, frame_out, sizeof(xmedia_video_frame_info));
    ret = xmedia_vgs_add_task_rotation(job_handle, &task_info, XMEDIA_VIDEO_ROTATION_0);
    if (ret != XMEDIA_SUCCESS) {
        printf("add vgs task failed\n");
        goto EXIT2;
    }

    ret = xmedia_vgs_submit_job(job_handle);
    if (ret != XMEDIA_SUCCESS) {
        printf("submit vgs job failed\n");
        goto EXIT2;
    }

    ret = xmedia_vgs_wait_job(job_handle, 2000);
    if (ret != XMEDIA_SUCCESS) {
        printf("wait vgs job failed\n");
        goto EXIT1;
    }

    xmedia_vgs_exit();

    return XMEDIA_SUCCESS;

EXIT2:
    xmedia_vgs_cancel_job(job_handle);
EXIT1:
    xmedia_vgs_exit();

    return XMEDIA_FAILURE;
}
static xmedia_void vpss_ochn_uncontinuous_dump(xmedia_s32 pipe_id, xmedia_s32 ochn_id,
    xmedia_u32 frame_cnt, xmedia_char *yuv_name)
{
    xmedia_s32 ret;
    xmedia_u32 cnt = frame_cnt;
    xmedia_s32 millisec = -1;
    xmedia_handle vb_handle = VB_INVALID_HANDLE;
    xmedia_handle tmp_handle = VB_INVALID_HANDLE;
    xmedia_vb_base_info base_info;
    xmedia_vb_cal_cfg cal_cfg;
    xmedia_video_frame_info tmp_frame = {0};
    xmedia_video_frame_info frame_out = {0};
    xmedia_vb_config vb_config = {0};
    xmedia_u32 pool_num = 0;
    xmedia_s32 vb_pool_init = 0;

    while (cnt--) {
        if (xmedia_vpss_acquire_ochn_frame(pipe_id, ochn_id, &g_vpss_frame_info, millisec) != XMEDIA_SUCCESS) {
            printf("pipe(%d) ochn(%d) get frame failed!\n", pipe_id, ochn_id);
            usleep(1000);
            continue;
        }

        printf("pipe(%d) ochn(%d) video_fmt(%d) save frame %d!\n", pipe_id, ochn_id,
            g_vpss_frame_info.frame.video_fmt, cnt);

        if (g_vpss_frame_info.frame.video_fmt != XMEDIA_VIDEO_FMT_LINEAR) {
            base_info.align = 0;
            base_info.bit_width = g_vpss_frame_info.frame.bit_width;
            base_info.cmp_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
            base_info.width = g_vpss_frame_info.frame.width;
            base_info.height = g_vpss_frame_info.frame.height;
            base_info.pixel_fmt = g_vpss_frame_info.frame.pixel_fmt;

            if(vb_pool_init == 0) {
                base_info.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;
                ret = xmedia_vb_get_buffer_config(&base_info, &cal_cfg);
                if (ret != XMEDIA_SUCCESS) {
                    printf("get buffer config failed with 0x%x\n", ret);
                    goto RELEASE;
                }

                vb_config.common_pool[pool_num].block_size = cal_cfg.vb_size;
                vb_config.common_pool[pool_num].block_cnt = 1;
                vb_config.common_pool[pool_num].map_mode = XMEDIA_VB_MAP_MODE_NONE;
                pool_num++;
                vb_config.max_pool_cnt = pool_num;

                ret = xmedia_vb_init(&vb_config);
                if (ret != XMEDIA_SUCCESS) {
                    printf("vb init failed\n");
                    goto RELEASE;
                }

                vb_pool_init = 1;
            }

            memcpy(&tmp_frame, &g_vpss_frame_info, sizeof(xmedia_video_frame_info));

            base_info.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;
            ret = xmedia_vb_get_buffer_config(&base_info, &cal_cfg);
            if (ret != XMEDIA_SUCCESS) {
                printf("get buffer config failed with 0x%x\n", ret);
                goto FREEVB;
            }

            vb_handle = xmedia_vb_get_block(-1, cal_cfg.vb_size, XMEDIA_NULL);
            if (vb_handle == VB_INVALID_HANDLE) {
                printf("get block failed\n");
                goto FREEVB;
            }

            memcpy(&frame_out, &tmp_frame, sizeof(xmedia_video_frame_info));
            frame_out.pool_id = xmedia_vb_handle_to_pool_id(vb_handle);
            frame_out.frame.addr.y_phy_addr = xmedia_vb_handle_to_phy_addr(vb_handle);
            frame_out.frame.addr.y_head_phy_addr = frame_out.frame.addr.y_phy_addr;
            frame_out.frame.addr.c_phy_addr = frame_out.frame.addr.y_phy_addr + cal_cfg.main_y_size;
            frame_out.frame.stride.y_stride = cal_cfg.main_stride;
            frame_out.frame.stride.c_stride = cal_cfg.main_stride;
            frame_out.frame.compress_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
            frame_out.frame.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;

            ret = vpss_ochn_dump_transform_format(&g_vpss_frame_info, &tmp_frame, &frame_out);
            if (ret != XMEDIA_SUCCESS) {
                printf("pipe %d ochn %d vpss_ochn_dump_transform_format failed\n", pipe_id, ochn_id);
                goto FREEVB;
            }
        } else {
            memcpy(&frame_out, &g_vpss_frame_info, sizeof(xmedia_video_frame_info));
        }

        vpss_ochn_dump_8bit_yuv(&frame_out.frame, g_vpss_fd);
FREEVB:
        if (tmp_handle != VB_INVALID_HANDLE) {
            xmedia_vb_release_block(tmp_handle);
            tmp_handle = VB_INVALID_HANDLE;
        }

        if (vb_handle != VB_INVALID_HANDLE) {
            xmedia_vb_release_block(vb_handle);
            vb_handle = VB_INVALID_HANDLE;
        }
RELEASE:
        ret = xmedia_vpss_release_ochn_frame(pipe_id, ochn_id, &g_vpss_frame_info);
        if (ret != XMEDIA_SUCCESS) {
            printf("pipe(%d) ochn(%d) release frame failed!\n", pipe_id, ochn_id);
            break;
        }

        g_vpss_frame_info.pool_id = VB_INVALID_POOLID;
    }

    if(vb_pool_init == 1) {
        ret = xmedia_vb_exit();
        if (ret != XMEDIA_SUCCESS) {
            printf("vb exit fail!\n");
            return;
        }
    }
    return;
}

static xmedia_void vpss_ochn_continuous_dump(xmedia_s32 pipe_id, xmedia_s32 ochn_id,
    xmedia_u32 frame_cnt, xmedia_char *yuv_name)
{
    xmedia_s32 ret;
    xmedia_u32 cnt = frame_cnt;
    xmedia_s32 millisec = -1;
    xmedia_u32 get_frame_cnt = 0;
    xmedia_handle vb_handle = VB_INVALID_HANDLE;
    xmedia_handle tmp_handle = VB_INVALID_HANDLE;
    xmedia_vb_base_info base_info;
    xmedia_vb_cal_cfg cal_cfg;
    xmedia_video_frame_info tmp_frame = {0};
    xmedia_video_frame_info frame_out = {0};
    xmedia_vb_config vb_config = {0};
    xmedia_u32 pool_num = 0;
    xmedia_s32 vb_pool_init = 0;

    while (cnt--) {
        if (xmedia_vpss_acquire_ochn_frame(pipe_id, ochn_id, &g_vpss_multi_frame_info[get_frame_cnt], millisec) !=
            XMEDIA_SUCCESS) {
            printf("pipe(%d) ochn(%d) get frame failed!\n", pipe_id, ochn_id);
            usleep(1000);
            continue;
        }

        get_frame_cnt++;
        if (get_frame_cnt >= VPSS_MAX_FRAME_CNT) {
            printf("pipe(%d) ochn(%d) get frame cnt err!\n", pipe_id, ochn_id);
            return;
        }
    }

    cnt = 0;

    while (cnt < get_frame_cnt) {
        printf("pipe(%d) ochn(%d) save frame %d!\n", pipe_id, ochn_id, (get_frame_cnt - cnt - 1));

        if (g_vpss_multi_frame_info[cnt].frame.video_fmt != XMEDIA_VIDEO_FMT_LINEAR) {
            base_info.align = 0;
            base_info.bit_width = g_vpss_multi_frame_info[cnt].frame.bit_width;
            base_info.cmp_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
            base_info.width = g_vpss_multi_frame_info[cnt].frame.width;
            base_info.height = g_vpss_multi_frame_info[cnt].frame.height;
            base_info.pixel_fmt = g_vpss_multi_frame_info[cnt].frame.pixel_fmt;

            if(vb_pool_init == 0) {
                if (g_vpss_frame_info.frame.video_fmt == XMEDIA_VIDEO_FMT_TILE_16x4) {
                    base_info.video_fmt = XMEDIA_VIDEO_FMT_TILE_64x4;
                    ret = xmedia_vb_get_buffer_config(&base_info, &cal_cfg);
                    if (ret != XMEDIA_SUCCESS) {
                        printf("get buffer config failed with 0x%x\n", ret);
                        goto RELEASE;
                    }

                    vb_config.common_pool[pool_num].block_size = cal_cfg.vb_size;
                    vb_config.common_pool[pool_num].block_cnt = 1;
                    vb_config.common_pool[pool_num].map_mode = XMEDIA_VB_MAP_MODE_NONE;
                    pool_num++;
                }

                base_info.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;
                ret = xmedia_vb_get_buffer_config(&base_info, &cal_cfg);
                if (ret != XMEDIA_SUCCESS) {
                    printf("get buffer config failed with 0x%x\n", ret);
                    goto RELEASE;
                }

                vb_config.common_pool[pool_num].block_size = cal_cfg.vb_size;
                vb_config.common_pool[pool_num].block_cnt = 1;
                vb_config.common_pool[pool_num].map_mode = XMEDIA_VB_MAP_MODE_NONE;
                pool_num++;
                vb_config.max_pool_cnt = pool_num;

                ret = xmedia_vb_init(&vb_config);
                if (ret != XMEDIA_SUCCESS) {
                    printf("vb init failed\n");
                    goto RELEASE;
                }

                vb_pool_init = 1;
            }

            if (g_vpss_multi_frame_info[cnt].frame.video_fmt == XMEDIA_VIDEO_FMT_TILE_16x4) {
                base_info.video_fmt = XMEDIA_VIDEO_FMT_TILE_64x4;
                ret = xmedia_vb_get_buffer_config(&base_info, &cal_cfg);
                if (ret != XMEDIA_SUCCESS) {
                    printf("get buffer config failed with 0x%x\n", ret);
                    goto RELEASE;
                }

                tmp_handle = xmedia_vb_get_block(-1, cal_cfg.vb_size, XMEDIA_NULL);
                if (tmp_handle == VB_INVALID_HANDLE) {
                    printf("get block failed\n");
                    goto RELEASE;
                }

                memcpy(&tmp_frame, &g_vpss_multi_frame_info[cnt], sizeof(xmedia_video_frame_info));
                tmp_frame.pool_id = xmedia_vb_handle_to_pool_id(tmp_handle);
                tmp_frame.frame.addr.y_phy_addr = xmedia_vb_handle_to_phy_addr(tmp_handle);
                tmp_frame.frame.addr.y_head_phy_addr = tmp_frame.frame.addr.y_phy_addr;
                tmp_frame.frame.addr.c_phy_addr = tmp_frame.frame.addr.y_phy_addr + cal_cfg.main_y_size;
                tmp_frame.frame.stride.y_stride = cal_cfg.main_stride;
                tmp_frame.frame.stride.c_stride = cal_cfg.main_stride;
                tmp_frame.frame.compress_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
                tmp_frame.frame.video_fmt = XMEDIA_VIDEO_FMT_TILE_64x4;
            } else {
                memcpy(&tmp_frame, &g_vpss_multi_frame_info[cnt], sizeof(xmedia_video_frame_info));
            }

            base_info.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;
            ret = xmedia_vb_get_buffer_config(&base_info, &cal_cfg);
            if (ret != XMEDIA_SUCCESS) {
                printf("get buffer config failed with 0x%x\n", ret);
                goto FREEVB;
            }

            vb_handle = xmedia_vb_get_block(-1, cal_cfg.vb_size, XMEDIA_NULL);
            if (vb_handle == VB_INVALID_HANDLE) {
                printf("get block failed\n");
                goto FREEVB;
            }

            memcpy(&frame_out, &tmp_frame, sizeof(xmedia_video_frame_info));
            frame_out.pool_id = xmedia_vb_handle_to_pool_id(vb_handle);
            frame_out.frame.addr.y_phy_addr = xmedia_vb_handle_to_phy_addr(vb_handle);
            frame_out.frame.addr.y_head_phy_addr = frame_out.frame.addr.y_phy_addr;
            frame_out.frame.addr.c_phy_addr = frame_out.frame.addr.y_phy_addr + cal_cfg.main_y_size;
            frame_out.frame.stride.y_stride = cal_cfg.main_stride;
            frame_out.frame.stride.c_stride = cal_cfg.main_stride;
            frame_out.frame.compress_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
            frame_out.frame.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;

            ret = vpss_ochn_dump_transform_format(&g_vpss_multi_frame_info[cnt], &tmp_frame, &frame_out);
            if (ret != XMEDIA_SUCCESS) {
                printf("pipe %d ochn %d vpss_ochn_dump_transform_format failed\n", pipe_id, ochn_id);
                goto FREEVB;
            }

        } else {
            memcpy(&frame_out, &g_vpss_multi_frame_info[cnt], sizeof(xmedia_video_frame_info));
        }

        vpss_ochn_dump_8bit_yuv(&frame_out.frame, g_vpss_fd);
FREEVB:
        if (tmp_handle != VB_INVALID_HANDLE) {
            xmedia_vb_release_block(tmp_handle);
            tmp_handle = VB_INVALID_HANDLE;
        }

        if (vb_handle != VB_INVALID_HANDLE) {
            xmedia_vb_release_block(vb_handle);
            vb_handle = VB_INVALID_HANDLE;
        }
RELEASE:
        ret = xmedia_vpss_release_ochn_frame(pipe_id, ochn_id, &g_vpss_multi_frame_info[cnt]);
        if (ret != XMEDIA_SUCCESS) {
            printf("pipe(%d) ochn(%d) release frame failed!\n", pipe_id, ochn_id);
            cnt++;
            continue;
        }

        g_vpss_multi_frame_info[cnt].pool_id = VB_INVALID_POOLID;

        cnt++;
    }

    if(vb_pool_init == 1) {
        ret = xmedia_vb_exit();
        if (ret != XMEDIA_SUCCESS) {
            printf("vb exit fail!\n");
            return;
        }
    }

    return;
}

xmedia_void vpss_ochn_misc_dump(xmedia_s32 pipe_id, xmedia_s32 ochn_id, xmedia_u32 frame_cnt)
{
    xmedia_s32 ret;
    xmedia_char yuv_name[128];
    xmedia_char pixel_format[10];
    xmedia_char sys_chmod[140];
    xmedia_u32 cnt = frame_cnt;
    xmedia_u32 depth = 1;
    xmedia_s32 millisec = -1;

    ret = xmedia_vpss_init();
    if (ret != XMEDIA_SUCCESS) {
        printf("vpss init failed!\n");
        return;
    }

    if (ochn_id < VPSS_MAX_PHY_OCHN_NUM) {
        xmedia_vpss_ochn_attr ochn_attr = {0};

        ret = xmedia_vpss_get_ochn_attr(pipe_id, ochn_id, &ochn_attr);
        if (ret != XMEDIA_SUCCESS) {
            printf("pipe(%d) ochn(%d) get ochn attr failed!\n", pipe_id, ochn_id);
            return;
        }

        g_vpss_ori_depth = ochn_attr.depth;
        ochn_attr.depth = depth;

        ret = xmedia_vpss_set_ochn_attr(pipe_id, ochn_id, &ochn_attr);
        if (ret != XMEDIA_SUCCESS) {
            printf("pipe(%d) ochn(%d) set ochn attr failed!\n", pipe_id, ochn_id);
            vpss_ochn_restore(pipe_id, ochn_id);
            return;
        }
    } else {
        xmedia_vpss_ext_ochn_attr ext_ochn_attr = {0};

        ret = xmedia_vpss_get_ext_ochn_attr(pipe_id, ochn_id, &ext_ochn_attr);
        if (ret != XMEDIA_SUCCESS) {
            printf("pipe(%d) ochn(%d) get ext ochn attr failed!\n", pipe_id, ochn_id);
            return;
        }

        g_vpss_ori_depth = ext_ochn_attr.depth;
        ext_ochn_attr.depth = depth;

        ret = xmedia_vpss_set_ext_ochn_attr(pipe_id, ochn_id, &ext_ochn_attr);
        if (ret != XMEDIA_SUCCESS) {
            printf("pipe(%d) ochn(%d) set ext ochn attr failed!\n", pipe_id, ochn_id);
            vpss_ochn_restore(pipe_id, ochn_id);
            return;
        }
    }

    g_vpss_depth_flag = 1;

    memset(&g_vpss_frame_info, 0, sizeof(xmedia_video_frame_info));
    g_vpss_frame_info.pool_id = VB_INVALID_POOLID;

    if (xmedia_vpss_acquire_ochn_frame(pipe_id, ochn_id, &g_vpss_frame_info, millisec) != XMEDIA_SUCCESS) {
        printf("pipe(%d) ochn(%d) get frame error, now exit!\n", pipe_id, ochn_id);
        goto EXIT;
    }

    switch (g_vpss_frame_info.frame.pixel_fmt) {
        case XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420:
        case XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_420:
            snprintf(pixel_format, 10, "P420");
            break;

        case XMEDIA_VIDEO_PIXEL_FMT_YUV_400:
            snprintf(pixel_format, 10, "YUV400");
            break;

        default:
            snprintf(pixel_format, 10, "P420");
            break;
    }

    snprintf(yuv_name, 128, "./pipe%d_ochn%d_%dx%d_%s_%d.yuv", pipe_id, ochn_id,
        g_vpss_frame_info.frame.width, g_vpss_frame_info.frame.height, pixel_format, cnt);

    printf("pipe(%d) ochn(%d) dump frame to file: \"%s\"\n", pipe_id, ochn_id, yuv_name);

    fflush(stdout);

    ret = xmedia_vpss_release_ochn_frame(pipe_id, ochn_id, &g_vpss_frame_info);
    if (ret != XMEDIA_SUCCESS) {
        printf("pipe(%d) ochn(%d) release frame failed!\n", pipe_id, ochn_id);
        goto EXIT;
    }

    g_vpss_frame_info.pool_id = VB_INVALID_POOLID;

    g_vpss_fd = fopen(yuv_name, "wb");
    if (g_vpss_fd == XMEDIA_NULL) {
        printf("open file failed:%s!\n", strerror(errno));
        goto EXIT;
    }

    if (g_vpss_continuous_dump == 0) {
        vpss_ochn_uncontinuous_dump(pipe_id, ochn_id, frame_cnt, yuv_name);
    } else {
        vpss_ochn_continuous_dump(pipe_id, ochn_id, frame_cnt, yuv_name);
    }

    sprintf(sys_chmod, "chmod 666 %s", yuv_name);
    system(sys_chmod);

EXIT:
    vpss_ochn_restore(pipe_id, ochn_id);

    return;
}

static void usage(void)
{
    printf(
        "\n"
        "*************************************************\n"
        "Usage: ./vpss_ochn_dump [pipe_id] [ochn_id] [frame_cnt] [mode]\n"
        "1)pipe_id: \n"
        "   vpss pipe id\n"
        "2)ochn_id: \n"
        "   vpss ochn id\n"
        "3)frame_cnt: \n"
        "   the count of frame to be dumped\n"
        "4)mode: \n"
        "   0: uncontinuous dump\n"
        "   1: continuous dump\n"
        "*)Example:\n"
        "   e.g : ./vpss_ochn_dump 0 0 1 0\n"
        "   e.g : ./vpss_ochn_dump 1 0 2 1\n"
        "*************************************************\n"
        "\n");
}

xmedia_s32 main(int argc, char* argv[])
{
    xmedia_u32 frame_cnt;
    xmedia_u32 i;

    printf("\nNOTICE: This tool only can be used for TESTING !!!\n");
    printf("\tTo see more usage, please enter: ./vpss_ochn_dump -h\n\n");

    if (argc > 1) {
        if (!strncmp(argv[1], "-h", 2)) {
            usage();
            exit(XMEDIA_SUCCESS);
        }
    }

    if (argc < 5) {
        usage();
        exit(XMEDIA_SUCCESS);
    }

    g_vpss_pipe_id = atoi(argv[1]);

    if (!VALUE_BETWEEN(g_vpss_pipe_id, 0, VPSS_MAX_PIPE_NUM - 1)) {
        printf("vpss pipe id must be [0,%d]!\n\n", VPSS_MAX_PIPE_NUM - 1);
        return -1;
    }

    g_vpss_ochn_id = atoi(argv[2]);

    if (!VALUE_BETWEEN(g_vpss_ochn_id, 0, VPSS_MAX_OCHN_NUM - 1)) {
        printf("vpss ochn id must be [0,%d]!\n\n", VPSS_MAX_OCHN_NUM - 1);
        return -1;
    }

    frame_cnt = atoi(argv[3]);
    if (frame_cnt < 1) {
        printf("frame_cnt can't be small than 1!\n\n");
        return -1;
    }

    g_vpss_continuous_dump = atoi(argv[4]);

    if (!VALUE_BETWEEN(g_vpss_continuous_dump, 0, 1)) {
        printf("vpss dump mode must be [0,1]!\n\n");
        return -1;
    }

    if ((g_vpss_continuous_dump == 1) && (frame_cnt > VPSS_MAX_FRAME_CNT)) {
        printf("frame_cnt must be [1,%d] in continuous mode!\n\n", VPSS_MAX_FRAME_CNT);
        return -1;
    }

    g_vpss_depth_flag = 0;
    g_vpss_signal_flag = 0;
    g_vpss_ori_depth = 0;
    g_vpss_fd = XMEDIA_NULL;
    g_vpss_frame_info.pool_id = VB_INVALID_POOLID;

    for (i = 0; i < VPSS_MAX_FRAME_CNT; i++) {
        g_vpss_multi_frame_info[i].pool_id = VB_INVALID_POOLID;
    }

    signal(SIGINT, vpss_ochn_dump_handle_signal);
    signal(SIGTERM, vpss_ochn_dump_handle_signal);

    vpss_ochn_misc_dump(g_vpss_pipe_id, g_vpss_ochn_id, frame_cnt);

    return XMEDIA_SUCCESS;
}
