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
#include <ctype.h>

#include "log_common.h"
#include "ra_common.h"

#define TOOL_NAME "reg_analyzer"

extern int reg_demo_dump(reg_analyzer_info_s *ra_info);
extern int reg_demo_define(reg_analyzer_info_s *ra_info);
extern int reg_demo_find(reg_analyzer_info_s *ra_info);

extern int light_visys_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_visys_reg_define(reg_analyzer_info_s *ra_info);
extern int light_visys_reg_find(reg_analyzer_info_s *ra_info);

extern int light_isp_reg_dump(reg_analyzer_info_s *ra_info);
extern int light_isp_reg_define(reg_analyzer_info_s *ra_info);
extern int light_isp_reg_find(reg_analyzer_info_s *ra_info);


static module_instance_t light_module_instance_array[] = {
	{"demo",	reg_demo_dump,		reg_demo_define,	reg_demo_find},
	{"visys",	light_visys_reg_dump,	light_visys_reg_define,	light_visys_reg_find},
	{"isp0",	light_isp_reg_dump,	light_isp_reg_define,	light_isp_reg_find},
};

void show_moduel_list()
{
	int i;
	int module_count = sizeof(light_module_instance_array)/sizeof(light_module_instance_array[0]);
	for (i = 0; i < module_count; i++) {
		printf("%s  ", light_module_instance_array[i].module_name);
			}
	printf("\n");
}


static void help(void)
{
	printf("usage: %s [-m module_name] [-o reg_offset] [-v reg_value] \n"
	       "                    [-f find_reg_name]\n", TOOL_NAME);
	printf("  example: %s -m demo -o 0x00000010\n", TOOL_NAME);
	printf("           %s -m demo -o 0x00000010 -v 0x0001ff7b\n", TOOL_NAME);
	printf("           %s -m demo -r DEMO_REG1\n", TOOL_NAME);
	printf("           %s -m demo -r DEMO_REG0 -v 0x0001ff7b\n", TOOL_NAME);
	printf("           %s -m demo -f _REG0\n", TOOL_NAME);
}

static int parse_cmd_params(int argc,char *argv[], reg_analyzer_info_s *ra_info)
{
	int param_cnt = 1;
	memset(ra_info, 0, sizeof(*ra_info));
	ra_info->operate = RA_OP_INVALID;
	ra_info->reg_offset = INVALID_MAGIC32;
	ra_info->reg_value = INVALID_MAGIC32;

	if (argc == 1) {
		//help();
		return -1;
	}

	while (param_cnt < argc) {
		//printf("argv[param_cnt]=%s\n", argv[param_cnt]);
		if (0 == strncmp (argv[param_cnt], "-m", 2)) {
			strncpy(ra_info->module_name, argv[param_cnt+1], sizeof(ra_info->module_name));
			param_cnt += 2;
		} else if (0 == strncmp (argv[param_cnt], "-o", 2) ||
			   0 == strncmp (argv[param_cnt], "addr", 2)) {
			sscanf(strlwr(argv[param_cnt+1]), "%x", &ra_info->reg_offset);
			param_cnt += 2;
		} else if (0 == strncmp (argv[param_cnt], "-r", 2)) {
			sscanf(argv[param_cnt+1], "%s", ra_info->reg_name);
			param_cnt += 2;
		} else if (0 == strncmp (argv[param_cnt], "-v", 2) ||
			   0 == strncmp (argv[param_cnt], "value", 2)) {
			sscanf(strlwr(argv[param_cnt+1]), "%x", &ra_info->reg_value);
			param_cnt += 2;
		} else if (0 == strncmp (argv[param_cnt], "-f", 2)) {
			sscanf(argv[param_cnt+1], "%s", ra_info->reg_name);
			ra_info->operate = RA_OP_FIND_REG;
			param_cnt += 2;
		} else {
			return -1;
		}
	}

	if (strlen(ra_info->module_name) == 0) {
		printf("Please input valid module name, such as: ");
		show_moduel_list();
		return -1;
	}

	if (ra_info->operate == RA_OP_FIND_REG) {
		return 0;
	}

	if (ra_info->reg_offset == INVALID_MAGIC32 && strlen(ra_info->reg_name) ==0 )
		return -1;

	if (ra_info->reg_value != INVALID_MAGIC32)
		ra_info->operate = RA_OP_SHOW_DUMP;
	else
		ra_info->operate = RA_OP_SHOW_REG_DEFINE;

	if (ra_info->operate == RA_OP_INVALID)
		return -1;

	return 0;
}

int main(int argc,char *argv[])
{
	int i, ret = 0;
	int module_count = sizeof(light_module_instance_array)/sizeof(light_module_instance_array[0]);
	reg_analyzer_info_s ra_info;
	module_instance_t *module_inst = NULL;

	ret = parse_cmd_params(argc, argv, &ra_info);
	//_dump_ra_info(&ra_info);
	if (ret != 0) {
		printf_red("Invalid syntax.\n");
		help();
		return ret;
	}

	for (i = 0; i < module_count; i++) {
		if(strcmp(light_module_instance_array[i].module_name, ra_info.module_name) == 0) {
			module_inst = &light_module_instance_array[i];
		}
	}

	if (module_inst == NULL) {
		printf("Please input valid module name, such as: ");
		show_moduel_list();
		return -1;
	}

	if (ra_info.operate == RA_OP_SHOW_REG_DEFINE && module_inst->reg_define != NULL) {
		ret = module_inst->reg_define(&ra_info);
		if (ret != 0) {
			printf_red("The register or offset can't find in defination\n");
			return ret;
		}
	} else if (ra_info.operate == RA_OP_SHOW_DUMP && module_inst->reg_dump != NULL) {
		ret = module_inst->reg_dump(&ra_info);
		if (ret != 0) {
			printf_red("The register or offset can't find in defination\n");
			return ret;
		}
	} else if (ra_info.operate == RA_OP_FIND_REG && module_inst->reg_find != NULL) {
		ret = module_inst->reg_find(&ra_info);
		if (ret != 0) {
			printf_red("The register or offset can't find in defination\n");
			return ret;
		}
	}

	return 0;
}

