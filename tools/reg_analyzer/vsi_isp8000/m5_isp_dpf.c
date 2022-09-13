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
#define REG_BASE 0x2800

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_DPF_MODE", "Mode Control for De-noising Pre-Filter Block",
			0x00002800, 0x00000000,  9,  9, "use_nf_gain",		"0: DPF_NF_GAINs will not be used. (Default)" LINE_PADDING_6TAB
										"1: DPF_NF_GAINs will be used."},
	{NULL, NULL,	0x00002800, 0x00000000,  8,  8, "lsc_gain_en",		"0: LSC gain will not be processed. Use LSC gain factor of 1. (Default)" LINE_PADDING_6TAB
										"1: LSC gain will be processed"},
	{NULL, NULL,	0x00002800, 0x00000000,  7,  7, "awb_gain_en",		"Only relevant when use_nf_gain==0 and ISP_CTRL::ISP_AWB_ENABLE==1" LINE_PADDING_6TAB
										"0: ISP_AWB gains will not be processed. Use AWB gain factor of 1. (Default)" LINE_PADDING_6TAB
										"1: ISP_AWB gains will be processed"},
	{NULL, NULL,	0x00002800, 0x00000000,  6,  6, "nll_equ_segm",		"0: equidistant segmentation for NLL (Default)" LINE_PADDING_6TAB
										"1: optimized logarithmic like segmentation for Noise Level Lookup (NLL)"},
	{NULL, NULL,	0x00002800, 0x00000000,  5,  5, "rb_filter_size9x9",	"0: Wide Red and Blue filter kernel size of 13x9 (7x5 active) pixels (Default)" LINE_PADDING_6TAB
										"1: Red and Blue filter kernel size of 9x9 (5x5 active) pixels"},
	{NULL, NULL,	0x00002800, 0x00000000,  4,  4, "r_filter_off",		"0: filter R pixels (Default)" LINE_PADDING_6TAB
										"1: disable filter processing for red pixels (R)"},
	{NULL, NULL,	0x00002800, 0x00000000,  3,  3, "gr_filter_off",	"0: filter GB pixels (Default)" LINE_PADDING_6TAB
										"1: disable filter processing for green pixels in green/blue lines (GB)"},
	{NULL, NULL,	0x00002800, 0x00000000,  2,  2, "gb_filter_off",	"0: filter GB pixels (Default)" LINE_PADDING_6TAB
										"1: disable filter processing for green pixels in green/blue lines (GB)"},
	{NULL, NULL,	0x00002800, 0x00000000,  1,  1, "b_filter_off",		"0: filter B pixels (Default); 1: disable filter processing for blue pixels (B)"},
	{NULL, NULL,	0x00002800, 0x00000000,  0,  0, "cfg_dpf_enable",	"De-noising pre-filter; 0: Bypass DPF (Default); 1: Enable DPF"},
};


int light_isp_dpf_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_dpf_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_dpf_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

