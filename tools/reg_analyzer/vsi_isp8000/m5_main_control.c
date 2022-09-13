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
#define REG_BASE 0x0000

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */


	{"VI_CCL", "Clock Control Register",
			0x00000000, 0x00000000,  2,  2, "vi_ccl_dis",		"Clock Control Logic disable." LINE_PADDING_6TAB
										"  0: processing/cfg-clocks for all sub modules enabled" LINE_PADDING_6TAB
										"  1: processing/cfg-clocks for all sub modules disabled " LINE_PADDING_6TAB
										"     without access to ID and VI_CCL register"},
	{NULL, NULL,	0x00000000, 0x00000000,  1,  1, "vi_ccl_dis_status",	"Status of vi_ccl[2] bit (copy of vi_ccl[2])"},

	{"VI_ICCL",  "Internal Clock Control Register",
			0x00000010, 0x000FFF7B, 19, 19, "stitch_3_clk_en", "unused"},
	{NULL, NULL,	0x00000010, 0x000FFF7B, 18, 18, "stitch_2_clk_en", "unused"},
	{NULL, NULL,	0x00000010, 0x000FFF7B, 17, 17, "reserved0", "reserved0"},
	{NULL, NULL,	0x00000010, 0x000FFF7B, 16, 16, "mcm_rd_clk_en", "MCM read clock enable; 0: power safe ; 1: processing mode"},
	{NULL, NULL,	0x00000010, 0x000FFF7B, 15, 15, "mcm_wr_clk_en", "0: power safe ; 1: processing mode"},
	{NULL, NULL,	0x00000010, 0x000FFF7B, 14, 14, "stitch_1_clk_en", "unused"},
	{NULL, NULL,	0x00000010, 0x000FFF7B, 13, 13, "stitch_0_clk_en", "stitch0 clock enable; 0: power safe ; 1: processing mode"},
	{NULL, NULL,	0x00000010, 0x000FFF7B, 12, 12, "vi_mipi2_clk_enable", "unused"},
	{NULL, NULL,	0x00000010, 0x000FFF7B, 11, 11, "vi_mipi_clk_enable", "unused"},
	{NULL, NULL,	0x00000010, 0x000FFF7B, 10, 10, "vi_smia_clk_enable", "unused"},
	{NULL, NULL,	0x00000010, 0x000FFF7B,  9,  9, "vi_simp_clk_enable", "unused"},
	{NULL, NULL,	0x00000010, 0x000FFF7B,  8,  8, "vi_ie_clk_enable", "unused"},
	{NULL, NULL,	0x00000010, 0x000FFF7B,  6,  6, "vi_mi_clk_enable", "Memory interface clock enable; 0: power safe ; 1: processing mode"},
	{NULL, NULL,	0x00000010, 0x000FFF7B,  5,  5, "vi_jpeg_clk_enable", "unused"},
	{NULL, NULL,	0x00000010, 0x000FFF7B,  4,  4, "vi_srsz_clk_enable", "Self-picture resize clock enable; 0: power safe ; 1: processing mode"},
	{NULL, NULL,	0x00000010, 0x000FFF7B,  3,  3, "vi_mrsz_clk_enable", "Main resize clock enable; 0: power safe; 1: processing mode"},
	{NULL, NULL,	0x00000010, 0x000FFF7B,  1,  1, "vi_cp_clk_enable", "CPROC processing clock enable; 0: power safe ; 1: processing mode"},
	{NULL, NULL,	0x00000010, 0x000FFF7B,  0,  0, "vi_isp_clk_enable", "ISP processing clock enable; 0: power safe ; 1: processing mode"},

	{"VI_IRCL",  "Internal Reset Control Register",
			0x00000014, 0x00000000, 20, 20, "soft_rst_mi_wrapper", "mi wrapper software reset; 0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000, 19, 19, "soft_rst_stitch_3", "unused"},
	{NULL, NULL,	0x00000014, 0x00000000, 18, 18, "soft_rst_stitch_2", "unused"},
	{NULL, NULL,	0x00000014, 0x00000000, 17, 17, "soft_rst_fe", "0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000, 16, 16, "soft_rst_mcm_rd", "0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000, 15, 15, "soft_rst_mcm_wr", "MCM write software reset; 0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000, 14, 14, "soft_rst_stitch_1", "unused"},
	{NULL, NULL,	0x00000014, 0x00000000, 13, 13, "soft_rst_stitch_0", "stitch0 software reset; 0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000, 12, 12, "vi_mipi2_soft_rst", "unused"},
	{NULL, NULL,	0x00000014, 0x00000000, 11, 11, "vi_mipi_soft_rst", "unused"},
	{NULL, NULL,	0x00000014, 0x00000000, 10, 10, "vi_smia_soft_rst", "unused"},
	{NULL, NULL,	0x00000014, 0x00000000,  9,  9, "vi_simp_soft_rst", "unused"},
	{NULL, NULL,	0x00000014, 0x00000000,  8,  8, "vi_ie_soft_rst", "unused"},
	{NULL, NULL,	0x00000014, 0x00000000,  7,  7, "vi_marvin_rst", "Hardware reset of entire image signal processor; 0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000,  6,  6, "vi_mi_soft_rst", "Memory interface software reset; 0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000,  5,  5, "vi_jpeg_soft_rst", "unused"},
	{NULL, NULL,	0x00000014, 0x00000000,  4,  4, "vi_srsz_soft_rst", "Self-picture resize software reset; 0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000,  3,  3, "vi_mrsz_soft_rst", "Main-picture resize software reset; 0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000,  2,  2, "vi_ycs_soft_rst", "Y/C splitter software reset; 0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000,  1,  1, "vi_cp_soft_rst", "Color processing software reset; 0: Processing mode; 1: reset state"},
	{NULL, NULL,	0x00000014, 0x00000000,  0,  0, "vi_isp_soft_rst", "ISP software reset; 0: Processing mode; 1: reset state"},

	{"VI_DPCL", "Data Path Control Register",
			0x00000018, 0x00000000, 27, 26, "if_select3",		"unused"},
	{NULL, NULL,	0x00000018, 0x00000000, 25, 24, "if_select2",		"unused"},
	{NULL, NULL,	0x00000018, 0x00000000, 23, 22, "next_sensor_id",	"unused"},
	{NULL, NULL,	0x00000018, 0x00000000, 21, 20, "sensor_id",		"Present frame id"},
	{NULL, NULL,	0x00000018, 0x00000000, 19, 19, "reserved2",		"reserved2"},
	{NULL, NULL,	0x00000018, 0x00000000, 18, 18, "strm_mux",		"1:raw out 0:yuv out"},
	{NULL, NULL,	0x00000018, 0x00000000, 17, 16, "if_select1",		"unused"},
	{NULL, NULL,	0x00000018, 0x00000000, 15, 15, "reserved1",		"unused"},
	{NULL, NULL,	0x00000018, 0x00000000, 14, 12, "vi_chan_mode",		"Enable Main Path, Self Path 1, Self Path 2." LINE_PADDING_6TAB
										"Note: More than one bit may be set to enable more than one path." LINE_PADDING_6TAB
										"Bit 12: Enable Main Path; Bit 13: Enable Self Path 1; Bit 14: Enable Self Path 2"},
	{NULL, NULL,	0x00000018, 0x00000000, 11, 11, "vi_dma_spmux",		"xml doc unused, but in code:" LINE_PADDING_6TAB
										"0: data from camera interface to self resize" LINE_PADDING_6TAB
										"1: data from DMA read port to self resize"},
	{NULL, NULL,	0x00000018, 0x00000000, 10, 10, "vi_dma_iemux",		"xml doc unused, but in code:" LINE_PADDING_6TAB
										"0: data from camera interface to image effects" LINE_PADDING_6TAB
										"1: data from DMA read port to image effects"},
	{NULL, NULL,	0x00000018, 0x00000000,  9,  8, "if_select0",		"Input interface for sensor0" LINE_PADDING_6TAB
										"0: parallel interface; 1: SMIA-interface (Reserved); 2: Unused; 3: HDR Stitch output"},
	{NULL, NULL,	0x00000018, 0x00000000,  6,  4, "vi_dma_switch",	"xml doc unused, but in code:" LINE_PADDING_6TAB
										"0: path to SPMUX" LINE_PADDING_6TAB
										"1: path to Superimpose" LINE_PADDING_6TAB
										"2: path to Image Effects" LINE_PADDING_6TAB
										"3: path to JPEG" LINE_PADDING_6TAB
										"4: path to ISP"},
	{NULL, NULL,	0x00000018, 0x00000000,  3,  2, "reserved0",		"unused"},
	{NULL, NULL,	0x00000018, 0x00000000,  1,  0, "vi_mp_mux",		"xml doc unused, but in code:" LINE_PADDING_6TAB
										"00: reserved (future: data from DMA read port to JPEG encoder)" LINE_PADDING_6TAB
										"01: data from main resize to MI, uncompressed" LINE_PADDING_6TAB
										"    (now also used for RAW data bypass)" LINE_PADDING_6TAB
										"10: data from main resize to JPEG encoder"},
};


int light_isp_main_control_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_main_control_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_main_control_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

