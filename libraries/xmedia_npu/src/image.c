// SPDX-License-Identifier: GPL-2.0
/*
 * CRC-32 and file helpers of libxmedia_npu, which libxmedia_cl uses to load
 * and dump model images, and the code+data image header it checks them
 * against.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Image header: 128 bytes, words little endian */
struct npu_image_hdr {
	uint32_t hcrc;		/* CRC-32 of bytes 4..127 */
	char name[64];
	uint32_t reserved[2];
	uint32_t platform;
	uint32_t magic;		/* "ZZZZ" */
	uint32_t cd_size;	/* code + data */
	uint32_t data_crc;
	uint32_t code_crc;
	uint32_t shdr_crc;
	uint32_t code_size;
	uint32_t code_off;
	uint32_t data_size;
	uint32_t data_off;
	uint32_t shdr_num;
	uint32_t shdr_entsize;
	uint32_t shdr_off;
};

_Static_assert(sizeof(struct npu_image_hdr) == 128, "image header is 128 bytes");

#define IMAGE_MAGIC		0x5a5a5a5a
#define IMAGE_SHDR_ENTSIZE	80

/* Plain CRC-32 (reflected 0xedb88320), without the pre/post inversion */
uint32_t xmedia_crc32(uint32_t crc, const uint8_t *buf, int len)
{
	static uint32_t table[256];
	int i, j;

	if (!table[1]) {
		for (i = 0; i < 256; i++) {
			uint32_t c = i;

			for (j = 0; j < 8; j++)
				c = c & 1 ? 0xedb88320 ^ (c >> 1) : c >> 1;
			table[i] = c;
		}
	}
	while (len-- > 0)
		crc = table[(crc ^ *buf++) & 0xff] ^ (crc >> 8);
	return crc;
}

uint32_t xmedia_crc32_buf(const uint8_t *buf, int len, uint32_t *crc)
{
	*crc = ~xmedia_crc32(0xffffffff, buf, len);
	return *crc;
}

uint32_t xmedia_crc32_buf_with_return(const uint8_t *buf, int len)
{
	return ~xmedia_crc32(0xffffffff, buf, len);
}

int get_file_size(const char *file)
{
	FILE *fp = fopen(file, "r");
	long size;

	if (!fp) {
		printf("cannot open %s\n", file);
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fclose(fp);
	return size;
}

/* CRC-32 of @size bytes of @file from @off, or of all of it */
static int file_crc32(const char *file, long off, size_t size, int whole)
{
	uint8_t buf[256];
	uint32_t crc = 0xffffffff;
	size_t n;
	FILE *fp;

	fp = fopen(file, "r");
	if (!fp) {
		printf("cannot open %s\n", file);
		return -1;
	}
	fseek(fp, off, SEEK_SET);
	while (whole || size) {
		n = fread(buf, 1, whole || size > sizeof(buf) ?
			  sizeof(buf) : size, fp);
		if (!n)
			break;
		crc = xmedia_crc32(crc, buf, n);
		if (!whole)
			size -= n;
	}
	fclose(fp);
	return ~crc;
}

int get_file_crc32(const char *file)
{
	return file_crc32(file, 0, 0, 1);
}

int get_patial_file_crc32(const char *file, int off, size_t size)
{
	return file_crc32(file, off, size, 0);
}

int copy_buff_to_file(const void *buf, const char *file, int off,
		      size_t size)
{
	FILE *fp = fopen(file, "r+");

	if (!fp)
		fp = fopen(file, "w+");
	if (!fp)
		return -1;
	fseek(fp, off, SEEK_SET);
	fwrite(buf, 1, size, fp);
	return fclose(fp);
}

int copy_file_to_buff(const char *file, void *buf, int off, size_t size)
{
	FILE *fp = fopen(file, "r");

	if (!fp)
		return -1;
	fseek(fp, off, SEEK_SET);
	fread(buf, 1, size, fp);
	return fclose(fp);
}

/* Appends @size bytes of @src, from @off, to @dst */
int copy_file_to_file_size(const char *src, const char *dst, int off,
			   size_t size)
{
	uint8_t buf[512];
	FILE *in, *out;
	size_t n;
	int ret;

	in = fopen(src, "r");
	if (!in)
		return -1;
	out = fopen(dst, "a");
	if (!out) {
		fclose(in);
		return -1;
	}
	fseek(in, off, SEEK_SET);
	while (size) {
		n = fread(buf, 1, size > sizeof(buf) ? sizeof(buf) : size, in);
		if (!n)
			break;
		fwrite(buf, 1, n, out);
		size -= n;
	}
	fclose(in);
	ret = fclose(out);
	return ret;
}

/* Writes all of @src into @dst at @off; @keep keeps the rest of @dst */
int copy_file_to_file(const char *src, const char *dst, int off, int keep)
{
	uint8_t buf[512];
	FILE *in, *out;
	size_t n;

	in = fopen(src, "r");
	if (!in)
		return -1;
	out = fopen(dst, keep ? "r+" : "w+");
	if (!out) {
		fclose(in);
		return -1;
	}
	fseek(out, off, SEEK_SET);
	while ((n = fread(buf, 1, sizeof(buf), in)))
		fwrite(buf, 1, n, out);
	fclose(in);
	return fclose(out);
}

int get_image_hdr(const char *file, void *hdr)
{
	FILE *fp = fopen(file, "r");

	if (!fp)
		return -1;
	fread(hdr, 1, sizeof(struct npu_image_hdr), fp);
	return fclose(fp);
}

size_t get_image_data(const char *file, int off, size_t size, void *buf)
{
	FILE *fp = fopen(file, "r");
	size_t n;

	if (!fp)
		return 0;
	fseek(fp, off, SEEK_SET);
	n = fread(buf, 1, size, fp);
	fclose(fp);
	return n;
}

/* A header for an image of @code followed by @data, without its own CRC */
struct npu_image_hdr *init_image_header(const char *code, const char *data)
{
	struct npu_image_hdr *hdr = calloc(1, sizeof(*hdr));

	if (!hdr)
		return NULL;
	strncpy(hdr->name, "code_data_model.bin", sizeof(hdr->name));
	hdr->magic = IMAGE_MAGIC;
	hdr->code_size = get_file_size(code);
	hdr->code_crc = get_file_crc32(code);
	hdr->code_off = sizeof(*hdr);
	hdr->data_size = get_file_size(data);
	hdr->data_crc = get_file_crc32(data);
	hdr->data_off = hdr->code_off + hdr->code_size;
	hdr->shdr_entsize = IMAGE_SHDR_ENTSIZE;
	hdr->shdr_off = hdr->data_off + hdr->data_size;
	hdr->cd_size = hdr->code_size + hdr->data_size;
	return hdr;
}

void *init_section_header(int num)
{
	return calloc(IMAGE_SHDR_ENTSIZE * num, 1);
}

int image_check_hcrc(const struct npu_image_hdr *hdr, const char *file)
{
	return hdr->hcrc == (uint32_t)get_patial_file_crc32(file, 4,
						sizeof(*hdr) - 4);
}

int image_check_dcrc(const struct npu_image_hdr *hdr, const char *file)
{
	return hdr->data_crc == (uint32_t)get_patial_file_crc32(file,
						hdr->data_off, hdr->data_size);
}

int image_check_ccrc(const struct npu_image_hdr *hdr, const char *file)
{
	return hdr->code_crc == (uint32_t)get_patial_file_crc32(file,
						hdr->code_off, hdr->code_size);
}

int image_check_shcrc(const struct npu_image_hdr *hdr, const char *file)
{
	return hdr->shdr_crc == (uint32_t)get_patial_file_crc32(file,
				hdr->shdr_off, hdr->shdr_entsize * hdr->shdr_num);
}

int image_print_contents(const struct npu_image_hdr *hdr)
{
	puts("************************************************************");
	printf("Image Name:     %.64s\n", hdr->name);
	printf("Image Platform: %d\n", hdr->platform);
	printf("Image CDSize:   %d\n", hdr->cd_size);
	printf("Image HCRC:     %x\n", hdr->hcrc);
	printf("Code Size:      %d\n", hdr->code_size);
	printf("Code CRC:       %x\n", hdr->code_crc);
	printf("data Size:      %d\n", hdr->data_size);
	printf("data CRC32:     %x\n", hdr->data_crc);
	printf("shdr Num :      %d\n", hdr->shdr_num);
	printf("shdr Entsize:   %d\n", hdr->shdr_entsize);
	printf("shdr Size:      %d\n", hdr->shdr_entsize * hdr->shdr_num);
	printf("shdr CRC32:     %x\n", hdr->shdr_crc);
	return puts("************************************************************");
}

/* Validates an image file against its header, printing the header if good */
int get_image(const char *file)
{
	struct npu_image_hdr hdr;

	memset(&hdr, 0, sizeof(hdr));
	get_image_hdr(file, &hdr);
	if (!image_check_hcrc(&hdr, file)) {
		puts("Bad Header Checksum");
		return -1;
	}
	if (!image_check_ccrc(&hdr, file)) {
		puts("Bad Code Checksum");
		return -1;
	}
	if (!image_check_dcrc(&hdr, file)) {
		puts("Bad Data Checksum");
		return -1;
	}
	if (!image_check_shcrc(&hdr, file)) {
		puts("Bad SH Checksum");
		return -1;
	}
	image_print_contents(&hdr);
	return 0;
}

void close_image_header(void *hdr)
{
	free(hdr);
}

void close_section_header(void *shdr)
{
	free(shdr);
}
