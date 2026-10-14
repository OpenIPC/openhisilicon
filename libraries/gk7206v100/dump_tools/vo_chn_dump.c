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
#include "xmedia_vo.h"

#define VALUE_BETWEEN(x,min,max) (((x) >= (min)) && ((x) <= (max)))

static volatile sig_atomic_t g_vo_signal_flag = 0;
static xmedia_vo_layer g_vo_layer_id = 0;
static xmedia_vo_chn g_vo_chn_id = 0;
static xmedia_video_frame_info g_vo_frame_info;

static FILE* g_vo_fd = XMEDIA_NULL;

static void vo_dump_8bit_yuv(xmedia_video_frame *video_frame, FILE *fd)
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


void vo_chn_dump_handle_signal(xmedia_s32 signal)
{
    if (g_vo_signal_flag != 0) {
        return;
    }

    if (signal == SIGINT || signal == SIGTERM) {
        g_vo_signal_flag = 1;
    }

    return;
}

xmedia_void vo_misc_dump(xmedia_vo_layer layer_id, xmedia_vo_chn chn_id, xmedia_u32 frame_cnt)
{
    xmedia_s32 ret;
    xmedia_char yuv_name[128];
    xmedia_char pixel_format[10];
    xmedia_char sys_chmod[140];
    xmedia_s32 millisec = -1;
    xmedia_vo_chn_attr chn_attr;
    xmedia_s32 i = 0;

    ret = xmedia_vo_init();
    if (ret != XMEDIA_SUCCESS) {
        printf("vo init failed!\n");
        return;
    }

    ret = xmedia_vo_get_chn_attr(layer_id, chn_id, &chn_attr);
    if (ret != XMEDIA_SUCCESS) {
        printf("pipe(%d) ochn(%d) get ochn attr failed!\n", layer_id, chn_id);
        goto OUT;
    }
    memset(&g_vo_frame_info, 0, sizeof(xmedia_video_frame_info));
    g_vo_frame_info.pool_id = VB_INVALID_POOLID;

    for(i = 0;i < frame_cnt;i++) {
        if (xmedia_vo_acquire_chn_frame(layer_id, chn_id, &g_vo_frame_info, millisec) != XMEDIA_SUCCESS) {
            printf("layer(%d) chn(%d) get frame error for 10 times, now exit!\n", layer_id, chn_id);
            goto OUT;
        }
        if(g_vo_fd == XMEDIA_NULL) {
            switch (g_vo_frame_info.frame.pixel_fmt) {
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
            snprintf(yuv_name, 128, "./layer%d_chn%d_%dx%d_%s_%d.yuv", layer_id, chn_id,
                g_vo_frame_info.frame.width, g_vo_frame_info.frame.height, pixel_format, frame_cnt);

            printf("layer(%d) chn(%d) dump frame to file: \"%s\"\n", layer_id, chn_id, yuv_name);
            fflush(stdout);

            g_vo_fd = fopen(yuv_name, "wb");
            if (g_vo_fd == XMEDIA_NULL) {
                printf("open file failed:%s!\n", strerror(errno));
                goto OUT;
            }
        }

        vo_dump_8bit_yuv(&g_vo_frame_info.frame, g_vo_fd);

        ret = xmedia_vo_release_chn_frame(layer_id, chn_id, &g_vo_frame_info);
        if (ret != XMEDIA_SUCCESS) {
            printf("layer(%d) chn(%d) release frame failed!\n", layer_id, chn_id);
            goto OUT;
        }
        memset(&g_vo_frame_info, 0, sizeof(xmedia_video_frame_info));
        g_vo_frame_info.pool_id = VB_INVALID_POOLID;
    }

    sprintf(sys_chmod, "chmod 666 %s", yuv_name);
    system(sys_chmod);

OUT:
    if(g_vo_fd != XMEDIA_NULL) {
        fclose(g_vo_fd);
        g_vo_fd = XMEDIA_NULL;
    }
    if(g_vo_frame_info.pool_id != VB_INVALID_POOLID) {
        ret = xmedia_vo_release_chn_frame(layer_id, chn_id, &g_vo_frame_info);
        if (ret != XMEDIA_SUCCESS) {
            printf("layer(%d) chn(%d) release frame failed!\n", layer_id, chn_id);
        }
    }
    xmedia_vo_exit();
    return;
}

static void usage(void)
{
    printf(
        "\n"
        "*************************************************\n"
        "Usage: ./vo_chn_dump [dev_id] [chn_id] [frame_cnt]\n"
        "1)dev_id: \n"
        "   vo dev id\n"
        "2)chn_id: \n"
        "   vo chn id\n"
        "3)frame_cnt: \n"
        "   the count of frame to be dumped\n"
        "*)Example:\n"
        "   e.g : ./vo_chn_dump 0 0 1\n"
        "   e.g : ./vo_chn_dump 1 0 2\n"
        "*************************************************\n"
        "\n");
}

xmedia_s32 main(int argc, char* argv[])
{
    xmedia_u32 frame_cnt;

    printf("\nNOTICE: This tool only can be used for TESTING !!!\n");
    printf("\tTo see more usage, please enter: ./vo_chn_dump -h\n\n");

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

    g_vo_layer_id = atoi(argv[1]);

    if (!VALUE_BETWEEN(g_vo_layer_id, 0, XMEDIA_VO_DEV_MAX - 1)) {
        printf("vo dev id must be [0,%d]!\n\n", XMEDIA_VO_DEV_MAX - 1);
        return -1;
    }

    g_vo_chn_id = atoi(argv[2]);

    if (!VALUE_BETWEEN(g_vo_chn_id, 0, VO_MAX_CHN_NUM - 1)) {
        printf("vo chn id must be [0,%d]!\n\n", VO_MAX_CHN_NUM - 1);
        return -1;
    }

    frame_cnt = atoi(argv[3]);
    if (frame_cnt < 1) {
        printf("frame_cnt can't be small than 1!\n\n");
        return -1;
    }

    g_vo_signal_flag = 0;
    g_vo_fd = XMEDIA_NULL;
    g_vo_frame_info.pool_id = VB_INVALID_POOLID;

    signal(SIGINT, vo_chn_dump_handle_signal);
    signal(SIGTERM, vo_chn_dump_handle_signal);

    vo_misc_dump(g_vo_layer_id, g_vo_chn_id, frame_cnt);

    return XMEDIA_SUCCESS;
}
