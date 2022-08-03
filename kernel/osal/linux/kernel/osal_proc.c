/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/version.h>

#include "osal.h"

#define OSAL_PROC_DEBUG 0

static struct osal_list_head list;
static osal_proc_entry_t *proc_entry = NULL;

static int osal_seq_show(struct seq_file *s, void *p)
{
	osal_proc_entry_t *oldsentry = s->private;
	osal_proc_entry_t sentry;
	if (oldsentry == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	memset(&sentry, 0, sizeof(osal_proc_entry_t));
	/* only these two parameters are used */
	sentry.seqfile = s;
	sentry.private = oldsentry->private;
	oldsentry->read(&sentry);
	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
static ssize_t osal_procwrite(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	osal_proc_entry_t *sentry =
		((struct seq_file *)(file->private_data))->private;
	return sentry->write(sentry, (char *)buf, count, (long long *)ppos);
}
#else
static ssize_t osal_procwrite(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	osal_proc_entry_t *item = PDE_DATA(file_inode(file));

	if ((item != NULL) && (item->write != NULL)) {
		return item->write(item, buf, count, (long long *)ppos);
	}

	return -ENOSYS;
}
#endif

static int osal_procopen(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
	osal_proc_entry_t *sentry = PDE(inode)->data;
#else
	osal_proc_entry_t *sentry = PDE_DATA(inode);
#endif
	if ((sentry != NULL) && (sentry->open != NULL)) {
		sentry->open(sentry);
	}
	return single_open(file, osal_seq_show, sentry);
}

static struct file_operations osal_proc_ops = { .owner = THIS_MODULE,
						.open = osal_procopen,
						.read = seq_read,
						.write = osal_procwrite,
						.llseek = seq_lseek,
						.release = single_release };

osal_proc_entry_t *osal_create_proc(const char *name, osal_proc_entry_t *parent)
{
	struct proc_dir_entry *entry = NULL;
	osal_proc_entry_t *sentry = NULL;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
	if (parent == NULL) {
		osal_printk("%s - parent is NULL!\n", __FUNCTION__);
		entry = create_proc_entry(name, 0, NULL);
	} else {
		osal_printk("%s - parent is not NULL! parent=%pK\n",
			    __FUNCTION__, parent->proc_dir_entry);
		entry = create_proc_entry(name, 0, parent->proc_dir_entry);
	}
	if (entry == NULL) {
		osal_printk("%s - create_proc_entry failed!\n", __FUNCTION__);
		return NULL;
	}
	sentry = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
	if (sentry == NULL) {
		if (parent != NULL) {
			remove_proc_entry(name, parent->proc_dir_entry);
		} else {
			remove_proc_entry(name, NULL);
		}
		osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
		return NULL;
	}

	osal_memset(sentry, 0, sizeof(struct osal_proc_dir_entry));

	osal_strncpy(sentry->name, name, sizeof(sentry->name) - 1);
	sentry->proc_dir_entry = entry;
	sentry->open = NULL;
	entry->proc_fops = &osal_proc_ops;
	entry->data = sentry;
#else
	sentry = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
	if (sentry == NULL) {
		osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
		return NULL;
	}

	osal_memset(sentry, 0, sizeof(struct osal_proc_dir_entry));

	osal_strncpy(sentry->name, name, sizeof(sentry->name) - 1);

	if (parent == NULL) {
		entry = proc_create_data(name, 0, NULL, &osal_proc_ops, sentry);
	} else {
		entry = proc_create_data(name, 0, parent->proc_dir_entry,
					 &osal_proc_ops, sentry);
	}
	if (entry == NULL) {
		osal_printk("%s - create_proc_entry failed!\n", __FUNCTION__);
		kfree(sentry);
		sentry = NULL;
		return NULL;
	}
	sentry->proc_dir_entry = entry;
	sentry->open = NULL;
#endif
	osal_list_add_tail(&(sentry->node), &list);
	return sentry;
}

void osal_remove_proc(const char *name, osal_proc_entry_t *parent)
{
	struct osal_proc_dir_entry *sproc = NULL;

	if (name == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return;
	}
	if (parent != NULL) {
		remove_proc_entry(name, parent->proc_dir_entry);
	} else {
		remove_proc_entry(name, NULL);
	}
	osal_list_for_each_entry(sproc, &list, node)
	{
		if (osal_strncmp(sproc->name, name, sizeof(sproc->name)) == 0) {
			osal_list_del(&(sproc->node));
			break;
		}
	}
	if (sproc != NULL) {
		kfree(sproc);
	}
}

osal_proc_entry_t *osal_create_proc_entry(const char *name,
					  osal_proc_entry_t *parent)
{
	parent = proc_entry;

	return osal_create_proc(name, parent);
}
EXPORT_SYMBOL(osal_create_proc_entry);

void osal_remove_proc_entry(const char *name, osal_proc_entry_t *parent)
{
	parent = proc_entry;
	osal_remove_proc(name, parent);
	return;
}
EXPORT_SYMBOL(osal_remove_proc_entry);

osal_proc_entry_t *osal_proc_mkdir(const char *name, osal_proc_entry_t *parent)
{
	struct proc_dir_entry *proc = NULL;
	struct osal_proc_dir_entry *sproc = NULL;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
	if (parent != NULL) {
		proc = proc_mkdir(name, parent->proc_dir_entry);
		osal_printk("%s - parent is not NULL!\n", __FUNCTION__);
		// proc = create_proc_entry(name, S_IFDIR | S_IRUGO | S_IXUGO, parent->proc_dir_entry);
	} else {
		proc = proc_mkdir(name, NULL);
		osal_printk("%s - parent is NULL! proc=%pK \n", __FUNCTION__,
			    proc);
		// proc = create_proc_entry(name, S_IFDIR | S_IRUGO | S_IXUGO, NULL);
	}
	if (proc == NULL) {
		osal_printk("%s - proc_mkdir failed!\n", __FUNCTION__);
		return NULL;
	}
	// osal_memset(proc, 0x00, sizeof(proc));
	sproc = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
	if (sproc == NULL) {
		if (parent != NULL) {
			remove_proc_entry(name, parent->proc_dir_entry);
		} else {
			remove_proc_entry(name, NULL);
		}
		osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
		return NULL;
	}

	osal_memset(sproc, 0, sizeof(struct osal_proc_dir_entry));

	osal_strncpy(sproc->name, name, sizeof(sproc->name) - 1);
	sproc->proc_dir_entry = proc;
	// sproc->read = NULL;
	// sproc->write = NULL;
	// proc->proc_fops = NULL;
	proc->data = sproc;
#else
	sproc = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
	if (sproc == NULL) {
		osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
		return NULL;
	}

	osal_memset(sproc, 0, sizeof(struct osal_proc_dir_entry));

	osal_strncpy(sproc->name, name, sizeof(sproc->name) - 1);

	if (parent != NULL) {
		proc = proc_mkdir_data(name, 0, parent->proc_dir_entry, sproc);
	} else {
		proc = proc_mkdir_data(name, 0, NULL, sproc);
	}
	if (proc == NULL) {
		osal_printk("%s - proc_mkdir failed!\n", __FUNCTION__);
		kfree(sproc);
		sproc = NULL;
		return NULL;
	}
	sproc->proc_dir_entry = proc;
#endif
	osal_list_add_tail(&(sproc->node), &list);
	return sproc;
}
EXPORT_SYMBOL(osal_proc_mkdir);

void osal_remove_proc_root(const char *name, osal_proc_entry_t *parent)
{
	struct osal_proc_dir_entry *sproc = NULL;
	if (name == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return;
	}
	if (parent != NULL) {
		remove_proc_entry(name, parent->proc_dir_entry);
	} else {
		remove_proc_entry(name, NULL);
	}
	osal_list_for_each_entry(sproc, &list, node)
	{
		if (osal_strncmp(sproc->name, name, sizeof(sproc->name)) == 0) {
			osal_list_del(&(sproc->node));
			break;
		}
	}
	if (sproc != NULL) {
		kfree(sproc);
	}
}

int osal_seq_printf(osal_proc_entry_t *entry, const char *fmt, ...)
{
	struct seq_file *s = (struct seq_file *)(entry->seqfile);
	va_list args;
	int r = 0;

	va_start(args, fmt);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 3, 0)
	r = seq_vprintf(s, fmt, args);
#else
	seq_vprintf(s, fmt, args);
#endif
	va_end(args);

	return r;
}
EXPORT_SYMBOL(osal_seq_printf);

void osal_proc_init(void)
{
	OSAL_INIT_LIST_HEAD(&list);
	proc_entry = osal_proc_mkdir("umap", OSAL_NULL);
	if (proc_entry == OSAL_NULL) {
		osal_printk("test init, proc mkdir error!\n");
	}
}
void osal_proc_exit(void)
{
	osal_remove_proc_root("umap", OSAL_NULL);
}
