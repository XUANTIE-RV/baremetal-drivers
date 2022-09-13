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

#define REG_BASE 0x5300

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_DENOISE3D2_CTRL", "Denoise3D2 ctrl",
			0x00005300, 0x00000000, 12, 12, "denoise3d_v20_soft_rst", "Denoise3D2:soft reset bit"},
	{NULL, NULL,	0x00005300, 0x00000000, 11, 11, "denoise3d_v20_invgamma_en", "Denoise3D2: inv gamma  enable bit"},
	{NULL, NULL,	0x00005300, 0x00000000, 10, 10, "denoise3d_v20_pregamma_en", "Denoise3D2: pregamma  enable bit"},
	{NULL, NULL,	0x00005300, 0x00000000,  9,  9, "denoise3d_v20_erode_en", "Denoise3D2: motion erosion  enable bit"},
	{NULL, NULL,	0x00005300, 0x00000000,  8,  8, "denoise3d_v20_motion_conv_en", "Denoise3D2: motion convergence enable bit"},
	{NULL, NULL,	0x00005300, 0x00000000,  7,  7, "denoise3d_v20_inv_dgain_en", "Denoise3D2: inv digital gain enable bit"},
	{NULL, NULL,	0x00005300, 0x00000000,  6,  6, "denoise3d_v20_inv_awb_gain_en", "Denoise3D2: inv awb  gain enable bit"},
	{NULL, NULL,	0x00005300, 0x00000000,  5,  5, "denoise3d_v20_comp_luma_en", "Denoise3D2: luma compensation enable bit"},
	{NULL, NULL,	0x00005300, 0x00000000,  4,  4, "denoise3d_v20_ref_reset", "Denoise3D2: soft reset bit"},
	{NULL, NULL,	0x00005300, 0x00000000,  3,  3, "denoise3d_v20_motion_dilate_enable", "Denoise3D2: motion dilation enable bit"},
	{NULL, NULL,	0x00005300, 0x00000000,  2,  2, "denoise3d_v20_nlm_enable", "Denoise3D2: 2dnr enable bit"},
	{NULL, NULL,	0x00005300, 0x00000000,  1,  1, "denoise3d_v20_tnr_enable", "Denoise3D2: tnr enable bit"},
	{NULL, NULL,	0x00005300, 0x00000000,  0,  0, "denoise3d_v20_enable", "Denoise3D2: enable bit"},


	{"ISP_DENOISE3D2_PREGAMMA_Y_0", "Denoise3D2 Pregamma Y LUT Register",
			0x00005400, 0x00000FFF, 29, 24, "denoise3d2_pregamma_y_2_a", "Denoise 3DV2 Pregamma Y 2 [11:6]"},
	{NULL, NULL,	0x00005400, 0x00000FFF, 23, 12, "denoise3d2_pregamma_y_1", "Denoise 3DV2 Pregamma Y 1"},
	{NULL, NULL,	0x00005400, 0x00000FFF, 11,  0, "denoise3d2_pregamma_y_0", "Denoise 3DV2 Pregamma Y 0"},
};


int light_isp_denoise3d2_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_denoise3d2_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_denoise3d2_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

