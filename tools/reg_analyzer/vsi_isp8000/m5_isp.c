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

#define MRV_ISP_BASE 0x0400
#define REG_BASE MRV_ISP_BASE

static const reg_field_s reg_desc[] = {
	/* structure field order
	reg_name,                 offset,reset_value,msb,lsb, field_name,              field_desc */

	{"ISP_CTRL", "ISP Global control register",
			0x00000400, 0x80000000, 31, 31, "disable_isp_clk",	"1: disable ISP clock of SRAM."},
	{NULL, NULL,	0x00000400, 0x80000000, 23, 23, "pp_write_sel",		"1: select yuv output 0: select raw output"},
	{NULL, NULL,	0x00000400, 0x80000000, 22, 22, "ir_raw_out",		"0: disable; 1: IR data output from main path"},
	{NULL, NULL,	0x00000400, 0x80000000, 20, 20, "regs_wdrth_data_mux",	""},
	{NULL, NULL,	0x00000400, 0x80000000, 19, 19, "digi_gain_en",		"0:digital gain disable; 1:digital gain enable"},
	{NULL, NULL,	0x00000400, 0x80000000, 18, 18, "dpf_raw_out",		"0: Disable; 1: Enable DPF raw output (from March 2018)"},
	{NULL, NULL,	0x00000400, 0x80000000, 17, 17, "cnr_en",		"0: Disable; 1: CNR is enabled. (from March 2018)"},
	{NULL, NULL,	0x00000400, 0x80000000, 16, 16, "enable_12bit_unpack",	"0:disable; 1: enable 12-bit unpack for DMA read"},
	{NULL, NULL,	0x00000400, 0x80000000, 15, 15, "enable_12bit_packed",	"0:disable; 1: enable 12-bit pack"},
	{NULL, NULL,	0x00000400, 0x80000000, 14, 14, "csm_c_range",		"Color Space Matrix chrominance clipping range for ISP output" LINE_PADDING_6TAB
										"0: CbCr range 64..960 [16..240] according to ITU-R BT.601 standard" LINE_PADDING_6TAB
										"1: full UV range 0..1023 [0..255]" LINE_PADDING_6TAB
										"Numbers in brackets are for 8 bit resolution." LINE_PADDING_6TAB
										"This bit also configures the YCbCr sequence align block accordingly."},
	{NULL, NULL,	0x00000400, 0x80000000, 13, 13, "csm_y_range",		"Color Space Matrix luminance clipping range for ISP output" LINE_PADDING_6TAB
										"0: Y range 64..940 [16..235] according to ITU-R BT.601 standard" LINE_PADDING_6TAB
										"1: full Y range 0..1023 [0..255]" LINE_PADDING_6TAB
										"Numbers in brackets are for 8 bit resolution." LINE_PADDING_6TAB
										"This bit also configures the YCbCr sequence align block accordingly."},
	{NULL, NULL,	0x00000400, 0x80000000, 12, 12, "flash_mode",		"0: sensor interface works independently from flash control unit" LINE_PADDING_6TAB
										"1: one frame is captured when signaled by flash control unit"},
	{NULL, NULL,	0x00000400, 0x80000000, 11, 11, "gamma_out_enable",	"Gamma ON/OFF"},
	{NULL, NULL,	0x00000400, 0x80000000, 10, 10, "gen_cfg_upd",		"1: generate frame synchronous configuration signal at the output of ISP for" LINE_PADDING_6TAB
										"   shadow registers of the following processing modules, write only"},
	{NULL, NULL,	0x00000400, 0x80000000,  9,  9, "cfg_upd",		"1: immediately configure (update) shadow registers, write only"},
	{NULL, NULL,	0x00000400, 0x80000000,  8,  8, "gen_cfg_upd_fix",	"1: make gen_cfg_upd a level signal, effect since then.; " LINE_PADDING_6TAB
										"0: gen_cfg_upd is a pulse signal, only effect for current frame."},
	{NULL, NULL,	0x00000400, 0x80000000,  7,  7, "awb_enable",		"Auto white balance ON/OFF"},
	{NULL, NULL,	0x00000400, 0x80000000,  6,  6, "gamma_in_enable",	"Sensor De-gamma ON/OFF"},
	{NULL, NULL,	0x00000400, 0x80000000,  4,  4, "inform_enable",	"input formatter. 0: disabled; 1: enabled" LINE_PADDING_6TAB
										"The ISP input formatter is enabled or disabled by this bit immediately," LINE_PADDING_6TAB
										"But always starts or stops acquisition frame synchronously."},
	{NULL, NULL,	0x00000400, 0x80000000,  3,  1, "isp_mode",		"000 - RAW picture with BT.601 sync (ISP bypass)" LINE_PADDING_6TAB
										"001 - ITU-R BT.656 (YUV with embedded sync)" LINE_PADDING_6TAB
										"010 - ITU-R BT.601 (YUV input with H/Vsync signals)" LINE_PADDING_6TAB
										"011 - Bayer RGB processing with H/Vsync signals" LINE_PADDING_6TAB
										"100 - data mode (ISP bypass, sync signals interpreted as data enable)" LINE_PADDING_6TAB
										"101 - Bayer RGB processing with BT.656 synchronization" LINE_PADDING_6TAB
										"110 - RAW picture with ITU-R BT.656 synchronization (ISP bypass)" LINE_PADDING_6TAB
										"111 - reserved" LINE_PADDING_6TAB
										"Side effect: If RAW, ...[Refer to Doc]"},
	{NULL, NULL,	0x00000400, 0x80000000,  0,  0, "isp_enable", 		"ISP data output. 0: disabled ; 1: enabled" LINE_PADDING_6TAB
										"Controls output formatter frame synchronously, if isp_gen_cfg_upd is" LINE_PADDING_6TAB
										"Used to activate this bit. For immediate update isp_cfg_upd must be used."},


	{"ISP_ACQ_PROP", "ISP acquisition properties",
			0x00000404, 0x00000000, 31, 31, "hdr_en",			"in HDR mode, ISP input data bit width is 20"},
	{NULL, NULL,	0x00000404, 0x00000000, 29, 29, "sample_edge_1",		"Sample edge control for sensor 2. " LINE_PADDING_6TAB
											"0: use input sclk; 1: add half cycle delay of sclk"},
	{NULL, NULL,	0x00000404, 0x00000000, 28, 28, "sample_edge_0",		"Sample edge control for sensor 1." LINE_PADDING_6TAB
											"0: use input sclk; 1: add half cycle delay of sclk"},
	{NULL, NULL,	0x00000404, 0x00000000, 27, 26, "two_cycle_data_sel",		"two_cycle_data_sel[2:0]. " LINE_PADDING_6TAB
											"0:select the same bits of data0 and data1;" LINE_PADDING_6TAB
											"1: select all bits of data0 and data1 low bits"},
	{NULL, NULL,	0x00000404, 0x00000000, 25, 25, "change_two_cycle_data_sequence", "0: data0 is high bits, data1 is low bits;" LINE_PADDING_6TAB
											"1: data1 is high bits, data0 is low bits"},
	{NULL, NULL,	0x00000404, 0x00000000, 24, 24, "two_cycle_merge_enable",	"0: not merge; 1: two sequence data merge to one pixel"},
	{NULL, NULL,	0x00000404, 0x00000000, 23, 20, "input_bayer_format",		"0: no more than bit16; 1~5: reserved; 6: bit18; 7: bit20"},
	{NULL, NULL,	0x00000404, 0x00000000, 19, 17, "pin_mapping",			"Bit mapping from LSB to MSB:" LINE_PADDING_6TAB
											"000 - normal 12-bit external Interface" LINE_PADDING_6TAB
											"001 - mapping Low 10 bit to High 10 bits, append 2 zeroes as LSBs" LINE_PADDING_6TAB
											"010 - mapping Low 8 bit to High 8 bits, append 4 zeroes as LSBs" LINE_PADDING_6TAB
											"011 - mapping Middle 8 bit to High 8 bits, append 4 zeroes as LSBs" LINE_PADDING_6TAB
											"100...111 - Reserved (from March 2018)"},
	{NULL, NULL,	0x00000404, 0x00000000, 16, 16, "yuv_dma_sel", 			"0: use align or conversion data for isp_is input. " LINE_PADDING_6TAB
											"1: use DMA yuv read data for isp_is input. (from March 2018)"},
	{NULL, NULL,	0x00000404, 0x00000000, 15, 15, "rgb_dma_sel",			"0: use input formatter data for latency fifo. "  LINE_PADDING_6TAB
											"1: use DMA rgb read data for latency fifo.. (from March 2018)"},
	{NULL, NULL,	0x00000404, 0x00000000, 14, 12, "input_selection",		"It not defined in reg.xml, but defines in code:" LINE_PADDING_6TAB
											"000- 12Bit external Interface" LINE_PADDING_6TAB
											"001- 10Bit Interface, append 2 zeroes as LSBs" LINE_PADDING_6TAB
											"010- 10Bit Interface, append 2 MSBs as LSBs" LINE_PADDING_6TAB
											"011- 8Bit Interface, append 4 zeroes as LSBs" LINE_PADDING_6TAB
											"100- 8Bit Interface, append 4 MSBs as LSBs" LINE_PADDING_6TAB
											"101...111 reserved"},
	{NULL, NULL,	0x00000404, 0x00000000, 11, 11, "field_inv",			"0: do not swap odd and even fields; 1: swap odd and even fields"},
	{NULL, NULL,	0x00000404, 0x00000000, 10,  9, "field_selection",		"00- sample all fields (don't care about fields); "  LINE_PADDING_6TAB
											"01- sample only even fields; " LINE_PADDING_6TAB
											"10- sample only odd fields;" LINE_PADDING_6TAB
											"11- reserved"},
	{NULL, NULL,	0x00000404, 0x00000000,  8,  7, "ccir_seq",			"00- YCbYCr; 01- YCrYCb; 10- CbYCrY; 11- CrYCbY"},
	{NULL, NULL,	0x00000404, 0x00000000,  6,  5, "conv_422",			"00- cosited color subsampling Y0Cb0Cr0 – Y1" LINE_PADDING_6TAB
											"01- interleaved color subsampling Y0Cb0 – Y1Cr1 (not recommended) " LINE_PADDING_6TAB
											"10- non-cosited color subsampling Y0Cb(0+1)/2 – Y1Cr(0+1)/2 " LINE_PADDING_6TAB
											"11- reserved"},
	{NULL, NULL,	0x00000404, 0x00000000,  4,  3, "bayer_pat",			"Color components from sensor, starting with top left position" LINE_PADDING_6TAB
											"in sampled frame (reprogram with ISP_ACQ_H_OFFS, ISP_ACQ_V_OFFS)" LINE_PADDING_6TAB
											"00- first line: RGRG..., second line: GBGB..." LINE_PADDING_6TAB
											"01- first line: GRGR..., second line: BGBG..." LINE_PADDING_6TAB
											"10- first line: GBGB..., second line: RGRG..." LINE_PADDING_6TAB
											"11- first line: BGBG..., second line: GRGR..." LINE_PADDING_6TAB
											"This configuration applies for the black level " LINE_PADDING_6TAB
											"area after cropping by the input formatter."},
	{NULL, NULL,	0x00000404, 0x00000000,  2,  2, "vsync_pol",			"Vertical sync polarity. 0: high active; 1: low active"},
	{NULL, NULL,	0x00000404, 0x00000000,  1,  1, "hsync_pol",			"Horizontal sync polarity. 0: high active; 1: low active"},
	{NULL, NULL,	0x00000404, 0x00000000,  0,  0, "sample_edge",			"0- negative edge sampling; 1- positive edge sampling"},



	{"ISP_ACQ_H_OFFS", "Horizontal input offset",	0x00000408, 0x00000000, 14,  0, "acq_h_offs", "Horizontal sample offset in 8-bit samples (YUV: 4 samples = 2 pixels)"},
	{"ISP_ACQ_H_SIZE", "Horizontal input size",	0x00000410, 0x00001000, 14,  0, "acq_h_size",	"Horizontal sample size in 12-bit samples." LINE_PADDING_6TAB
													" YUV input: 2 samples = 1 pixel, else 1 sample = 1 pixel;[Refer doc]"},
	{"ISP_ACQ_V_OFFS", "Vertical input offset",	0x0000040c, 0x00000000, 13,  0, "acq_v_offs", "Vertical sample offset in lines"},
	{"ISP_ACQ_V_SIZE", "Vertical input size",	0x00000414, 0x00000C00, 13,  0, "acq_v_size",	"Vertical sample size in lines"},

	{"ISP_ACQ_NR_FRAMES", "Number of frames to be captured",
							0x00000418, 0x00000000,  9,  0, "acq_nr_frames", "Number of input frames to be sampled (0 = continuous)"},


	{"ISP_AWB_PROP", "Auto white balance properties",
			0x00000510, 0x00000000, 31, 31, "awb_meas_mode","0: near white discrimination mode using YCbCr color space" LINE_PADDING_6TAB
									"1: RGB based measurement mode"},
	{NULL, NULL,	0x00000510, 0x00000000,  2,  2, "awb_max_en",	"0: disable; 1: enable. Not valid in RGB measurement mode."},
	{NULL, NULL,	0x00000510, 0x00000000,  1,  0, "awb_mode",	"00: no measurement"  LINE_PADDING_6TAB
									"01: reserved" LINE_PADDING_6TAB
									"10: measurement of YCbCr means (AWB_MEAS_MODE = 0)" LINE_PADDING_6TAB
									"    or RGB means (AWB_MEAS_MODE = 1)" LINE_PADDING_6TAB
									"11: reserved"},

	{"ISP_AWB_GAIN_G", "Auto white balance gain green",
			0x00000538, 0x01000100, 25, 16, "awb_gain_gr",	"Gain value for green component in red line 100h = 1, " LINE_PADDING_6TAB
									"  Unsigned integer value, range 0 to 4 with 8 bit fractional part"},
	{NULL, NULL,	0x00000538, 0x01000100,  9,  0, "awb_gain_gb",	"Gain value for green component in blue line 100h = 1, "  LINE_PADDING_6TAB
									"  Unsigned integer value, range 0 to 4 with 8 bit fractional part"},

	{"ISP_AWB_GAIN_RB", "Auto white balance gain red and blue",
			0x0000053c, 0x01000100, 25, 16, "awb_gain_r",	"Gain value for red component in red line 100h = 1, " LINE_PADDING_6TAB
									"  Unsigned integer value, range 0 to 4 with 8 bit fractional part"},
	{NULL, NULL,	0x0000053c, 0x01000100,  9,  0, "awb_gain_b",	"Gain value for blue component in blue line 100h = 1, " LINE_PADDING_6TAB
									"  Unsigned integer value, range 0 to 4 with 8 bit fractional part"},

	{"ISP_OUT_H_OFFS", "Horizontal offset of output window",	0x00000594, 0x00000000, 13,  0, "isp_out_h_offs",	"Horizontal pic offset in lines"},
	{"ISP_OUT_V_OFFS", "Vertical offset of output window",		0x00000598, 0x00000000, 13,  0, "isp_out_v_offs",	"Vertical pic offset in lines"},
	{"ISP_OUT_H_SIZE", "Output horizontal picture size",		0x0000059C, 0x00000000, 14,  0, "isp_out_h_size",	"Horizontal picture size in pixel if ISP_MODE is set to: ...[Refer doc]"},
	{"ISP_OUT_V_SIZE", "Output vertical picture size",		0x000005A0, 0x00000000, 13,  0, "isp_out_v_size",	"Vertical picture size in lines"},


	{"ISP_DEMOSAIC", "Demosaic parameters",
			0x000005a4, 0x00000004, 10, 10, "demosaic_bypass",	"0: normal operation for RGB Bayer Pattern input" LINE_PADDING_6TAB
										"1: demosaicing bypass for Black&White input data"},
	{NULL, NULL,	0x000005a4, 0x00000004,  7,  0, "demosaic_th",		"Threshold for Bayer demosaicing texture detection." LINE_PADDING_6TAB
										"  This value shifted left 4bit is compared with the" LINE_PADDING_6TAB
										"  difference of the vertical and horizontal 12-Bit wide" LINE_PADDING_6TAB
										"  texture indicators, to decide if the vertical or"LINE_PADDING_6TAB
										"  horizontal texture flag must be set." LINE_PADDING_6TAB
										"0x00: maximum edge sensitivity" LINE_PADDING_6TAB
										"0xFF: no texture detection"},

	{"ISP_FLAGS_SHD", "Flags (current status) of certain signals and Shadow regs for enable signals",
			0x000005a8, 0x00000000, 31, 31, "s_hsync",		"State of ISP input port s_hsync, for test purposes"},
	{NULL, NULL,	0x000005a8, 0x00000000, 30, 30, "s_vsync",		"State of ISP input port s_vsync, for test purposes"},
	{NULL, NULL,	0x000005a8, 0x00000000, 27, 16, "s_data",		"State of ISP input port s_data, for test purposes"},
	{NULL, NULL,	0x000005a8, 0x00000000,  2,  2, "inform_field",		"Current field information (0=odd, 1=even)"},
	{NULL, NULL,	0x000005a8, 0x00000000,  1,  1, "inform_en_shd",	"Input formatter enable shadow register"},
	{NULL, NULL,	0x000005a8, 0x00000000,  0,  0, "isp_on_shd",		"ISP enable shadow register" LINE_PADDING_6TAB
										"  Shows if ISP currently outputs data (1) or not (0)"},

	{"ISP_IMSC", "Interrupt mask",
			0x000005bc, 0x00000000, 26, 26, "imsc_sensor0_dataloss", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 25, 25, "imsc_sensor1_dataloss", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 24, 24, "imsc_sensor2_dataloss", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 23, 23, "imsc_sensor3_dataloss", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 22, 22, "imsc_ae2_dataloss", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 21, 21, "imsc_awb2_dataloss", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 20, 20, "imsc_tdnr", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 19, 19, "imsc_vsm_done", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 18, 18, "imsc_exp_end", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 17, 17, "imsc_fl_cap", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 16, 16, "imsc_tdnr1", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 15, 15, "imsc_hist_end", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 14, 14, "imsc_afm_fin", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 13, 13, "imsc_afm_lum_of", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 12, 12, "imsc_afm_sum_of", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 11, 11, "imsc_sh_off", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000, 10, 10, "imsc_sh_on", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000,  9,  9, "imsc_fl_off", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000,  8,  8, "imsc_fl_on", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000,  7,  7, "imsc_h_start", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000,  6,  6, "imsc_v_start", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000,  5,  5, "imsc_frame_in", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000,  4,  4, "imsc_awb_done", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000,  3,  3, "imsc_size_err", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000,  2,  2, "imsc_dataloss", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000,  1,  1, "imsc_frame", "0: mask out; 1: enable interrupt"},
	{NULL, NULL,	0x000005bc, 0x00000000,  0,  0, "imsc_isp_off", "0: mask out; 1: enable interrupt"},


	{"ISP_RIS", "Raw interrupt status",
			0x000005c0, 0x00000000, 26, 26, "ris_sensor0_dataloss", "sensor0 data loss"},
	{NULL, NULL,	0x000005c0, 0x00000000, 25, 25, "ris_sensor1_dataloss", "sensor1 data loss"},
	{NULL, NULL,	0x000005c0, 0x00000000, 24, 24, "ris_sensor2_dataloss", "sensor2 data loss"},
	{NULL, NULL,	0x000005c0, 0x00000000, 23, 23, "ris_sensor3_dataloss", "sensor3 data loss"},
	{NULL, NULL,	0x000005c0, 0x00000000, 22, 22, "ris_ae2_dataloss", "aev2 data loss"},
	{NULL, NULL,	0x000005c0, 0x00000000, 21, 21, "ris_awb2_dataloss", "awb2 data loss"},
	{NULL, NULL,	0x000005c0, 0x00000000, 20, 20, "ris_tdnr", "TDNR reference frame FIFO empty"},
	{NULL, NULL,	0x000005c0, 0x00000000, 19, 19, "ris_vsm_done", "VSM module complete"},
	{NULL, NULL,	0x000005c0, 0x00000000, 18, 18, "ris_exp_end", "Exposure measurement complete"},
	{NULL, NULL,	0x000005c0, 0x00000000, 17, 17, "ris_fl_cap", "Signaling captured frame"},
	{NULL, NULL,	0x000005c0, 0x00000000, 16, 16, "ris_tdnr1", "write cover read"},
	{NULL, NULL,	0x000005c0, 0x00000000, 15, 15, "ris_hist_end", "Histogram measurement ready." LINE_PADDING_6TAB
									"(Old or new histogram measurement)"},
	{NULL, NULL,	0x000005c0, 0x00000000, 14, 14, "ris_afm_fin", "AF measurement finished: this interrupt is setwhen the first" LINE_PADDING_6TAB
									"complete frame is calculated after enabling the AF measurement"},
	{NULL, NULL,	0x000005c0, 0x00000000, 13, 13, "ris_afm_lum_of", "Auto focus luminance overflow"},
	{NULL, NULL,	0x000005c0, 0x00000000, 12, 12, "ris_afm_sum_of", "Auto focus sum overflow"},
	{NULL, NULL,	0x000005c0, 0x00000000, 11, 11, "ris_sh_off", "Mechanical shutter is switched off"},
	{NULL, NULL,	0x000005c0, 0x00000000, 10, 10, "ris_sh_on", "Mechanical shutter is switched on"},
	{NULL, NULL,	0x000005c0, 0x00000000,  9,  9, "ris_fl_off", "Flash light is switched off"},
	{NULL, NULL,	0x000005c0, 0x00000000,  8,  8, "ris_fl_on", "Flash light is switched on"},
	{NULL, NULL,	0x000005c0, 0x00000000,  7,  7, "ris_h_start", "Start edge of h_sync"},
	{NULL, NULL,	0x000005c0, 0x00000000,  6,  6, "ris_v_start", "Start edge of v_sync"},
	{NULL, NULL,	0x000005c0, 0x00000000,  5,  5, "ris_frame_in", "Sampled input frame is complete"},
	{NULL, NULL,	0x000005c0, 0x00000000,  4,  4, "ris_awb_done", "White balancing measurement cycle is complete; results can be read out"},
	{NULL, NULL,	0x000005c0, 0x00000000,  3,  3, "ris_size_err", "Picture size violation occurred; incorrect programming"},
	{NULL, NULL,	0x000005c0, 0x00000000,  2,  2, "ris_dataloss", "Loss of data occurred within a line, processing failure"},
	{NULL, NULL,	0x000005c0, 0x00000000,  1,  1, "ris_frame", "Frame was completely put out"},
	{NULL, NULL,	0x000005c0, 0x00000000,  0,  0, "ris_isp_off", "ISP output was disabled (vsynced) due to f_cnt reached or manual"},

	{"ISP_MIS", "Masked interrupt status",	  0x000005c4, 0x00000000, 26, 26, "mis_sensor0_dataloss", "sensor0 data loss"},
	{NULL, NULL,	0x000005c4, 0x00000000, 25, 25, "mis_sensor1_dataloss", "sensor1 data loss"},
	{NULL, NULL,	0x000005c4, 0x00000000, 24, 24, "mis_sensor2_dataloss", "sensor2 data loss"},
	{NULL, NULL,	0x000005c4, 0x00000000, 23, 23, "mis_sensor3_dataloss", "sensor3 data loss"},
	{NULL, NULL,	0x000005c4, 0x00000000, 22, 22, "mis_ae2_dataloss", "aev2 data loss"},
	{NULL, NULL,	0x000005c4, 0x00000000, 21, 21, "mis_awb2_dataloss", "awb2 data loss"},
	{NULL, NULL,	0x000005c4, 0x00000000, 20, 20, "mis_tdnr", "TDNR reference frame FIFO empty"},
	{NULL, NULL,	0x000005c4, 0x00000000, 19, 19, "mis_vsm_done", "VSM module complete"},
	{NULL, NULL,	0x000005c4, 0x00000000, 18, 18, "mis_exp_end", "Exposure measurement complete"},
	{NULL, NULL,	0x000005c4, 0x00000000, 17, 17, "mis_fl_cap", "Signaling captured frame"},
	{NULL, NULL,	0x000005c4, 0x00000000, 16, 16, "mis_tdnr1", "write cover read"},
	{NULL, NULL,	0x000005c4, 0x00000000, 15, 15, "mis_hist_end", "Histogram measurement ready." LINE_PADDING_6TAB
									"(Old or new histogram measurement)"},
	{NULL, NULL,	0x000005c4, 0x00000000, 14, 14, "mis_afm_fin", "AF measurement finished: this interrupt is set when the first" LINE_PADDING_6TAB
									"complete frame is calculated after enabling the AF measurement"},
	{NULL, NULL,	0x000005c4, 0x00000000, 13, 13, "mis_afm_lum_of", "Auto focus luminance overflow"},
	{NULL, NULL,	0x000005c4, 0x00000000, 12, 12, "mis_afm_sum_of", "Auto focus sum overflow"},
	{NULL, NULL,	0x000005c4, 0x00000000, 11, 11, "mis_sh_off", "Mechanical shutter is switched off"},
	{NULL, NULL,	0x000005c4, 0x00000000, 10, 10, "mis_sh_on", "Mechanical shutter is switched on"},
	{NULL, NULL,	0x000005c4, 0x00000000,  9,  9, "mis_fl_off", "Flash light is switched off"},
	{NULL, NULL,	0x000005c4, 0x00000000,  8,  8, "mis_fl_on", "Flash light is switched on"},
	{NULL, NULL,	0x000005c4, 0x00000000,  7,  7, "mis_h_start", "Start edge of h_sync"},
	{NULL, NULL,	0x000005c4, 0x00000000,  6,  6, "mis_v_start", "Start edge of v_sync"},
	{NULL, NULL,	0x000005c4, 0x00000000,  5,  5, "mis_frame_in", "Sampled input frame is complete"},
	{NULL, NULL,	0x000005c4, 0x00000000,  4,  4, "mis_awb_done", "White balancing measurement cycle is complete; results can be read out"},
	{NULL, NULL,	0x000005c4, 0x00000000,  3,  3, "mis_size_err", "Picture size violation occurred; incorrect programming"},
	{NULL, NULL,	0x000005c4, 0x00000000,  2,  2, "mis_dataloss", "Loss of data occurred within a line, processing failure"},
	{NULL, NULL,	0x000005c4, 0x00000000,  1,  1, "mis_frame", "Frame was completely put out"},
	{NULL, NULL,	0x000005c4, 0x00000000,  0,  0, "mis_isp_off", "ISP output was disabled (vsynced) due to f_cnt reached or manual"},

	{"ISP_ICR", "Interrupt clear register",
			0x000005c8, 0x00000000, 26, 26, "icr_sensor0_dataloss", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 25, 25, "icr_sensor1_dataloss", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 24, 24, "icr_sensor2_dataloss", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 23, 23, "icr_sensor3_dataloss", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 22, 22, "icr_ae2_dataloss", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 21, 21, "icr_awb2_dataloss", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 20, 20, "icr_tdnr", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 19, 19, "icr_vsm_done", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 18, 18, "icr_exp_end", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 17, 17, "icr_fl_cap", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 16, 16, "icr_tdnr1", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 15, 15, "icr_hist_end", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 14, 14, "icr_afm_fin", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 13, 13, "icr_afm_lum_of", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 12, 12, "icr_afm_sum_of", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 11, 11, "icr_sh_off", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000, 10, 10, "icr_sh_on", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000,  9,  9, "icr_fl_off", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000,  8,  8, "icr_fl_on", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000,  7,  7, "icr_h_start", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000,  6,  6, "icr_v_start", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000,  5,  5, "icr_frame_in", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000,  4,  4, "icr_awb_done", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000,  3,  3, "icr_size_err", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000,  2,  2, "icr_dataloss", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000,  1,  1, "icr_frame", "Clear interrupt"},
	{NULL, NULL,	0x000005c8, 0x00000000,  0,  0, "icr_isp_off", "Clear interrupt"},

	{"ISP_ISR", "Interrupt set register",
			0x000005cc, 0x00000000, 26, 26, "isr_sensor0_dataloss", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 25, 25, "isr_sensor1_dataloss", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 24, 24, "isr_sensor2_dataloss", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 23, 23, "isr_sensor3_dataloss", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 22, 22, "isr_ae2_dataloss", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 21, 21, "isr_awb2_dataloss", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 20, 20, "isr_tdnr", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 19, 19, "isr_vsm_done", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 18, 18, "isr_exp_end", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 17, 17, "isr_fl_cap", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 16, 16, "isr_tdnr1", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 15, 15, "isr_hist_end", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 14, 14, "isr_afm_fin", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 13, 13, "isr_afm_lum_of", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 12, 12, "isr_afm_sum_of", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 11, 11, "isr_sh_off", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000, 10, 10, "isr_sh_on", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000,  9,  9, "isr_fl_off", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000,  8,  8, "isr_fl_on", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000,  7,  7, "isr_h_start", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000,  6,  6, "isr_v_start", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000,  5,  5, "isr_frame_in", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000,  4,  4, "isr_awb_done", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000,  3,  3, "isr_size_err", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000,  2,  2, "isr_dataloss", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000,  1,  1, "isr_frame", "Set interrupt"},
	{NULL, NULL,	0x000005cc, 0x00000000,  0,  0, "isr_isp_off", "Set interrupt"},

	{"ISP_ERR", "ISP error register",
			0x0000063c, 0x00000000,  2,  2, "outform_size_err",	"Size error is generated in outmux submodule"},
	{NULL, NULL,	0x0000063c, 0x00000000,  1,  1, "is_size_err",		"Size error is generated in image stabilization submodule"},
	{NULL, NULL,	0x0000063c, 0x00000000,  0,  0, "inform_size_err",	"Size error is generated in inform submodule"},

	{"ISP_ERR_CLR", "ISP error clear register",
			0x00000640, 0x00000000,  2,  2, "outform_size_err_clr",	"Size error is cleared"},
	{NULL, NULL,	0x00000640, 0x00000000,  1,  1, "is_size_err_clr",	"Size error is cleared"},
	{NULL, NULL,	0x00000640, 0x00000000,  0,  0, "inform_size_err_clr",	"Size error is cleared"},

};



int light_isp_isp_reg_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_desc, count);
}

int light_isp_isp_reg_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_define(ra_info, reg_desc, count);
}

int light_isp_isp_reg_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_desc) / sizeof(reg_desc[0]);
	return ra_dump_reg_find(ra_info, reg_desc, count);
}

