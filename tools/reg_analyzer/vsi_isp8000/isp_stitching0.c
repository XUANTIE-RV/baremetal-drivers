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

#define ISP_STITCHING0_BASE 0x3300
#define REG_BASE ISP_STITCHING0_BASE


static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name, reg_desc,         offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_STITCHING0_CTRL", "HDR Stitch Control Register",
			0x00003300, 0x00000000, 26, 26, "share_en",		"Enable/Disable share of sram.  0: Disable share sram;  1: Enable share sram"},
	{NULL, NULL,	0x00003300, 0x00000000, 25, 25, "digital_gain_en_2",	"Enable/Disable dgain of very short frame. 0: Disable the dgain; 1: Enable the dgain"},
	{NULL, NULL,	0x00003300, 0x00000000, 24, 24, "digital_gain_en_1",	"Enable/Disable dgain of short frame.  0: Disable the dgain; 1: Enable the dgain"},
	{NULL, NULL,	0x00003300, 0x00000000, 23, 23, "digital_gain_en_0",	"Enable/Disable dgain of long frame.  0: Disable the dgain; 1: Enable the dgain"},
	{NULL, NULL,	0x00003300, 0x00000000, 22, 22, "regs_inform_en",	"Enable/Disable inform module.  0: Disable the inform; 1: Enable the inform"},
	{NULL, NULL,	0x00003300, 0x00000000, 21, 21, "vsync_pol",		"VSYNC polarity reversal"},
	{NULL, NULL,	0x00003300, 0x00000000, 20, 20, "hsync_pol",		"HSYNC polarity reversal"},
	{NULL, NULL,	0x00003300, 0x00000000, 19, 19, "awb_gain_enable",	"Enable/Disable AWB Gain module. 0: Disable the AWB gain; 1: Enable the AWB gain"},
	{NULL, NULL,	0x00003300, 0x00000000, 18, 18, "cfg_upd",		"Configuration update"},
	{NULL, NULL,	0x00003300, 0x00000000, 17, 17, "gen_cfg_upd",		"Generate configuration update"},
	{NULL, NULL,	0x00003300, 0x00000000, 16, 16, "gen_cfg_upd_fix",	"Auto configuration update in the end of frame"},
	{NULL, NULL,	0x00003300, 0x00000000, 14, 13, "bypass_select",	"Bypass frame select.  0: long frame; 1: short frame; 2: very short frame"},
	{NULL, NULL,	0x00003300, 0x00000000, 12, 12, "linear_combine_enable","linear combination enable"},
	{NULL, NULL,	0x00003300, 0x00000000, 11, 11, "base_frame_selection",	"base frame select"},
	{NULL, NULL,	0x00003300, 0x00000000, 10,  9, "combination_mode",	"combination_mode"},
	{NULL, NULL,	0x00003300, 0x00000000,  8,  8, "channel_config_bit",	"channel_config_bit"},
	{NULL, NULL,	0x00003300, 0x00000000,  7,  7, "b10_enable_bit",	"10 bit enable"},
	{NULL, NULL,	0x00003300, 0x00000000,  6,  6, "lin_enable_bit",	"reserved"},
	{NULL, NULL,	0x00003300, 0x00000000,  5,  5, "vs_enable_bit",	"very short frame enable"},
	{NULL, NULL,	0x00003300, 0x00000000,  4,  3, "bayer_pattern",	"bayer pattern. 00: RGGB; 01: GRBG; 10: GBRB; 11: BGGR"},
	{NULL, NULL,	0x00003300, 0x00000000,  2,  2, "soft_reset_flag",	"Activate/deactivate Stitch software reset.  0: software reset un-active; 1: software reset active"},
	{NULL, NULL,	0x00003300, 0x00000000,  1,  1, "mono_input_flag",	"reserved"},
	{NULL, NULL,	0x00003300, 0x00000000,  0,  0, "combine_enable_bit",	"Enable/Disable Stitch module. 0: Disable the Stitch module; 1: Enable the Stitch module"},


	{"ISP_STITCHING0_FRAME_WIDTH", "Stitching frame width",		0x00003304, 0x00000000, 13,  0, "stitching_frame_width",	"Frame width"},
	{"ISP_STITCHING0_FRAME_HEIGHT","Stitching frame height",	0x00003308, 0x00000000, 13,  0, "stitching_frame_height",	"Frame height"},

	{"ISP_STITCHING0_EXPOSURE_BIT", "Stitching frame data bit depth",
			0x0000330C, 0x05050505, 31, 24, "stitching_l_bit_depth", "Exposure bit for LS; valid range: [0..255]"},
	{NULL, NULL,	0x0000330C, 0x05050505, 23, 16, "stitching_s_bit_depth", "Exposure bit for VS; valid range: [0..255]"},
	{NULL, NULL,	0x0000330C, 0x05050505, 15,  8, "stitching_vs_bit_depth", "Exposure bit for S; valid range: [0..255]"},
	{NULL, NULL,	0x0000330C, 0x05050505,  7,  0, "stitching_ls_bit_depth", "Exposure bit for L; valid range: [0..255]"},

	{"ISP_STITCHING0_COLOR_WEIGHT", "Channel color weight for gray computation",
			0x00003310, 0x001D464D, 23, 16, "stitching_color_weight_2", "B color weight for Y conversion. Valid range: [0..255]"},
	{NULL, NULL,	0x00003310, 0x001D464D, 15,  8, "stitching_color_weight_1", "G color weight for Y conversion. Valid range: [0..255]"},
	{NULL, NULL,	0x00003310, 0x001D464D,  7,  0, "stitching_color_weight_0", "R color weight for Y conversion.. Valid range: [0..255]"},

	{"ISP_STITCHING0_BLS_EXP_0_A", "Black level for exposure 0 channel A",
			0x00003314, 0x01000000, 31, 16, "stitching_digital_gain_exp_0_r", "Digital gain value for exposure 0 red pixel"},
	{NULL, NULL,	0x00003314, 0x01000000, 11,  0, "stitching_bls_exp_0_a", "Black level value for exposure 0 channel A"},


	{"ISP_STITCHING0_BLS_EXP_0_B", "Black level for exposure 0 channel B",
			0x00003318, 0x01000000, 31, 16, "stitching_digital_gain_exp_0_g", "Digital gain value for exposure 0 blue pixel"},
	{NULL, NULL,	0x00003318, 0x01000000, 11,  0, "stitching_bls_exp_0_b", "Black level value for exposure 0 channel B"},



	{"ISP_STITCHING0_RATIO_LS", "Exposure ratio between Long and Short",
			0x00003344, 0x00400400, 23, 12, "stitching_ratio_long_short_1", "Exposure merge ratio from S to L (greater than 1)"},
	{NULL, NULL,	0x00003344, 0x00400400, 11,  0, "stitching_ratio_long_short_0", "Exposure merge ratio from S to L (less than 1)"},


	{"ISP_STITCHING0_LONG_EXPOSURE", "Long exposure time",			0x00003360, 0x00000040, 11,  0, "stitching_long_exposure_time", "Exposure time for long exposure frame"},
	{"ISP_STITCHING0_SHORT_EXPOSURE", "Short exposure time",		0x00003364, 0x00000008, 11,  0, "stitching_short_exposure_time", "Exposure time for short exposure frame"},
	{"ISP_STITCHING0_VERY_SHORT_EXPOSURE", "Very short exposure time",	0x00003368, 0x00000008, 11,  0, "stitching_very_short_exposure_time", "Exposure time for very short exposure frame"},
	{"ISP_STITCHING0_HDR_MODE", "HDR combination mode",			0x0000336C, 0x00000000,  3,  0, "stitching_hdr_input_format_type", "Input Sensor HDR mode:" LINE_PADDING_6TAB
														"  0: dual DCG mode 3x12-bit (default)" LINE_PADDING_6TAB
														"  1: DOL 3 Frame 3x12-bit" LINE_PADDING_6TAB
														"  2: 3x12-bit line by line without waiting" LINE_PADDING_6TAB
														"  3: 16-bit compressed data + 12-bit RAW" LINE_PADDING_6TAB
														"  4: 2x12-bit dual DCG without waiting" LINE_PADDING_6TAB
														"  5: DOL2 frame or 1 CG+VS sx12-bit RAW" LINE_PADDING_6TAB
														"  6: L+S 2x12-bit RAW"},

	{"ISP_STITCHING0_OUT_HBLANK", "HDR blanking in horizontal direction",
			0x00003370, 0x00100080, 31, 16, "stitching_dummy_s_hblank", "Dummy line horizontal blank for short exposure"},
	{NULL, NULL,	0x00003370, 0x00100080, 15,  0, "stitching_out_hblank", "Output data horizontal blank time"},


	{"ISP_STITCHING0_OUT_VBLANK", "HDR blanking in vertical direction",
			0x00003374, 0x00100020, 31, 16, "stitching_dummy_vs_hblank", "Dummy line horizontal blank for very short exposure"},
	{NULL, NULL,	0x00003374, 0x00100020, 15,  0, "stitching_out_vblank", "Output data vertical blank time"},


	{"ISP_STITCHING0_INTERRUPT_STATUS", "Stitching interrupt status",	0x00003378, 0x00000000, 2,  0, "stitching_interrupt_status", "  Unknown yet"},
	{"ISP_STITCHING0_COMPRESS_X0", "Compression LUT x index",		0x0000337C, 0x00000000, 9,  0, "compress_lut_first_valid_x", "  Compression LUT first valid x index"},
	{"ISP_STITCHING0_COMPRESS_X0_SHD", "Compression LUT x index shadow",	0x00003380, 0x00000000, 9,  0, "compress_lut_first_valid_shd_x", "  Compression LUT first valid x index shadow"},


	{"ISP_STITCHING0_COMPRESS_LUT_0", "Compression LUT data 0",
			0x000033A0, 0x00000000, 29, 20, "stitching_compress_lut_2", "VS compression LUT2"},
	{NULL, NULL,	0x000033A0, 0x00000000, 19, 10, "stitching_compress_lut_1", "VS compression LUT1"},
	{NULL, NULL,	0x000033A0, 0x00000000,  9,  0, "stitching_compress_lut_0", "VS compression LUT0"},

	{"ISP_STITCHING0_COMPRESS_LUT_1", "Compression LUT data 1",
			0x000033A4, 0x00000000, 29, 20, "stitching_compress_lut_5", "VS compression LUT5"},
	{NULL, NULL,	0x000033A4, 0x00000000, 19, 10, "stitching_compress_lut_4", "VS compression LUT4"},
	{NULL,  NULL,	0x000033A4, 0x00000000,  9,  0, "stitching_compress_lut_3", "VS compression LUT3"},

	{"ISP_STITCHING0_COMPRESS_LUT_4", "Compression LUT data 4",
			0x000033B0, 0x00000000, 29, 20, "stitching_compress_lut_14", "VS compression LUT14"},
	{NULL, NULL,	0x000033B0, 0x00000000, 19, 10, "stitching_compress_lut_13", "VS compression LUT13"},
	{NULL, NULL,	0x000033B0, 0x00000000,  9,  0, "stitching_compress_lut_12", "VS compression LUT12"},

	{"ISP_STITCHING0_COMPRESS_LUT_SHD_0", "Compression LUT data 0 shadow",
			0x000033B4, 0x00000000, 29, 20, "stitching_compress_lut_shd_2", "Shadow of VS compression LUT2"},
	{NULL, NULL,	0x000033B4, 0x00000000, 19, 10, "stitching_compress_lut_shd_1", "Shadow of VS compression LUT1"},
	{NULL, NULL,	0x000033B4, 0x00000000,  9,  0, "stitching_compress_lut_shd_0", "Shadow of VS compression LUT0"},

	{"ISP_STITCHING0_EXP0_AWB_GAIN_G", "AWB gain for exposure 0 channel G",
			0x000033C8, 0x01000100, 25, 16, "stitching_exp0_awb_gain_gr", "Stitching AWB GR gain for exposure 0"},
	{NULL, NULL,	0x000033C8, 0x01000100,  9,  0, "stitching_exp0_awb_gain_gb", "Stitching AWB GB gain for exposure 0"},

	{"ISP_STITCHING0_EXP0_AWB_GAIN_RB", "AWB gain for exposure 0 channel RB",
			0x000033CC, 0x01000100, 25, 16, "stitching_exp0_awb_gain_r", "Stitching AWB R gain for exposure 0"},
	{NULL, NULL,	0x000033CC, 0x01000100,  9,  0, "stitching_exp0_awb_gain_b", "Stitching AWB B gain for exposure 0"},


	{"ISP_STITCHING0_EXP1_AWB_GAIN_G", "AWB gain for exposure 1 channel G",
			0x000033D0, 0x01000100, 25, 16, "stitching_exp1_awb_gain_gr", "Stitching AWB GR gain for exposure 1"},
	{NULL, NULL,	0x000033D0, 0x01000100,  9,  0, "stitching_exp1_awb_gain_gb", "Stitching AWB GB gain for exposure 1"},


	{"ISP_STITCHING0_EXP1_AWB_GAIN_RB", "AWB gain for exposure 1 channel RB",
			0x000033D4, 0x01000100, 25, 16, "stitching_exp1_awb_gain_r", "Stitching AWB R gain for exposure 1"},
	{NULL, NULL,	0x000033D4, 0x01000100,  9,  0, "stitching_exp1_awb_gain_b", "Stitching AWB B gain for exposure 1"},


	{"ISP_STITCHING0_LONG_SAT_PARAMS", "Stitching saturation params for long exposure",
			0x000033E0, 0x007FF000, 23, 12, "stitching_long_sat_thresh", "Stitching long saturation threshold"},
	{NULL, NULL,	0x000033E0, 0x007FF000,  8,  0, "stitching_long_sat_combine_weight", "Stitching long combine weight"},

	{"ISP_STITCHING0_EXP2_AWB_GAIN_G", "AWB gain for exposure 2 channel G",
			0x000033D8, 0x01000100, 25, 16, "stitching_exp2_awb_gain_gr", "Stitching AWB GR gain for exposure 2"},
	{NULL, NULL,	0x000033D8, 0x01000100,  9,  0, "stitching_exp2_awb_gain_gb", "Stitching AWB GB gain for exposure 2"},

	{"ISP_STITCHING0_EXP2_AWB_GAIN_RB", "AWB gain for exposure 2 channel RB",
			0x000033DC, 0x01000100, 25, 16, "stitching_exp2_awb_gain_r", "Stitching AWB R gain for exposure 2"},
	{NULL, NULL,	0x000033DC, 0x01000100,  9,  0, "stitching_exp2_awb_gain_b", "Stitching AWB B gain for exposure 2"},

	{"ISP_STITCHING0_IMSC", "IMSC Interrupt Mask",
			0x000033E4, 0x00000000,  8,  8, "stitching_imsc_hdr_hist_irq_vs", "Stitching HDR hist VS interrupt mask"},
	{NULL, NULL,	0x000033E4, 0x00000000,  7,  7, "stitching_imsc_hdr_hist_irq_s", "Stitching HDR hist S interrupt mask"},
	{NULL,	NULL,	0x000033E4, 0x00000000,  6,  6, "stitching_imsc_hdr_hist_irq_l", "Stitching HDR hist L interrupt mask"},
	{NULL,	NULL,	0x000033E4, 0x00000000,  5,  5, "stitching_imsc_hdr_exp_complete_vs", "Stitching HDR exp statistic VS interrupt mask"},
	{NULL,	NULL,	0x000033E4, 0x00000000,  4,  4, "stitching_imsc_hdr_exp_complete_s", "Stitching HDR exp statistic S interrupt mask"},
	{NULL,	NULL,	0x000033E4, 0x00000000,  3,  3, "stitching_imsc_hdr_exp_complete_l", "Stitching HDR exp statistic L interrupt mask"},
	{NULL,	NULL,	0x000033E4, 0x00000000,  2,  2, "stitching_imsc_fifo_empty", "Stitching HDR FIFO empty L interrupt mask"},
	{NULL,	NULL,	0x000033E4, 0x00000000,  1,  1, "stitching_imsc_exp_err2", "Stitching HDR very short exp time error L interrupt mask"},
	{NULL,	NULL,	0x000033E4, 0x00000000,  0,  0, "stitching_imsc_exp_err1", "Stitching HDR short exp time error L interrupt mask"},


	{"ISP_STITCHING0_RIS", "RIS Raw Interrupt Status",
			0x000033E8, 0x00000000,  8,  8, "stitching_ris_hdr_hist_irq_vs", "Stitching HDR hist VS interrupt status"},
	{NULL, NULL,	0x000033E8, 0x00000000,  7,  7, "stitching_ris_hdr_hist_irq_s", "Stitching HDR hist S interrupt status"},
	{NULL, NULL,	0x000033E8, 0x00000000,  6,  6, "stitching_ris_hdr_hist_irq_l", "Stitching HDR hist L interrupt status"},
	{NULL, NULL,	0x000033E8, 0x00000000,  5,  5, "stitching_ris_hdr_exp_complete_vs", "Stitching HDR exp statistic VS interrupt status"},
	{NULL, NULL,	0x000033E8, 0x00000000,  4,  4, "stitching_ris_hdr_exp_complete_s", "Stitching HDR exp statistic S interrupt status"},
	{NULL, NULL,	0x000033E8, 0x00000000,  3,  3, "stitching_ris_hdr_exp_complete_l", "Stitching HDR exp statistic L interrupt status"},
	{NULL, NULL,	0x000033E8, 0x00000000,  2,  2, "stitching_ris_fifo_empty", "Stitching HDR FIFO empty L interrupt status"},
	{NULL, NULL,	0x000033E8, 0x00000000,  1,  1, "stitching_ris_exp_err2", "Stitching HDR very short exp time error L interrupt status"},
	{NULL, NULL,	0x000033E8, 0x00000000,  0,  0, "stitching_ris_exp_err1", "Stitching HDR short exp time error L interrupt status"},


	{"ISP_STITCHING0_MIS", "MIS Masked Interrupt Status",
			0x000033EC, 0x00000000,  8,  8, "stitching_mis_hdr_hist_irq_vs", "Stitching HDR hist VS interrupt masked status"},
	{NULL, NULL,	0x000033EC, 0x00000000,  7,  7, "stitching_mis_hdr_hist_irq_s", "Stitching HDR hist S interrupt masked status"},
	{NULL, NULL,	0x000033EC, 0x00000000,  6,  6, "stitching_mis_hdr_hist_irq_l", "Stitching HDR hist L interrupt masked status"},
	{NULL, NULL,	0x000033EC, 0x00000000,  5,  5, "stitching_mis_hdr_exp_complete_vs", "Stitching HDR exp statistic VS interrupt masked status"},
	{NULL, NULL,	0x000033EC, 0x00000000,  4,  4, "stitching_mis_hdr_exp_complete_s", "Stitching HDR exp statistic S interrupt masked status"},
	{NULL, NULL,	0x000033EC, 0x00000000,  3,  3, "stitching_mis_hdr_exp_complete_l", "Stitching HDR exp statistic L interrupt masked status"},
	{NULL, NULL,	0x000033EC, 0x00000000,  2,  2, "stitching_mis_fifo_empty", "Stitching HDR FIFO empty L interrupt masked status"},
	{NULL, NULL,	0x000033EC, 0x00000000,  1,  1, "stitching_mis_exp_err2", "Stitching HDR very short exp time error\n\t L interrupt masked status"},
	{NULL, NULL,	0x000033EC, 0x00000000,  0,  0, "stitching_mis_exp_err1", "Stitching HDR short exp time error L interrupt \n\tMasked status"},


	{"ISP_STITCHING0_ICR", "ICR Interrupt Clear Register",
			0x000033F0, 0x00000000,  8,  8, "stitching_icr_hdr_hist_irq_vs", "Stitching HDR hist VS interrupt clear"},
	{NULL, NULL,	0x000033F0, 0x00000000,  7,  7, "stitching_icr_hdr_hist_irq_s", "Stitching HDR hist S interrupt clear"},
	{NULL, NULL,	0x000033F0, 0x00000000,  6,  6, "stitching_icr_hdr_hist_irq_l", "Stitching HDR hist L interrupt clear"},
	{NULL, NULL,	0x000033F0, 0x00000000,  5,  5, "stitching_icr_hdr_exp_complete_vs", "Stitching HDR exp statistic VS interrupt clear"},
	{NULL, NULL,	0x000033F0, 0x00000000,  4,  4, "stitching_icr_hdr_exp_complete_s", "Stitching HDR exp statistic S interrupt clear"},
	{NULL, NULL,	0x000033F0, 0x00000000,  3,  3, "stitching_icr_hdr_exp_complete_l", "Stitching HDR exp statistic L interrupt clear"},
	{NULL, NULL,	0x000033F0, 0x00000000,  2,  2, "stitching_icr_fifo_empty", "Stitching HDR FIFO empty L interrupt clear"},
	{NULL, NULL,	0x000033F0, 0x00000000,  1,  1, "stitching_icr_exp_err2", "Stitching HDR very short exp time error L interrupt clear"},
	{NULL, NULL,	0x000033F0, 0x00000000,  0,  0, "stitching_icr_exp_err1", "Stitching HDR short exp time error L interrupt clear"},


	{"ISP_STITCHING0_ISR", "ISR Interrupt Set Register",
			0x000033F4, 0x00000000,  8,  8, "stitching_isr_hdr_hist_irq_vs", "Stitching HDR hist VS interrupt set"},
	{NULL, NULL,	0x000033F4, 0x00000000,  7,  7, "stitching_isr_hdr_hist_irq_s", "Stitching HDR hist S interrupt set"},
	{NULL, NULL,	0x000033F4, 0x00000000,  6,  6, "stitching_isr_hdr_hist_irq_l", "Stitching HDR hist L interrupt set"},
	{NULL, NULL,	0x000033F4, 0x00000000,  5,  5, "stitching_isr_hdr_exp_complete_vs", "Stitching HDR exp statistic VS interrupt set"},
	{NULL, NULL,	0x000033F4, 0x00000000,  4,  4, "stitching_isr_hdr_exp_complete_s", "Stitching HDR exp statistic S interrupt set"},
	{NULL, NULL,	0x000033F4, 0x00000000,  3,  3, "stitching_isr_hdr_exp_complete_l", "Stitching HDR exp statistic L interrupt set"},
	{NULL, NULL,	0x000033F4, 0x00000000,  2,  2, "stitching_isr_fifo_empty", "Stitching HDR FIFO empty L interrupt set"},
	{NULL, NULL,	0x000033F4, 0x00000000,  1,  1, "stitching_isr_exp_err2", "Stitching HDR very short exp time error L interrupt set"},
	{NULL, NULL,	0x000033F4, 0x00000000,  0,  0, "stitching_isr_exp_err1", "Stitching HDR short exp time error L interrupt set"},
};


int light_isp_sitching0_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_sitching0_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_sitching0_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

