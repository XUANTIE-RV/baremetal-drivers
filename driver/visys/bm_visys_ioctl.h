/*
 * Copyright (C) 2021 Alibaba Group Holding Limited
 * Author: LuChongzhi <chongzhi.lcz@alibaba-inc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _BM_VISYS_IOCTL_H_
#define _BM_VISYS_IOCTL_H_

#include <linux/ioctl.h>

enum {
	BMVISYSIOC_WRITE_REG = 0x100,
	BMVISYSIOC_READ_REG,
};

struct bm_visys_reg_t {
	unsigned int offset;
	unsigned int value;
};

#endif /* _BM_VISYS_IOCTL_H_ */
