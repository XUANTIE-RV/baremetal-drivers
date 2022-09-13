/*
 * Copyright (C) 2021 Alibaba Group Holding Limited
 * Author: LuChongzhi <chongzhi.lcz@alibaba-inc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _BM_ISP_IOCTL_H_
#define _BM_ISP_IOCTL_H_

#include <linux/ioctl.h>

enum {
	BMISPIOC_WRITE_REG = 0x100,
	BMISPIOC_READ_REG,
};

struct bm_isp_reg_t {
	unsigned int offset;
	unsigned int value;
};

#endif /* _BM_ISP_IOCTL_H_ */
