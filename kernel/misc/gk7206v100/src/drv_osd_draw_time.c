#include "drv_misc.h"

#ifdef SUPPORT_OSD_TIME

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/timekeeping.h>
#include <linux/rtc.h>
#include <linux/time64.h>
#include "xmedia_errcode.h"
#include "drv_dev_defines.h"
#include "drv_osd_draw_time.h"
#include "drv_export.h"

typedef struct __osd_time_ext {
    misc_osd_time_attr_t osd_time_attr;
    osd_draw_context_t osd_draw_ctx;
    struct __osd_time_ext *next;
}osd_time_ext_t;

osd_time_ext_t *g_osd_time_ext_hdl = XMEDIA_NULL;
xmedia_s32 g_tz_minuteswest = 0;

#define FONT_ORIGINAL_WIDTH         8
#define FONT_ORIGINAL_HEIGHT        8

// 8x8数字字体点阵 (0-9和冒号)
static const uint8_t font_8x8[13][8] = {
    {0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C}, // 0
    {0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C}, // 1
    {0x3C, 0x66, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x7E}, // 2
    {0x3C, 0x66, 0x06, 0x1C, 0x06, 0x06, 0x66, 0x3C}, // 3
    {0x0C, 0x1C, 0x2C, 0x4C, 0x7E, 0x0C, 0x0C, 0x0C}, // 4
    {0x7E, 0x60, 0x7C, 0x06, 0x06, 0x06, 0x66, 0x3C}, // 5
    {0x3C, 0x66, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x3C}, // 6
    {0x7E, 0x06, 0x0C, 0x0C, 0x18, 0x18, 0x18, 0x18}, // 7
    {0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x66, 0x3C}, // 8
    {0x3C, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x66, 0x3C}, // 9
    {0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00}, // :
    {0x00, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x00}, // -
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 空格
};

// 初始化显示上下文
void osd_draw_context_init(osd_draw_context_t *ctx, int width, int height, int stride, int font_size_scale) {
    ctx->width = width;
    ctx->height = height;
    ctx->stride = stride;
    ctx->font_size = (font_size_scale < 1) ? 1 : font_size_scale;  // 最小字体大小为1
    ctx->text_color = ARGB1555_YELLOW;
    ctx->bg_color = ARGB1555_BLACK;

    // 初始化缓存
    memset(ctx->last_draw_str, 0, sizeof(ctx->last_draw_str));
    ctx->display_initialized = 0;
}

// 设置颜色
void osd_set_colors(osd_draw_context_t *ctx, argb1555_t text_color, argb1555_t bg_color) {
    ctx->text_color = text_color;
    ctx->bg_color = bg_color;
}

// 清空屏幕
void osd_clear_screen(osd_draw_context_t *ctx) {
    int i = 0, j = 0;
    argb1555_t *p = NULL;

    if (!ctx->buffer) {
        MISC_TRACE(MODULE_DBG_ERR,"Error: Buffer is NULL\n");
        return;
    }

    for (i = 0; i < ctx->height; i++) {
        for (j = 0; j < ctx->width; j++) {
            p = (argb1555_t*)(ctx->buffer + i * ctx->stride + j * sizeof(argb1555_t));
            *p = ctx->bg_color;
        }
    }
}

// 绘制像素
void draw_pixel(osd_draw_context_t *ctx, int x, int y, argb1555_t color) {
    argb1555_t *p = NULL;

    if (!ctx->buffer)
        return;

    if (x >= 0 && x < ctx->width && y >= 0 && y < ctx->height) {
        p = (argb1555_t*)(ctx->buffer + y * ctx->stride + x * sizeof(argb1555_t));
        *p = color;
    }
}

// 绘制缩放后的像素块 (用于字体放大)
void draw_scaled_pixel(osd_draw_context_t *ctx, int x, int y, int scale, argb1555_t color) {
    int dx = 0, dy = 0;
    for (dy = 0; dy < scale; dy++) {
        for (dx = 0; dx < scale; dx++) {
            draw_pixel(ctx, x + dx, y + dy, color);
        }
    }
}

// 绘制字符
void draw_char(osd_draw_context_t *ctx, int x, int y, char c, argb1555_t color) {
    int char_index, scale, row, col;
    uint8_t row_data;

    if (c >= '0' && c <= '9') {
        char_index = c - '0';
    } else if (c == ':') {
        char_index = 10;
    } else if (c == '-') {
        char_index = 11;  // 减号
    } else {
        char_index = 12;  // 空格或其他字符
    }

    scale = ctx->font_size;

    for (row = 0; row < FONT_ORIGINAL_WIDTH; row++) {
        row_data = font_8x8[char_index][row];
        for (col = 0; col < FONT_ORIGINAL_HEIGHT; col++) {
            if (row_data & (1 << (7 - col))) {
                // 绘制放大的像素
                draw_scaled_pixel(ctx,
                    x + col * scale,
                    y + row * scale,
                    scale, color);
            }
        }
    }
}

// 绘制字符串
void osd_draw_str(osd_draw_context_t *ctx, int x, int y, const char *draw_str, int draw_str_size, argb1555_t color) {
    int i;
    int pos_x = x;
    int char_width = FONT_ORIGINAL_WIDTH * ctx->font_size + ctx->font_size;  // 字符宽度 + 间距

    for (i = 0; draw_str[i] != '\0'; i++) {
        draw_char(ctx, pos_x, y, draw_str[i], color);
        pos_x += char_width;
    }
}

// 绘制单个字符（增量更新用）
void draw_single_char(osd_draw_context_t *ctx, int x, int y, char c, argb1555_t color) {
    int char_width, char_height;
    int px, py;

    // 先清除该字符区域
    char_width = FONT_ORIGINAL_WIDTH * ctx->font_size;
    char_height = FONT_ORIGINAL_HEIGHT * ctx->font_size;

    for (py = y; py < y + char_height; py++) {
        for (px = x; px < x + char_width; px++) {
            draw_pixel(ctx, px, py, ctx->bg_color);
        }
    }

    // 绘制新字符
    draw_char(ctx, x, y, c, color);
}

// 获取字符宽度
int get_char_width(osd_draw_context_t *ctx) {
    return FONT_ORIGINAL_WIDTH * ctx->font_size + ctx->font_size;
}

// 获取字符高度
int get_char_height(osd_draw_context_t *ctx) {
    return FONT_ORIGINAL_HEIGHT * ctx->font_size;
}

// 获取时钟显示所需的尺寸
void get_osd_time_size(osd_draw_context_t *ctx, int *width, int *height) {
    // 格式: "HH:MM:SS YYYY-MM-DD" 总共19个字符
    *width = (TOTAL_OSD_STRING_LEN - 1) * get_char_width(ctx);
    *height = get_char_height(ctx);
}

// 增量更新日期（只更新变化的字符）
void update_str_incremental(osd_draw_context_t *ctx, int date_x, int date_y, const char *draw_str, int draw_str_size) {
    int char_width, i, char_x;

    if (!ctx->display_initialized) {
        // 第一次显示，绘制全部字符
        osd_draw_str(ctx, date_x, date_y, draw_str, draw_str_size, ctx->text_color);
        strcpy(ctx->last_draw_str, draw_str);
        return;
    }

    char_width = get_char_width(ctx);

    // 比较每个字符，只更新变化的字符
    for (i = 0; i < sizeof(ctx->last_draw_str); i++) {  // 日期字符串长度是10 "YYYY-MM-DD"
        if (draw_str[i] != ctx->last_draw_str[i]) {
            char_x = date_x + i * char_width;
            draw_single_char(ctx, char_x, date_y, draw_str[i], ctx->text_color);
        }
    }

    strcpy(ctx->last_draw_str, draw_str);
}

// 绘制时钟
void osd_draw_str_ex(osd_draw_context_t *ctx, const char *draw_str, int draw_str_size, uint8_t *buffer) {
    int char_width, char_height;
    int total_width, total_height;
    int start_x, start_y;
    int i;

    if (!buffer) {
        MISC_TRACE(MODULE_DBG_ERR,"Error: Buffer is NULL\n");
        return;
    }

    ctx->buffer = buffer;

    // 计算显示位置（时间和日期在同一行）
    char_width = get_char_width(ctx);
    char_height = get_char_height(ctx);

    // 总文本宽度：时间(8字符) + 空格(1字符) + 日期(10字符) = 19字符
    total_width = (TOTAL_OSD_STRING_LEN - 1) * char_width;
    total_height = char_height;

    start_x = (ctx->width - total_width) / 2;
    start_y = (ctx->height - total_height) / 2;

    // 确保位置有效
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;

    // 第一次显示时清屏并绘制边框
    if (!ctx->display_initialized) {
        osd_clear_screen(ctx);

        // 绘制装饰边框
        for (i = 0; i < ctx->width; i++) {
            draw_pixel(ctx, i, 0, ARGB1555_GRAY);
            draw_pixel(ctx, i, ctx->height - 1, ARGB1555_GRAY);
        }
        for (i = 0; i < ctx->height; i++) {
            draw_pixel(ctx, 0, i, ARGB1555_GRAY);
            draw_pixel(ctx, ctx->width - 1, i, ARGB1555_GRAY);
        }

        // 绘制分隔符（时间和日期之间的空格）
        //int separator_x = start_x + DATE_STRING_LEN * char_width;
        //draw_char(ctx, separator_x, start_y, ' ', ctx->bg_color);

        ctx->display_initialized = 1;
    }

    // 更新时间（增量更新）
    update_str_incremental(ctx, start_x, start_y, draw_str, draw_str_size);
}

// 应用时区偏移
xmedia_void apply_timezone_offset(struct timespec64 *ts, xmedia_s32 tz_minuteswest)
{
    // 将时区偏移转换为秒（西区为正值，需要减去）
    xmedia_slong tz_seconds = tz_minuteswest * 60;

    // 调整时间
    ts->tv_sec -= tz_seconds;

    // 处理纳秒溢出
    if (ts->tv_nsec < 0) {
        ts->tv_sec--;
        ts->tv_nsec += NSEC_PER_SEC;
    }
    if (ts->tv_nsec >= NSEC_PER_SEC) {
        ts->tv_sec++;
        ts->tv_nsec -= NSEC_PER_SEC;
    }
}

xmedia_s32 get_local_time_string(xmedia_char *time_buf, size_t time_buf_size)
{
    struct timespec64 ts_utc, ts_local;
    struct tm tm_local;

    // 获取UTC时间
    ktime_get_real_ts64(&ts_utc);

    // 计算本地时间
    ts_local = ts_utc;
    apply_timezone_offset(&ts_local, g_tz_minuteswest);

    time64_to_tm(ts_local.tv_sec, 0, &tm_local);

    snprintf(time_buf, time_buf_size,
             "%04ld-%02d-%02d %02d:%02d:%02d",
             tm_local.tm_year + 1900, tm_local.tm_mon + 1, tm_local.tm_mday,
             tm_local.tm_hour, tm_local.tm_min, tm_local.tm_sec);

    return XMEDIA_SUCCESS;
}

xmedia_s32 misc_ctl_osd_time_create(xmedia_ulong arg)
{
    xmedia_s32 s32Ret = XMEDIA_SUCCESS, s32Ret2 = XMEDIA_SUCCESS;
    xmedia_s32 i = 0;
    xmedia_rgn_canvas_info canvas_info;
    char time_buf[TOTAL_OSD_STRING_LEN];
    osd_time_ext_t *osd_time_ext_hdl = XMEDIA_NULL, *p = XMEDIA_NULL;

    osd_time_ext_hdl = osal_vmalloc(sizeof(osd_time_ext_t));
    if (osd_time_ext_hdl == XMEDIA_NULL) {
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(osd_time_ext_hdl, 0, sizeof(osd_time_ext_t));
    memcpy(&osd_time_ext_hdl->osd_time_attr, (xmedia_void *)arg, sizeof(misc_osd_time_attr_t));

    if(g_osd_time_ext_hdl == XMEDIA_NULL) {
        s32Ret = export_rgn_init();
        if (s32Ret != XMEDIA_SUCCESS) {
            MISC_TRACE(MODULE_DBG_ERR, "export_region_init failed! ret:0x%x\n", s32Ret);
            goto EXIT1;
        }
    }

    s32Ret = export_rgn_create(osd_time_ext_hdl->osd_time_attr.rgn_hdl, &osd_time_ext_hdl->osd_time_attr.rgn_attr);
    if (s32Ret != XMEDIA_SUCCESS) {
        MISC_TRACE(MODULE_DBG_ERR, "export_region_create failed! ret:0x%x\n", s32Ret);
        goto EXIT1;
    }

    for(i = 0; i < osd_time_ext_hdl->osd_time_attr.rgn_attach_num; i++) {
        s32Ret = export_rgn_attach_to_chn(osd_time_ext_hdl->osd_time_attr.rgn_hdl, &osd_time_ext_hdl->osd_time_attr.attach_obj[i].mpp_chn, 
                &osd_time_ext_hdl->osd_time_attr.attach_obj[i].chn_attr);
        if (s32Ret != XMEDIA_SUCCESS) {
            MISC_TRACE(MODULE_DBG_ERR, "export_rgn_attach_to_chn failed with %#x!\n", s32Ret);
            goto EXIT2;
        }
    }

    s32Ret = export_rgn_get_canvas_info(osd_time_ext_hdl->osd_time_attr.rgn_hdl, &canvas_info);
    if (s32Ret != XMEDIA_SUCCESS) {
        MISC_TRACE(MODULE_DBG_ERR, "export_rgn_get_canvas_info failed with %#x!\n", s32Ret);
        goto EXIT3;
    }

    osd_draw_context_init(&osd_time_ext_hdl->osd_draw_ctx, canvas_info.size.width, canvas_info.size.height, canvas_info.stride, osd_time_ext_hdl->osd_time_attr.font_size_scale);
    osd_set_colors(&osd_time_ext_hdl->osd_draw_ctx, (argb1555_t)osd_time_ext_hdl->osd_time_attr.text_color, (argb1555_t)osd_time_ext_hdl->osd_time_attr.bg_color);
    get_local_time_string(time_buf, sizeof(time_buf));

    osd_draw_str_ex(&osd_time_ext_hdl->osd_draw_ctx, time_buf, sizeof(time_buf), canvas_info.virt_addr);

    s32Ret = export_rgn_update_canvas(osd_time_ext_hdl->osd_time_attr.rgn_hdl);
    if (s32Ret != XMEDIA_SUCCESS) {
        MISC_TRACE(MODULE_DBG_ERR, "export_rgn_update_canvas failed with %#x!\n", s32Ret);
        goto EXIT3;
    }

    if(g_osd_time_ext_hdl == XMEDIA_NULL)
        g_osd_time_ext_hdl = osd_time_ext_hdl;
    else {
        p = g_osd_time_ext_hdl;
        while(p->next != XMEDIA_NULL) {
            p = p->next;
        }
        p->next = osd_time_ext_hdl;
    }

    return XMEDIA_SUCCESS;

EXIT3:
    for(i = 0; i < osd_time_ext_hdl->osd_time_attr.rgn_attach_num; i++) {
        s32Ret2 = export_rgn_detach_from_chn(osd_time_ext_hdl->osd_time_attr.rgn_hdl, &osd_time_ext_hdl->osd_time_attr.attach_obj[i].mpp_chn);
        if(s32Ret2 != XMEDIA_SUCCESS) {
            MISC_TRACE(MODULE_DBG_ERR, "export_rgn_detach_from_chn failed!\n");
        }
    }

EXIT2:
    s32Ret2 = export_rgn_destroy(osd_time_ext_hdl->osd_time_attr.rgn_hdl);
    if (s32Ret2 != XMEDIA_SUCCESS) {
        MISC_TRACE(MODULE_DBG_ERR, "export_rgn_destroy failed with %#x!\n", s32Ret2);
    }

EXIT1:
    osal_vfree(osd_time_ext_hdl);
    osd_time_ext_hdl = XMEDIA_NULL;
    return s32Ret;
}

xmedia_s32 misc_ctl_osd_time_update_timezone(xmedia_ulong arg)
{
    xmedia_s32 tz_minuteswest = *(xmedia_s32*)arg;

    /* Verify we're within the +-15 hrs range */
    if (tz_minuteswest > 15*60 || tz_minuteswest < -15*60) {
        MISC_TRACE(MODULE_DBG_ERR, "invaild tz_minuteswest:%d,i it should be within the +-15 hrs range!\n", tz_minuteswest);
        return XMEDIA_FAILURE;
    }

    g_tz_minuteswest = tz_minuteswest;
    MISC_TRACE(MODULE_DBG_DEBUG, "set tz_minuteswest:%d!\n", g_tz_minuteswest);
    return XMEDIA_SUCCESS;
}

xmedia_s32 misc_ctl_osd_time_destroy(xmedia_ulong arg)
{
    xmedia_s32 i = 0;
    xmedia_s32 s32Ret = XMEDIA_SUCCESS;
    misc_osd_time_attr_t *osd_time_attr = XMEDIA_NULL;
    osd_time_ext_t *p = XMEDIA_NULL, *tmp = XMEDIA_NULL;
    xmedia_s32 have_found = 0;

    osd_time_attr = (misc_osd_time_attr_t*)arg;
    if(osd_time_attr == XMEDIA_NULL)
    {
        MISC_TRACE(MODULE_DBG_ERR, "input parameter is NULL!\n");
        return XMEDIA_FAILURE;
    }

    if(g_osd_time_ext_hdl == XMEDIA_NULL) {
        MISC_TRACE(MODULE_DBG_ERR, "osd time destory failed! because not create this object!!!\n");
        return XMEDIA_ERRCODE_NOT_EXIST;
    }
    else if(g_osd_time_ext_hdl->osd_time_attr.rgn_hdl == osd_time_attr->rgn_hdl) {
        p = g_osd_time_ext_hdl;
        g_osd_time_ext_hdl = g_osd_time_ext_hdl->next;
    }
    else if(g_osd_time_ext_hdl->next == XMEDIA_NULL) {
        MISC_TRACE(MODULE_DBG_ERR, "osd time destory failed! because not create this object!!!\n");
        return XMEDIA_ERRCODE_NOT_EXIST;
    }
    else {
        p = g_osd_time_ext_hdl;
        while(p->next != XMEDIA_NULL) {
            if(p->next->osd_time_attr.rgn_hdl == osd_time_attr->rgn_hdl) {
                have_found = 1;
                tmp = p->next;
                p->next = p->next->next;
                p = tmp;
                break;
            }

            p = p->next;
        }

        if(!have_found) {
            MISC_TRACE(MODULE_DBG_ERR, "osd time destory failed! because not create this object!!!\n");
            return XMEDIA_ERRCODE_NOT_EXIST;
        }
    }


    for(i = 0; i < osd_time_attr->rgn_attach_num; i++) {
        s32Ret = export_rgn_detach_from_chn(osd_time_attr->rgn_hdl, &osd_time_attr->attach_obj[i].mpp_chn);
        if(s32Ret !=XMEDIA_SUCCESS) {
            MISC_TRACE(MODULE_DBG_ERR, "export_rgn_detach_from_chn failed!\n");
        }
    }

    s32Ret = export_rgn_destroy(osd_time_attr->rgn_hdl);
    if (s32Ret != XMEDIA_SUCCESS) {
        MISC_TRACE(MODULE_DBG_ERR, "export_rgn_destroy failed with %#x!\n", s32Ret);
    }

    if(p)
        osal_vfree(p);

    if(g_osd_time_ext_hdl == XMEDIA_NULL) {
        export_rgn_exit();
    }
    return s32Ret;
}

xmedia_s32 osd_time_update(misc_osd_time_attr_t *osd_time_attr, osd_draw_context_t *osd_draw_ctx)
{
    xmedia_s32 s32Ret = XMEDIA_SUCCESS;
    xmedia_rgn_canvas_info canvas_info;
    char time_buf[TOTAL_OSD_STRING_LEN];

    if(osd_time_attr == XMEDIA_NULL || osd_draw_ctx == XMEDIA_NULL) {
        MISC_TRACE(MODULE_DBG_ERR, "osd time object not create!\n");
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    s32Ret = export_rgn_get_canvas_info(osd_time_attr->rgn_hdl, &canvas_info);
    if (s32Ret != XMEDIA_SUCCESS) {
        MISC_TRACE(MODULE_DBG_ERR, "export_rgn_get_canvas_info failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    get_local_time_string(time_buf, sizeof(time_buf));

    //MISC_TRACE(MODULE_DBG_ERR, "osd time:%s!\n", time_buf);

    osd_draw_str_ex(osd_draw_ctx, time_buf, sizeof(time_buf), canvas_info.virt_addr);

    s32Ret = export_rgn_update_canvas(osd_time_attr->rgn_hdl);
    if (s32Ret != XMEDIA_SUCCESS) {
        MISC_TRACE(MODULE_DBG_ERR, "export_rgn_update_canvas failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

xmedia_s32 osd_time_update_all()
{
    xmedia_s32 s32Ret = XMEDIA_SUCCESS;
    osd_time_ext_t *p = g_osd_time_ext_hdl;

    while(p != XMEDIA_NULL) {
        s32Ret |= osd_time_update(&p->osd_time_attr, &p->osd_draw_ctx);
        p = p->next;
    }
    return s32Ret;
}

xmedia_s32 misc_ctl_osd_time_update(xmedia_ulong arg)
{
    osd_time_ext_t *p = XMEDIA_NULL;
    xmedia_s32 have_found = 0;
    misc_osd_time_attr_t *osd_time_attr = (misc_osd_time_attr_t *)arg;

    if(osd_time_attr == XMEDIA_NULL) {
        MISC_TRACE(MODULE_DBG_ERR, "input parameter is NULL!\n");
        return XMEDIA_ERRCODE_NULL_PTR;
    }


    if(g_osd_time_ext_hdl == XMEDIA_NULL) {
        MISC_TRACE(MODULE_DBG_ERR, "osd time update failed! because not create this object!!!\n");
        return XMEDIA_ERRCODE_NOT_EXIST;
    }
    else if(g_osd_time_ext_hdl->osd_time_attr.rgn_hdl == osd_time_attr->rgn_hdl) {
        p = g_osd_time_ext_hdl;
    }
    else if(g_osd_time_ext_hdl->next == XMEDIA_NULL) {
        MISC_TRACE(MODULE_DBG_ERR, "osd time update failed! because not create this object!!!\n");
        return XMEDIA_ERRCODE_NOT_EXIST;
    }
    else {
        p = g_osd_time_ext_hdl;
        while(p->next != XMEDIA_NULL) {
            if(p->next->osd_time_attr.rgn_hdl == osd_time_attr->rgn_hdl) {
                have_found = 1;
                break;
            }
            p = p->next;
        }

        if(!have_found) {
            MISC_TRACE(MODULE_DBG_ERR, "osd time update failed! because not create this object!!!\n");
            return XMEDIA_ERRCODE_NOT_EXIST;
        }
    }
    return osd_time_update(osd_time_attr, &p->osd_draw_ctx);
}

#endif
