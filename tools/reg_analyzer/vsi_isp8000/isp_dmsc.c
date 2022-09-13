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

#define REG_BASE 0x00003e00

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_DMSC_CTRL", "Demosaic control register",
			0x00003E00, 0x0400288E, 31, 24, "demosaic_thr",		"Threshold for Bayer demosaicing texture detection." LINE_PADDING_6TAB
										" This value shifted 4-bit is compared width the different of the vertical" LINE_PADDING_6TAB
										" and horizontal 12bit wide texture indicators, to decide if the vertical or" LINE_PADDING_6TAB
										" horizontal texture flag must be set." LINE_PADDING_6TAB
										"0x00: maximum edge sensitivity" LINE_PADDING_6TAB
										"0xff: no texture detection"},
	{NULL, NULL,	0x00003E00, 0x0400288E, 21, 16, "denoise_strength",	"green channel smoothing filter results account for the weight of the final result, the larger the blur"},
	{NULL, NULL,	0x00003E00, 0x0400288E, 14, 14, "frame_clr_sel",	"0:frame clear by vsync. 1:frame clear by frame end"},
	{NULL, NULL,	0x00003E00, 0x0400288E, 13, 13, "frame_end_en",		"frame clear by frame end"},
	{NULL, NULL,	0x00003E00, 0x0400288E, 12,  8, "sharpen_size",		"green channel sharpen size, the larger the sharpen edge, the greater the graininess"},
	{NULL, NULL,	0x00003E00, 0x0400288E,  7,  7, "vsync_pos_en",		"frame clear by VSYNC"},
	{NULL, NULL,	0x00003E00, 0x0400288E,  6,  6, "soft_rst_inner",	"soft reset"},
	{NULL, NULL,	0x00003E00, 0x0400288E,  5,  5, "sharpen_line_enable",	"0: disable line sharpen, 1: enable line sharpen"},
	{NULL, NULL,	0x00003E00, 0x0400288E,  4,  4, "skin_enable",		"0: disable skin area filter, 1: enable skin area filter"},
	{NULL, NULL,	0x00003E00, 0x0400288E,  3,  3, "depurple_enable",	"0: disable depurple, 1: enable depurple"},
	{NULL, NULL,	0x00003E00, 0x0400288E,  2,  2, "demoire_enable",	"0: disable demoire, 1: enable demoire"},
	{NULL, NULL,	0x00003E00, 0x0400288E,  1,  1, "sharpen_enable",	"0: disable sharpen, 1: enable sharpen"},
	{NULL, NULL,	0x00003E00, 0x0400288E,  0,  0, "demosaic_bypass",	"0: do interpolation, 1: bypass interpolation"},


	{"ISP_DMSC_INTP_THR", "Direction select threshold register",
			0x00003E04, 0x00000000, 23, 12, "interplationdir_thr_max",	"direction select threshold value in bright area, the interpolation " LINE_PADDING_6TAB
											" operation between maximum threshold and minimal threshold"},
	{NULL, NULL,	0x00003E04, 0x00000000, 11,  0, "interplationdir_thr_min",	"direction select threshold value in dark area, the interpolation "  LINE_PADDING_6TAB
											" operation between maximum threshold and minimal threshold"},

	{"ISP_DMSC_DMOI_CTRL", "Demoire ctrl register",
			0x00003E08, 0x00001F08, 13,  8, "demoire_area_thr", "judgment threshold of moire area"},
	{NULL, NULL,	0x00003E08, 0x00001F08,  5,  0, "demoire_sat_shrink", "moire pixel forced saturation, 32=no color"},


	{"ISP_DMSC_CAC_CTRL", "Control register for chromatic aberration correction",
			0x00003E44, 0x00000000,  3,  3, "cac_h_clip_mode", 	"defines the red/blue pixel shift in horizon direction" LINE_PADDING_6TAB
										"0:set horizon vector to +/-4 pixel" LINE_PADDING_6TAB
										"1:set horizon vector to +/-4 or +/-5 pixel " LINE_PADDING_6TAB
										"  depending on the position in bayer raster"},
	{NULL, NULL,	0x00003E44, 0x00000000,  2,  1, "cac_v_clip_mode",	"defines the red/blue pixel shift in vertical direction" LINE_PADDING_6TAB
										"00:set vertical vector to +/-2 pixel" LINE_PADDING_6TAB
										"01:set vertical vector to +/-3 pixel" LINE_PADDING_6TAB
										"10:set vertical vector to +/-3 or +/-4 pixel " LINE_PADDING_6TAB
										"   depending on the position in bayer raster" LINE_PADDING_6TAB
										"11:reserved"},
	{NULL, NULL,	0x00003E44, 0x00000000,  0,  0, "cac_enable",		"0: chromatic aberration correction off" LINE_PADDING_6TAB
										"1: chromatic aberration correction on"},

	{"ISP_DMSC_SIZE_CTRL", "demoire size ctrl register",
			0x00003E60, 0x01000000, 31, 16, "dummy_hblk_value", "dummy lines hblank cycles"},
	{NULL, NULL,	0x00003E60, 0x01000000, 15,  0, "img_hsize", "dmsc process image hsize"},



	{"ISP_DMSC_SHAP_FACT", "Sharpen factor register",
			0x00003E14, 0x00008008, 20, 12, "sharpen_factor_black",	"Sharpen factor of black edge; 0 means no sharpen"},
	{NULL, NULL,	0x00003E14, 0x00008008,  8,  0, "sharpen_factor_white",	"Sharpen factor of white edge; 0 means no sharpen"},

	{"ISP_DMSC_CTRL_SHD", "Demosaic control shadow register",
			0x00003E64, 0x0400080E, 31, 24, "demosaic_thr",		"Threshold for Bayer demosaicing texture detection." LINE_PADDING_6TAB
										" This value shifted 4-bit is compared width the different of the vertical" LINE_PADDING_6TAB
										" and horizontal 12bit wide texture indicators, to decide if the vertical or" LINE_PADDING_6TAB
										" horizontal texture flag must be set." LINE_PADDING_6TAB
										"0x00: maximum edge sensitivity" LINE_PADDING_6TAB
										"0xff: no texture detection"},
	{NULL, NULL,	0x00003E64, 0x0400080E, 21, 16, "denoise_strength",	"green channel smoothing filter results account for the weight of the final result, the larger the blur"},
	{NULL, NULL,	0x00003E64, 0x0400080E, 14, 14, "frame_clr_sel",	"0:frame clear by vsync. 1:frame clear by frame end"},
	{NULL, NULL,	0x00003E64, 0x0400080E, 13, 13, "frame_end_en",		"frame clear by frame end"},
	{NULL, NULL,	0x00003E64, 0x0400080E, 12,  8, "sharpen_size",		"green channel sharpen size, the larger the sharpen edge, the greater the graininess"},
	{NULL, NULL,	0x00003E64, 0x0400080E,  7,  7, "vsync_pos_en",		"frame clear by VSYNC"},
	{NULL, NULL,	0x00003E64, 0x0400080E,  6,  6, "soft_rst_inner",	"soft reset"},
	{NULL, NULL,	0x00003E64, 0x0400080E,  5,  5, "sharpen_line_enable",	"0: disable line sharpen, 1: enable line sharpen"},
	{NULL, NULL,	0x00003E64, 0x0400080E,  4,  4, "skin_enable",		"0: disable skin area filter, 1: enable skin area filter"},
	{NULL, NULL,	0x00003E64, 0x0400080E,  3,  3, "depurple_enable",	"0: disable depurple, 1: enable depurple"},
	{NULL, NULL,	0x00003E64, 0x0400080E,  2,  2, "demoire_enable",	"0: disable demoire, 1: enable demoire"},
	{NULL, NULL,	0x00003E64, 0x0400080E,  1,  1, "sharpen_enable",	"0: disable sharpen, 1: enable sharpen"},
	{NULL, NULL,	0x00003E64, 0x0400080E,  0,  0, "demosaic_bypass",	"0: do interpolation, 1: bypass interpolation"},

};


int light_isp_dmsc_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_dmsc_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_dmsc_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

