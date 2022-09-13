/*
 * Copyright (C) 2021 Alibaba Group Holding Limited
 * Author: LuChongzhi <chongzhi.lcz@alibaba-inc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#include <asm/io.h>

#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/mm.h>

#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/poll.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/debugfs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>

#include <linux/of.h>


#define BM_DEMO_NAME "bm_demo"
#define BM_DRIVER_MAXCNT 2

struct bm_demo_driver_dev
{
	struct cdev cdev;
	dev_t devt;
	struct class *class;
	struct mutex vvmutex;
	unsigned int device_idx;
	struct work_struct demo_wq;
	wait_queue_head_t demo_wait;
	int irq_num;
	void *private;
};

#if 0
struct work_struct* pWorkQueue = NULL;
static unsigned int bm_demo_major = 0;
static unsigned int bm_demo_minor = 0;
struct class *bm_demo_class;
static unsigned int devise_register_index = 0;
static bool bm_demo_irq = false;
#endif

static unsigned int bm_demo_poll(struct file * filp, poll_table *wait)
{
	return 0;
}

void bm_demo_work(struct work_struct *work)
{
}

irqreturn_t bm_demo_irq(int irq, void *dev_id)
{
	pr_info("enter %s\n", __func__);
	return IRQ_HANDLED;
}

static int bm_demo_open(struct inode * inode, struct file * file)
{
	pr_info("enter %s\n", __func__);
	return 0;
};

static long bm_demo_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	pr_info("enter %s\n", __func__);
	return 0;
};

static int bm_demo_release(struct inode * inode, struct file * file)
{
	pr_info("enter %s\n", __func__);
	return 0;
};

static int bm_demo_mmap(struct file *pFile, struct vm_area_struct *vma)
{
	pr_info("enter %s\n", __func__);
	return 0;
};

struct file_operations bm_demo_fops = {
	.owner = THIS_MODULE,
	.open = bm_demo_open,
	.release = bm_demo_release,
	.unlocked_ioctl = bm_demo_ioctl,
	.mmap = bm_demo_mmap,
	.poll = bm_demo_poll,
};

static int bm_demo_probe(struct platform_device *pdev)
{
	pr_info("enter %s\n", __func__);
	return 0;
}

static int bm_demo_remove(struct platform_device *pdev)
{
	pr_info("enter %s\n", __func__);
	return 0;
}

static const struct of_device_id bm_demo_of_match[] = {
	{ .compatible = "thead,bm-demo", },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, bm_demo_of_match);

static struct platform_driver bm_demo_driver = {
	.probe		= bm_demo_probe,
	.remove		= bm_demo_remove,
	.driver = {
		.name  = BM_DEMO_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(bm_demo_of_match),
	}
};

static int __init bm_demo_init_module(void)
{
	int ret = 0;

	pr_info("enter %s\n", __func__);
	ret = platform_driver_register(&bm_demo_driver);
	if (ret) {
		pr_err("register platform driver failed.\n");
		return ret;
	}

	return ret;
}

static void __exit bm_demo_exit_module(void)
{
	pr_info("enter %s\n", __func__);
	platform_driver_unregister(&bm_demo_driver);
}

module_init(bm_demo_init_module);
module_exit(bm_demo_exit_module);

MODULE_AUTHOR("Lu Chongzhi");
MODULE_DESCRIPTION("BAREMETAL-DEMO");
MODULE_LICENSE("GPL");
