#ifndef DRV_OSD_DRAW_TIME_H
#define DRV_OSD_DRAW_TIME_H

#ifdef __KERNEL__
#include <linux/types.h>
#endif

#include "xmedia_type.h"
#include "drv_misc.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SUPPORT_OSD_TIME
// ARGB1555颜色定义 (1位alpha + 5位RGB)
typedef unsigned short argb1555_t;

// 颜色宏定义
#define ARGB1555(a, r, g, b) (((a) ? 0x8000 : 0) | (((r) & 0xF8) << 7) | (((g) & 0xF8) << 2) | (((b) & 0xF8) >> 3))

#define ARGB1555_BLACK     ARGB1555(1, 0, 0, 0)
#define ARGB1555_WHITE     ARGB1555(1, 255, 255, 255)
#define ARGB1555_RED       ARGB1555(1, 255, 0, 0)
#define ARGB1555_GREEN     ARGB1555(1, 0, 255, 0)
#define ARGB1555_BLUE      ARGB1555(1, 0, 0, 255)
#define ARGB1555_YELLOW    ARGB1555(1, 255, 255, 0)
#define ARGB1555_CYAN      ARGB1555(1, 0, 255, 255)
#define ARGB1555_MAGENTA   ARGB1555(1, 255, 0, 255)
#define ARGB1555_GRAY      ARGB1555(1, 128, 128, 128)
#define ARGB1555_DARKBLUE  ARGB1555(1, 0, 0, 128)
#define ARGB1555_ORANGE    ARGB1555(1, 255, 165, 0)

// 时间字符串长度是8 "HH:MM:SS"
#define TIME_STRING_LEN             8

// 日期字符串长度是10 "YYYY-MM-DD"
#define DATE_STRING_LEN             10

// 总文本宽度：时间(8字符) + 空格(1字符) + 日期(10字符) + 结束符=20字符
#define TOTAL_OSD_STRING_LEN            20

// 显示上下文结构体
typedef struct {
    unsigned char *buffer;    // 显示buffer地址
    int width;             // 屏幕宽度
    int height;            // 屏幕高度
    int stride;
    int font_size;         // 字体大小
    argb1555_t text_color; // 文字颜色
    argb1555_t bg_color;   // 背景颜色

    // 缓存上一次显示的内容，用于增量更新
    char last_draw_str[TOTAL_OSD_STRING_LEN];
    int display_initialized; // 是否已经初始化显示
} osd_draw_context_t;

// 函数声明
void osd_draw_context_init(osd_draw_context_t *ctx, int width, int height, int stride, int font_size_scale);
void osd_set_colors(osd_draw_context_t *ctx, argb1555_t text_color, argb1555_t bg_color);
void osd_clear_screen(osd_draw_context_t *ctx);
//void draw_pixel(osd_draw_context_t *ctx, int x, int y, argb1555_t color);
//void draw_char(osd_draw_context_t *ctx, int x, int y, char c, argb1555_t color);
void osd_draw_str(osd_draw_context_t *ctx, int x, int y, const char *draw_str, int draw_str_size, argb1555_t color);
void osd_draw_str_ex(osd_draw_context_t *ctx, const char *draw_str, int draw_str_size, unsigned char *buffer);
void osd_get_min_size(osd_draw_context_t *ctx, int *width, int *height);

xmedia_s32 misc_ctl_osd_time_create(xmedia_ulong arg);
xmedia_s32 misc_ctl_osd_time_destroy(xmedia_ulong arg);
xmedia_s32 misc_ctl_osd_time_update(xmedia_ulong arg);
xmedia_s32 misc_ctl_osd_time_update_timezone(xmedia_ulong arg);
xmedia_s32 osd_time_update_all(xmedia_void);

#endif  //SUPPORT_OSD_TIME

#ifdef __cplusplus
}
#endif

#endif
