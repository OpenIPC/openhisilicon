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

static volatile sig_atomic_t g_vpss_signal_flag = 0;
static xmedia_u32 g_vpss_continuous_dump = 0;

static xmedia_s32 g_vpss_pipe_id = 0;
static xmedia_s32 g_vpss_ichn_id = 0;
static xmedia_video_frame_info g_vpss_frame_info;
static xmedia_video_frame_info g_vpss_multi_frame_info[VPSS_MAX_FRAME_CNT];

static FILE* g_vpss_fd = XMEDIA_NULL;

static void vpss_ichn_dump_8bit_yuv(xmedia_video_frame *video_frame, FILE *fd)
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

static xmedia_s32 vpss_ichn_restore(xmedia_s32 pipe_id, xmedia_s32 ichn_id)
{
    xmedia_s32 ret = XMEDIA_FAILURE;
    xmedia_u32 i;

    if (g_vpss_continuous_dump == 0) {
        if (g_vpss_frame_info.pool_id != VB_INVALID_POOLID) {
            ret = xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_frame_info);
            if (ret != XMEDIA_SUCCESS) {
                printf("pipe(%d) ichn(%d) release frame failed!\n", pipe_id, ichn_id);
            }

            g_vpss_frame_info.pool_id = VB_INVALID_POOLID;
        }
    } else {
        for (i = 0; i < VPSS_MAX_FRAME_CNT; i++) {
            if (g_vpss_multi_frame_info[i].pool_id != VB_INVALID_POOLID) {
                ret = xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_multi_frame_info[i]);
                if (ret != XMEDIA_SUCCESS) {
                    printf("pipe(%d) ichn(%d) release frame failed!\n", pipe_id, ichn_id);
                }

                g_vpss_multi_frame_info[i].pool_id = VB_INVALID_POOLID;
            }
        }
    }

    if (g_vpss_fd != XMEDIA_NULL) {
        fclose(g_vpss_fd);
        g_vpss_fd = XMEDIA_NULL;
    }

    ret = xmedia_vpss_exit();
    if (ret != XMEDIA_SUCCESS) {
        printf("vpss exit failed!\n");
    }

    return XMEDIA_SUCCESS;
}

void vpss_ichn_dump_handle_signal(xmedia_s32 signal)
{
    if (g_vpss_signal_flag != 0) {
        exit(-1);
    }

    if (signal == SIGINT || signal == SIGTERM) {
        g_vpss_signal_flag++;
        vpss_ichn_restore(g_vpss_pipe_id, g_vpss_ichn_id);
        g_vpss_signal_flag--;
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }

    exit(-1);
}

static xmedia_s32 vpss_ichn_dump_transform_format(xmedia_video_frame_info *frame_in,
    xmedia_video_frame_info *frame_out)
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

    memcpy(&task_info.img_in, frame_in, sizeof(xmedia_video_frame_info));
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

static xmedia_void vpss_ichn_uncontinuous_dump(xmedia_s32 pipe_id, xmedia_s32 ichn_id,
    xmedia_u32 frame_cnt, xmedia_char *yuv_name)
{
    xmedia_s32 ret;
    xmedia_u32 cnt = frame_cnt;
    xmedia_s32 millisec = -1;
    xmedia_handle vb_handle = VB_INVALID_HANDLE;
    xmedia_vb_base_info base_info;
    xmedia_vb_cal_cfg cal_cfg;
    xmedia_video_frame_info frame_out = {0};

    while (cnt--) {
        if (xmedia_vpss_acquire_ichn_frame(pipe_id, ichn_id, &g_vpss_frame_info, millisec) != XMEDIA_SUCCESS) {
            printf("pipe(%d) ichn(%d) get frame failed!\n", pipe_id, ichn_id);
            usleep(1000);
            continue;
        }

        printf("pipe(%d) ichn(%d) save frame %d!\n", pipe_id, ichn_id, cnt);

        if (g_vpss_frame_info.frame.video_fmt != XMEDIA_VIDEO_FMT_LINEAR) {
            base_info.align = 0;
            base_info.bit_width = g_vpss_frame_info.frame.bit_width;
            base_info.cmp_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
            base_info.width = g_vpss_frame_info.frame.width;
            base_info.height = g_vpss_frame_info.frame.height;
            base_info.pixel_fmt = g_vpss_frame_info.frame.pixel_fmt;
            base_info.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;

            ret = xmedia_vb_get_buffer_config(&base_info, &cal_cfg);
            if (ret != XMEDIA_SUCCESS) {
                printf("get buffer config failed with 0x%x\n", ret);
                xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_frame_info);
                return;
            }

            vb_handle = xmedia_vb_get_block(-1, cal_cfg.vb_size, XMEDIA_NULL);
            if (vb_handle == VB_INVALID_HANDLE) {
                printf("get block failed\n");
                xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_frame_info);
                return;
            }

            memcpy(&frame_out, &g_vpss_frame_info, sizeof(xmedia_video_frame_info));
            frame_out.pool_id = xmedia_vb_handle_to_pool_id(vb_handle);
            frame_out.frame.addr.y_phy_addr = xmedia_vb_handle_to_phy_addr(vb_handle);
            frame_out.frame.addr.y_head_phy_addr = frame_out.frame.addr.y_phy_addr;
            frame_out.frame.addr.c_phy_addr = frame_out.frame.addr.y_phy_addr + cal_cfg.main_y_size;
            frame_out.frame.stride.y_stride = cal_cfg.main_stride;
            frame_out.frame.stride.c_stride = cal_cfg.main_stride;
            frame_out.frame.compress_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
            frame_out.frame.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;

            ret = vpss_ichn_dump_transform_format(&g_vpss_frame_info, &frame_out);
            if (ret != XMEDIA_SUCCESS) {
                printf("pipe %d ichn %d vpss_ichn_dump_transform_format failed\n", pipe_id, ichn_id);
                xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_frame_info);
                return;
            }
        } else {
            memcpy(&frame_out, &g_vpss_frame_info, sizeof(xmedia_video_frame_info));
        }

        vpss_ichn_dump_8bit_yuv(&frame_out.frame, g_vpss_fd);

        if (g_vpss_frame_info.frame.video_fmt != XMEDIA_VIDEO_FMT_LINEAR) {
            xmedia_vb_release_block(vb_handle);
        }

        ret = xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_frame_info);
        if (ret != XMEDIA_SUCCESS) {
            printf("pipe(%d) ichn(%d) release frame failed!\n", pipe_id, ichn_id);
            return;
        }

        g_vpss_frame_info.pool_id = VB_INVALID_POOLID;
    }

    return;
}

static xmedia_void vpss_ichn_continuous_dump(xmedia_s32 pipe_id, xmedia_s32 ichn_id,
    xmedia_u32 frame_cnt, xmedia_char *yuv_name)
{
    xmedia_s32 ret;
    xmedia_u32 cnt = frame_cnt;
    xmedia_s32 millisec = -1;
    xmedia_u32 get_frame_cnt = 0;
    xmedia_handle vb_handle = VB_INVALID_HANDLE;
    xmedia_vb_base_info base_info;
    xmedia_vb_cal_cfg cal_cfg;
    xmedia_video_frame_info frame_out = {0};

    while (cnt--) {
        if (xmedia_vpss_acquire_ichn_frame(pipe_id, ichn_id, &g_vpss_multi_frame_info[get_frame_cnt], millisec) !=
            XMEDIA_SUCCESS) {
            printf("pipe(%d) ichn(%d) get frame failed!\n", pipe_id, ichn_id);
            usleep(1000);
            continue;
        }

        get_frame_cnt++;
        if (get_frame_cnt >= VPSS_MAX_FRAME_CNT) {
            printf("pipe(%d) ichn(%d) get frame cnt err!\n", pipe_id, ichn_id);
            return;
        }
    }

    cnt = 0;

    while (cnt < get_frame_cnt) {
        printf("pipe(%d) ichn(%d) save frame %d!\n", pipe_id, ichn_id, (get_frame_cnt - cnt - 1));

        if (g_vpss_multi_frame_info[cnt].frame.video_fmt != XMEDIA_VIDEO_FMT_LINEAR) {
            base_info.align = 0;
            base_info.bit_width = g_vpss_multi_frame_info[cnt].frame.bit_width;
            base_info.cmp_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
            base_info.width = g_vpss_multi_frame_info[cnt].frame.width;
            base_info.height = g_vpss_multi_frame_info[cnt].frame.height;
            base_info.pixel_fmt = g_vpss_multi_frame_info[cnt].frame.pixel_fmt;
            base_info.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;

            ret = xmedia_vb_get_buffer_config(&base_info, &cal_cfg);
            if (ret != XMEDIA_SUCCESS) {
                printf("get buffer config failed with 0x%x\n", ret);
                xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_multi_frame_info[cnt]);
                return;
            }

            vb_handle = xmedia_vb_get_block(-1, cal_cfg.vb_size, XMEDIA_NULL);
            if (vb_handle == VB_INVALID_HANDLE) {
                printf("get block failed\n");
                xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_multi_frame_info[cnt]);
                return;
            }

            memcpy(&frame_out, &g_vpss_multi_frame_info[cnt], sizeof(xmedia_video_frame_info));
            frame_out.pool_id = xmedia_vb_handle_to_pool_id(vb_handle);
            frame_out.frame.addr.y_phy_addr = xmedia_vb_handle_to_phy_addr(vb_handle);
            frame_out.frame.addr.y_head_phy_addr = frame_out.frame.addr.y_phy_addr;
            frame_out.frame.addr.c_phy_addr = frame_out.frame.addr.y_phy_addr + cal_cfg.main_y_size;
            frame_out.frame.stride.y_stride = cal_cfg.main_stride;
            frame_out.frame.stride.c_stride = cal_cfg.main_stride;
            frame_out.frame.compress_mode = XMEDIA_VIDEO_COMPRESS_MODE_NONE;
            frame_out.frame.video_fmt = XMEDIA_VIDEO_FMT_LINEAR;

            ret = vpss_ichn_dump_transform_format(&g_vpss_multi_frame_info[cnt], &frame_out);
            if (ret != XMEDIA_SUCCESS) {
                printf("pipe %d ichn %d vpss_ichn_dump_transform_format failed\n", pipe_id, ichn_id);
                xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_multi_frame_info[cnt]);
                return;
            }
        } else {
            memcpy(&frame_out, &g_vpss_multi_frame_info[cnt], sizeof(xmedia_video_frame_info));
        }

        vpss_ichn_dump_8bit_yuv(&frame_out.frame, g_vpss_fd);

        if (g_vpss_multi_frame_info[cnt].frame.video_fmt != XMEDIA_VIDEO_FMT_LINEAR) {
            xmedia_vb_release_block(vb_handle);
        }

        ret = xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_multi_frame_info[cnt]);
        if (ret != XMEDIA_SUCCESS) {
            printf("pipe(%d) ichn(%d) release frame failed!\n", pipe_id, ichn_id);
            return;
        }

        g_vpss_multi_frame_info[cnt].pool_id = VB_INVALID_POOLID;

        cnt++;
    }

    return;
}

xmedia_void vpss_ichn_misc_dump(xmedia_s32 pipe_id, xmedia_s32 ichn_id, xmedia_u32 frame_cnt)
{
    xmedia_s32 ret;
    xmedia_char yuv_name[128];
    xmedia_char pixel_format[10];
    xmedia_char sys_chmod[140];
    xmedia_u32 cnt = frame_cnt;
    xmedia_s32 millisec = -1;

    ret = xmedia_vpss_init();
    if (ret != XMEDIA_SUCCESS) {
        printf("vpss init failed!\n");
        return;
    }

    memset(&g_vpss_frame_info, 0, sizeof(xmedia_video_frame_info));
    g_vpss_frame_info.pool_id = VB_INVALID_POOLID;

    if (xmedia_vpss_acquire_ichn_frame(pipe_id, ichn_id, &g_vpss_frame_info, millisec) != XMEDIA_SUCCESS) {
        printf("pipe(%d) ichn(%d) get frame error, now exit!\n", pipe_id, ichn_id);
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

    snprintf(yuv_name, 128, "./pipe%d_ichn%d_%dx%d_%s_%d.yuv", pipe_id, ichn_id,
        g_vpss_frame_info.frame.width, g_vpss_frame_info.frame.height, pixel_format, cnt);

    printf("pipe(%d) ichn(%d) dump frame to file: \"%s\"\n", pipe_id, ichn_id, yuv_name);

    fflush(stdout);

    ret = xmedia_vpss_release_ichn_frame(pipe_id, ichn_id, &g_vpss_frame_info);
    if (ret != XMEDIA_SUCCESS) {
        printf("pipe(%d) ichn(%d) release frame failed!\n", pipe_id, ichn_id);
        goto EXIT;
    }

    g_vpss_frame_info.pool_id = VB_INVALID_POOLID;

    g_vpss_fd = fopen(yuv_name, "wb");
    if (g_vpss_fd == XMEDIA_NULL) {
        printf("open file failed:%s!\n", strerror(errno));
        goto EXIT;
    }

    if (g_vpss_continuous_dump == 0) {
        vpss_ichn_uncontinuous_dump(pipe_id, ichn_id, frame_cnt, yuv_name);
    } else {
        vpss_ichn_continuous_dump(pipe_id, ichn_id, frame_cnt, yuv_name);
    }

    sprintf(sys_chmod, "chmod 666 %s", yuv_name);
    system(sys_chmod);

EXIT:
    vpss_ichn_restore(pipe_id, ichn_id);

    return;
}

static void usage(void)
{
    printf(
        "\n"
        "*************************************************\n"
        "Usage: ./vpss_ichn_dump [pipe_id] [ichn_id] [frame_cnt] [mode]\n"
        "1)pipe_id: \n"
        "   vpss pipe id\n"
        "2)ichn_id: \n"
        "   vpss ichn id\n"
        "3)frame_cnt: \n"
        "   the count of frame to be dumped\n"
        "*)Example:\n"
        "   e.g : ./vpss_ichn_dump 0 0 1\n"
        "*************************************************\n"
        "\n");
}

xmedia_s32 main(int argc, char* argv[])
{
    xmedia_u32 frame_cnt;
    xmedia_u32 i;

    printf("\nNOTICE: This tool only can be used for TESTING !!!\n");
    printf("\tTo see more usage, please enter: ./vpss_ichn_dump -h\n\n");

    if (argc > 1) {
        if (!strncmp(argv[1], "-h", 2)) {
            usage();
            exit(XMEDIA_SUCCESS);
        }
    }

    if (argc < 4) {
        usage();
        exit(XMEDIA_SUCCESS);
    }

    g_vpss_pipe_id = atoi(argv[1]);

    if (!VALUE_BETWEEN(g_vpss_pipe_id, 0, VPSS_MAX_PIPE_NUM - 1)) {
        printf("vpss pipe id must be [0,%d]!\n\n", VPSS_MAX_PIPE_NUM - 1);
        return -1;
    }

    g_vpss_ichn_id = atoi(argv[2]);

    if (!VALUE_BETWEEN(g_vpss_ichn_id, 0, VPSS_MAX_ICHN_NUM - 1)) {
        printf("vpss ichn id must be [0,%d]!\n\n", VPSS_MAX_ICHN_NUM - 1);
        return -1;
    }

    frame_cnt = atoi(argv[3]);
    if (frame_cnt < 1) {
        printf("frame_cnt can't be small than 1!\n\n");
        return -1;
    }

    if (!VALUE_BETWEEN(g_vpss_continuous_dump, 0, 1)) {
        printf("vpss dump mode must be [0,1]!\n\n");
        return -1;
    }

    if ((g_vpss_continuous_dump == 1) && (frame_cnt > VPSS_MAX_FRAME_CNT)) {
        printf("frame_cnt must be [1,%d] in continuous mode!\n\n", VPSS_MAX_FRAME_CNT);
        return -1;
    }

    g_vpss_signal_flag = 0;
    g_vpss_fd = XMEDIA_NULL;
    g_vpss_frame_info.pool_id = VB_INVALID_POOLID;

    for (i = 0; i < VPSS_MAX_FRAME_CNT; i++) {
        g_vpss_multi_frame_info[i].pool_id = VB_INVALID_POOLID;
    }

    signal(SIGINT, vpss_ichn_dump_handle_signal);
    signal(SIGTERM, vpss_ichn_dump_handle_signal);

    vpss_ichn_misc_dump(g_vpss_pipe_id, g_vpss_ichn_id, frame_cnt);

    return XMEDIA_SUCCESS;
}
