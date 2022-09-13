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

#define REG_BASE 0x00002100

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_HIST256_PROP", "Histogram properties",
			0x00002100, 0x00000000,  9,  3, "stepsize",	"Histogram pre-divider" LINE_PADDING_6TAB
									"Process every (step size)th pixel; all other pixels are skipped" LINE_PADDING_6TAB
									"0,1,2: not allowed" LINE_PADDING_6TAB
									"3: process every third input pixel" LINE_PADDING_6TAB
									"4: process every fourth input pixel" LINE_PADDING_6TAB
									"..." LINE_PADDING_6TAB
									"7FH: process every 127th pixel"},
	{NULL, NULL,	0x00002100, 0x00000000,  2,  0, "hist_mode",	"Histogram mode, luminance is taken at ISP output before" LINE_PADDING_6TAB
									" output formatter, RGB is taken at xtalk output" LINE_PADDING_6TAB
									"0: disable, no measurements" LINE_PADDING_6TAB
									"1: RGB combined histogram" LINE_PADDING_6TAB
									"2: R histogram" LINE_PADDING_6TAB
									"3: G histogram" LINE_PADDING_6TAB
									"4: B histogram" LINE_PADDING_6TAB
									"5: Y (luminance) histogram" LINE_PADDING_6TAB
									"6, 7: must not be used"},
};


int light_isp_hist256_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_hist256_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_hist256_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

