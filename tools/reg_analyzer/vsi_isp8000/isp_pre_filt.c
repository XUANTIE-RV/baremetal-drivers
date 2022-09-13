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

#define REG_BASE 0x00004000

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_PRE_FILT_CTRL", "Pre-filter Mode control register",
			0x00004000, 0x00C08802, 23, 23, "part_two_enable",	"pre part enable"},
	{NULL, NULL,	0x00004000, 0x00C08802, 22, 22, "part_one_enable",	"post part enable"},
	{NULL, NULL,	0x00004000, 0x00C08802, 21, 21, "soft_rst_pre_filt",	"Soft reset. 0: release; 1: reset"},
	{NULL, NULL,	0x00004000, 0x00C08802, 20, 13, "demosaic_threshold",	"Threshold for Bayer demosaicing texture detection. " LINE_PADDING_6TAB
										"This value shifted left 4 bits is compared with the " LINE_PADDING_6TAB
										"difference of the vertical and horizontal 12 bit wide " LINE_PADDING_6TAB
										"texture indicators, to determine if the vertical or " LINE_PADDING_6TAB
										"horizontal texture flag must be set." LINE_PADDING_6TAB
										"0x00: maximum edge sensitivity" LINE_PADDING_6TAB
										"0xFF: no texture detection"},
	{NULL, NULL,	0x00004000, 0x00C08802, 12,  9, "stage1_select",	"Green filter stage 1 select (range 0x0...0x8)" LINE_PADDING_6TAB
										"0x0: maximum blurring; 0x4: Default; " LINE_PADDING_6TAB
										"0x7: minimum blurring; 0x8: filter stage1 bypass" LINE_PADDING_6TAB
										"For a detailed description refer to chapter " LINE_PADDING_6TAB
										"'Filter (Sharpen/Blur) Programming' " LINE_PADDING_6TAB
										"in the Programming Guide, Part 2."},
	{NULL, NULL,	0x00004000, 0x00C08802,  8,  7, "out_rgb_bayer_pattern","0: RGGB; 1: GRBG; 2: GBRG; 3: BGGR"},
	{NULL, NULL,	0x00004000, 0x00C08802,  6,  3, "rgbir_bayer_pattern",	"0: BGGIR/BAYER PATTERN (Default); 1: GRIRG; 2: RGGIR; 3: GBIRG;" LINE_PADDING_6TAB
										"4: GIRRG; 5: IRGGB; 6: GIRBG; 7: IRGGR; 8: RGIRB; 9: GRBI;" LINE_PADDING_6TAB
										"10: IRBRG; 11: BIRGR; 12: BGIRR; 13: GBRIR; 14: IRRBG; 15: RIRGB"},
	{NULL, NULL,	0x00004000, 0x00C08802,  2,  2, "green_filt_enable",	"Enable/disable green channel filter" LINE_PADDING_6TAB
										"0: disable green filter (Default)" LINE_PADDING_6TAB
										"1: enable green filter"},
	{NULL, NULL,	0x00004000, 0x00C08802,  1,  1, "green_filt_mode",	"0: green filter static mode (active filter factor = FILT_FAC_MID)" LINE_PADDING_6TAB
										"1: dynamic noise reduction/sharpen (Default)"},
	{NULL, NULL,	0x00004000, 0x00C08802,  0,  0, "isp_pre_filt_enable",	"0: bypass pre_filt (Default); 1: enable pre_filt"},
};


int light_isp_pre_filt_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_pre_filt_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_pre_filt_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

