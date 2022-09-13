
/*
 * Copyright (C) 2021 Alibaba Group Holding Limited
 * Author: LuChongzhi <chongzhi.lcz@alibaba-inc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "ra_common.h"

#define ISP_EXP_BAYER_BASE 0x2680
#define REG_BASE ISP_EXP_BAYER_BASE

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name, reg_desc,                  offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_EXP_H_SIZE_SHD",	"ISP_EXP_H_SIZE_SHD",  0x00002680, 0x00000000, 10,  0, "exp_h_size", "Shadow Horizontal size of one block in pixels."},
};


int light_isp_exp_bayer_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_exp_bayer_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_exp_bayer_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

