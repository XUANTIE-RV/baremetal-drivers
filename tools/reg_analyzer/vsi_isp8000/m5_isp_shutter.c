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

#define REG_BASE 0x00000680

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_SH_CTRL", "Mechanical shutter control",
			0x00000680, 0x00000000,  4,  4, "sh_open_pol",	"Shutter_open polarity. 0: high active; 1: low active"},
	{NULL, NULL,	0x00000680, 0x00000000,  3,  3, "sh_trig_pol",	"Mechanical shutter trigger edge." LINE_PADDING_6TAB
									"0: use negative edge of trigger signal" LINE_PADDING_6TAB
									"1: use positive edge of trigger signal"},
	{NULL, NULL,	0x00000680, 0x00000000,  2,  2, "sh_trig_src",	"Mechanical shutter trigger source." LINE_PADDING_6TAB
									"0: use “vds_vsync” for trigger event" LINE_PADDING_6TAB
									"1: use “shutter_trig” for trigger event"},
	{NULL, NULL,	0x00000680, 0x00000000,  1,  1, "sh_rep_en",	"Mechanical shutter repetition enable." LINE_PADDING_6TAB
									"0: shutter is opened only once" LINE_PADDING_6TAB
									"1: shutter is opened with the repetition rate of the trigger signal"},
	{NULL, NULL,	0x00000680, 0x00000000,  0,  0, "sh_en",	"Mechanical shutter. 0: disable; 1: enable"},

};


int light_isp_shutter_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_shutter_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_shutter_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

