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

#define REG_BASE 0x00002600

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_EXP_CONF", "Exposure Control",
			0x00002600, 0x00000000, 31, 31, "exp_alt_mode",		"0: luminance calculation according to Y=16+0.25R+0.5G+0.1094B" LINE_PADDING_6TAB
										"1: luminance calculation according to Y=(R+G+B) x 0.332"},
	{NULL, NULL,	0x00002600, 0x00000000,  3,  2, "exp_src_select",	"data source select  1:wdr  0:csm"},
	{NULL, NULL,	0x00002600, 0x00000000,  1,  1, "autostop",		"0: continuous measurement; 1: stop measuring after a complete frame"},


	{"ISP_EXPV2_CTRL", "raw exposure control",
			0x000026A0, 0x00000000,  3,  2, "expv2_input_select",	"00: degamma output; 01: awb_gain output; 10: WDR3 output; 11: not used"},
	{NULL, NULL,	0x000026A0, 0x00000000,  1,  1, "expv2_autostop",	"0: continous measurement; 1: stop measuring after a complete frame"},
	{NULL, NULL,	0x000026A0, 0x00000000,  0,  0, "expv2_enable",		"1: start measuring a frame" LINE_PADDING_6TAB
										"The exp block will reset this bit and halt after " LINE_PADDING_6TAB
										"completing one frame if bit 'autostop' is set to 1"},
};

int light_isp_exposure_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_exposure_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_exposure_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

