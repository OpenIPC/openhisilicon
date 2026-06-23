/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/notifier.h>
#include <linux/slab.h>
#include <linux/reboot.h>
#include <linux/module.h>
#include "securec.h"
#include "ot_osal.h"

OSAL_LIST_HEAD(nb_list);

struct nb_node {
    struct osal_notifier_block *osal_nb;
    struct notifier_block *nb;
    struct osal_list_head node;
};

static struct osal_notifier_block *osal_find_ob(const struct notifier_block *nb)
{
    struct osal_list_head *this_nb = NULL;
    if (osal_list_empty(&nb_list)) {
        osal_printk("find nb failed! nb_list is empty!\n");
        return NULL;
    }
    osal_list_for_each(this_nb, &nb_list) {
        struct nb_node *ns = osal_list_entry(this_nb, struct nb_node, node);
        if (ns->nb == nb) {
            return ns->osal_nb;
        }
    }
    osal_printk("find ob failed!\n");
    return NULL;
}

static int osal_del_nb(const struct osal_notifier_block *nb)
{
    struct osal_list_head *this_nb = NULL;
    struct osal_list_head *next = NULL;
    if (osal_list_empty(&nb_list)) {
        osal_printk("find nb failed! nb_list is empty!\n");
        return OSAL_FAILURE;
    }
    osal_list_for_each_safe(this_nb, next, &nb_list) {
        struct nb_node *ns = osal_list_entry(this_nb, struct nb_node, node);
        if (ns->osal_nb == nb) {
            osal_list_del(this_nb);
            kfree(ns);
            return OSAL_SUCCESS;
        }
    }
    osal_printk("del nb failed!\n");
    return OSAL_FAILURE;
}

static int osal_notifier(struct notifier_block *nb, unsigned long action, void *data)
{
    struct osal_notifier_block *ob = osal_find_ob(nb);
    if ((ob != NULL) && (ob->notifier_call != NULL)) {
        return ob->notifier_call(ob, action, data);
    }
    return OSAL_SUCCESS;
}

int osal_register_reboot_notifier(struct osal_notifier_block *ob)
{
    struct notifier_block *nb = NULL;
    struct nb_node *node = NULL;

    if (ob == NULL) {
        osal_printk("%s input param error!\n", __FUNCTION__);
        return OSAL_FAILURE;
    }

    nb = kmalloc(sizeof(struct notifier_block), GFP_KERNEL);
    if (nb == NULL) {
        osal_printk("osal_register_reboot_notifier malloc nb failed!\n");
        return OSAL_FAILURE;
    }
    (void)memset_s(nb, sizeof(struct notifier_block), 0, sizeof(struct notifier_block));

    node = kmalloc(sizeof(struct nb_node), GFP_KERNEL);
    if (node == NULL) {
        osal_printk("osal_register_reboot_notifier kmalloc nb_node failed!\n");
        kfree(nb);
        return OSAL_FAILURE;
    }
    (void)memset_s(node, sizeof(struct nb_node), 0, sizeof(struct nb_node));

    nb->notifier_call = osal_notifier;
    register_reboot_notifier(nb);
    ob->notifier_block = nb;
    node->osal_nb = ob;
    node->nb = nb;
    osal_list_add(&(node->node), &nb_list);
    return OSAL_SUCCESS;
}
EXPORT_SYMBOL(osal_register_reboot_notifier);

int osal_unregister_reboot_notifier(struct osal_notifier_block *nb)
{
    if ((nb != NULL) && (nb->notifier_block != NULL)) {
        osal_del_nb(nb);
        unregister_reboot_notifier((struct notifier_block *)nb->notifier_block);
        kfree((struct notifier_block *)nb->notifier_block);
    }
    return OSAL_SUCCESS;
}
EXPORT_SYMBOL(osal_unregister_reboot_notifier);
