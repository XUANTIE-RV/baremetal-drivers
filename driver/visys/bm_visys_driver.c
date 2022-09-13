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

#include "bm_printk.h"
#include "bm_visys_ioctl.h"

#define BM_DRIVER_NAME "bm_visys"
#define BM_DRIVER_MAXCNT 1

struct bm_visys_drvdata {
	struct cdev cdev;
	dev_t devt;
	struct class *class;
	struct mutex mutex;
	unsigned int device_idx;
	void __iomem *base;
	void __iomem *reset;
	//int irq_num;
	void *private;	// can be bm_visys_drvdata_private, but not use now
};

struct bm_visys_drvdata_private {
	int private_tmp;
};

static struct class *bm_driver_class;
static unsigned int bm_driver_major = 0;
static unsigned int bm_driver_minor = 0;
static unsigned int device_register_index = 0;

static struct bm_visys_drvdata *g_pdrvdata;

#define check_retval(x)\
	do {\
		if ((x))\
			return -EIO;\
	} while (0)

static unsigned int bm_visys_poll(struct file * filp, poll_table *wait)
{
	return 0;
}

void bm_visys_work(struct work_struct *work)
{
}

irqreturn_t bm_visys_irq(int irq, void *dev_id)
{
	bm_info("enter %s\n", __func__);
	return IRQ_HANDLED;
}

static int bm_visys_open(struct inode * inode, struct file * file)
{
	struct bm_visys_drvdata *drvdata;

	bm_info("enter %s\n", __func__);

	drvdata = container_of(inode->i_cdev, struct bm_visys_drvdata, cdev);
	file->private_data = drvdata;

	return 0;
};

int k_bm_visys_write_reg(uint32_t offset, uint32_t value)
{
	writel(value, g_pdrvdata->base + offset);
	bm_info("%s  addr 0x%08x val 0x%08x\n", __func__, offset, value);
	return 0;
}

int k_bm_visys_read_reg(uint32_t offset, uint32_t *value)
{
	*value = readl(g_pdrvdata->base + offset);
	bm_info("%s  addr 0x%08x val 0x%08x\n", __func__, offset, *value);
	return 0;
}

EXPORT_SYMBOL(k_bm_visys_write_reg);
EXPORT_SYMBOL(k_bm_visys_read_reg);

static int bm_visys_write_reg(struct bm_visys_drvdata *drvdata, void *__user args)
{
	struct bm_visys_reg_t reg;
	check_retval(copy_from_user(&reg, args, sizeof(reg)));
	writel(reg.value, drvdata->base + reg.offset);
	bm_info("%s  addr 0x%08x val 0x%08x\n", __func__, reg.offset, reg.value);
	return 0;
}

static int bm_visys_read_reg(struct bm_visys_drvdata *drvdata, void *__user args)
{
	struct bm_visys_reg_t reg;
	check_retval(copy_from_user(&reg, args, sizeof(reg)));
	reg.value = readl(drvdata->base + reg.offset);
	check_retval(copy_to_user(args, &reg, sizeof(reg)));
	bm_info("%s  addr 0x%08x val 0x%08x\n", __func__, reg.offset, reg.value);
	return 0;
}

static long bm_visys_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	struct bm_visys_drvdata *drvdata;
	bm_info("enter %s\n", __func__);

	drvdata = file->private_data;
	if (drvdata == NULL) {
		bm_err("%s:file private is null point error\n", __func__);
		return  -ENOMEM;
	}

	mutex_lock(&drvdata->mutex);
	switch (cmd) {
	case BMVISYSIOC_WRITE_REG:
		ret = bm_visys_write_reg(drvdata, (void *)arg);
		break;
	case BMVISYSIOC_READ_REG:
		ret = bm_visys_read_reg(drvdata, (void *)arg);
		break;
	default:
		ret = -EPERM;
		bm_err("%s: unsupported command %d", __func__, cmd);
		break;
	}
	mutex_unlock(&drvdata->mutex);
	return ret;
};

static int bm_visys_release(struct inode * inode, struct file * file)
{
	bm_info("enter %s\n", __func__);
	return 0;
};

static int bm_visys_mmap(struct file *pFile, struct vm_area_struct *vma)
{
	bm_info("enter %s\n", __func__);
	return 0;
};

struct file_operations bm_visys_fops = {
	.owner = THIS_MODULE,
	.open = bm_visys_open,
	.release = bm_visys_release,
	.unlocked_ioctl = bm_visys_ioctl,
	.mmap = bm_visys_mmap,
	.poll = bm_visys_poll,
};

static int bm_visys_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct bm_visys_drvdata *drvdata;
	struct resource *iores_mem;
	u32 value;

	bm_info("enter %s\n", __func__);
	pdev->id = device_register_index;
	if (pdev->id >= BM_DRIVER_MAXCNT) {
		bm_err("%s:pdev id is %d error\n", __func__, pdev->id);
		return  -EINVAL;
	}

	drvdata = devm_kzalloc(&pdev->dev,sizeof(struct bm_visys_drvdata), GFP_KERNEL);
	if (drvdata == NULL) {
		bm_err("%s:alloc struct drvdata error\n", __func__);
		return  -ENOMEM;
	}

    g_pdrvdata = drvdata;

	iores_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	drvdata->base = devm_ioremap_resource(&pdev->dev, iores_mem);
	bm_info("%s: [%s%d]: drvdata->base=0x%px, phy_addr base=0x%llx\n", __func__,
		BM_DRIVER_NAME, pdev->id, drvdata->base, iores_mem->start);
	drvdata->reset = NULL;
	drvdata->device_idx = pdev->id;
	mutex_init(&drvdata->mutex);
	//drvdata->irq_num = platform_get_irq(pdev, 0);
	//bm_info("%s:[%s%d]: pdriver_dev->irq_num=%d\n", __func__,
	//	BM_VISYS_NAME, pdev->id, drvdata->irq_num);

	platform_set_drvdata(pdev, drvdata);

	if (pdev->id == 0) {
		if (bm_driver_major == 0) {
			ret = alloc_chrdev_region(&drvdata->devt, 0, BM_DRIVER_MAXCNT, BM_DRIVER_NAME);
			if (ret != 0) {
				bm_err("%s:alloc_chrdev_region error\n", __func__);
				return ret;
			}
			bm_driver_major = MAJOR(drvdata->devt);
			bm_driver_minor = MINOR(drvdata->devt);
		} else {
			drvdata->devt = MKDEV(bm_driver_major, bm_driver_minor);
			ret = register_chrdev_region(drvdata->devt, BM_DRIVER_MAXCNT, BM_DRIVER_NAME);
			if (ret) {
				bm_err("%s:register_chrdev_region error\n", __func__);
				return ret;
			}
		}

		bm_driver_class = class_create(THIS_MODULE, BM_DRIVER_NAME);
		if (IS_ERR(bm_driver_class)) {
			bm_err("%s[%d]:class_create error!\n", __func__, __LINE__);
			return -EINVAL;
		}
	}

	drvdata->devt = MKDEV(bm_driver_major, bm_driver_minor + pdev->id);
	cdev_init(&drvdata->cdev, &bm_visys_fops);
	ret = cdev_add(&drvdata->cdev, drvdata->devt, 1);
	if ( ret ) {
		bm_err("%s[%d]:cdev_add error!\n", __func__, __LINE__);
		return ret;
	} else {
		bm_info("%s[%d]:cdev_add OK, major=%d, minor=%d\n", __func__, __LINE__,
			bm_driver_major, bm_driver_minor + pdev->id);
	}
	drvdata->class = bm_driver_class;
	device_create(drvdata->class, NULL, drvdata->devt,
		      drvdata, "%s%d", BM_DRIVER_NAME, pdev->id);

	value = readl(drvdata->base+0x4);
	bm_info("offset=04, value is:0x%08x\n", value);

	device_register_index++;
	bm_info("exit %s:[%s%d]\n", __func__, BM_DRIVER_NAME, pdev->id);

	return 0;

//err_register:
	// When error occore, should free memory, destory device, unregister ...
	// but now, ignore it
//	return ret;
}

static int bm_visys_remove(struct platform_device *pdev)
{
	struct bm_visys_drvdata *drvdata;

	bm_info("enter %s\n", __func__);
	device_register_index--;

	drvdata = platform_get_drvdata(pdev);
	//free_irq(drvdata->irq_num, drvdata);
	cdev_del(&drvdata->cdev);
	device_destroy(drvdata->class, drvdata->devt);
	unregister_chrdev_region(drvdata->devt, BM_DRIVER_MAXCNT);
	mutex_destroy(&drvdata->mutex);
	if (device_register_index == 0) {
		class_destroy(drvdata->class);
	}
	devm_kfree(&pdev->dev, drvdata);

	bm_info("exit %s\n", __func__);
	return 0;
}

static const struct of_device_id bm_visys_of_match[] = {
	{ .compatible = "thead,light-bm-visys", },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, bm_visys_of_match);

static struct platform_driver bm_visys_driver = {
	.probe		= bm_visys_probe,
	.remove		= bm_visys_remove,
	.driver = {
		.name  = BM_DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(bm_visys_of_match),
	}
};

static int __init bm_visys_init_module(void)
{
	int ret = 0;

	bm_info("enter %s\n", __func__);
	ret = platform_driver_register(&bm_visys_driver);
	if (ret) {
		bm_err("register platform driver failed.\n");
		return ret;
	}

	return ret;
}

static void __exit bm_visys_exit_module(void)
{
	bm_info("enter %s\n", __func__);
	platform_driver_unregister(&bm_visys_driver);
}

module_init(bm_visys_init_module);
module_exit(bm_visys_exit_module);

MODULE_AUTHOR("Lu Chongzhi");
MODULE_DESCRIPTION("BAREMETAL-VISYS");
MODULE_LICENSE("GPL");
