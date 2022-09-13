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

#include "log_common.h"
#include "ra_common.h"

extern int light_isp_main_control_reg_dump(reg_analyzer_info_s *ra_info);	// OFFSET=0x0000
extern int light_isp_main_control_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_main_control_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_isp_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_isp_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_isp_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_mcm_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_mcm_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_mcm_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_sitching0_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_sitching0_define(reg_analyzer_info_s *ra_info);
extern int light_isp_sitching0_find(reg_analyzer_info_s *ra_info);

extern int light_isp_exp_bayer_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_exp_bayer_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_exp_bayer_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_is_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_is_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_is_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_cproc_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_cproc_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_cproc_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_main_resize_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_main_resize_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_main_resize_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_denoise3d_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_denoise3d_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_denoise3d_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_dpf_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_dpf_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_dpf_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_denoise3d2_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_denoise3d2_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_denoise3d2_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_dpcc_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_dpcc_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_dpcc_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_compand_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_compand_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_compand_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_shutter_reg_dump(reg_analyzer_info_s *ra_info);	// OFFSET=0x0680
extern int light_isp_shutter_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_shutter_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_mi_reg_dump(reg_analyzer_info_s *ra_info);		// OFFSET=0x1300
extern int light_isp_mi_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_mi_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_jpe_reg_dump(reg_analyzer_info_s *ra_info);	// OFFSET=0x1800
extern int light_isp_jpe_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_jpe_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_afm_reg_dump(reg_analyzer_info_s *ra_info);	// OFFSET=0x2000
extern int light_isp_afm_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_afm_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_hist256_reg_dump(reg_analyzer_info_s *ra_info);	// OFFSET=0x2100
extern int light_isp_hist256_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_hist256_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_lsc_reg_dump(reg_analyzer_info_s *ra_info);	// OFFSET=0x2200
extern int light_isp_lsc_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_lsc_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_exposure_reg_dump(reg_analyzer_info_s *ra_info);	// OFFSET=0x2600
extern int light_isp_exposure_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_exposure_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_vsm_reg_dump(reg_analyzer_info_s *ra_info);	// OFFSET=0x2f00
extern int light_isp_vsm_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_vsm_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_ee_reg_dump(reg_analyzer_info_s *ra_info);		// OFFSET=0x3900
extern int light_isp_ee_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_ee_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_dmsc_reg_dump(reg_analyzer_info_s *ra_info);	// OFFSET=0x3e00
extern int light_isp_dmsc_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_dmsc_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_pre_filt_reg_dump(reg_analyzer_info_s *ra_info);	// OFFSET=0x3e00
extern int light_isp_pre_filt_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_pre_filt_reg_find(reg_analyzer_info_s *ra_info);


int light_isp_reg_dump(reg_analyzer_info_s *ra_info)
{
	if (light_isp_main_control_reg_dump(ra_info) == 0)	// OFFSET=0x0000
		return 0;

	if (light_isp_isp_reg_dump(ra_info) == 0)
		return 0;

	if (light_isp_mcm_reg_dump(ra_info) == 0)
		return 0;


	if (light_isp_mi_reg_dump(ra_info) == 0)
		return 0;

	if (light_isp_is_reg_dump(ra_info) == 0)
		return 0;

	if (light_isp_exp_bayer_reg_dump(ra_info) == 0)
		return 0;

	if (light_isp_sitching0_reg_dump(ra_info) == 0)
		return 0;

	if (light_isp_cproc_reg_dump(ra_info) ==0)
		return 0;

	if (light_isp_main_resize_reg_dump(ra_info)==0)
		return 0;

	if (light_isp_denoise3d_reg_dump(ra_info)==0)
		return 0;

	if(light_isp_dpf_reg_dump(ra_info)==0)
		return 0;

	if(light_isp_denoise3d2_reg_dump(ra_info)==0)
		return 0;

	if(light_isp_dpcc_reg_dump(ra_info)==0)
		return 0;


	if(light_isp_compand_reg_dump(ra_info)==0)
		return 0;

	if(light_isp_shutter_reg_dump(ra_info)==0)	// 0x0680
		return 0;

	if (light_isp_mi_reg_dump(ra_info) == 0)	// 0x1300
		return 0;

	if(light_isp_jpe_reg_dump(ra_info)==0)		// 0x1800
		return 0;

	if(light_isp_afm_reg_dump(ra_info)==0)		// 0x2000
		return 0;

	if(light_isp_hist256_reg_dump(ra_info)==0)	// 0x2100
		return 0;

	if(light_isp_lsc_reg_dump(ra_info)==0)		// 0x2200
		return 0;

	if(light_isp_exposure_reg_dump(ra_info)==0)	// 0x2600
		return 0;

	if(light_isp_vsm_reg_dump(ra_info)==0)		// 0x2f00
		return 0;

	if(light_isp_ee_reg_dump(ra_info)==0)		// 0x3900
		return 0;

	if (light_isp_dmsc_reg_dump(ra_info) == 0)	// 0x3e00
		return 0;

	if (light_isp_pre_filt_reg_dump(ra_info) == 0)	// 0x4000
		return 0;

	//printf_red("%s Can't find register name='%s' offset=0x%x\n",
	//	ra_info->module_name,
	//	ra_info->reg_name,
	//	ra_info->reg_offset);
	return -1;
}

int light_isp_reg_define(reg_analyzer_info_s *ra_info)
{
	if (light_isp_main_control_reg_define(ra_info) == 0)	// OFFSET=0x0000
		return 0;

	if (light_isp_isp_reg_define(ra_info) == 0)
		return 0;

	if (light_isp_mcm_reg_define(ra_info) == 0)
		return 0;

	if (light_isp_sitching0_define(ra_info) ==0)
		return 0;

	if (light_isp_exp_bayer_reg_define(ra_info) == 0)
		return 0;

	if (light_isp_is_reg_define(ra_info) == 0)
		return 0;

	if (light_isp_cproc_reg_define(ra_info) ==0)
		return 0;

	if (light_isp_main_resize_reg_define(ra_info) ==0)
		return 0;

	if (light_isp_denoise3d_reg_define(ra_info) ==0)
		return 0;

	if (light_isp_dpf_reg_define(ra_info) ==0)
		return 0;

	if (light_isp_denoise3d2_reg_define(ra_info) ==0)
		return 0;

	if (light_isp_dpcc_reg_define(ra_info) ==0)
		return 0;

	if (light_isp_compand_reg_define(ra_info) ==0)
		return 0;

	if (light_isp_shutter_reg_define(ra_info) ==0)		// 0x0680
		return 0;

	if (light_isp_mi_reg_define(ra_info) == 0)		// 0x1300
		return 0;

	if (light_isp_jpe_reg_define(ra_info) ==0)		// 0x1800
		return 0;

	if (light_isp_afm_reg_define(ra_info) ==0)		// 0x2000
		return 0;

	if (light_isp_hist256_reg_define(ra_info) ==0)		// 0x2100
		return 0;

	if (light_isp_lsc_reg_define(ra_info) ==0)		// 0x2200
		return 0;

	if (light_isp_exposure_reg_define(ra_info) ==0)		// 0x2600
		return 0;

	if (light_isp_vsm_reg_define(ra_info) ==0)		// 0x2f00
		return 0;

	if (light_isp_ee_reg_define(ra_info) ==0)		// 0x3900
		return 0;

	if (light_isp_dmsc_reg_define(ra_info) == 0)		// 0x3e00
		return 0;

	if (light_isp_pre_filt_reg_define(ra_info) == 0)	// 0x4000
		return 0;

	//printf_red("%s Can't find register name='%s' offset=0x%x\n",
	//	ra_info->module_name,
	//	ra_info->reg_name,
	//	ra_info->reg_offset);
	return -1;
}

int light_isp_reg_find(reg_analyzer_info_s *ra_info)
{
	light_isp_main_control_reg_find(ra_info);	// OFFSET=0x0000

	light_isp_isp_reg_find(ra_info);

	light_isp_mcm_reg_find(ra_info);

	light_isp_sitching0_find(ra_info);

	light_isp_exp_bayer_reg_find(ra_info);

	light_isp_is_reg_find(ra_info);

	light_isp_cproc_reg_find(ra_info);

	light_isp_main_resize_reg_find(ra_info);

	light_isp_denoise3d_reg_find(ra_info);

	light_isp_dpf_reg_find(ra_info);

	light_isp_denoise3d2_reg_find(ra_info);

	light_isp_dpcc_reg_find(ra_info);

	light_isp_compand_reg_find(ra_info);

	light_isp_shutter_reg_find(ra_info);		// 0x0680

	light_isp_mi_reg_find(ra_info);			// 0x1300

	light_isp_jpe_reg_find(ra_info);		// 0x1800

	light_isp_afm_reg_find(ra_info);		// 0x2000

	light_isp_hist256_reg_find(ra_info);		// 0x2100

	light_isp_lsc_reg_find(ra_info);		// 0x2200

	light_isp_exposure_reg_find(ra_info);		// 0x2600

	light_isp_vsm_reg_find(ra_info);		// 0x2f00

	light_isp_ee_reg_find(ra_info);			// 0x3900

	light_isp_dmsc_reg_find(ra_info);		// 0x3e00

	light_isp_pre_filt_reg_find(ra_info);		// 0x4000

	return 0;
}

