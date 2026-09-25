#ifndef __STREAMER_H__
#define __STREAMER_H__

//TODO:keep it for future when we refactor C++ to C
#ifdef __cplusplus
extern "C" {
#endif

#define STREAMER_SEEK_START 0
#define STREAMER_SEEK_END -1
#include <stddef.h>
typedef struct xmnpu_streamer_s {
    void *hd;
    size_t (*write)(void *hd, const void *ptr, size_t size);
    int (*seek)(void *hd, int off);
    size_t (*read)(void *hd, void *ptr, size_t size);
    size_t (*get_total_size)(void *hd);
} xmnpu_streamer_t;

xmnpu_streamer_t *xmnpu_open_streamer(void);
void xmnpu_close_streamer(xmnpu_streamer_t *strm);

#ifdef __cplusplus
}
#endif

#endif /* __STREAMER_H__ */
