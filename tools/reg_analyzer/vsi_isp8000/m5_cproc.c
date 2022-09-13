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

#define REG_BASE 0x800
static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */
	{"CPROC_CTRL", "Global control register",
			0x00000800, 0x00000000,  3,  3, "cproc_c_out_range",	"Color processing chrominance pixel clipping range at output" LINE_PADDING_6TAB
										"  0: CbCr_out clipping range [16..240] according to ITU-R BT.601 standard" LINE_PADDING_6TAB
										"  1: Full UV_out clipping range [0..255]"},
	{NULL, NULL,	0x00000800, 0x00000000,  2,  2, "cproc_y_in_range",	"Color processing luminance input range (offset processing)" LINE_PADDING_6TAB
										"  0: Y_in range [64..940] according to ITU-R BT.601 standard;" LINE_PADDING_6TAB
										"    offset of 64 will be subtracted from Y_in" LINE_PADDING_6TAB
										"  1: Y_in full range [0..1023]; no offset will be subtracted from Y_in"},
	{NULL, NULL,	0x00000800, 0x00000000,  1,  1, "cproc_y_out_range",	"Color processing luminance output clipping range" LINE_PADDING_6TAB
										"  0: Y_out clipping range [16..235];" LINE_PADDING_6TAB
										"    offset of 16 is added to Y_out according to ITU-R BT.601 standard" LINE_PADDING_6TAB
										"  1: Y_out clipping range [0..255]; no offset is added to Y_out"},
	{NULL, NULL,	0x00000800, 0x00000000,  0,  0, "cproc_enable",		"Color processing enable" LINE_PADDING_6TAB
										"  0: Color processing is bypassed" LINE_PADDING_6TAB
										"  1: Color processing is active" LINE_PADDING_6TAB
										"  2 * 10 bit input data are truncated to 2 * 8 bit output data" LINE_PADDING_6TAB
										"  output data are rounded to 2 * 8 bit and clipping is active"},
};

int light_isp_cproc_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_cproc_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_cproc_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

