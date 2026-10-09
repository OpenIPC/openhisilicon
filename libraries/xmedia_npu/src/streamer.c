// SPDX-License-Identifier: GPL-2.0
/*
 * In-memory streamer (streamer.h): libxmedia_cl serialises a compiled
 * graph into one and reads it back. Writes always append; reads start at
 * the position the last seek left.
 */

#include <stdlib.h>
#include <string.h>

#include "streamer.h"

struct mem_stream {
	char *buf;
	size_t size;		/* bytes written */
	size_t cap;
	size_t pos;		/* read position */
};

static size_t mem_write(void *hd, const void *ptr, size_t size)
{
	struct mem_stream *ms = hd;
	size_t cap = ms->cap ? ms->cap : 4096;
	char *buf;

	while (cap - ms->size < size)
		cap *= 2;
	if (cap != ms->cap) {
		buf = realloc(ms->buf, cap);
		if (!buf)
			return 0;
		ms->buf = buf;
		ms->cap = cap;
	}
	memcpy(ms->buf + ms->size, ptr, size);
	ms->size += size;
	return size;
}

static int mem_seek(void *hd, int off)
{
	struct mem_stream *ms = hd;

	ms->pos = off < 0 ? ms->size : (size_t)off;
	return 0;
}

static size_t mem_read(void *hd, void *ptr, size_t size)
{
	struct mem_stream *ms = hd;

	if (ms->pos >= ms->size)
		return 0;
	if (size > ms->size - ms->pos)
		size = ms->size - ms->pos;
	memcpy(ptr, ms->buf + ms->pos, size);
	ms->pos += size;
	return size;
}

static size_t mem_get_total_size(void *hd)
{
	return ((struct mem_stream *)hd)->size;
}

xmnpu_streamer_t *xmnpu_open_streamer(void)
{
	xmnpu_streamer_t *strm = calloc(1, sizeof(*strm));

	if (!strm)
		return NULL;
	strm->hd = calloc(1, sizeof(struct mem_stream));
	if (!strm->hd) {
		free(strm);
		return NULL;
	}
	strm->write = mem_write;
	strm->seek = mem_seek;
	strm->read = mem_read;
	strm->get_total_size = mem_get_total_size;
	return strm;
}

void xmnpu_close_streamer(xmnpu_streamer_t *strm)
{
	struct mem_stream *ms;

	if (!strm)
		return;
	ms = strm->hd;
	if (ms)
		free(ms->buf);
	free(ms);
	free(strm);
}
