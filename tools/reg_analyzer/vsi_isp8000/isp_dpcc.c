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

#define REG_BASE 0x00002900

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_DPCC_MODE", "Global control register",
			0x00002900, 0x00000004,  2,  2, "stage1_enable",	"0: bypass stage1; 1: enable stage1 (Default)"},
	{NULL, NULL,	0x00002900, 0x00000004,  1,  1, "grayscale_mode",	"0: BAYER DATA INPUT (Default)"  LINE_PADDING_6TAB
										" 1: enable gray scale data input from black and white sensors (without color filter array)"},
	{NULL, NULL,	0x00002900, 0x00000004,  0,  0, "dpcc_enable",		"0: bypass DPCC (Default); 1: enable DPCC"},
};


int light_isp_dpcc_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_dpcc_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_dpcc_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

