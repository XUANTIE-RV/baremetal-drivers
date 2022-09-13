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

#define REG_BASE 0x1300
static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"MI_CTRL", "Global control register",
			0x00001300, 0x00000000, 24, 24, "sp2_raw2_continuous",		"Enables continuous mode. If set to 1, the same frame is read back over and over."},
	{NULL, NULL,	0x00001300, 0x00000000, 23, 23, "sp2_raw2_start", 		"Enables DMA access."},
	{NULL, NULL,	0x00001300, 0x00000000, 22, 22, "pp_dma_continuous", 		"Enables continuous mode. If set to 1, the same frame is read back over and over."},
	{NULL, NULL,	0x00001300, 0x00000000, 21, 21, "pp_dma_start",			"Enables DMA access."},
	{NULL, NULL,	0x00001300, 0x00000000, 20, 20, "sp2_raw2_write_path_enable",	"Enables sp2_raw2_write path."},
	{NULL, NULL,	0x00001300, 0x00000000, 19, 19, "pp_write_path_enable", 	"Enables pp write path."},
	{NULL, NULL,	0x00001300, 0x00000000, 18, 18, "mcm_g2_raw1_path_enable", 	"Enables mcm_g2_raw1 path."},
	{NULL, NULL,	0x00001300, 0x00000000, 17, 17, "mcm_g2_raw0_path_enable", 	"Enables mcm_g2_raw0 path."},
	{NULL, NULL,	0x00001300, 0x00000000, 16, 16, "mcm_raw_rdma_start_con",	"Enables continuous mode. If set to 1, ... [Refer to Doc]."},
	{NULL, NULL,	0x00001300, 0x00000000, 15, 15, "mcm_raw_rdma_start", 		"Enables DMA access."},
	{NULL, NULL,	0x00001300, 0x00000000, 14, 14, "mcm_raw_rdma_path_enable",	"Enables mcm_raw_rdma path."},
	{NULL, NULL,	0x00001300, 0x00000000, 13, 13, "sp2_raw_rdma_start_con",	"Enables continuous mode. If set to 1, the same frame is read back over and over.."},
	{NULL, NULL,	0x00001300, 0x00000000, 12, 12, "sp2_raw_rdma_start",		"Enables DMA access."},
	{NULL, NULL,	0x00001300, 0x00000000, 11, 11, "sp2_raw_rdma_path_enable",	"Enables sp2_raw_rdma path."},
	{NULL, NULL,	0x00001300, 0x00000000, 10, 10, "sp2_ycbcr_rdma_start_con",	"Enables continuous mode. If set to 1, the same frame is read back over and over.."},
	{NULL, NULL,	0x00001300, 0x00000000,  9,  9, "sp2_ycbcr_rdma_start",		"Enables DMA access."},
	{NULL, NULL,	0x00001300, 0x00000000,  8,  8, "sp2_ycbcr_rdma_path_enable",	"Enables sp2_ycbcr_rdma path."},
	{NULL, NULL,	0x00001300, 0x00000000,  7,  7, "mcm_raw1_path_enable",		"Enables mcm_raw1 path."},
	{NULL, NULL,	0x00001300, 0x00000000,  6,  6, "mcm_raw0_path_enable",		"Enables mcm_raw0 path."},
	{NULL, NULL,	0x00001300, 0x00000000,  5,  5, "sp2_raw_path_enable",		"Enables sp2_raw path."},
	{NULL, NULL,	0x00001300, 0x00000000,  4,  4, "sp2_ycbcr_path_enable",	"Enables sp2_ycbcr path."},
	{NULL, NULL,	0x00001300, 0x00000000,  3,  3, "sp1_ycbcr_path_enable",	"Enables sp1_ycbcr path."},
	{NULL, NULL,	0x00001300, 0x00000000,  2,  2, "mp_jdp_path_enable",		"Enables mp_jdp path."},
	{NULL, NULL,	0x00001300, 0x00000000,  1,  1, "mp_raw_path_enable",		"Enables mp_raw path."},
	{NULL, NULL,	0x00001300, 0x00000000,  0,  0, "mp_ycbcr_path_enable",		"Enables mp_ycbcr path."},

	{"MI_CTRL_SHD",  "Global control internal shadow register",
			0x00001304, 0x00000000, 20, 20, "sp2_raw2_write_path_enable",	"Enables sp2_raw2_write path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000, 19, 19, "pp_write_path_enable",		"Enables pp write path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000, 18, 18, "mcm_g2_raw1_path_enable",	"Enables mcm_raw1 path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000, 17, 17, "mcm_g2_raw0_path_enable",	"Enables mcm_raw0 path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000, 16, 16, "mcm_raw_rdma_start_con",	"Unused"},
	{NULL,	NULL,  0x00001304, 0x00000000, 15, 15, "mcm_raw_rdma_start",		"Unused"},
	{NULL,	NULL,  0x00001304, 0x00000000, 14, 14, "mcm_raw_rdma_path_enable",	"Unused"},
	{NULL,	NULL,  0x00001304, 0x00000000, 13, 13, "sp2_raw_rdma_start_con",	"Unused"},
	{NULL,	NULL,  0x00001304, 0x00000000, 12, 12, "sp2_raw_rdma_start",		"Unused"},
	{NULL,	NULL,  0x00001304, 0x00000000, 11, 11, "sp2_raw_rdma_path_enable",	"Unused"},
	{NULL,	NULL,  0x00001304, 0x00000000, 10, 10, "sp2_ycbcr_rdma_start_con",	"Unused"},
	{NULL,	NULL,  0x00001304, 0x00000000,	9,  9, "mcm_raw1_path_enable",		"Enables mcm_raw1 path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000,	8,  8, "mcm_raw0_path_enable",		"Enables mcm_raw0 path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000,	5,  5, "sp2_raw_path_enable",		"Enables sp2_raw path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000,	4,  4, "sp2_ycbcr_path_enable",		"Enables sp2_ycbcr path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000,	3,  3, "sp1_ycbcr_path_enable",		"Enables sp1_ycbcr path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000,	2,  2, "mp_jdp_path_enable",		"Enables mp_jdp path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000,	1,  1, "mp_raw_path_enable",		"Enables mp_raw path. 0: Disabled; 1: enabled"},
	{NULL,	NULL,  0x00001304, 0x00000000,	0,  0, "mp_ycbcr_path_enable",		"Enables mp_ycbcr path. 0: Disabled; 1: enabled"},



	{"MI_QOS",  "AXI Bus QOS value configuration",
			0x00001308, 0x00000000, 31, 28, "mcm_qos_rd", "QoS value of AXI bus for MCM read"},
	{NULL, NULL,	0x00001308, 0x00000000, 27, 24, "mcm_qos_wr", "QoS value of AXI bus for MCM write"},
	{NULL, NULL,	0x00001308, 0x00000000, 23, 20, "sp2_qos_rd", "QoS value of AXI bus bus for sp2 read"},
	{NULL, NULL,	0x00001308, 0x00000000, 19, 16, "sp2_qos_wr", "QoS value of AXI bus bus for sp2 write"},
	{NULL, NULL,	0x00001308, 0x00000000, 11,  8, "sp1_qos_wr", "QoS value of AXI bus bus for sp1 write"},
	{NULL, NULL,	0x00001308, 0x00000000,  3,  0, "mp_qos_wr", "QoS value of AXI bus bus for mp write"},


	{"MI_QOS2",  "AXI Bus QOS value configuration",
			0x0000130C, 0x00000000,  7,  4, "rt_qos_rd", "QoS value of AXI bus for RT read"},
	{NULL, NULL,	0x0000130C, 0x00000000,  3,  0, "rt_qos_wr", "QoS value of AXI bus for RT write"},


	{"MI_MP_CTRL", "MP control register",
			0x00001310, 0x00000000,  5,  5, "mp_init_offset_en",	"Enables updating of the offset counters shadow registers for" LINE_PADDING_6TAB
										"main path to the programmed register init values." LINE_PADDING_6TAB
										"MI_MP_Y/CB/CR_OFFS_CNT_INIT-> MI_MP_Y/CB/CR_OFFS_CNT_SHD" LINE_PADDING_6TAB
										"The update will be executed either when a forced software " LINE_PADDING_6TAB
										"update occurs (in register MI_MP_CTRL bit cfg_upd = 1) or " LINE_PADDING_6TAB
										"when an automatic configuration update signal arrives at the MI input port. " LINE_PADDING_6TAB
										"The latter is split into main and self picture, so only " LINE_PADDING_6TAB
										"the corresponding main/self shadow registers are affected."},
	{NULL, NULL,	0x00001310, 0x00000000,  4,  4, "mp_init_base_en",	"Enables updating of the base address and buffer size shadow " LINE_PADDING_6TAB
										"registers for main picture to the programmed register init values." LINE_PADDING_6TAB
										"MI_MP_Y/CB/CR_BASE_AD_INIT-> MI_MP_Y/CB/CR_OFFS_CNT_SHD" LINE_PADDING_6TAB
										"MI_MP_Y/CB/CR_SIZE_INIT-> MI_MP_Y/CB/CR_SIZE_SHD" LINE_PADDING_6TAB
										"The update will be executed either when a forced software " LINE_PADDING_6TAB
										"update occurs (in register MI_MP_CTRL bit cfg_upd = 1) or " LINE_PADDING_6TAB
										"when an automatic config update signal arrives at the MI input port. " LINE_PADDING_6TAB
										"The latter is split into main and self picture. So only the " LINE_PADDING_6TAB
										"corresponding main/self shadow registers are affected. " LINE_PADDING_6TAB
										"After a picture skip has been performed init_offset_en " LINE_PADDING_6TAB
										"selects between skip restart and skip init mode " LINE_PADDING_6TAB
										"(see bit skip in register MI_MP_CTRL)."},
	{NULL, NULL,	0x00001310, 0x00000000,  3,  3, "mp_mi_cfg_upd",	"Forced configuration update. Leads to an immediate update of the shadow " LINE_PADDING_6TAB
										"registers. Depending on the two init enable bits in the MI_MP_CTRL register" LINE_PADDING_6TAB
										"(init_offset_en and init_base_en) the offset counter, base address and" LINE_PADDING_6TAB
										"buffer size shadow registers are also updated."},
	{NULL, NULL,	0x00001310, 0x00000000,  2,  2, "mp_mi_skip",		"Skip of current or next starting main picture:" LINE_PADDING_6TAB
										"Aborts writing of main picture image data of the current frame to RAM" LINE_PADDING_6TAB
										"  (after the current burst transmission has been completed). " LINE_PADDING_6TAB
										"  Further main picture data up to the end of the current frame are discarded." LINE_PADDING_6TAB
										"No further macroblock line interrupt (mblk_line), no wrap around interrupt " LINE_PADDING_6TAB
										"  for main picture (wrap_mp_y/cb/cr) and no fill level interrupt (fill_mp_y) are generated." LINE_PADDING_6TAB
										"  Skip does not affect the generation of the main path frame end interrupt (mp_frame_end)." LINE_PADDING_6TAB
										"Skip does not affect the processing of self picture and its corresponding interrupts " LINE_PADDING_6TAB
										"  namely the self path frame end interrupt (sp_frame_end)." LINE_PADDING_6TAB
										"The byte counter (register MI_BYTE_CNT) is not affected. " LINE_PADDING_6TAB
										"  It produces the correct number of JPEG or RAW data bytes " LINE_PADDING_6TAB
										"  at the end of the current (skipped) frame." LINE_PADDING_6TAB
										"After a skip has been performed, the offset counter for the main picture " LINE_PADDING_6TAB
										"  at the start of the following frame are set depending on the bit " LINE_PADDING_6TAB
										"init_offset_en in register MI_MP_CTRL: " LINE_PADDING_6TAB
										"- Skip restart mode (init_offset_en = 0)" LINE_PADDING_6TAB
										"  The offset counters of the main picture are restarted at the old start values of " LINE_PADDING_6TAB
										"  the previous skipped frame." LINE_PADDING_6TAB
										"- Skip init mode (init_offset_en = 1)" LINE_PADDING_6TAB
										"  The offset counters of the main picture are initialized with the register " LINE_PADDING_6TAB
										"  contents of the offset counter init registers without any additional forced" LINE_PADDING_6TAB
										"  software update or automatic config update."},
	{NULL, NULL,	0x00001310, 0x00000000,  1,  1, "mp_auto_update",	"Automatic update of configuration registers for main path at frame end."},
	{NULL, NULL,	0x00001310, 0x00000000,  0,  0, "mp_pingpong_enable",	"Pingpong mode of configuration registers for main path at frame end."},


	{"MI_MP_FMT", "MI main path format",
			0x00001314, 0x00000000, 17, 17, "mp_wr_jdp_dp_bit",		"0: 32bit; 1: 64bit;"},
	{NULL, NULL,	0x00001314, 0x00000000, 16, 16, "mp_wr_yuv_word_aligned_msb_en","0: 10bit yuv in LSB of 16bit; 1: 10bit yuv in MSB of 16bit;"},
	{NULL, NULL,	0x00001314, 0x00000000, 15, 15, "mp_wr_yuv_word_aligned",	"0: disable; 1:enable"},
	{NULL, NULL,	0x00001314, 0x00000000, 14, 13, "mp_wr_yuv_nvy",		"00: Put Y first => YC1C2; 01: Put Y second => C1YC2; 10: Put Y Third => C1C2Y"},
	{NULL, NULL,	0x00001314, 0x00000000, 12, 12, "mp_wr_yuv_nv21",		"0: Put U before V; 1: Put V before U"},
	{NULL, NULL,	0x00001314, 0x00000000, 11, 10, "mp_wr_raw_aligned",		"00: unaligned; 01: aligned mode0; 10: aligned mode1; 11: Reserved"},
	{NULL, NULL,	0x00001314, 0x00000000,  9,  9, "mp_wr_yuv_aligned",		"0: unaligned; 1:aligned"},
	{NULL, NULL,	0x00001314, 0x00000000,  8,  6, "mp_wr_raw_bit",		"000: RAW8; 001: RAW10; 010: RAW12; 011: RAW14; 100: RAW16; 101: RAW20, or Reserved."},
	{NULL, NULL,	0x00001314, 0x00000000,  5,  4, "mp_wr_yuv_str",		"00: YUV/RGB SemiPlanar; 01: YUV Interleave, RGB Raster; 10: YUV/RGB Planar; 11: Y Only"},
	{NULL, NULL,	0x00001314, 0x00000000,  3,  2, "mp_wr_yuv_fmt",		"00: YUV420; 01: YUV422; 10: YUV444; 11: Reserved"},
	{NULL, NULL,	0x00001314, 0x00000000,  1,  1, "mp_wr_yuv_bit",		"0: 8 bit YUV/RGB; 1: 10 bit YUV/RGB"},
	{NULL, NULL,	0x00001314, 0x00000000,  0,  0, "mp_wr_jdp_fmt",		"Output format. 0: DPCC; 1: JPEG"},


	{"MI_MP_BUS_CFG", "MP AXI Bus configuration",
			0x00001318, 0x00000000, 27, 24, "mp_wr_swap_jdp",	"Same as mp_wr_swap_y"},
	{NULL, NULL,	0x00001318, 0x00000000, 23, 20, "mp_wr_swap_raw",	"Same as mp_wr_swap_y"},
	{NULL, NULL,	0x00001318, 0x00000000, 19, 16, "mp_wr_swap_v",		"Same as mp_wr_swap_y"},
	{NULL, NULL,	0x00001318, 0x00000000, 15, 12, "mp_wr_swap_u",		"Same as mp_wr_swap_y"},
	{NULL, NULL,	0x00001318, 0x00000000, 11,  8, "mp_wr_swap_y",		"Swap bits control for 16 byte data width on AXI bus;" LINE_PADDING_6TAB
										"YUV interleave or RGB raster mode share the same bits." LINE_PADDING_6TAB
										" Bit 0: to swap bytes" LINE_PADDING_6TAB
										" Bit 1: to swap words" LINE_PADDING_6TAB
										" Bit 2 to swap double words" LINE_PADDING_6TAB
										" Bit 3 to swap four words"},
	{NULL, NULL,	0x00001318, 0x00000000,  7,  6, "mp_rd_issue_cap",	"Unused"},
	{NULL, NULL,	0x00001318, 0x00000000,  5,  4, "mp_wr_issue_cap",	"Maximum outstanding issue capability. 00:4; 01:8; 10:16; 11:32"},
	{NULL, NULL,	0x00001318, 0x00000000,  3,  2, "mp_rd_burst_len",	"Unused"},
	{NULL, NULL,	0x00001318, 0x00000000,  1,  0, "mp_wr_burst_len",	"Maximum burst length. 00:4; 01:8; 10:16; 11:32"},


	{"MI_MP_BUS_ID", "MP AXI Bus ID configuration",
			0x0000131c, 0x00000000, 25, 25, "mp_bus_sw_en", "Control whether BUS works, and set to 1’b1 before ISP works"},
	{NULL, NULL,	0x0000131c, 0x00000000, 24, 24, "mp_rd_id_en", "Unused"},
	{NULL, NULL,	0x0000131c, 0x00000000, 23, 16, "mp_rd_id_cfg", "Unused"},
	{NULL, NULL,	0x0000131c, 0x00000000,  8,  8, "mp_wr_id_en", "MP AXI ports write ID configuration enable"},
	{NULL, NULL,	0x0000131c, 0x00000000,  7,  0, "mp_wr_id_cfg", "MP AXI ports write ID configuration"},

	{"MI_MP_BUS_TIMEO", "MP AXI Bus control register",
			0x00001320, 0x00000000, 31, 31, "mp_bus_timeo_en", "MP AXI bus time out waiting enable"},
	{NULL, NULL,	0x00001320, 0x00000000, 30,  0, "mp_bus_timeo", "MP AXI bus time out waiting clock cycle numbers"},


	{"MI_MP_Y_BASE_AD_INIT", "Base address for main picture Y component",
			0x00001324, 0x00000000, 31,  0, "mp_y_base_ad_init",	"Base address of main picture Y component ring buffer." LINE_PADDING_6TAB
										"Programmed value becomes effective (visible in corresponding shadow register)" LINE_PADDING_6TAB
										"after a soft reset, a forced software update or an automatic configuration update."},


	{"MI_MP_Y_SIZE_INIT", "Size of main picture Y component in frame buffer(ring buffer)",
			0x00001328, 0x00000000, 28,  0, "mp_y_size_init",	"Size of main picture Y component ring buffer." LINE_PADDING_6TAB
										"Programmed value becomes effective (visible in corresponding shadow register)" LINE_PADDING_6TAB
										"after a soft reset, a forced software update or an automatic configuration update."},

	{"MI_MP_Y_OFFS_CNT_INIT", "Offset counter init value for main picture Y",
			0x0000132c, 0x00000000, 28,  0, "mp_y_offs_cnt_init",	"Offset counter init value of main picture Y component ring buffer." LINE_PADDING_6TAB
										"Programmed value becomes effective (visible in corresponding shadow register)" LINE_PADDING_6TAB
										"after a soft reset, a forced software update or an automatic configuration update."},

	{"MI_MP_Y_LLENGTH", "Line length of main picture Y",
			0x00001330, 0x00000000, 31,  0, "mp_y_llengh",	"Line length of Main Path picture Y component or RGB picture in bytes, " LINE_PADDING_6TAB
									"also known as line stride. [Refer doc]"},

	{"MI_MP_Y_PIC_WIDTH", "Image width of the Y component in pixels for main path",
			0x00001334, 0x00000000, 31,	0, "mp_y_pic_width",	"Image width of the main picture Y component or RGB picture in pixels.[Refer doc]"},

	{"MI_MP_Y_PIC_HEIGHT", "Image height of the Y component in pixels for main path",
			0x00001338, 0x00000000, 31,  0, "mp_y_pic_height",	"Image height of the y component or RGB picture in pixels.[Refer doc]"},

	{"MI_MP_Y_PIC_SIZE", "Image memory size of the Y component in bytes for main path",
			0x0000133c, 0x00000000, 31,  0, "mp_y_pic_size",	"Image size of the Y component or RGB picture in bytes.[Refer doc]"},

	{"MI_MP_CB_BASE_AD_INIT", "Base address for main picture Cb component ring buffer",
			0x00001340, 0x00000000, 31,  0, "mp_cb_base_ad_init",	"Base address of main picture Cb component ring buffer.[Refer doc]"},


	{"MI_MP_CB_SIZE_INIT", "Size of main picture Cb component ring buffer",
		0x00001344, 0x00000000, 27,	0, "mp_cb_size_init", "Size of main picture Cb component ring buffer.[Refer doc]"},


	{"MI_MP_CB_OFFS_CNT_INIT", "Offset counter init value for main picture Cb component ring buffer",
		0x00001348, 0x00000000, 27,  0, "mp_cb_offs_cnt_init", "Offset counter init value of main picture Cb component ring buffer.[Refer doc]"},


	{"MI_MP_CR_BASE_AD_INIT", "Base address for main picture Cr component ring buffer",
		0x0000134c, 0x00000000, 31,  0, "mp_cr_base_ad_init", "Base address of main picture Cr component ring buffer.[Refer doc]"},

	{"MI_MP_CR_SIZE_INIT", "Size of main picture Cr component ring buffer",
		REG_BASE+0x050, 0x00000000, 27,  0, "mp_cr_size_init",		"Size of main picture Cr component ring buffer.[Refer doc]"},

	{"MI_MP_CR_OFFS_CNT_INIT", "Offset counter init value for main picture Cr component ring buffer",
		0x00001354, 0x00000000, 27,  0, "mp_cr_offs_cnt_init", "Offset counter init value of main picture Cr component ring buffer.[Refer doc]"},

	{"MI_MP_Y_BASE_AD_INIT2", "Base address 2 (ping pong) for main picture Y component",
		0x00001358, 0x00000000, 31,  0, "mp_y_base_ad_init2", "Second ping pong base address of main picture Y component buffer.[Refer doc]"},

	{"MI_MP_CB_BASE_AD_INIT2",	"Base address 2 (pingpong) for main picture Cb component ring buffer",
		0x0000135C, 0x00000000, 31,  0, "mp_cb_base_ad_init2",	"Second ping pong base address of main picture Cb component buffer[Refer doc]"},

	{"MI_MP_CR_BASE_AD_INIT2",	"Base address 2 (pingpong) for main picture Cr component ring buffer",
		0x00001360, 0x00000000, 31,  0, "mp_cr_base_ad_init2",	"Second ping pong base address of main picture Cr component buffer.[Refer doc]"},

	{"MI_MP_Y_BASE_AD_SHD", "Base address shadow register for main picture Y component ring buffer",
		0x00001370, 0x00000000, 31,  0, "mp_y_base_ad", "Shadow Base address of main picture Y component ring buffer."},

	{"MI_MP_RAW_BASE_AD_INIT","Follow Y",	0x00001394, 0x00000000, 31,  0, "mp_raw_base_ad_init",	"Base address of main picture RAW component ring buffer.[Refer doc]"},
	{"MI_MP_RAW_SIZE_INIT", "Follow Y",	0x00001398, 0x00000000, 28,  0, "mp_raw_size_init", "Size of main picture RAW data ring buffer.[Refer doc]"},
	{"MI_MP_RAW_OFFS_CNT_INIT", "Follow Y",	0x0000139c, 0x00000000, 28,  0, "mp_raw_offs_cnt_init", "Offset counter initial value of main picture RAW data ring buffer.[Refer doc]"},
	{"MI_MP_RAW_LLENGTH", "Follow Y",	0x000013a0, 0x00000000, 31,  0, "mp_raw_llengh", "Line length of Main Path picture RAW data in bytes, also known as line stride. [Refer doc]"},
	{"MI_MP_RAW_PIC_WIDTH", "Follow Y",	0x000013a4, 0x00000000, 31,  0, "mp_raw_pic_width", "Image width of the Main Path picture RAW component or RGB picture in pixels.[Refer doc]"},
	{"MI_MP_RAW_PIC_HEIGHT", "Follow Y",	0x000013A8, 0x00000000, 14,  0, "mp_raw_pic_height",	"Image height of Main Path picture RAW data in pixels[Refer doc]"},
	{"MI_MP_RAW_PIC_SIZE", "Follow Y",	0x000013AC, 0x00000000, 24,  0, "mp_raw_pic_size",	"Image size of the RAW component in bytes[Refer doc]"},

	{"MI_MP_JDP_BASE_AD_INIT", "Follow Y",	0x000013C0, 0x00000000, 31,  0, "mp_jdp_base_ad_init",	"Base address of main picture jpeg or dpcc ring buffer.[Refer doc]"},
	{"MI_MP_JDP_SIZE_INIT", "Follow Y",	0x000013C4, 0x00000000, 28,  0, "mp_jdp_size_init",	"Size of main picture jpeg or dpcc ring buffer.[Refer doc]"},
	{"MI_MP_JDP_OFFS_CNT_INIT", "Follow Y",	0x000013C8, 0x00000000, 28,  0, "mp_jdp_offs_cnt_init",	"Offset counter init value of main picture jpeg or dpcc data ring buffer.[Refer doc]"},
	{"MI_MP_JDP_LLENGTH", "Follow Y",	0x000013CC, 0x00000000, 31,  0, "mp_jdp_llength",	"Line length of mp picture jpeg data in pixels, also known as line stride.[Refer doc]"},
	{"MI_MP_JDP_PIC_WIDTH", "Follow Y",	0x000013D0, 0x00000000, 14,  0, "mp_jdp_pic_width",	"Image width of the main picture JPEG component picture in pixels.[Refer doc]"},
	{"MI_MP_JDP_PIC_HEIGHT", "Follow Y",	0x000013D4, 0x00000000, 14,  0, "mp_jdp_pic_height",	"Image height of the main picture JPEG component picture in pixels.[Refer doc]"},
	{"MI_MP_JDP_PIC_SIZE", "Follow Y",	0x000013D8, 0x00000000, 24,  0, "mp_jdp_pic_size",	"Image size of the main picture JPEG component picture in bytes.[Refer doc]"},


	{"MI_MP_STATUS_CLR", "FIFO full clear",
			0x000013ec, 0x00000000,  4,  4, "mp_jdp_fifo_full", "Clear status of JDP FIFO full flag in Main path"},
	{NULL, NULL,	0x000013ec, 0x00000000,  3,  3, "mp_raw_fifo_full", "Clear status of RAW FIFO full flag in Main path"},
	{NULL, NULL,	0x000013ec, 0x00000000,  2,  2, "mp_cr_fifo_full", "Clear status of Cr FIFO full flag in Main path"},
	{NULL, NULL,	0x000013ec, 0x00000000,  1,  1, "mp_cb_fifo_full", "Clear status of Cb FIFO full flag in Main path"},
	{NULL, NULL,	0x000013ec, 0x00000000,  0,  0, "mp_y_fifo_full", "Clear status of Y FIFO full flag in Main path"},


	{"MI_MP_CTRL_STATUS", "FIFO Full STATUS, Y,U,V,RAW,JDP",
			0x000013f0, 0x00000000,  4,  4, "mp_jdp_fifo_full", "FIFO full flag of JDP FIFO in Main path asserted since last clear"},
	{NULL, NULL,	0x000013f0, 0x00000000,  3,  3, "mp_raw_fifo_full", "FIFO full flag of RAW FIFO in Main path asserted since last clear"},
	{NULL, NULL,	0x000013f0, 0x00000000,  2,  2, "mp_cr_fifo_full", "FIFO full flag of Cr FIFO in Main path asserted since last clear"},
	{NULL, NULL,	0x000013f0, 0x00000000,  1,  1, "mp_cb_fifo_full", "FIFO full flag of Cb FIFO in Main path asserted since last clear"},
	{NULL, NULL,	0x000013f0, 0x00000000,  0,  0, "mp_y_fifo_full", "FIFO full flag of Y FIFO in Main path asserted since last clear"},

	{"MI_SP2_CTRL", "SP2 control register",
			0x000014e4, 0x00000000,  9,  9, "sp2_rd_raw_cfg_update",	"Forced configuration update which leads to an immediate update of the shadow registers for this RAW read path."},
	{NULL, NULL,	0x000014e4, 0x00000000,  8,  8, "sp2_rd_raw_auto_update",	"unused"},
	{NULL, NULL,	0x000014e4, 0x00000000,  7,  7, "sp2_rd_yuv_cfg_update",	"unused"},
	{NULL, NULL,	0x000014e4, 0x00000000,  6,  6, "sp2_rd_yuv_auto_update",	"unused"},
	{NULL, NULL,	0x000014e4, 0x00000000,  5,  5, "sp2_init_offset_en",		"Enables updating of the offset counters shadow registers [Refer doc]"},
	{NULL, NULL,	0x000014e4, 0x00000000,  4,  4, "sp2_init_base_en",		"Enables updating of the base address and buffer size shadow registers[Refer doc]"},
	{NULL, NULL,	0x000014e4, 0x00000000,  3,  3, "sp2_mi_cfg_upd",		"Forced configuration update which leads to an immediate update of the shadow registers.[Refer doc]"},
	{NULL, NULL,	0x000014e4, 0x00000000,  2,  2, "sp2_mi_skip",			"Skip of current or next starting main picture."},
	{NULL, NULL,	0x000014e4, 0x00000000,  1,  1, "sp2_auto_update",		"Automatic update of configuration registers for Self Path 2 at frame end. 0: disabled; 1: enabled"},
	{NULL, NULL,	0x000014e4, 0x00000000,  0,  0, "sp2_pingpong_enable",		"Ping-pong mode of configuration registers for Self Path 2 at frame end. 0: disabled; 1: enabled"},


	{"MI_SP2_FMT",	"SP2 pixel format control register",
			0x000014e8, 0x00000000, 29, 29, "sp2_wr_yuv_word_aligned_msb_en","0: 10bit yuv in LSB of 16bit;  1: 10bit yuv in MSB of 16bit;"},
	{NULL, NULL,	0x000014e8, 0x00000000, 28, 28, "sp2_wr_yuv_word_aligned", 	"0: disable; 1:enable"},
	{NULL, NULL,	0x000014e8, 0x00000000, 27, 26, "sp2_rd_yuv_nvy", 		"00: Y channel in the first position; 01: Y in the second; 10: Y in the third; 11: Reserved" LINE_PADDING_6TAB
											"  When in YUV444 or RGB888 interleave: " LINE_PADDING_6TAB
											"  00: Y channel in the first and third position; 01: Y channel in the second and fourth position"},
	{NULL, NULL,	0x000014e8, 0x00000000, 25, 25, "sp2_rd_yuv_nv21", 		"0: No swap; 1: Swap Cr and Cb channel for semi-planar and YUV444 or RGB interleave and YUV422 interleave"},
	{NULL, NULL,	0x000014e8, 0x00000000, 24, 23, "sp2_rd_raw_aligned",		"00: Unaligned; 01: Aligned mode0; 10: Aligned mode1; 11: Reserved"},
	{NULL, NULL,	0x000014e8, 0x00000000, 22, 22, "sp2_rd_yuv_aligned",		"0: Unaligned; 1: Aligned"},
	{NULL, NULL,	0x000014e8, 0x00000000, 21, 19, "sp2_rd_raw_bit",		"000: RAW8; 001: RAW10; 010: RAW12; 011: RAW14; 100: RAW16; 101: RAW20, or Reserved"},
	{NULL, NULL,	0x000014e8, 0x00000000, 18, 17, "sp2_rd_yuv_str",		"00: YUV/RGB Semi-Planar; 01: YUV Interleave, RGB Raster; 10: YUV/RGB Planar; 11: Reserved"},
	{NULL, NULL,	0x000014e8, 0x00000000, 16, 15, "sp2_rd_yuv_fmt",		"00: YUV420; 01: YUV422; 10: YUV444; 11: Reserved"},
	{NULL, NULL,	0x000014e8, 0x00000000, 14, 14, "sp2_rd_yuv_bit",		"0: 8 bit YUV/RGB; 1: 10 bit YUV/RGB"},
	{NULL, NULL,	0x000014e8, 0x00000000, 13, 12, "sp2_wr_yuv_nvy",		"00: Y channel in the first position; 01: Y in the second; 10: Y in the third; 11: Reserved" LINE_PADDING_6TAB
											"  When in YUV444 or RGB888 interleave:  00:Y channel in 1st and 3rd position;  01:Y channel in the 2nd and 4th position"},
	{NULL, NULL,	0x000014e8, 0x00000000, 11, 11, "sp2_wr_yuv_nv21",		"0: No swap; 1: Swap Cr and Cb channel for semi-planar and YUV444 or RGB interleave and YUV422 interleave"},
	{NULL, NULL,	0x000014e8, 0x00000000, 10,  9, "sp2_wr_raw_aligned",		"00: Unaligned; 01: Aligned mode0; 10: Aligned mode1; 11: Reserved"},
	{NULL, NULL,	0x000014e8, 0x00000000,  8,  8, "sp2_wr_yuv_aligned",		"0: Unaligned; 1:Aligned"},
	{NULL, NULL,	0x000014e8, 0x00000000,  7,  5, "sp2_wr_raw_bit",		"000: RAW8; 001: RAW10; 010: RAW12; 011: RAW14; 100: RAW16; 101: RAW20, or Reserved"},
	{NULL, NULL,	0x000014e8, 0x00000000,  4,  3, "sp2_wr_yuv_str",		"00: YUV/RGB SemiPlanar; 01: YUV Interleave, RGB Raster; 10: YUV/RGB Planar; 11: Y Only"},
	{NULL, NULL,	0x000014e8, 0x00000000,  2,  1, "sp2_wr_yuv_fmt",		"00: YUV420; 01: YUV422; 10: YUV444; 11: Reserved"},
	{NULL, NULL,	0x000014e8, 0x00000000,  0,  0, "sp2_wr_yuv_bit",		"0: 8 bit YUV/RGB; 1: 10 bit YUV/RGB"},

	{"MI_SP2_RAW_BASE_AD_INIT", "Follow main path RAW",
			0x00001568, 0x00000000, 31,  0, "sp2_raw_base_ad_init",		"Base address of Self Path 2 picture RAW component ring buffer[Refer doc]"},
	{"MI_SP2_DMA_RAW_PIC_START_AD", "RAW component image start address",
			0x000015bc, 0x00000000, 31,  0, "sp2_dma_raw_pic_start_ad",	"Image start address of the RAW component"},

	{"MI_SP2_DMA_RAW_PIC_WIDTH", "RAW component image width",
			0x000015c0, 0x00000000, 14,  0, "sp2_dma_raw_pic_width", "Image width of the RAW component in pixels."},

	{"MI_SP2_DMA_RAW_STATUS", "DMA raw status",    0x000015e0, 0x00000000,	0,  0, "sp2_dma_raw_active", "If set, DMA RAW access is active."},

	{"MI_MCM_CTRL", "MCM Path MI control register",
			0x00001600, 0x00000000,  6,  6, "mcm_rd_cfg_upd",	"Forced configuration update which leads to an immediate " LINE_PADDING_6TAB
										"update of the shadow registers for the MCM read path."},
	{NULL, NULL,	0x00001600, 0x00000000,  5,  5, "mcm_rd_auto_update",	"unused"},
	{NULL, NULL,	0x00001600, 0x00000000,  4,  4, "mcm_init_offset_en",	"Enables updating of the offset counters shadow registers " LINE_PADDING_6TAB
										"for MCM path to the programmed register init values.[Refer doc]"},
	{NULL, NULL,	0x00001600, 0x00000000,  3,  3, "mcm_init_base_en",	"Enables updating of the base address and buffer size shadow registers" LINE_PADDING_6TAB
										"for main picture to the programmed register init values. [Refer doc]"},
	{NULL, NULL,	0x00001600, 0x00000000,  2,  2, "mcm_wr_cfg_upd",	"Forced configuration update which leads to an immediate " LINE_PADDING_6TAB
										"update of the shadow registers. [Refer doc]"},
	{NULL, NULL,	0x00001600, 0x00000000,  1,  1, "mcm_wr_skip",		"Skip of current or next starting MCM picture."},
	{NULL, NULL,	0x00001600, 0x00000000,  0,  0, "mcm_wr_auto_update",	"Automatic update of configuration registers for Self Path 1 " LINE_PADDING_6TAB
										"at frame end. 0: Disabled; 1: Enabled"},



	{"MI_MCM_FMT", "MCM pixel format control register",
			0x00001604, 0x00000000, 17, 16, "mcm_wr1_fmt_aligned",	"00:  unaligned, 01: aligned mode0, 10: aligned mode1, 11: Reserved"},
	{NULL, NULL,	0x00001604, 0x00000000, 15, 14, "mcm_wr0_fmt_aligned",	"00:  unaligned, 01: aligned mode0, 10: aligned mode1, 11: Reserved"},
	{NULL, NULL,	0x00001604, 0x00000000, 13, 12, "mcm_rd_fmt_aligned",	"00:  unaligned, 01: aligned mode0, 10: aligned mode1, 11: Reserved"},
	{NULL, NULL,	0x00001604, 0x00000000, 11,  8, "mcm_wr1_raw_bit",	"000: RAW8, 001: RAW10, 010: RAW12, 011: RAW14, 100: RAW16, 101: RAW20 or Reserved"},
	{NULL, NULL,	0x00001604, 0x00000000,  7,  4, "mcm_wr0_raw_bit",	"000: RAW8, 001: RAW10, 010: RAW12, 011: RAW14, 100: RAW16, 101: RAW20 or Reserved"},
	{NULL, NULL,	0x00001604, 0x00000000,  3,  0, "mcm_rd_raw_bit",	"000: RAW8, 001: RAW10, 010: RAW12, 011: RAW14, 100: RAW16, 101: RAW20 or Reserved"},


	{"MI_MCM_BUS_CFG", "MCM AXI Bus cfg",
			0x00001608, 0x00000000, 19, 16, "mcm_rd_swap_raw",	"Swap bits control for 16 byte data width on AXI bus; " LINE_PADDING_6TAB
										"YUV interleave and RGB raster mode share the same bits." LINE_PADDING_6TAB
										"Bit 0 to swap bytes; Bit 1 to swap words; Bit 2 to swap double words; Bit 3 to swap four words"},
	{NULL, NULL,	0x00001608, 0x00000000, 15, 12, "mcm_wr0_swap_raw",	"Same as field: mcm_rd_swap_raw"},
	{NULL, NULL,	0x00001608, 0x00000000, 11,  8, "mcm_wr1_swap_raw",	"Same as field: mcm_rd_swap_raw"},
	{NULL, NULL,	0x00001608, 0x00000000,  7,  6, "mcm_rd_issue_cap",	"Maximum outstanding issue capability. 00: 4; 01: 8; 10: 16; 11: 32"},
	{NULL, NULL,	0x00001608, 0x00000000,  5,  4, "mcm_wr_issue_cap",	"Maximum outstanding issue capability. 00: 4; 01: 8; 10: 16; 11: 32"},
	{NULL, NULL,	0x00001608, 0x00000000,  3,  2, "mcm_rd_burst_len",	"Maximum burst length. 00:4; 01:8; 10:16, 11:Reserved"},
	{NULL, NULL,	0x00001608, 0x00000000,  1,  0, "mcm_wr_burst_len",	"Maximum burst length. 00:4; 01:8; 10:16, 11:Reserved"},

	{"MI_MCM_RAW0_BASE_AD_INIT", "Follow Main Path RAW",	0x00001614, 0x00000000, 31,  0, "mcm_raw0_base_ad_init","Base address of MCM Path picture RAW0 component ring buffer[Refer doc]"},
	{"MI_MCM_RAW0_SIZE_INIT", "Follow Main Path RAW",	0x00001618, 0x00000000, 28,  0, "mcm_raw0_size_init",	"Size of MCM Path picture RAW0 data ring buffer.[Refer doc]"},

	{"MI_MCM_BUS_ID", "MCM AXI Bus ID configuration",
			0x0000160C, 0x00000000, 27, 27, "mcm_bus_sw_en",	"Control whether BUS works, and set to 1’b1 before ISP works"},
	{NULL, NULL,	0x0000160C, 0x00000000, 26, 26, "mcm_rd_id_en",	"MCM AXI ports read ID configuration enable"},
	{NULL, NULL,	0x0000160C, 0x00000000, 25, 18, "mcm_rd_id_cfg",	"MCM AXI ports read ID configuration"},
	{NULL, NULL,	0x0000160C, 0x00000000,  8,  8, "mcm_wr1_id_en",	"MCM AXI ports write1 ID configuration enable"},
	{NULL, NULL,	0x0000160C, 0x00000000,  7,  0, "mcm_wr1_id_cfg",	"MCM AXI ports write1 ID configuration"},

	{"MI_MCM_RAW0_LLENGTH", "Follow Main Path RAW",    0x00001620, 0x00000000, 31,  0, "mcm_raw0_llength",		"Line length of MCM Path picture RAW0 data in bytes, also known as line stride[Refer doc]"},
	{"MI_MCM_RAW0_PIC_WIDTH", "Follow Main Path RAW",  0x00001624, 0x00000000, 14,  0, "mcm_raw0_pic_width",	"Image width of the MCM Path picture RAW component in pixels.[Refer doc]"},
	{"MI_MCM_RAW0_PIC_HEIGHT", "Follow Main Path RAW", 0x00001628, 0x00000000, 14,  0, "mcm_raw0_pic_height",	"Image height of the RAW0 component in pixels.[Refer doc]"},
	{"MI_MCM_RAW0_PIC_SIZE", "Follow Main Path RAW",   0x0000162c, 0x00000000, 24,  0, "mcm_raw0_pic_size",		"Image size of the RAW0 component in bytes(mcm_raw0_llength * mcm_raw0_pic_height).[Refer doc]"},

	{"MI_MCM_RAW1_BASE_AD_INIT", "Follow Main Path RAW",	0x00001640, 0x00000000, 31,  0, "mcm_raw1_base_ad_init","Base address of MCM Path picture RAW1 component ring buffer"},
	{"MI_MCM_RAW1_SIZE_INIT", "Follow Main Path RAW",	0x00001644, 0x00000000, 28,  0, "mcm_raw1_size_init",	"Size of MCM Path picture RAW1 data ring buffer."},

	{"MI_MCM_RAW1_LLENGTH", "Follow Main Path RAW",    0x0000164c, 0x00000000, 31,  0, "mcm_raw1_llength",		"Line length of MCM Path picture RAW1 data in bytes, also known as line stride[Refer doc]"},
	{"MI_MCM_RAW1_PIC_WIDTH", "Follow Main Path RAW",  0x00001650, 0x00000000, 14,  0, "mcm_raw1_pic_width",	"Image width of the MCM Path picture RAW1 component in pixels.[Refer doc]"},
	{"MI_MCM_RAW1_PIC_HEIGHT", "Follow Main Path RAW", 0x00001654, 0x00000000, 14,  0, "mcm_raw1_pic_height",	"Image height of the RAW1 component in pixels.[Refer doc]"},
	{"MI_MCM_RAW1_PIC_SIZE", "Follow Main Path RAW",   0x00001658, 0x00000000, 24,  0, "mcm_raw1_pic_size",		"Image size of the RAW1 component in bytes(mcm_raw1_llength * mcm_raw1_pic_height).[Refer doc]"},

	{"MI_MCM_DMA_RAW_PIC_START_AD", "RAW component image start address",	0x0000166c, 0x00000000, 31,  0, "mcm_dma_raw_pic_start_ad",	"Image start address of the RAW component."},
	{"MI_MCM_DMA_RAW_PIC_WIDTH", "RAW component image width",		0x00001670, 0x00000000, 14,  0, "mcm_dma_raw_pic_width",	"Image width of the RAW component in pixels."},
	{"MI_MCM_DMA_RAW_PIC_LLENGTH", "RAW component original line length",	0x00001674, 0x00000000, 31,  0, "mcm_dma_raw_pic_llength",	"Line length of the RAW component in bytes of " LINE_PADDING_6TAB
																		"the original image in memory.For an uncropped image, " LINE_PADDING_6TAB
																		"where lines follow each other without offset" LINE_PADDING_6TAB
																		"(no line stride), line length must match image width."},
	{"MI_MCM_DMA_RAW_PIC_SIZE", "RAW component image memory size in bytes",	0x00001678, 0x00000000, 27,  0, "mcm_dma_raw_pic_size",		"Image size of the RAW component in bytes which has to be" LINE_PADDING_6TAB
																		" the RAW line length multiplied by the RAW image height" LINE_PADDING_6TAB
																		"(dma_raw_llength * dma_raw_pic_height)."},

	{"MI_MCM_DMA_STATUS", "MCM_DMA status register",    0x0000168c, 0x00000000,  0,  0, "mcm_dma_active", "0: MCM DMA access is not active; 1: MCM DMA access is active"},
	{"MI_MCM_DMA_RAW_PIC_LVAL", "MCM_DMA RAW Component line valid bytes number, 128 bit Alignment.",
			0x00001690, 0x00000000, 15,  0, "mcm_dma_raw_width_bytes",	"Sizes occupied by valid data of a line; " LINE_PADDING_6TAB
											"should be a multiple of 16" LINE_PADDING_6TAB
											"Note: bus width=16 bytes"},

	{"MI_IMSC", "Interrupt Mask",
			0x000016c0, 0x00000000, 27, 27, "fill_mp_y", "Mask bit for fill level interrupt of main picture Y, JPEG or RAW data"},
	{NULL, NULL,	0x000016c0, 0x00000000, 26, 26, "mblk_line", "Mask bit for macroblock line interrupt of main picture " LINE_PADDING_6TAB
									"(16 lines of Y, 8 lines of Cb and 8 lines of Cr are written into RAM)"},
	{NULL, NULL,	0x000016c0, 0x00000000, 25, 25, "mp_handshk_int", "Mask bit for handshake interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 24, 24, "mcm_dma_raw_ready", "Mask bit for mcm_dma RAW ready interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 23, 23, "sp2_dma_raw_ready", "Mask bit for sp2_dma RAW ready interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 22, 22, "sp2_dma_ycbcr_ready", "Mask bit for sp2_dma YCbCr ready interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 21, 21, "wrap_mcm_raw1", "Mask bit for MCM RAW1 address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 20, 20, "wrap_mcm_raw0", "Mask bit for MCM RAW0 address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 19, 19, "wrap_sp2_raw", "Mask bit for Self Path 2 RAW address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 18, 18, "wrap_sp2_cr", "Mask bit for Self Path 2 Cr address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 17, 17, "wrap_sp2_cb", "Mask bit for Self Path 2 Cb address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 16, 16, "wrap_sp2_y", "Mask bit for Self Path 2 Y address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 15, 15, "wrap_sp1_cr", "Mask bit for Self Path 1 Cr address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 14, 14, "wrap_sp1_cb", "Mask bit for Self Path 1 Cb address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 13, 13, "wrap_sp1_y", "Mask bit for Self Path 1 Y address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 12, 12, "wrap_mp_jdp", "Mask bit for Main Path JDP address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 11, 11, "wrap_mp_raw", "Mask bit for Main Path RAW address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000, 10, 10, "wrap_mp_cr", "Mask bit for Main Path Cr address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000,  9,  9, "wrap_mp_cb", "Mask bit for Main Path Cb address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000,  8,  8, "wrap_mp_y", "Mask bit for Main Path Y address wrap interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000,  7,  7, "mcm_raw1_frame_end", "Mask bit for MCM RAW1 end of frame interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000,  6,  6, "mcm_raw0_frame_end", "Mask bit for MCM RAW0 end of frame interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000,  5,  5, "sp2_raw_frame_end", "Mask bit for Self Path 2 RAW end of frame interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000,  4,  4, "sp2_ycbcr_frame_end", "Mask bit for Self Path 2 YCbCr end of frame interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000,  3,  3, "sp1_ycbcr_frame_end", "Mask bit for Self Path 1 YCbCr end of frame interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000,  2,  2, "mp_jdp_frame_end", "Mask bit for Main Path JDP end of frame interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000,  1,  1, "mp_raw_frame_end", "Mask bit for Main Path RAW end of frame interrupt"},
	{NULL, NULL,	0x000016c0, 0x00000000,  0,  0, "mp_ycbcr_frame_end", "Mask bit for Main Path YCbCr end of frame interrupt." LINE_PADDING_6TAB
										"0: Interrupt masked; 1: Interrupt active"},

	{"MI_MIS", "Masked Interrupt Status",
			0x000016d0, 0x00000000, 27, 27, "fill_mp_y",		"Masked status of fill level interrupt of main picture Y, JPEG or RAW data"},
	{NULL, NULL,	0x000016d0, 0x00000000, 26, 26, "mblk_line",		"Masked status of macroblock line interrupt of main picture" LINE_PADDING_6TAB
										"(16 lines of Y, 8 lines of Cb and 8 lines of Cr are written into RAM)"},
	{NULL, NULL,	0x000016d0, 0x00000000, 25, 25, "mp_handshk_int",	"Masked status of handshake interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 24, 24, "mcm_dma_raw_ready",	"Masked status of mcm_dma RAW ready interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 23, 23, "sp2_dma_raw_ready",	"Masked status of sp2_dma RAW ready interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 22, 22, "sp2_dma_ycbcr_ready",	"Masked status of sp2_dma YCbCr ready interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 21, 21, "wrap_mcm_raw1",	"Masked status of MCM RAW1 address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 20, 20, "wrap_mcm_raw0",	"Masked status of MCM RAW0 address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 19, 19, "wrap_sp2_raw",		"Masked status of Self Path2 RAW address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 18, 18, "wrap_sp2_cr",		"Masked status of Self Path2 Cr address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 17, 17, "wrap_sp2_cb",		"Masked status of Self Path2 Cb address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 16, 16, "wrap_sp2_y",		"Masked status of Self Path2 y address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 15, 15, "wrap_sp1_cr",		"Masked status of Self Path1 Cr address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 14, 14, "wrap_sp1_cb",		"Masked status of Self Path1 Cb address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 13, 13, "wrap_sp1_y",		"Masked status of Self Path1 y address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 12, 12, "wrap_mp_jdp",		"Masked status of Main Path JDP address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 11, 11, "wrap_mp_raw",		"Masked status of Main Path RAW address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000, 10, 10, "wrap_mp_cr",		"Masked status of Main Path Cr address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000,  9,  9, "wrap_mp_cb",		"Masked status of Main Path Cb address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000,  8,  8, "wrap_mp_y",		"Masked status of Main Path Y address wrap interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000,  7,  7, "mcm_raw1_frame_end",	"Masked status of MCM RAW1 end of frame interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000,  6,  6, "mcm_raw0_frame_end",	"Masked status of MCM RAW0 end of frame interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000,  5,  5, "sp2_raw_frame_end",	"Masked status of Self Path2 RAW end of frame interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000,  4,  4, "sp2_ycbcr_frame_end",	"Masked status of Self Path2 YCbCr end of frame interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000,  3,  3, "sp1_ycbcr_frame_end",	"Masked status of Self Path1 YCbCr end of frame interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000,  2,  2, "mp_jdp_frame_end",	"Masked status of Main Path JDP end of frame interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000,  1,  1, "mp_raw_frame_end",	"Masked status of Main Path RAW end of frame interrupt"},
	{NULL, NULL,	0x000016d0, 0x00000000,  0,  0, "mp_ycbcr_frame_end",	"Masked status of Main Path YCbCr end of frame interrupt"},

	{"MI_ICR", "Interrupt Clear Register",
			0x000016d8, 0x00000000, 27, 27, "fill_mp_y", "Clear fill level interrupt of main picture Y, JPEG or RAW data"},
	{NULL, NULL,	0x000016d8, 0x00000000, 26, 26, "mblk_line", "Clear macroblock line interrupt of main picture "  LINE_PADDING_6TAB
									" (16 lines of Y, 8 lines of Cb and 8 lines of Cr are written into RAM)"},
	{NULL, NULL,	0x000016d8, 0x00000000, 25, 25, "mp_handshk_int", "Clear handshake interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 24, 24, "mcm_dma_raw_ready", "Clear mcm_dma RAW ready interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 23, 23, "sp2_dma_raw_ready", "Clear sp2_dma RAW ready interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 22, 22, "sp2_dma_ycbcr_ready", "Clear sp2_dma YCbCr ready interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 21, 21, "wrap_mcm_raw1", "Clear MCM RAW1 address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 20, 20, "wrap_mcm_raw0", "Clear MCM RAW0 address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 19, 19, "wrap_sp2_raw", "Clear Self Path2 RAW address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 18, 18, "wrap_sp2_cr", "Clear Self Path2 Cr address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 17, 17, "wrap_sp2_cb", "Clear Self Path2 Cb address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 16, 16, "wrap_sp2_y", "Clear Self Path2 y address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 15, 15, "wrap_sp1_cr", "Clear Self Path1 Cr address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 14, 14, "wrap_sp1_cb", "Clear Self Path1 Cb address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 13, 13, "wrap_sp1_y", "Clear Self Path1 Y address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 12, 12, "wrap_mp_jdp", "Clear Main Path JDP address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 11, 11, "wrap_mp_raw", "Clear Main Path RAW address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000, 10, 10, "wrap_mp_cr", "Clear Main Path Cr address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000,  9,  9, "wrap_mp_cb", "Clear Main Path Cb address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000,  8,  8, "wrap_mp_y", "Clear Main Path Y address wrap interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000,  7,  7, "mcm_raw1_frame_end", "Clear MCM RAW1 end of frame interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000,  6,  6, "mcm_raw0_frame_end", "Clear MCM RAW0 end of frame interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000,  5,  5, "sp2_raw_frame_end", "Clear Self Path2 RAW end of frame interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000,  4,  4, "sp2_ycbcr_frame_end", "Clear Self Path2 YCbCr end of frame interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000,  3,  3, "sp1_ycbcr_frame_end", "Clear Self Path1 YCbCr end of frame interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000,  2,  2, "mp_jdp_frame_end", "Clear Main Path JDP end of frame interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000,  1,  1, "mp_raw_frame_end", "Clear Main Path RAW end of frame interrupt"},
	{NULL, NULL,	0x000016d8, 0x00000000,  0,  0, "mp_ycbcr_frame_end", "Clear Main Path YCbCr end of frame interrupt"},


	{"MI_RIS", "Interrupt Set Register",
			0x000016e0, 0x00000000, 27, 27, "fill_mp_y", "RAW status of fill level interrupt of main picture Y, JPEG or RAW data"},
	{NULL, NULL,	0x000016e0, 0x00000000, 26, 26, "mblk_line", "RAW status of macroblock line interrupt of main picture " LINE_PADDING_6TAB
								"(16 lines of Y, 8 lines of Cb and 8 lines of Cr are written into RAM)"},
	{NULL, NULL,	0x000016e0, 0x00000000, 25, 25, "mp_handshk_int", "RAW status of handshake interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 24, 24, "mcm_dma_raw_ready", "RAW status of mcm_dma RAW ready interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 23, 23, "sp2_dma_raw_ready", "RAW status of sp2_dma RAW ready interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 22, 22, "sp2_dma_ycbcr_ready", "RAW status of sp2_dma YCbCr ready interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 21, 21, "wrap_mcm_raw1", "RAW status of MCM RAW1 address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 20, 20, "wrap_mcm_raw0", "RAW status of MCM RAW0 address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 19, 19, "wrap_sp2_raw", "RAW status of Self Path2 RAW address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 18, 18, "wrap_sp2_cr", "RAW status of Self Path2 Cr address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 17, 17, "wrap_sp2_cb", "RAW status of Self Path2 Cb address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 16, 16, "wrap_sp2_y", "RAW status of Self Path2 Y address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 15, 15, "wrap_sp1_cr", "RAW status of Self Path1 Cr address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 14, 14, "wrap_sp1_cb", "RAW status of Self Path1 Cb address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 13, 13, "wrap_sp1_y", "RAW status of Self Path1 Y address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 12, 12, "wrap_mp_jdp", "RAW status of Main Path JDP address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 11, 11, "wrap_mp_raw", "RAW status of Main Path RAW address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000, 10, 10, "wrap_mp_cr", "RAW status of Main Path Cr address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000,  9,  9, "wrap_mp_cb", "RAW status of Main Path Cb address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000,  8,  8, "wrap_mp_y", "RAW status of Main Path Y address wrap interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000,  7,  7, "mcm_raw1_frame_end", "RAW status of MCM RAW1 end of frame interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000,  6,  6, "mcm_raw0_frame_end", "RAW status of MCM RAW0 end of frame interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000,  5,  5, "sp2_raw_frame_end", "RAW status of Self Path2 RAW end of frame interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000,  4,  4, "sp2_ycbcr_frame_end", "RAW status of Self Path2 YCbCr end of frame interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000,  3,  3, "sp1_ycbcr_frame_end", "RAW status of Self Path1 YCbCr end of frame interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000,  2,  2, "mp_jdp_frame_end", "RAW status of Main Path JDP end of frame interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000,  1,  1, "mp_raw_frame_end", "RAW status of Main Path RAW end of frame interrupt"},
	{NULL, NULL,	0x000016e0, 0x00000000,  0,  0, "mp_ycbcr_frame_end", "RAW status of Main Path YCbCr end of frame interrupt"},


	{"MI_MIS2", "Interrupt Set Register",
			0x000016f0, 0x00000000, 26, 26, "ppw_y_buf_full", "status of ppw y buf full ready interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 25, 25, "ppw_u_buf_full", "status of ppw u buf full ready interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 24, 24, "ppw_v_buf_full", "status of ppw v buf full ready interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 23, 23, "ppr_y_buf_full", "status of ppr y buf full ready interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 22, 22, "sp2_raw2_w_buf_full", "status of spw_raw2 wr buf full interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 21, 21, "sp2_raw2_r_buf_full", "status of spw_raw2 rd buf full interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 20, 20, "hdr_w_buf_full", "status of mi hdr wr buf full interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 19, 19, "hdr_r_buf_full", "status of mi hdr rd buf full interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 18, 18, "sp2_raw2_dma_ready", "status of spw_raw2 ready interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 17, 17, "ppr_dma_ready", "status of pp_dma yuv ready interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 16, 16, "wrap_sp2_raw", "status of sp2_raw2 address wrap interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 15, 15, "wrap_ppw_cr", "status of ppw_cr address wrap interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 14, 14, "wrap_ppw_cb", "status of ppw_cb address wrap interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 13, 13, "wrap_ppw_y", "status of ppw_y address wrap interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 12, 12, "sp2_raw2_frame_end", "status of spp2_raw2 end of frame interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 11, 11, "ppw_frame_end", "status of ppw end of frame interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000, 10, 10, "hdr_vs_dma_ready", "status of hdr_vs ready interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000,  9,  9, "hdr_s_dma_ready", "status of hdr_s ready interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000,  8,  8, "hdr_l_dma_ready", "status of hdr_l ready interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000,  7,  7, "wrap_hdr_vs", "status of hdr_vs address wrap interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000,  6,  6, "wrap_hdr_s", "status of hdr_s address wrap interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000,  5,  5, "wrap_hdr_l", "status of hdr_l address wrap interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000,  4,  4, "hdr_vs_frame_end", "status of hdr_vs end of frame interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000,  3,  3, "hdr_s_frame_end", "status of hdr_s end of frame interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000,  2,  2, "hdr_l_frame_end", "status of hdr_l end of frame interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000,  1,  1, "mi_rt_bus_buserr", "status of Memory Interface RT Path bus buserr interrupt"},
	{NULL, NULL,	0x000016f0, 0x00000000,  0,  0, "mi_rt_bus_timeo", "status of Memory Interface RT Path bus timeout interrupt"},
};

int light_isp_mi_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_mi_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_mi_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

