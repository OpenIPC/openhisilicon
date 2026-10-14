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
#include "xmedia_video_common.h"

#define MAX_FRAME_CNT   100
#define MAX_FRAME_WIDTH 3840

#define MAX_WAIT_TIME   1000 // 获取帧最长等待时间，单位ms

#define RAW_DUMP_DEPTH  2

#define FILE_NAME_LEN   256

#define r_printf(fmt, ...)                                                                  \
    do {                                                                                    \
        printf("\e[31m\e[1m [%s]-%d: " fmt "\e[0m", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

xmedia_void vi_bayer_dump_usage(xmedia_void)
{
    printf("\n"
           "***************************************************************\n"
           "Usage: ./vi_bayer_dump [pipe] [frame_cnt] [ratio_show]\n"
           "pipe: \n"
           "    0 ~ 7: Vi pipe 0 ~ 7, default is 0\n"
           "frame_cnt: \n"
           "    The count of frame to be dump, default is 1\n"
           "ratio_show: \n"
           "    Whether the file name of raw show ratio info, default is 0\n"
           "e.g: ./vi_bayer_dump 0 1 1\n"
           "***************************************************************\n"
           "\n");
    exit(1);
}

xmedia_s32 get_dump_pipe(xmedia_s32 pipe, xmedia_vi_dev_bind_pipe_attr *dev_bind_pipe)
{
    xmedia_s32 ret;
    xmedia_s32 dev;
    xmedia_s32 i, j;
    xmedia_bool is_get_dev = XMEDIA_FALSE;
    xmedia_vi_grp_config grp_config;
    xmedia_bool is_wdr_pipe = XMEDIA_FALSE;
    xmedia_vi_dev_bind_pipe_attr dev_bind_pipe_tmp;

    for (i = 0; i < VI_MAX_DEV_NUM; i++) {
        ret = xmedia_vi_get_pipe_by_dev(i, &dev_bind_pipe_tmp);
        if (ret == XMEDIA_SUCCESS) {
            for (j = 0; j < dev_bind_pipe_tmp.pipe_num; j++) {
                if (pipe == dev_bind_pipe_tmp.pipe_id[j]) {
                    dev = i;
                    is_get_dev = XMEDIA_TRUE;
                    break;
                }
            }
        }
    }

    if (is_get_dev == XMEDIA_FALSE) {
        r_printf("pipe %d get bind dev failed\n", pipe);
        return XMEDIA_FAILURE;
    }

    memset(dev_bind_pipe, 0, sizeof(xmedia_vi_dev_bind_pipe_attr));

    ret = xmedia_vi_get_pipe_by_dev(dev, dev_bind_pipe);
    if (ret != XMEDIA_SUCCESS) {
        r_printf("xmedia_vi_get_pipe_by_dev failed with 0x%x\n", ret);
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < VI_MAX_GRP_NUM; i++) {
        ret = xmedia_vi_get_grp_config(i, &grp_config);
        if (ret == XMEDIA_SUCCESS) {
            if (grp_config.grp_type == XMEDIA_VI_GRP_TYPE_WDR) {
                for (j = 0; j < VI_MAX_WDR_NUM; j++) {
                    if (grp_config.wdr_grp_config.pipe_id[j] == pipe) {
                        is_wdr_pipe = XMEDIA_TRUE;
                        break;
                    }
                }
            }
        }
    }

    if (is_wdr_pipe == XMEDIA_FALSE) {
        dev_bind_pipe->pipe_num = 1;
        dev_bind_pipe->pipe_id[0] = pipe;
    }

    return XMEDIA_SUCCESS;
}

xmedia_char *compress_mode_to_string(xmedia_video_compress_mode compress_mode)
{
    if (compress_mode == XMEDIA_VIDEO_COMPRESS_MODE_NONE) {
        return "cmp_none";
    } else if (compress_mode == XMEDIA_VIDEO_COMPRESS_MODE_LINE) {
        return "cmp_line";
    } else if (compress_mode == XMEDIA_VIDEO_COMPRESS_MODE_SEG) {
        return "cmp_seg";
    } else {
        return "cmp_xxx";
    }
}

xmedia_u32 data_width_to_num(xmedia_video_data_width data_width)
{
    if (data_width == XMEDIA_VIDEO_DATA_WIDTH_8) {
        return 8;
    } else if (data_width == XMEDIA_VIDEO_DATA_WIDTH_10) {
        return 10;
    } else if (data_width == XMEDIA_VIDEO_DATA_WIDTH_12) {
        return 12;
    } else if (data_width == XMEDIA_VIDEO_DATA_WIDTH_14) {
        return 14;
    } else if (data_width == XMEDIA_VIDEO_DATA_WIDTH_16) {
        return 16;
    } else {
        return 0;
    }
}

xmedia_s32 save_uncompress_raw_to_file(xmedia_video_frame *frame, FILE *fd)
{
    xmedia_s32 i;
    xmedia_u32 row;
    xmedia_u32 stride;
    xmedia_u64 phy_addr, size;
    xmedia_u8 *data = XMEDIA_NULL;
    xmedia_u8 *data_tmp = XMEDIA_NULL;
    xmedia_u8 *usr_page_addr = XMEDIA_NULL;
    xmedia_u16 *buf_tmp = XMEDIA_NULL;
    xmedia_u32 tmp;
    xmedia_u64 value;
    xmedia_u32 out_cnt;

    stride = frame->stride.y_stride;

    size = stride * frame->height;

    phy_addr = frame->addr.y_phy_addr;

    usr_page_addr = xmedia_mmz_map(phy_addr, size, XMEDIA_FALSE);
    if (usr_page_addr == XMEDIA_NULL) {
        r_printf("mmz map phy 0x%llx size %llu failed\n", phy_addr, size);
        return XMEDIA_FAILURE;
    }

    if (frame->bit_width != XMEDIA_VIDEO_DATA_WIDTH_8 && frame->bit_width != XMEDIA_VIDEO_DATA_WIDTH_16) {
        buf_tmp = (xmedia_u16 *)malloc(frame->width * 2); // 文件非紧凑排布，10,12,14bits不超两倍
        if (buf_tmp == XMEDIA_NULL) {
            r_printf("malloc failed\n");
            xmedia_mmz_unmap(usr_page_addr);
            return XMEDIA_FAILURE;
        }
    }

    data = usr_page_addr;

    printf("saving Y............\n");

    for (row = 0; row < frame->height; row++) {
        out_cnt = 0;
        if (frame->bit_width == XMEDIA_VIDEO_DATA_WIDTH_8) {
            fwrite(data, frame->width, 1, fd);
        } else if (frame->bit_width == XMEDIA_VIDEO_DATA_WIDTH_16) {
            fwrite(data, frame->width, 2, fd);
        } else if (frame->bit_width == XMEDIA_VIDEO_DATA_WIDTH_10) {
            // 4 pixels consist of 5 bytes
            tmp = frame->width / 4;
            for (i = 0; i < tmp; i++) {
                // byte4 byte3 byte2 byte1 byte0
                data_tmp = data + 5 * i;
                value = data_tmp[0] + ((xmedia_u32)data_tmp[1] << 8) + ((xmedia_u32)data_tmp[2] << 16) +
                    ((xmedia_u32)data_tmp[3] << 24) + ((xmedia_u64)data_tmp[4] << 32);
                buf_tmp[out_cnt++] = value & 0x3ff;
                buf_tmp[out_cnt++] = (value >> 10) & 0x3ff;
                buf_tmp[out_cnt++] = (value >> 20) & 0x3ff;
                buf_tmp[out_cnt++] = (value >> 30) & 0x3ff;
            }
            fwrite(buf_tmp, frame->width, 2, fd);
        } else if (frame->bit_width == XMEDIA_VIDEO_DATA_WIDTH_12) {
            // 2 pixels consist of 3 bytes
            tmp = frame->width / 2;
            for (i = 0; i < tmp; i++) {
                // byte2 byte1 byte0
                data_tmp = data + 3 * i;
                value = data_tmp[0] + ((xmedia_u32)data_tmp[1] << 8) + ((xmedia_u32)data_tmp[2] << 16);
                buf_tmp[out_cnt++] = value & 0xfff;
                buf_tmp[out_cnt++] = (value >> 12) & 0xfff;
            }
            fwrite(buf_tmp, frame->width, 2, fd);
        } else if (frame->bit_width == XMEDIA_VIDEO_DATA_WIDTH_14) {
            // 4 pixels consist of 7 bytes
            tmp = frame->width / 4;
            for (i = 0; i < tmp; i++) {
                // byte6 byte5 byte4 byte3 byte2 byte1 byte0
                data_tmp = data + 7 * i;
                value = data_tmp[0] + ((xmedia_u32)data_tmp[1] << 8) + ((xmedia_u32)data_tmp[2] << 16) +
                    ((xmedia_u32)data_tmp[3] << 24) + ((xmedia_u64)data_tmp[4] << 32) +
                    ((xmedia_u64)data_tmp[5] << 40) + ((xmedia_u64)data_tmp[6] << 48);
                buf_tmp[out_cnt++] = value & 0x3fff;
                buf_tmp[out_cnt++] = (value >> 14) & 0x3fff;
                buf_tmp[out_cnt++] = (value >> 28) & 0x3fff;
                buf_tmp[out_cnt++] = (value >> 42) & 0x3fff;
            }
            fwrite(buf_tmp, frame->width, 2, fd);
        }
        data += frame->stride.y_stride;
    }

    if (frame->bit_width != XMEDIA_VIDEO_DATA_WIDTH_8 && frame->bit_width != XMEDIA_VIDEO_DATA_WIDTH_16) {
        free(buf_tmp);
    }

    xmedia_mmz_unmap(usr_page_addr);

    return XMEDIA_SUCCESS;
}

xmedia_s32 save_compress_raw_to_file(xmedia_video_frame *frame, FILE *fd)
{
    // todo
    return XMEDIA_SUCCESS;
}

xmedia_s32 save_raw_to_file(xmedia_video_frame *frame, FILE *fd)
{
    if (frame->compress_mode == XMEDIA_VIDEO_COMPRESS_MODE_NONE) {
        return save_uncompress_raw_to_file(frame, fd);
    } else {
        return save_compress_raw_to_file(frame, fd);
    }
}

xmedia_s32 dump_linear_bayer(xmedia_s32 pipe, xmedia_u32 cnt, xmedia_u32 ratio_show)
{
    xmedia_s32 i, j;
    xmedia_s32 ret;
    FILE *fd = NULL;
    xmedia_char file_name[FILE_NAME_LEN] = { 0 };
    xmedia_video_frame_info *frame_info = XMEDIA_NULL;

    frame_info = malloc(sizeof(xmedia_video_frame_info) * cnt);
    if (frame_info == XMEDIA_NULL) {
        r_printf("malloc failed\n");
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < cnt; i++) {
        ret = xmedia_vi_acquire_pipe_frame(pipe, &frame_info[i], MAX_WAIT_TIME);
        if (ret != XMEDIA_SUCCESS) {
            r_printf("linear: get pipe %d frame failed with 0x%x", pipe, ret);
            r_printf("only get %d frames\n", i);
            break;
        }

        printf("linear: get pipe %d frame %d ok\n", pipe, i);
    }

    if (i == 0) {
        free(frame_info);
        return XMEDIA_FAILURE;
    }

    // make file name
    if (ratio_show == 0) {
        snprintf(file_name, FILE_NAME_LEN, "./vi_pipe_%d_%ux%u_%d_%dbits_%s_%d.raw", pipe, frame_info[0].frame.width,
                 frame_info[0].frame.height, cnt, data_width_to_num(frame_info[0].frame.bit_width),
                 compress_mode_to_string(frame_info[0].frame.compress_mode), ratio_show);
    } else {
        // todo
        snprintf(file_name, FILE_NAME_LEN, "./vi_pipe_%d_%ux%u_%d_%dbits_%s_%d.raw", pipe, frame_info[0].frame.width,
                 frame_info[0].frame.height, cnt, data_width_to_num(frame_info[0].frame.bit_width),
                 compress_mode_to_string(frame_info[0].frame.compress_mode), ratio_show);
    }

    fd = fopen(file_name, "wb");
    if (fd == NULL) {
        r_printf("open file faied\n");
        goto failed;
    }

    // todo，压缩文件，先保存压缩param

    for (j = 0; j < i; j++) {
        printf("frame %d save start\n", j);
        save_raw_to_file(&frame_info[j].frame, fd);
        printf("frame %d save completed\n", j);
        xmedia_vi_release_pipe_frame(pipe, &frame_info[j]);
    }

    fclose(fd);

    free(frame_info);

    return XMEDIA_SUCCESS;

failed:
    for (j = 0; j < i; j++) {
        xmedia_vi_release_pipe_frame(pipe, &frame_info[j]);
    }

    free(frame_info);

    return XMEDIA_FAILURE;
}

xmedia_s32 dump_wdr_bayer(xmedia_vi_dev_bind_pipe_attr *bind_attr, xmedia_u32 cnt, xmedia_u32 ratio_show)
{
    xmedia_s32 i, j, k;
    xmedia_s32 ret;
    FILE *fd = NULL;
    xmedia_u32 frame_cnt = 0;
    xmedia_char file_name[FILE_NAME_LEN] = { 0 };
    xmedia_video_frame_info *frame_info[VI_MAX_WDR_FRAME_NUM] = { XMEDIA_NULL };

    for (i = 0; i < bind_attr->pipe_num; i++) {
        frame_info[i] = malloc(sizeof(xmedia_video_frame_info) * cnt);
        if (frame_info[i] == XMEDIA_NULL) {
            r_printf("pipe %d malloc frame info failed\n", bind_attr->pipe_id[i]);
            for (j = 0; j < i; j++) {
                free(frame_info[j]);
            }
            return XMEDIA_FAILURE;
        }
    }

    for (i = 0; i < cnt; i++) {
        for (j = 0; j < bind_attr->pipe_num; j++) {
            ret = xmedia_vi_acquire_pipe_frame(bind_attr->pipe_id[j], &frame_info[j][i], MAX_WAIT_TIME);
            if (ret != XMEDIA_SUCCESS) {
                r_printf("wdr: get pipe %d frame failed with 0x%x", bind_attr->pipe_id[j], ret);
                r_printf("only get %d frames\n", i);
                frame_cnt = i;
                for (k = 0; k < bind_attr->pipe_num; k++) {
                    xmedia_vi_release_pipe_frame(bind_attr->pipe_id[k], &frame_info[k][i]);
                    goto save_frame;
                }
            } else {
                printf("wdr: get pipe %d frame %d ok\n", bind_attr->pipe_id[j], i);
            }
        }
    }

    frame_cnt = i;

    // make file name
    if (ratio_show == 0) {
        snprintf(file_name, FILE_NAME_LEN, "./vi_pipe_%d_%ux%u_%d_%dbits_%s_%d_wdr.raw", bind_attr->pipe_id[0],
                 frame_info[0][0].frame.width, frame_info[0][0].frame.height, cnt * bind_attr->pipe_num,
                 data_width_to_num(frame_info[0][0].frame.bit_width),
                 compress_mode_to_string(frame_info[0][0].frame.compress_mode), ratio_show);
    } else {
        // todo
        snprintf(file_name, FILE_NAME_LEN, "./vi_pipe_%d_%ux%u_%d_%dbits_%s_%d_wdr.raw", bind_attr->pipe_id[0],
                 frame_info[0][0].frame.width, frame_info[0][0].frame.height, cnt * bind_attr->pipe_num,
                 data_width_to_num(frame_info[0][0].frame.bit_width),
                 compress_mode_to_string(frame_info[0][0].frame.compress_mode), ratio_show);
    }

    fd = fopen(file_name, "wb");
    if (fd == NULL) {
        r_printf("open file faied\n");
        goto end;
    }

save_frame:
    if (frame_cnt == 0) {
        for (i = 0; i < bind_attr->pipe_num; i++) {
            free(frame_info[i]);
        }
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < frame_cnt; i++) {
        for (j = 0; j < bind_attr->pipe_num; j++) {
            printf("pipe %d frame %d save start\n", bind_attr->pipe_id[j], i);
            save_raw_to_file(&frame_info[j][i].frame, fd);
            printf("pipe %d frame %d save completed\n", bind_attr->pipe_id[j], i);
        }
    }

end:
    if (fd != XMEDIA_NULL) {
        fclose(fd);
    }

    for (i = 0; i < frame_cnt; i++) {
        for (j = 0; j < bind_attr->pipe_num; j++) {
            xmedia_vi_release_pipe_frame(bind_attr->pipe_id[j], &frame_info[j][i]);
        }
    }

    for (i = 0; i < bind_attr->pipe_num; i++) {
        free(frame_info[i]);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 main(xmedia_s32 argc, xmedia_char *argv[])
{
    xmedia_s32 i;
    xmedia_s32 ret;
    xmedia_s32 pipe = 0;
    xmedia_u32 frame_cnt = 1;
    xmedia_u32 ratio_show = 0;
    xmedia_vi_dev_bind_pipe_attr dev_bind_pipe;
    xmedia_vi_frame_dump_attr dump_attr;
    xmedia_vi_frame_dump_attr dump_attr_tmp;

    printf("\nNOTIC: this tool only can be used for TESTING !!!\n");
    printf("\n To see more usage, please enter: ./vi_bayer_dump -h\n\n");

    if (argc > 1) {
        if (strncmp(argv[1], "-h", 2) == 0) {
            vi_bayer_dump_usage();
            exit(XMEDIA_SUCCESS);
        } else {
            pipe = atoi(argv[1]); // pipe
        }
    }

    if (argc > 2) {
        frame_cnt = atoi(argv[2]); // the frame number
        if (frame_cnt < 1 || frame_cnt > MAX_FRAME_CNT) {
            r_printf("invalid frame cnt %u, range [1, %d]\n", frame_cnt, MAX_FRAME_CNT);
            exit(XMEDIA_FAILURE);
        }
    }

    if (argc > 3) {
        if (atoi(argv[3]) > 0) {
            ratio_show = 1;
        } else {
            ratio_show = 0;
        }
    }

    ret = xmedia_vi_init();
    if (ret != XMEDIA_SUCCESS) {
        r_printf("vi init failed !\n");
        return ret;
    }

    ret = get_dump_pipe(pipe, &dev_bind_pipe);
    if (ret != XMEDIA_SUCCESS) {
        xmedia_vi_exit();
        r_printf("get_dump_pipe failed !\n");
        return ret;
    }

    for (i = 0; i < dev_bind_pipe.pipe_num; i++) {
        ret = xmedia_vi_get_pipe_frame_dump_attr(dev_bind_pipe.pipe_id[i], &dump_attr_tmp);
        if (ret != XMEDIA_SUCCESS) {
            xmedia_vi_exit();
            r_printf("get pipe %d dump attr failed with 0x%x\n", dev_bind_pipe.pipe_id[i], ret);
            return ret;
        }

        dump_attr.enable = XMEDIA_TRUE;
        dump_attr.depth = RAW_DUMP_DEPTH;
        ret = xmedia_vi_set_pipe_frame_dump_attr(dev_bind_pipe.pipe_id[i], &dump_attr);
        if (ret != XMEDIA_SUCCESS) {
            r_printf("set pipe %d dump attr failed with 0x%x\n", dev_bind_pipe.pipe_id[i], ret);
            xmedia_vi_exit();
            return ret;
        }
    }

    if (dev_bind_pipe.pipe_num == 1) {
        dump_linear_bayer(pipe, frame_cnt, ratio_show);
    } else {
        dump_wdr_bayer(&dev_bind_pipe, frame_cnt, ratio_show);
    }

    for (i = 0; i < dev_bind_pipe.pipe_num; i++) {
        ret = xmedia_vi_set_pipe_frame_dump_attr(dev_bind_pipe.pipe_id[i], &dump_attr_tmp);
        if (ret != XMEDIA_SUCCESS) {
            r_printf("set pipe %d dump attr failed with 0x%x\n", dev_bind_pipe.pipe_id[i], ret);
            xmedia_vi_exit();
            return ret;
        }
    }

    xmedia_vi_exit();

    return XMEDIA_SUCCESS;
}
