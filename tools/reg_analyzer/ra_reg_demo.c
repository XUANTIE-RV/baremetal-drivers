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

/* usage: reg_analyzer -m demo -o 0x10 -v 0x13572468 */
/* usage: reg_analyzer -m demo -r DEMO_REG1 */
static const reg_field_s reg_demo_desc[] = {
	/* structure field order
	reg_name,  offset,reset_value,msb,lsb, field_name,     field_desc */
	{"DEMO_REG0",	"This is demo register-0 description",
			0x10, 0x12345678, 31, 16, "reg0 field0",	"This is demo reg0's field0"},
	{NULL, NULL,	0x10, 0x0,        15, 0,  "reg0 field1",	"This is demo reg0's field1"},

	{"DEMO_REG1",	"This is demo register-0 description",
			0x14, 0x13572468, 31, 24, "reg1 field0",	"This is demo reg1's field0"},
	{NULL, NULL,	0x14, 0x0,        23, 0,  "reg1 field1",	"This is demo reg1's field1 desc line1" LINE_PADDING_6TAB
									"and reg1's field desc line 2" LINE_PADDING_6TAB
									"and reg1's field desc line 3"},
};

int reg_demo_dump(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_demo_desc) / sizeof(reg_demo_desc[0]);
	return ra_dump_reg_fields(ra_info, reg_demo_desc, count);
}

int reg_demo_define(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_demo_desc) / sizeof(reg_demo_desc[0]);
	return ra_dump_reg_define(ra_info, reg_demo_desc, count);
}

int reg_demo_find(reg_analyzer_info_s *ra_info)
{
	uint32_t count = sizeof(reg_demo_desc) / sizeof(reg_demo_desc[0]);
	return ra_dump_reg_find(ra_info, reg_demo_desc, count);
}

