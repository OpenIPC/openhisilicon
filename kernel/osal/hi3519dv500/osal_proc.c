/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/version.h>
#include "ot_osal.h"
#include "securec.h"
#include "osal_inner.h"
static osal_mutex g_mutex_proc;
static struct osal_list_head g_list;
static osal_proc_entry *g_proc_entry = NULL;

static osal_proc_entry *osal_proc_find_node(const char *name, const struct osal_list_head *list)
{
    osal_proc_entry *node = NULL;

    if (list == NULL || name == NULL) {
        osal_log("list or name is null!\n");
        return NULL;
    }
    osal_list_for_each_entry(node, list, node) {
        if (osal_strcmp(node->name, name) == 0) {
            return node;
        }
    }

    return NULL;
}

static int osal_seq_show(struct seq_file *s, void *p)
{
    osal_proc_entry *oldsentry = NULL;
    osal_proc_entry sentry;
    if (s == NULL) {
        return -1;
    }
    osal_unused(p);
    oldsentry = s->private;
    if (oldsentry == NULL || oldsentry->read == NULL) {
        osal_log("parameter invalid!\n");
        return -1;
    }
    (void)memset_s(&sentry, sizeof(osal_proc_entry), 0, sizeof(osal_proc_entry));
    /* only these two parameters are used */
    sentry.seqfile = s;
    sentry.private_data = oldsentry->private_data;
    oldsentry->read(&sentry);
    return 0;
}

static int osal_proc_cmd_call(osal_proc_entry *entry, unsigned int argc, char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX])
{
    unsigned int i;

    for (i = 0; i < entry->cmd_cnt; i++) {
        if (osal_strcmp(entry->cmd_list[i].name, argv[0]) == 0) {
            if (entry->cmd_list[i].handler != NULL) {
                return entry->cmd_list[i].handler(argc, argv, entry->private_data);
            }
        }
    }

    return -1;
}

static void osal_proc_parse_para(const char *buf,
                                 char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX],
                                 unsigned int count,
                                 unsigned int *argc)
{
    unsigned int i;
    unsigned int j = 0;
    unsigned int k = 0;

    if (count >= PROC_CMD_ALL_LENGTH_MAX) {
        printk("ERROR:count (%d) is to large!\n", count);
        return;
    }

    for (i = 0; i < count; i++) {
        /* ignore SPACE */
        while (buf[i] == 0x20) {
            i++;
        }

        if (buf[i] == 0xa) {
            break;
        }

        if ((k >= (PROC_CMD_SINGEL_LENGTH_MAX - 1)) || (j >= (PROC_CMD_NUM_MAX - 1))) {
            return;
        }
        argv[j][k] = buf[i];
        k++;
        if (i >= (PROC_CMD_ALL_LENGTH_MAX - 1)) {
            return;
        }
        if ((buf[i + 1] == 0x20) || (buf[i + 1] == 0xa)) {
            argv[j][k] = '\0';
            k = 0;
            j++;
        }
    }

    *argc = j;
}
static ssize_t osal_proc_cmd_process(osal_proc_entry *entry, const char __user *buf, size_t count)
{
    int ret = -1;
    unsigned int argc = 0;
    char proc_buf[PROC_CMD_ALL_LENGTH_MAX] = {0};
    char argv[PROC_CMD_NUM_MAX][PROC_CMD_SINGEL_LENGTH_MAX];

    if ((buf == NULL) || (entry == NULL) || entry->cmd_cnt == 0) {
        return -1;
    }

    if ((count >= PROC_CMD_ALL_LENGTH_MAX) || (count == 0)) {
        printk("ERROR:count (%lu) is to large or Zero!\n", (unsigned long)count);
        return -1;
    }

    if (osal_copy_from_user(proc_buf, buf, count)) {
        return -1;
    }

    proc_buf[PROC_CMD_ALL_LENGTH_MAX - 1] = '\0';

    (void)memset_s(&argv[0][0], PROC_CMD_NUM_MAX * PROC_CMD_SINGEL_LENGTH_MAX, 0,
        PROC_CMD_NUM_MAX * PROC_CMD_SINGEL_LENGTH_MAX);
    osal_proc_parse_para(proc_buf, &argv[0], count, &argc);
    if (argc == 0) {
        printk("ERROR:not vaild arg!\n");
        return -1;
    }

    ret = osal_proc_cmd_call(entry, argc, argv);
    if (ret != 0) {
        printk("cmd is not find or param is wrong!\n");
        argc = 1;
        (void)strncpy_s(argv[0], PROC_CMD_SINGEL_LENGTH_MAX, "help", strlen("help"));
        (void)osal_proc_cmd_call(entry, argc, argv);
    }

    return count;
}
static ssize_t osal_proc_write(struct file *file, const char __user *buf,
                              size_t count, loff_t *ppos)
{
    osal_proc_entry *entry = PDE_DATA(file_inode(file));
    osal_proc_cmd_process(entry, buf, count);
    if ((entry != NULL) && (entry->write != NULL)) {
        return entry->write(entry, buf, count, (long long *)ppos);
    }

    return -ENOSYS;
}

static int osal_proc_open(struct inode *inode, struct file *file)
{
    osal_proc_entry *sentry = PDE_DATA(inode);
    if ((sentry != NULL) && (sentry->open != NULL)) {
        sentry->open(sentry);
    }
    return single_open(file, osal_seq_show, sentry);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
static struct proc_ops g_osal_proc_ops = {
    .proc_open = osal_proc_open,
    .proc_read = seq_read,
    .proc_write = osal_proc_write,
    .proc_lseek = seq_lseek,
    .proc_release = single_release
};
#else
static struct file_operations g_osal_proc_ops = {
    .owner = THIS_MODULE,
    .open = osal_proc_open,
    .read = seq_read,
    .write = osal_proc_write,
    .llseek = seq_lseek,
    .release = single_release
};
#endif

static osal_proc_entry *osal_create_proc(const char *name, osal_proc_entry *parent)
{
    struct proc_dir_entry *entry = NULL;
    osal_proc_entry *sentry = NULL;

    if (name == NULL) {
        return NULL;
    }

    sentry = osal_proc_find_node(name, &g_list);
    if (sentry != NULL) {
        return sentry;
    }

    sentry = kmalloc(sizeof(osal_proc_entry), GFP_KERNEL);
    if (sentry == NULL) {
        osal_log("kmalloc failed!\n");
        return NULL;
    }

    (void)memset_s(sentry, sizeof(osal_proc_entry), 0, sizeof(osal_proc_entry));

    if (strncpy_s(sentry->name, OSAL_PROC_NAME_LENGTH, name, sizeof(sentry->name) - 1) != EOK) {
        osal_log("strncpy_s failed!\n");
        kfree(sentry);
        return NULL;
    }

    entry = proc_create_data(name, 0, parent->proc_dir_entry, &g_osal_proc_ops, sentry);
    if (entry == NULL) {
        osal_log("create_proc_entry failed!\n");
        kfree(sentry);
        return NULL;
    }
    sentry->proc_dir_entry = entry;
    sentry->open = NULL;
    osal_mutex_lock(&g_mutex_proc);
    osal_list_add_tail(&(sentry->node), &g_list);
    osal_mutex_unlock(&g_mutex_proc);
    return sentry;
}

osal_proc_entry *osal_create_proc_entry(const char *name, osal_proc_entry *parent)
{
    parent = g_proc_entry;
    if (parent == NULL || parent->proc_dir_entry == NULL) {
        printk("osal_create_proc_entry - parent is null.");
        return NULL;
    }

    return osal_create_proc(name, parent);
}
EXPORT_SYMBOL(osal_create_proc_entry);

static void osal_remove_proc_root(const char *name)
{
    if (g_proc_entry == NULL) {
        printk("osal_remove_proc_root - parameter is invalid.");
        return;
    }

    proc_remove(g_proc_entry->proc_dir_entry);
    kfree(g_proc_entry);
    g_proc_entry = NULL;
}

static void osal_remove_proc(const char *name, struct proc_dir_entry *entry)
{
    osal_proc_entry *sproc = NULL;

    if (osal_list_empty(&g_list) != 0) {
        return;
    }
    if (name == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    sproc = osal_proc_find_node(name, &g_list);
    if (sproc == NULL) {
        printk("%s - name: %s not found!\n", __FUNCTION__, name);
        return;
    }
    remove_proc_entry(name, entry);
    osal_list_del(&(sproc->node));
    kfree(sproc);
}

void osal_remove_proc_entry(const char *name, osal_proc_entry *parent)
{
    parent = g_proc_entry;
    if (parent == NULL || parent->proc_dir_entry == NULL) {
        printk("osal_remove_proc_entry failed! parent is null.");
        return;
    }
    osal_remove_proc(name, parent->proc_dir_entry);
    return;
}
EXPORT_SYMBOL(osal_remove_proc_entry);

osal_proc_entry *osal_proc_mkdir(const char *name, osal_proc_entry *parent)
{
    struct proc_dir_entry *proc = NULL;
    osal_proc_entry *sproc = NULL;
    void *parent_proc_entry = NULL;

    if (name == NULL) {
        return NULL;
    }

    sproc = kmalloc(sizeof(osal_proc_entry), GFP_KERNEL);
    if (sproc == NULL) {
        osal_log("kmalloc failed!\n");
        return NULL;
    }

    (void)memset_s(sproc, sizeof(osal_proc_entry), 0, sizeof(osal_proc_entry));

    if (strncpy_s(sproc->name, OSAL_PROC_NAME_LENGTH, name, strlen(sproc->name)) != EOK) {
        kfree(sproc);
        osal_log("strncpy_s failed!\n");
        return NULL;
    }

    if (parent != NULL) {
        parent_proc_entry = parent->proc_dir_entry;
    }

    proc = proc_mkdir_data(name, 0, parent_proc_entry, sproc);
    if (proc == NULL) {
        kfree(sproc);
        osal_log("proc_mkdir failed!\n");
        return NULL;
    }
    sproc->proc_dir_entry = proc;
    osal_mutex_lock(&g_mutex_proc);
    osal_list_add_tail(&(sproc->node), &g_list);
    osal_mutex_unlock(&g_mutex_proc);
    return sproc;
}

void osal_seq_printf(void *seqfile, const char *fmt, ...)
{
    struct seq_file *s = NULL;
    va_list args;

    if (seqfile == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }

    s = (struct seq_file *)(seqfile);

    va_start(args, fmt);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 3, 0)
    (void)seq_vprintf(s, fmt, args);
#else
    seq_vprintf(s, fmt, args);
#endif
    va_end(args);
}
EXPORT_SYMBOL(osal_seq_printf);

void osal_proc_init(const char *name)
{
    int ret;
    if (g_proc_entry != NULL) {
        return;
    }
    ret = osal_mutex_init(&g_mutex_proc);
    if (ret != 0) {
        osal_log("workqueue mutex init failed! ret = %#x.\n", ret);
        return;
    }
    osal_mutex_lock(&g_mutex_proc);
    OSAL_INIT_LIST_HEAD(&g_list);
    osal_mutex_unlock(&g_mutex_proc);
    g_proc_entry = osal_proc_mkdir(name, NULL);
    if (g_proc_entry == NULL) {
        osal_log("osal_proc_init, proc mkdir error!\n");
    }
}
EXPORT_SYMBOL(osal_proc_init);

void osal_proc_exit(const char *name)
{
    if (osal_list_empty(&g_list) != 0) {
        return;
    }
    osal_remove_proc_root(name);
    osal_mutex_destroy(&g_mutex_proc);
    g_list.prev = NULL;
    g_list.next = NULL;
}
EXPORT_SYMBOL(osal_proc_exit);
