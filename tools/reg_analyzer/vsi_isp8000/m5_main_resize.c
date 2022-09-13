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

	{"MRSZ_CTRL", "Global control register",
			0x00000c00, 0x00000000, 10, 10, "auto_upd", "0: no update; 1: update shadow registers at frame end"},
	{NULL, NULL,	0x00000c00, 0x00000000,  9,  9, "cfg_upd", "Write 0: nothing happens; Write 1: update shadow registers; Read: Always 0"},
	{NULL, NULL,	0x00000c00, 0x00000000,  8,  8, "crop_enable", "0: Bypass crop ; 1: Enable picture crop"},
	{NULL, NULL,	0x00000c00, 0x00000000,  7,  7, "scale_vc_up", "0: Vertical chrominance downscaling selected ; 1: Vertical chrominance upscaling selected"},
	{NULL, NULL,	0x00000c00, 0x00000000,  6,  6, "scale_vy_up", "0: Vertical luminance downscaling selected ; 1: Vertical luminance upscaling selected"},
	{NULL, NULL,	0x00000c00, 0x00000000,  5,  5, "scale_hc_up", "0: Horizontal chrominance downscaling selected ; 1: Horizontal chrominance upscaling selected"},
	{NULL, NULL,	0x00000c00, 0x00000000,  4,  4, "scale_hy_up", "0: Horizontal luminance downscaling selected ; 1: Horizontal luminance upscaling selected"},
	{NULL, NULL,	0x00000c00, 0x00000000,  3,  3, "scale_vc_enable", "Vertical chrominance scaling unit; 0: Bypass; 1: enable"},
	{NULL, NULL,	0x00000c00, 0x00000000,  2,  2, "scale_vy_enable", "Vertical luminance scaling unit; 0: Bypass; 1: enable"},
	{NULL, NULL,	0x00000c00, 0x00000000,  1,  1, "scale_hc_enable", "Horizontal chrominance scaling unit; 0: Bypass; 1: enable"},
	{NULL, NULL,	0x00000c00, 0x00000000,  0,  0, "scale_hy_enable", "Horizontal luminance scaling unit ; 0: Bypass; 1: enable"},


	{"MRSZ_SCALE_HY", "Horizontal luminance scale factor register",
			0x00000c04, 0x00000000, 15,	0, "scale_hy",	"This register is set to the horizontal luminance downscale factor" LINE_PADDING_6TAB
									"or to the reciprocal of the horizontal luminance upscale factor."},

	{"MRSZ_SCALE_HCB", "Horizontal Cb scale factor register",
			0x00000c08, 0x00000000, 15,  0, "scale_hcb",	"This register is set to the horizontal Cb downscale factor " LINE_PADDING_6TAB
									"or to the reciprocal of the horizontal Cb upscale factor."},

	{"MRSZ_SCALE_HCR", "Horizontal Cr scale factor register",
			0x00000c0c, 0x00000000, 15,  0, "scale_hcr",	"This register is set to the horizontal Cr downscale factor " LINE_PADDING_6TAB
									"or to the reciprocal of the horizontal Cr upscale factor."},

	{"MRSZ_SCALE_VY", "Vertical luminance scale factor register",
			0x00000c10, 0x00000000, 15,  0, "scale_vy",	"This register is set to the vertical luminance downscale factor " LINE_PADDING_6TAB
									"or to the reciprocal of the vertical luminance upscale factor."},

	{"MRSZ_SCALE_VC", "Vertical chrominance scale factor register",
			0x00000c14, 0x00000000, 15,  0, "scale_vc",	"This register is set to the vertical chrominance downscale factor" LINE_PADDING_6TAB
									"or to the reciprocal of the vertical chrominance upscale factor."},

	{"MRSZ_FORMAT_CONV_CTRL", "Format conversion control",
			0x00000c6c, 0x00000000, 10, 10, "rsz_pack_format",	"0: YUV422/YUV420 planar; 1: YUV422/YUV420 semi_planar"},
	{NULL, NULL,	0x00000c6c, 0x00000000,  9,  9, "expand_8to10_method",	"0: add two MSBs in two LSBs; 1: add zeros in two LSBs"},
	{NULL, NULL,	0x00000c6c, 0x00000000,  8,  8, "expand_8to10_enable",	"0: disable 10 bit (add two zeros in two LSBs); 1: enable 10 bit"},
	{NULL, NULL,	0x00000c6c, 0x00000000,  7,  7, "cfg_422nocosited",	"0: YCbCr422 data are co-sited (Y0 Cb0 and Cr0 are sampled at the same position)" LINE_PADDING_6TAB
										"1: YCbCr422 data are non_co-sited (Cb and Cr samples are centered between Y samples)" LINE_PADDING_6TAB
										"   so modified interpolation is activated" LINE_PADDING_6TAB
										"Note: the programmed value becomes effective immediately after this register is set." LINE_PADDING_6TAB
										"Therefore, only write to this register if no picture data is sent to the Self Path."},
	{NULL, NULL,	0x00000c6c, 0x00000000,  6,  6, "cfg_cbcr_full",	"0: CbCr has a compressed range of [16..240]" LINE_PADDING_6TAB
										"1: CbCr has full range [0..255]" LINE_PADDING_6TAB
										"Note: the programmed value becomes effective immediately after this register is set. " LINE_PADDING_6TAB
										"Therefore, only write to this register if no picture data is sent to the Self Path."},
	{NULL, NULL,	0x00000c6c, 0x00000000,  5,  5, "cfg_y_full",		"0: Y has a compressed range of [16..235]" LINE_PADDING_6TAB
										"1: Y has full range [0..255]" LINE_PADDING_6TAB
										"Note: the programmed value becomes effective immediately after this register is set." LINE_PADDING_6TAB
										"Therefore, only write to this register if no picture data is sent to the Self Path."},
	{NULL, NULL,	0x00000c6c, 0x00000000,  4,  2, "rsz_output_format",	"Main Resize output format"LINE_PADDING_6TAB
										"000: YCbCr 4:0:0" LINE_PADDING_6TAB
										"001: YCbCr 4:2:0" LINE_PADDING_6TAB
										"010: YCbCr 4:2:2" LINE_PADDING_6TAB
										"011: YCbCr 4:4:4" LINE_PADDING_6TAB
										"100: RGB565" LINE_PADDING_6TAB
										"101: RGB666" LINE_PADDING_6TAB
										"110: RGB888" LINE_PADDING_6TAB
										"Reserved"},
	{NULL, NULL,	0x00000c6c, 0x00000000,  1,  0, "rsz_input_format",	"Main resize input format" LINE_PADDING_6TAB
										"00: YCbCr 4:0:0" LINE_PADDING_6TAB
										"01: YCbCr 4:2:0" LINE_PADDING_6TAB
										"10: YCbCr 4:2:2" LINE_PADDING_6TAB
										"11: YCbCr 4:4:4"},
};

int light_isp_main_resize_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_main_resize_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_main_resize_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

