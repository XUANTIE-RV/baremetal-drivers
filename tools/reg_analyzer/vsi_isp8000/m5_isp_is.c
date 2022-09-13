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

#define MRV_IS_BASE 0x2300
#define REG_BASE MRV_IS_BASE

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_IS_CTRL",	"Image Stabilization Control Register",
		0x00002300, 0x00000000,  0,  0, "is_en",	"Image stabilization state. 0: off ; 1: on"},

	{"ISP_IS_RECENTER", "Image Stabilization Re-center Register",
		0x00002304, 0x00000000,  0,  0, "is_recenter",	"0: re-center feature switched off; 1..7: re-centering by (cur_h/v_offs-H/V_OFFS)/2^RECENTER"},

	{"ISP_IS_H_OFFS", "Horizontal Offset of Output Window",
		0x00002308, 0x00000000, 13,  0, "is_h_offs",	"Horizontal picture offset in pixels"},

	{"ISP_IS_V_OFFS", "Vertical Offset of Output Window",
		0x0000230C, 0x00000000, 13,  0, "is_v_offs",	"Vertical picture offset in lines"},

	{"ISP_IS_H_SIZE", "Horizontal Output Picture Size",
		0x00002310, 0x00001000, 13,  0, "is_h_size",	"Horizontal picture size in pixels if ISP_MODE is set to: ..."},

	{"ISP_IS_V_SIZE", "Vertical Output Picture Size",
		0x00002314, 0x00001000, 13,  0, "is_v_size",	"Vertical picture size in lines"},
};


int light_isp_is_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_is_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_is_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

