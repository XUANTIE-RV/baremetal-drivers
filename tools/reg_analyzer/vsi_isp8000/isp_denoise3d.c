
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

#define ISP_DENOISE3D_BASE 0x3700
#define REG_BASE ISP_DENOISE3D_BASE

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name, reg_desc,                  offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_DENOISE3D_CTRL", "Denoise3D Control Register",
			0x00003700, 0x00000000, 10, 10, "denoise3d_read_ref_en",	"0: disable read ref data from SRAM; 1: Enable read ref data from SRAM"},
	{NULL, NULL,	0x00003700, 0x00000000,  9,  9, "denoise3d_raw_decompress_en",	"1: decompress enable; t0: decompress disable"},
	{NULL, NULL,	0x00003700, 0x00000000,  8,  8, "denoise3d_raw_compress_en",	"1: compress enable; 0: compress disable"},
	{NULL, NULL,	0x00003700, 0x00000000,  7,  7, "denoise3d_write_ref_en",	"0: disable write ref data to SRAM; 1: Enable write ref data to SRAM (Default)"},
};


int light_isp_denoise3d_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_denoise3d_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_denoise3d_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

