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

#define REG_BASE 0x00003900

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_EE_CTRL", "Edge Enhancement Control Register",
			0x00003900, 0x00000000, 18, 11, "edge_enhancement_source_strength",	"Edge Enhancement strength of process"},
	{NULL, NULL,	0x00003900, 0x00000000, 10,  3, "edge_enhancement_strength",		"Edge Enhancement strength of smooth filter"},
	{NULL, NULL,	0x00003900, 0x00000000,  2,  2, "edge_enhancement_soft_reset",		"Edge Enhancement software reset.  0: release;  1: reset"},
	{NULL, NULL,	0x00003900, 0x00000000,  1,  1, "edge_enhancement_input_sel",		"Edge Enhancement input data format select.  0: RGB;  1: YUV"},
	{NULL, NULL,	0x00003900, 0x00000000,  0,  0, "edge_enhancement_enable",		"Enable/Disable Edge Enhancement.  0: Disable the EE;  1: Enable the EE"},
};


int light_isp_ee_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_ee_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_ee_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

