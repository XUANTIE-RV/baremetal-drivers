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
#define REG_BASE 0x1200

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"MCM_CTRL",	"MCM control register",
			0x00001200, 0x00020000, 17, 17, "mcm_sensor_mem_bypass","0: use sensor latency memory in MCM; 1: not use sensor latency memory in MCM"},
	{NULL, NULL,	0x00001200, 0x00020000, 16, 14, "mcm_g2_wr1_fmt",	"Controls the input format for MCM_G2 channel 1 with same definition as mcm_wr0_fmt"},
	{NULL, NULL,	0x00001200, 0x00020000, 13, 11, "mcm_g2_wr0_fmt",	"Controls the input format for MCM_G2 channel 0 with same definition as mcm_wr0_fmt"},
	{NULL, NULL,	0x00001200, 0x00020000, 10,  8, "mcm_wr1_fmt",		"Controls the input format for MCM channel 1 with same definition as mcm_wr0_fmt"},
	{NULL, NULL,	0x00001200, 0x00020000,  7,  5, "mcm_wr0_fmt", 		"MCM Channel-0: 000: 8-bit; 001: 10-bit; 010: 12-bit; 011: 14-bit; 100: 16-bit; 101: 20-bit"},
	{NULL, NULL,	0x00001200, 0x00020000,  4,  1, "mcm_bypass_switch",	"Bypass switch. 0000: sensor0 bypass; 0001: sensor1 bypass; …; 1111: sensor15 bypass if any"},
	{NULL, NULL,	0x00001200, 0x00020000,  0,  0, "mcm_bypass_en",	"Bypass enable. 0: bypass disabled; 1: bypass enabled"},


	{"MCM_SIZE0",	"MCM dimension register 0",
			0x00001204, 0x00000000, 29, 16, "mcm_height0",		"Image height for write channel 0"},
	{NULL, NULL,	0x00001204, 0x00000000, 13,  0, "mcm_width0",		"Image width for write channel 0"},

	{"MCM_SIZE1",	"MCM dimension register 1",
			0x00001208, 0x00000000, 29, 16, "mcm_height1",		"Image height for write channel 1"},
	{NULL, NULL,	0x00001208, 0x00000000, 13,  0, "mcm_width1",		"Image width for write channel 1"},


	{"ISP_MCM_RD_CFG",	"MCM read format register",
			0x00001280, 0x00000000, 13,  0, "mcm_rd_fmt",		"input format for MCM read channel: " LINE_PADDING_6TAB
										"  000:8bit, 001:10bit, 010:12bit, 011:14bit, 100:16bit, 101:20bit"},

	{"MCM_RETIMING0",	"MCM retiming control register 0",
			0x00001284, 0x00000000, 31,  8, "mcm_vsync_blank",	"Minimum count of clock from previous frame end to the vsync of the next frame"},
	{NULL, NULL,	0x00001284, 0x00000000,  7,  0, "mcm_vsync_duration",	"Vsync active duration for retiming unit which indicate counts of clock"},

	{"MCM_RETIMING1",	"MCM retiming control register 1",
			0x00001288, 0x00000000, 31,  8, "mcm_vsync_blank",	"Minimum count of clock from previous frame end to the vsync of the next frame"},
	{NULL, NULL,	0x00001288, 0x00000000,  7,  0, "mcm_vsync_duration",	"Vsync active duration for retiming unit which indicate counts of clock"},

	{"MCM Unknown",	"What",	0x0000128C, 0x00000000, 31,  0, "unknown",	"unknown"},
};


int light_isp_mcm_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_mcm_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_mcm_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

