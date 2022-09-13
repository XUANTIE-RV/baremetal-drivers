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

#include <linux/of.h>

#include "bm_printk.h"
#include "bm_csi_ioctl.h"
#include "bm_csi_hw.h"
#include "dw-dphy-rx.h"
#include "bm_csi_dphy.h"

#define check_retval(x)\
	do {\
		if ((x))\
			return -EIO;\
	} while (0)

struct csi_format_context {
    uint32_t format;
    uint32_t width;
    uint32_t height;
};


int bm_csi_write_reg(struct bm_csi_drvdata *drvdata, void *__user args)
{
	struct bm_csi_reg_t reg;
	check_retval(copy_from_user(&reg, args, sizeof(reg)));
	writel(reg.value, drvdata->base + reg.offset);
	bm_info("%s  addr 0x%08x val 0x%08x\n", __func__, reg.offset, reg.value);
	return 0;
}

int bm_csi_read_reg(struct bm_csi_drvdata *drvdata, void *__user args)
{
	struct bm_csi_reg_t reg;
	check_retval(copy_from_user(&reg, args, sizeof(reg)));
	reg.value = readl(drvdata->base + reg.offset);
	check_retval(copy_to_user(args, &reg, sizeof(reg)));
	bm_info("%s  addr 0x%08x val 0x%08x\n", __func__, reg.offset, reg.value);
	return 0;
}

int bm_csi_reset(struct bm_csi_drvdata *drvdata, void *__user args)
{
	bm_info("%s: csi reset success\n", __func__);
    dw_csi_soc_reset();
    dw_mipi_csi_reset(&drvdata->csi_dev);
    dw_dphy_reset(drvdata->dphy->phy);
	return 0;
}

static uint32_t csi_power_on_sta = 0;

int bm_csi_en_power(struct bm_csi_drvdata *drvdata)
{
	bm_info("%s: csi set power\n", __func__);
	csi_power_on_sta = 1;
    dw_mipi_csi_s_power(&drvdata->csi_dev, csi_power_on_sta);
	return 0;
}

int bm_csi_dis_power(struct bm_csi_drvdata *drvdata)
{
	bm_info("%s: csi set power\n", __func__);
	csi_power_on_sta = 0;
    dw_mipi_csi_s_power(&drvdata->csi_dev, csi_power_on_sta);
	return 0;
}

int bm_csi_set_power(struct bm_csi_drvdata *drvdata, void *__user args)
{
	bm_info("%s: csi set power\n", __func__);
	check_retval(copy_from_user(&csi_power_on_sta, args, sizeof(csi_power_on_sta)));
    dw_mipi_csi_s_power(&drvdata->csi_dev, csi_power_on_sta);
	return 0;
}

int bm_csi_get_power(struct bm_csi_drvdata *drvdata, void *__user args)
{
	bm_info("%s: csi get power\n", __func__);
	check_retval(copy_to_user(args, &csi_power_on_sta, sizeof(csi_power_on_sta)));
	return 0;
}

int bm_csi_set_clock(struct bm_csi_drvdata *drvdata, void *__user args)
{
    struct dw_dphy_rx *dphy = drvdata->dphy;
    bm_info("%s: \n", __func__);
	check_retval(copy_from_user(&dphy->dphy_freq, args, sizeof(dphy->dphy_freq)));

	return 0;
}

int bm_csi_get_clock(struct bm_csi_drvdata *drvdata, void *__user args)
{
    struct dw_dphy_rx *dphy = drvdata->dphy;
	bm_info("%s: \n", __func__);
	check_retval(copy_to_user(args, &dphy->dphy_freq, sizeof(dphy->dphy_freq)));
	return 0;
}

int bm_csi_set_pixclk(struct bm_csi_drvdata *drvdata, void *__user args)
{
    uint32_t freq_hz = 0;
    bm_info("%s: \n", __func__);
	check_retval(copy_from_user(&freq_hz, args, sizeof(freq_hz)));
    return dw_csi_set_pixclk(freq_hz);
}

int bm_csi_get_pixclk(struct bm_csi_drvdata *drvdata, void *__user args)
{
    uint32_t freq_hz = 0;
	bm_info("%s: \n", __func__);
    dw_csi_get_pixclk(&freq_hz);
	check_retval(copy_to_user(args, &freq_hz, sizeof(freq_hz)));
    return 0;
}

int bm_csi_set_stream(struct bm_csi_drvdata *drvdata, void *__user args)
{
	bm_info("%s: \n", __func__);
	return 0;
}

int bm_csi_get_stream(struct bm_csi_drvdata *drvdata, void *__user args)
{
	bm_info("%s: \n", __func__);
	check_retval(copy_to_user(args, &csi_power_on_sta, sizeof(csi_power_on_sta)));
	return 0;
}

int bm_csi_set_fmt(struct bm_csi_drvdata *drvdata, void *__user args)
{
    struct dw_csi *csi_dev = &drvdata->csi_dev;
    u8 dt = 0;
    //struct csi_format_context fmt;
	bm_info("%s: \n", __func__);
	check_retval(copy_from_user(&dt, args, sizeof(dt)));
/*
    csi_dev->fmt->mbus_code = fmt.format;
    csi_dev->fmt->width = fmt.width;
    csi_dev->fmt->height = fmt.height;
    */

	csi_dev->ipi_dt = dt;
    dw_mipi_csi_set_ipi_fmt(csi_dev);
/*
    if(fmt.height > 0 && fmt.width > 0) {
        csi_dev->hw.ipi_color_mode = COLOR16;
        csi_dev->hw.ipi_auto_flush = 1;
        csi_dev->hw.ipi_adv_features = LINE_EVENT_SELECTION(EVSELAUTO);
        csi_dev->hw.htotal = fmt.width + csi_dev->hw.hsa + csi_dev->hw.hbp + csi_dev->hw.hsd;
        csi_dev->hw.vactive = fmt.height;
        csi_dev->hw.output = 0;
    }
    */

	return 0;
}

int bm_csi_get_fmt(struct bm_csi_drvdata *drvdata, void *__user args)
{
    struct dw_csi *csi_dev = &drvdata->csi_dev;
    struct csi_format_context fmt;
	bm_info("%s: \n", __func__);
    fmt.format = csi_dev->fmt->mbus_code;
    fmt.height = csi_dev->fmt->height;
    fmt.width = csi_dev->fmt->width;

	check_retval(copy_to_user(args, &fmt, sizeof(fmt)));
	return 0;
}

int bm_csi_set_vc_select(struct bm_csi_drvdata *drvdata, void *__user args)
{
    struct csi_vc_select_context vc;
    struct dw_csi *csi_dev = &drvdata->csi_dev;
	bm_info("%s: \n", __func__);
	check_retval(copy_from_user(&vc, args, sizeof(vc)));

    if (vc.ipi_idx == 1) {
        csi_dev->hw.ipi1_en = true;
        csi_dev->hw.virtual_ch = vc.vc_ch;
    } else if (vc.ipi_idx == 2) {
        csi_dev->hw.ipi2_en = true;
        csi_dev->hw.ipi2_virtual_ch = vc.vc_ch;
    } else if (vc.ipi_idx == 3) {
        csi_dev->hw.ipi3_en = true;
        csi_dev->hw.ipi3_virtual_ch = vc.vc_ch;
    }

	return 0;
}

int bm_csi_ipi_enable(struct bm_csi_drvdata *drvdata, void *__user args)
{
    int ipi_idx;
    struct dw_csi *csi_dev = &drvdata->csi_dev;
	bm_info("%s: \n", __func__);
	check_retval(copy_from_user(&ipi_idx, args, sizeof(ipi_idx)));

    if (ipi_idx == 1) {
        csi_dev->hw.ipi1_en = true;
    } else if (ipi_idx == 2) {
        csi_dev->hw.ipi2_en = true;
    } else if (ipi_idx == 3) {
        csi_dev->hw.ipi3_en = true;
    }

    dw_mipi_csi_start(csi_dev);
	return 0;
}

int bm_csi_ipi_disable(struct bm_csi_drvdata *drvdata, void *__user args)
{
    int ipi_idx;
    struct dw_csi *csi_dev = &drvdata->csi_dev;
	bm_info("%s: \n", __func__);
	check_retval(copy_from_user(&ipi_idx, args, sizeof(ipi_idx)));

    if (ipi_idx == 1) {
        csi_dev->hw.ipi1_en = 0;
    } else if (ipi_idx == 2) {
        csi_dev->hw.ipi2_en = 0;
    } else if (ipi_idx == 3) {
        csi_dev->hw.ipi3_en = 0;
    }

    dw_mipi_csi_stop(csi_dev);
	return 0;
}

int bm_csi_get_vc_select(struct bm_csi_drvdata *drvdata, void *__user args)
{
    struct csi_vc_select_context vc;
    struct dw_csi *csi_dev = &drvdata->csi_dev;
	bm_info("%s: \n", __func__);

    if (vc.ipi_idx == 1) {
       vc.vc_ch = csi_dev->hw.virtual_ch;
    } else if (vc.ipi_idx == 2) {
       vc.vc_ch = csi_dev->hw.ipi2_virtual_ch;
    } else if (vc.ipi_idx == 3) {
       vc.vc_ch = csi_dev->hw.ipi3_virtual_ch;
    }

	check_retval(copy_to_user(args, &vc, sizeof(vc)));

	return 0;
}

int bm_csi_set_lane_cfg(struct bm_csi_drvdata *drvdata, void *__user args)
{
    struct dw_csi *csi_dev = &drvdata->csi_dev;
	bm_info("%s: \n", __func__);

	check_retval(copy_from_user(&csi_dev->hw.num_lanes, args, sizeof(csi_dev->hw.num_lanes)));

	return 0;
}

typedef struct {
    int ipi_idx;
    int val;
} dw_csi_hsx_cfg_t;

int bm_csi_set_hsa(struct bm_csi_drvdata *drvdata, void *__user args)
{
    struct dw_csi *csi_dev = &drvdata->csi_dev;
    dw_csi_hsx_cfg_t cfg;
	bm_info("%s: \n", __func__);

	check_retval(copy_from_user(&cfg, args, sizeof(cfg)));
    if (cfg.ipi_idx == 1) {
        csi_dev->hw.hsa = cfg.val;
    } else if(cfg.ipi_idx == 2) {
        csi_dev->hw.ipi2_hsa = cfg.val;
    } else if(cfg.ipi_idx == 3) {
        csi_dev->hw.ipi3_hsa = cfg.val;
    } else {
        return -1;
    }


	return 0;
}

int bm_csi_set_hbp(struct bm_csi_drvdata *drvdata, void *__user args)
{
    struct dw_csi *csi_dev = &drvdata->csi_dev;
    dw_csi_hsx_cfg_t cfg;
	bm_info("%s: \n", __func__);

    check_retval(copy_from_user(&cfg, args, sizeof(cfg)));
    if (cfg.ipi_idx == 1) {
        csi_dev->hw.hbp = cfg.val;
    } else if(cfg.ipi_idx == 2) {
        csi_dev->hw.ipi2_hbp = cfg.val;
    } else if(cfg.ipi_idx == 3) {
        csi_dev->hw.ipi3_hbp = cfg.val;
    } else {
        return -1;
    }

	return 0;
}

int bm_csi_set_hsd(struct bm_csi_drvdata *drvdata, void *__user args)
{
    struct dw_csi *csi_dev = &drvdata->csi_dev;
    dw_csi_hsx_cfg_t cfg;
	bm_info("%s: \n", __func__);

	check_retval(copy_from_user(&cfg, args, sizeof(cfg)));
    if (cfg.ipi_idx == 1) {
        csi_dev->hw.hsd = cfg.val;
    } else if(cfg.ipi_idx == 2) {
        csi_dev->hw.ipi2_hsd = cfg.val;
    } else if(cfg.ipi_idx == 3) {
        csi_dev->hw.ipi3_hsd = cfg.val;
    } else {
        return -1;
    }

	return 0;
}
