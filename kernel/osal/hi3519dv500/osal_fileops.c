/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/fs.h>
#include <linux/version.h>
#include "ot_osal.h"
#include "securec.h"
#include "osal_errno.h"
#include "osal_inner.h"

static struct file *klib_fopen(const char *file, int flags, int mode)
{
    struct file *filp = filp_open(file, flags, mode);
    return (IS_ERR(filp)) ? NULL : filp;
}

static void klib_fclose(struct file *filp)
{
    if (filp != NULL) {
        filp_close(filp, NULL);
    }
    return;
}

static int klib_fwrite(const char *buf, unsigned long size, struct file *filp)
{
    int writelen;

    writelen = kernel_write(filp, buf, size, &filp->f_pos);
    return writelen;
}

static int klib_fread(char *buf, unsigned long size, struct file *filp)
{
    int readlen;
    readlen = kernel_read(filp, buf, size, &filp->f_pos);

    return readlen;
}

void *osal_klib_fopen(const char *file, int flags, int mode)
{
    if (file == NULL) {
        osal_log("file NULL!\n");
        return NULL;
    }

    return (void *)klib_fopen(file, flags, mode);
}
EXPORT_SYMBOL(osal_klib_fopen);

void osal_klib_fclose(void *filp)
{
    if (filp == NULL) {
        osal_log("filp NULL!\n");
        return;
    }

    klib_fclose((struct file *)filp);
}
EXPORT_SYMBOL(osal_klib_fclose);

int osal_klib_fwrite(const char *buf, unsigned long size, void *filp)
{
    if ((buf == NULL) || (filp == NULL)) {
        osal_log("buf&filp NULL!\n");
        return OSAL_FAILURE;
    }

    return klib_fwrite(buf, size, (struct file *)filp);
}
EXPORT_SYMBOL(osal_klib_fwrite);

int osal_klib_fread(char *buf, unsigned long size, void *filp)
{
    if ((buf == NULL) || (filp == NULL)) {
        osal_log("buf&filp NULL!\n");
        return OSAL_FAILURE;
    }

    return klib_fread(buf, size, (struct file *)filp);
}
EXPORT_SYMBOL(osal_klib_fread);

int osal_klib_fseek(long long offset, int whence, void *filp)
{
    int ret;
    loff_t res;

    if (filp == NULL) {
        osal_log("filp NULL!\n");
        return OSAL_FAILURE;
    }

    res = vfs_llseek(filp, offset, whence);
    ret = (int)res;
    if (res != (loff_t)ret) {
        ret = OSAL_EOVERFLOW;
    }

    return ret;
}
EXPORT_SYMBOL(osal_klib_fseek);

void osal_klib_fsync(void *filp)
{
    if (filp == NULL) {
        osal_log("filp NULL!\n");
        return;
    }

    vfs_fsync(filp, 0);
}
EXPORT_SYMBOL(osal_klib_fsync);
