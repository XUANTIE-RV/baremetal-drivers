/*
 * Copyright (C) 2021 Alibaba Group Holding Limited
 * Author: LuChongzhi <chongzhi.lcz@alibaba-inc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __RA_COMMON_H__
#define __RA_COMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define INVALID_MAGIC32 0xDEADBEEF

#define LINE_PADDING_6TAB "\n\t\t\t\t\t\t "
#define LINE_PADDING_9TAB "\n\t\t\t\t\t\t\t\t\t "

typedef struct {
	char     *reg_name;	/* Register name, can be null if it exists above, should <=24 chars */
	char     *reg_desc;	/* Register description, can be null if not exists */
	uint32_t  offset;	/* Register offset address */
	uint32_t  reset_value;	/* Register reset value */
	uint32_t  msb;		/* MSB for this field [31..0] */
	uint32_t  lsb;		/* LSB for this field [31..0] */
	char     *field_name;	/* Register field name */
	char     *field_desc;	/* Field description */
} reg_field_s;

/*
For example:
static const reg_field_s demo_reg_desc[] = {
	{"Demo reg", 	0x10, "Demo field1", 15, 0,  "This is demo reg's demo field1"},
	{NULL, 		0x10, "Demo field2", 31, 16, "This is demo reg's demo field2"},
};
*/

typedef struct {
	char    *ip_name;
	uint64_t ip_phy_addr;
} ip_phy_addr_s;

typedef enum {
	RA_OP_INVALID = -1,
	RA_OP_SHOW_DUMP = 0,
	RA_OP_SHOW_REG_DEFINE,
	RA_OP_FIND_REG,
} ra_op_e;

typedef struct {
	char module_name[64];
	uint32_t reg_offset;
	uint32_t reg_value;
	char reg_name[128];
	char field_name[128];
	ra_op_e operate;
} reg_analyzer_info_s;

void _dump_ra_info(reg_analyzer_info_s *ra_info);

int ra_dump_reg_fields(reg_analyzer_info_s *ra_info,
			const reg_field_s reg_field[], const uint32_t reg_field_count);

int ra_dump_reg_define(reg_analyzer_info_s *ra_info,
			const reg_field_s reg_field[], const uint32_t reg_field_count);

int ra_dump_reg_find(reg_analyzer_info_s *ra_info,
		     const reg_field_s reg_field[], const uint32_t reg_field_count);

///////////////////////////////////////


typedef int (*module_reg_dump)(reg_analyzer_info_s *ra_info);
typedef int (*module_reg_define)(reg_analyzer_info_s *ra_info);
typedef int (*module_reg_find)(reg_analyzer_info_s *ra_info);

typedef struct {
	const char		*module_name;
	module_reg_dump		reg_dump;
	module_reg_define	reg_define;
	module_reg_find		reg_find;
} module_instance_t;

int ra_search_reg_by_offset(reg_analyzer_info_s *ra_info);
int ra_search_reg_by_name(reg_analyzer_info_s *ra_info);

//////////////////////////////////////



char *strlwr(char *str);
void str_replace(char * cp, int n, char * str);
int stricmp(const char* p1, const char* p2);
char* stristr(const char* haystack, const char* needle);
#endif /* __RA_COMMON_H__ */

