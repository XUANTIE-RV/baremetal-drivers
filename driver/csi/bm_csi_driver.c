/*
 * Copyright (C) 2021 Alibaba Group Holding Limited
 * Author: liuyitong <yitong.lyt@alibaba-inc.com>
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
#include <linux/pm_runtime.h>

#include <linux/of.h>

#include "bm_printk.h"
#include "bm_csi_ioctl.h"
#include "bm_csi_hw.h"
#include "bm_csi_dphy.h"

#define BM_DRIVER_NAME "bm_csi"
#define BM_DRIVER_MAXCNT 3

static struct class *bm_driver_class;
static unsigned int bm_driver_major = 0;
static unsigned int bm_driver_minor = 0;
static unsigned int device_register_index = 0;

#define check_retval(x)\
	do {\
		if ((x))\
			return -EIO;\
	} while (0)

static unsigned int bm_csi_poll(struct file * filp, poll_table *wait)
{
	return 0;
}

void bm_csi_work(struct work_struct *work)
{
}

irqreturn_t bm_csi_irq(int irq, void *dev_id)
{
	bm_info("enter %s\n", __func__);
	return IRQ_HANDLED;
}

static int bm_csi_runtime_suspend(struct device *dev)
{
	struct bm_csi_drvdata *pdriver_dev = dev_get_drvdata(dev);
	dev_info(dev, "enter %s\n", __func__);

    if (pdriver_dev->pclk != NULL) {
	    clk_disable_unprepare(pdriver_dev->pclk);
    }

    if (pdriver_dev->pixclk!= NULL) {
	    clk_disable_unprepare(pdriver_dev->pixclk);
    }

    if (pdriver_dev->cfg_clk0!= NULL) {
	    clk_disable_unprepare(pdriver_dev->cfg_clk0);
    }

    if (pdriver_dev->cfg_clk1!= NULL) {
	    clk_disable_unprepare(pdriver_dev->cfg_clk1);
    }

    if (pdriver_dev->cfg_clk2!= NULL) {
	    clk_disable_unprepare(pdriver_dev->cfg_clk2);
    }

	pr_info("exit %s\n", __func__);
	return 0;
}
static int bm_csi_runtime_resume(struct device *dev)
{
	int ret = 0;
	struct bm_csi_drvdata *pdriver_dev = dev_get_drvdata(dev);
	dev_info(dev, "enter %s\n", __func__);

    if (pdriver_dev->pclk != NULL) {
	    ret = clk_prepare_enable(pdriver_dev->pclk);
	    if (ret < 0) {
	    	dev_err(dev, "could not prepare or enable csi pclk\n");
	    	clk_disable_unprepare(pdriver_dev->pclk);
            return -1;
	    }
    }

    if (pdriver_dev->pixclk!= NULL) {
	    ret = clk_prepare_enable(pdriver_dev->pixclk);
	    if (ret < 0) {
	    	dev_err(dev, "could not prepare or enable  pixclk\n");
	    	clk_disable_unprepare(pdriver_dev->pixclk);
            return -1;
	    }
    }

    if (pdriver_dev->cfg_clk0 != NULL) {
	    ret = clk_prepare_enable(pdriver_dev->cfg_clk0);
	    if (ret < 0) {
	    	dev_err(dev, "could not prepare or enable cfg_clk0\n");
	    	clk_disable_unprepare(pdriver_dev->cfg_clk0);
            return -1;
	    }
    }

    if (pdriver_dev->cfg_clk1 != NULL) {
	    ret = clk_prepare_enable(pdriver_dev->cfg_clk1);
	    if (ret < 0) {
	    	dev_err(dev, "could not prepare or enable cfg_clk1\n");
	    	clk_disable_unprepare(pdriver_dev->cfg_clk1);
            return -1;
	    }
    }

    if (pdriver_dev->cfg_clk2 != NULL) {
	    ret = clk_prepare_enable(pdriver_dev->cfg_clk2);
	    if (ret < 0) {
	    	dev_err(dev, "could not prepare or enable cfg_clk2\n");
	    	clk_disable_unprepare(pdriver_dev->cfg_clk2);
            return -1;
	    }
    }

	pr_info("%s csi Enabled clock\n", __func__);
	return 0;
}

static const struct dev_pm_ops bm_csi_runtime_pm_ops = {
	SET_RUNTIME_PM_OPS(bm_csi_runtime_suspend, bm_csi_runtime_resume, NULL)
};

static int bm_csi_open(struct inode * inode, struct file * file)
{
    int ret = 0;
	struct bm_csi_drvdata *drvdata;
    struct device *dev;

	bm_info("enter %s\n",  __func__);
	dev_info(dev, "open mipi-csi dev\n");

	drvdata = container_of(inode->i_cdev, struct bm_csi_drvdata, cdev);
	file->private_data = drvdata;
    dev = &drvdata->pdev->dev;
	dev_info(dev, "open mipi-csi dev\n");
    if (pm_runtime_get_sync(dev)) {
		ret = bm_csi_runtime_resume(dev);
		if (ret)
			pr_err("fail to resume csi %s %d\n", __func__, __LINE__);
	}

	return 0;
};

static long bm_csi_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	struct bm_csi_drvdata *drvdata;
	bm_info("enter %s\n", __func__);

	drvdata = file->private_data;
	if (drvdata == NULL) {
		bm_err("%s:file private is null point error\n", __func__);
		return  -ENOMEM;
	}

	mutex_lock(&drvdata->mutex);
	switch (cmd) {
	case BMCSI_IOC_WRITE_REG:
		ret = bm_csi_write_reg(drvdata, (void *)arg);
		break;
	case BMCSI_IOC_READ_REG:
		ret = bm_csi_read_reg(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_S_RESET:
        ret = bm_csi_reset(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_S_POWER:
        ret = bm_csi_set_power(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_G_POWER:
        ret = bm_csi_get_power(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_S_CLOCK:
        ret = bm_csi_set_clock(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_G_CLOCK:
        ret = bm_csi_get_clock(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_S_STREAM:
        ret = bm_csi_set_stream(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_G_STREAM:
        ret = bm_csi_get_stream(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_S_FMT:
        ret = bm_csi_set_fmt(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_G_FMT:
        ret = bm_csi_get_fmt(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_S_VC_SELECT:
        ret = bm_csi_set_vc_select(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_G_VC_SELECT:
        ret = bm_csi_get_vc_select(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_IPI_START:
        ret = bm_csi_ipi_enable(drvdata, (void *)arg);
        break;
    case BMCSI_IOC_IPI_STOP:
        ret = bm_csi_ipi_disable(drvdata, (void *)arg);
        break;
    case BMCSI_IOC_S_LANE_CFG:
        ret = bm_csi_set_lane_cfg(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_S_HSA_CFG:
        ret = bm_csi_set_hsa(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_S_HBP_CFG:
        ret = bm_csi_set_hbp(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_S_HSD_CFG:
        ret = bm_csi_set_hsd(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_SET_PIXCLK:
        ret = bm_csi_set_pixclk(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_GET_PIXCLK:
        ret = bm_csi_get_pixclk(drvdata, (void *)arg);
		break;
    case BMCSI_IOC_MAX:
		break;
	default:
		ret = -EPERM;
		bm_err("%s: unsupported command %d", __func__, cmd);
		break;
	}
	mutex_unlock(&drvdata->mutex);
	return ret;
};

static int bm_csi_release(struct inode * inode, struct file * file)
{
    int ret = 0;
    struct bm_csi_drvdata *drvdata;
    struct device *dev;

	bm_info("enter %s\n", __func__);

	drvdata = container_of(inode->i_cdev, struct bm_csi_drvdata, cdev);
	file->private_data = drvdata;
    dev = &drvdata->pdev->dev;
	dev_info(dev, "release mipi-csi dev\n");
    ret = bm_csi_dis_power(drvdata);
    if (ret) {
		pr_info("fail to disable csi power, %s %d\n", __func__, __LINE__);
	}

    ret = pm_runtime_put_sync(dev);
	if (ret) {
		pr_info("fail to resume csi %s %d\n", __func__, __LINE__);
	}

	return 0;
};

static int bm_csi_mmap(struct file *pFile, struct vm_area_struct *vma)
{
	bm_info("enter %s\n", __func__);
	return 0;
};

struct file_operations bm_csi_fops = {
	.owner = THIS_MODULE,
	.open = bm_csi_open,
	.release = bm_csi_release,
	.unlocked_ioctl = bm_csi_ioctl,
	.mmap = bm_csi_mmap,
	.poll = bm_csi_poll,
};

static int bm_csi_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct bm_csi_drvdata *drvdata;
	struct resource *iores_mem;
    unsigned int dphyglueiftester= 0;
    unsigned int sysreg_mipi_csi_ctrl = 0;
	u32 value;

	bm_info("enter %s\n", __func__);
	pdev->id = device_register_index;
	if (pdev->id >= BM_DRIVER_MAXCNT) {
		bm_err("%s:pdev id is %d error\n", __func__, pdev->id);
		return  -EINVAL;
	}

	drvdata = devm_kzalloc(&pdev->dev,sizeof(struct bm_csi_drvdata), GFP_KERNEL);
	if (drvdata == NULL) {
		bm_err("%s:alloc struct drvdata error\n", __func__);
		return  -ENOMEM;
	}

	iores_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	drvdata->base = devm_ioremap_resource(&pdev->dev, iores_mem);
	bm_info("%s: [%s%d]: drvdata->base=0x%px, phy_addr base=0x%llx\n", __func__,
		BM_DRIVER_NAME, pdev->id, drvdata->base, iores_mem->start);

	drvdata->reset = NULL;
	drvdata->device_idx = pdev->id;
	mutex_init(&drvdata->mutex);
	drvdata->irq_num = platform_get_irq(pdev, 0);
	bm_info("%s:[%s%d]: pdriver_dev->irq_num=%d\n", __func__, "BM_CSI", pdev->id, drvdata->irq_num);

    drvdata->pdev = pdev;

    /*parse clk info from dts*/
	drvdata->pclk = devm_clk_get(&pdev->dev, "pclk");
	if (IS_ERR(drvdata->pclk)) {
		dev_err(&pdev->dev, "failed to get pclk");
        drvdata->pclk = NULL;
	}

	drvdata->pixclk = devm_clk_get(&pdev->dev, "pixclk");
	if (IS_ERR(drvdata->pixclk)) {
		dev_err(&pdev->dev, "failed to get pixclk");
        drvdata->pixclk = NULL;
	}

	drvdata->cfg_clk0 = devm_clk_get(&pdev->dev, "cfg_clk0");
	if (IS_ERR(drvdata->cfg_clk0)) {
		dev_err(&pdev->dev, "failed to get cfg_clk0");
        drvdata->cfg_clk0 = NULL;
	}

    drvdata->cfg_clk1 = devm_clk_get(&pdev->dev, "cfg_clk1");
	if (IS_ERR(drvdata->cfg_clk1)) {
		dev_err(&pdev->dev, "failed to get cfg_clk1");
        drvdata->cfg_clk1 = NULL;
	}

    drvdata->cfg_clk2 = devm_clk_get(&pdev->dev, "cfg_clk2");
	if (IS_ERR(drvdata->cfg_clk2)) {
		dev_err(&pdev->dev, "failed to get cfg_clk2");
        drvdata->cfg_clk2 = NULL;
	}

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
	cdev_init(&drvdata->cdev, &bm_csi_fops);
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

    /*read version*/
	value = readl(drvdata->base + 0x0);
	bm_info("offset=04, value is:0x%08x\n", value);

    device_property_read_u32(&pdev->dev, "dphyglueiftester", &dphyglueiftester);
	bm_info("dphyglueiftester is:0x%x\n", dphyglueiftester);
    device_property_read_u32(&pdev->dev, "sysreg_mipi_csi_ctrl", &sysreg_mipi_csi_ctrl);
	bm_info("sysreg_mipi_csi_ctrl is:0x%x\n", sysreg_mipi_csi_ctrl);

    dw_dphy_rx_probe(pdev, dphyglueiftester, sysreg_mipi_csi_ctrl);
    dw_csi_probe(pdev);

    pm_runtime_enable(&pdev->dev);
	ret = bm_csi_runtime_resume(&pdev->dev);
	if (ret < 0) {
		dev_err(&pdev->dev, "fail to resume csi\n");
	}

	ret = bm_csi_runtime_suspend(&pdev->dev);
	if (ret < 0) {
		dev_err(&pdev->dev, "fail to suspend csi\n");
	}

	device_register_index++;
	bm_info("exit %s:[%s%d]\n", __func__, BM_DRIVER_NAME, pdev->id);

	return 0;
}

static int bm_csi_remove(struct platform_device *pdev)
{
	struct bm_csi_drvdata *drvdata;

	bm_info("enter %s\n", __func__);
    dw_dphy_rx_remove(pdev);
    dw_csi_remove(pdev);

	device_register_index--;
	drvdata = platform_get_drvdata(pdev);
	free_irq(drvdata->irq_num, drvdata);
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

static const struct of_device_id bm_csi_of_match[] = {
	{ .compatible = "thead,light-bm-csi", },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, bm_csi_of_match);

static struct platform_driver bm_csi_driver = {
	.probe		= bm_csi_probe,
	.remove		= bm_csi_remove,
	.driver = {
		.name  = BM_DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(bm_csi_of_match),
        .pm = &bm_csi_runtime_pm_ops,
	}
};

static int __init bm_csi_init_module(void)
{
	int ret = 0;

	bm_info("enter %s\n", __func__);
	ret = platform_driver_register(&bm_csi_driver);
	if (ret) {
		bm_err("register platform driver failed.\n");
		return ret;
	}

	return ret;
}

static void __exit bm_csi_exit_module(void)
{
	bm_info("enter %s\n", __func__);
	platform_driver_unregister(&bm_csi_driver);
}

module_init(bm_csi_init_module);
module_exit(bm_csi_exit_module);

MODULE_AUTHOR("Liu Yitong");
MODULE_DESCRIPTION("BAREMETAL-CSI");
MODULE_LICENSE("GPL");
