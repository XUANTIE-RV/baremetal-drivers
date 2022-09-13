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
#include <stdarg.h>

#include "log_common.h"
#include "ra_common.h"

static char reg_desc_tmp[4096];

int ra_dump_reg_fields(reg_analyzer_info_s *ra_info,
			const reg_field_s reg_field[], const uint32_t reg_field_count)
{
	int i = 0, pos=-1;
	int has_meet = 0;
	char *reg_name;
	char *reg_desc;
	uint32_t reg_reset_val;
	uint32_t reg_offset = ra_info->reg_offset;
	uint32_t reg_val = ra_info->reg_value;

	if (strlen(ra_info->reg_name) == 0 && ra_info->reg_offset == INVALID_MAGIC32) {
		printf("Invalid reg_name('%s') or reg_offset(0x%08x)\n",
			ra_info->reg_name, ra_info->reg_offset);
		return -1;
	}

	for (i = 0; i < reg_field_count; i++) {
		if (ra_info->reg_offset == reg_field[i].offset ||
		    (reg_field[i].reg_name != NULL &&
		     strncmp(reg_field[i].reg_name, ra_info->reg_name, strlen(reg_field[i].reg_name))==0)) {
			pos = i;
			reg_offset = reg_field[pos].offset;
			reg_desc = reg_field[pos].reg_desc;
			reg_name = reg_field[pos].reg_name;
			reg_reset_val = reg_field[pos].reset_value;
			break;
		}
	}
	if (pos < 0) {
		//printf_red("Can't find register offset=0x%x\n", reg_offset);
		return -1;
	}

	printf("\n%-24s %-10s  %10s  %10s  %s\n", "  Register Name", "Offset", "ResetValue", "CurrentVal", "Description");
	printf("===================================================================================\n");
	printf("%-24s 0x%08x  0x%08x  ", reg_name, reg_offset, reg_reset_val);
	if (reg_reset_val != reg_val)
		printf_green("0x%08x", reg_val);
	else
		printf("0x%08x", reg_val);
	printf("  %s\n", reg_desc);

	printf("\nBit[s]  Len  ResetValue  CurrentValue(Hex/Dec)  FieldName                Description\n");
	printf("===================================================================================\n");

	for (i = pos; i < reg_field_count; i++) {
		if (reg_field[i].offset != reg_offset) {
			if (has_meet == 1) {
				break;
			}
			continue;
		}

		uint32_t msb = reg_field[i].msb;
		uint32_t lsb = reg_field[i].lsb;
		uint32_t curr_field_val = reg_val;
		if (msb != 31 || lsb != 0) {
			curr_field_val = (reg_val >> lsb) & (~(0xffffffff << (msb - lsb + 1)));
		}
		uint32_t reset_field_val;
		uint32_t field_len = msb - lsb + 1;

		reset_field_val = (reg_reset_val >> lsb) & (~(0xffffffff << (msb - lsb + 1)));

		if (field_len == 1) {
			printf(" %5d ", lsb);
		}
		else {
			printf(" %2d:%2d ", msb, reg_field[i].lsb);
		}
		printf("%4d  ", field_len);
		printf("0x%-9x ", reset_field_val);
		if (curr_field_val != reset_field_val) {
			printf_green("0x%-8x/", curr_field_val);
			printf_green("%10u  ", curr_field_val);
		} else {
			printf("0x%-8x/", curr_field_val);
			printf("%10u  ", curr_field_val);
		}

		if (reg_field[i].field_name != NULL)
			printf("%-24s:", reg_field[i].field_name);
		else
			printf("%-24s", "");

		if (reg_field[i].field_desc != NULL) {
			if (strstr(reg_field[i].field_desc, LINE_PADDING_6TAB)) {
				strncpy(reg_desc_tmp, reg_field[i].field_desc, sizeof(reg_desc_tmp));
				char *p = strstr(reg_desc_tmp, LINE_PADDING_6TAB);
				while (p) {
					str_replace(p, strlen(LINE_PADDING_6TAB), LINE_PADDING_9TAB);
					p = p+strlen(LINE_PADDING_9TAB);
					p = strstr(p, LINE_PADDING_6TAB);
				}
				printf("%-s\n", reg_desc_tmp);
			} else {
				printf("%-s\n", reg_field[i].field_desc);
			}
		}
		else
			printf("%-s\n", "");

		if (has_meet == 0) {
			has_meet = 1;
		}
	}
	printf("\n");

	if (has_meet !=0 )
		return 0;
	else
		return -1;
}

int ra_dump_reg_define(reg_analyzer_info_s *ra_info,
			const reg_field_s reg_field[], const uint32_t reg_field_count)
{
	int i = 0, pos=-1;
	int has_meet = 0;
	char *reg_desc;
	uint32_t reg_offset;
	uint32_t reg_reset_val;

	if (strlen(ra_info->reg_name) == 0 && ra_info->reg_offset == INVALID_MAGIC32) {
		printf("Invalid reg_name('%s') or reg_offset(0x%08x)\n",
			ra_info->reg_name, ra_info->reg_offset);
		return -1;
	}

	for (i = 0; i < reg_field_count; i++) {
		if (ra_info->reg_offset == reg_field[i].offset ||
		    (reg_field[i].reg_name != NULL &&
		     strncmp(reg_field[i].reg_name, ra_info->reg_name, strlen(reg_field[i].reg_name))==0)) {
			pos = i;
			reg_offset = reg_field[pos].offset;
			reg_desc = reg_field[pos].reg_desc;
			reg_reset_val = reg_field[pos].reset_value;
			break;
		}
	}
	if (pos < 0) {
		//printf_red("Can't find register offset=0x%x\n", reg_offset);
		return -1;
	}

	printf("\n%-24s %-10s  %10s  %s\n", "Register Name", "Offset", "ResetValue", "Description");
	printf("===================================================================================\n");
	printf("%-24s 0x%08x  0x%08x  %s\n", reg_field[pos].reg_name, reg_offset, reg_reset_val, reg_desc);


	printf("\nBit[s]  Len  ResetValue  FieldName               Description\n");
	printf("===================================================================================\n");

	for (i = pos; i < reg_field_count; i++) {
		if (reg_field[i].offset != reg_offset) {
			if (has_meet == 1) {
				break;
			}
			continue;
		}

		uint32_t msb = reg_field[i].msb;
		uint32_t lsb = reg_field[i].lsb;

		uint32_t reset_field_val;
		uint32_t field_len = msb - lsb + 1;

		reset_field_val = (reg_reset_val >> lsb) & (~(0xffffffff << (msb - lsb + 1)));

		if (field_len == 1) {
			printf(" %5d ", lsb);
		}
		else {
			printf(" %2d:%2d ", msb, reg_field[i].lsb);
		}
		printf("%4d  ", field_len);
		printf("0x%-9x ", reset_field_val);

		if (reg_field[i].field_name != NULL)
			printf("%-23s:", reg_field[i].field_name);
		else
			printf("%-23s", "");

		if (reg_field[i].field_desc != NULL)
			printf("%-s\n", reg_field[i].field_desc);
		else
			printf("%-s\n", "");

		if (has_meet == 0) {
			has_meet = 1;
		}
	}
	printf("\n");

	if (has_meet !=0 )
		return 0;
	else
		return -1;

}

int ra_dump_reg_find(reg_analyzer_info_s *ra_info,
		     const reg_field_s reg_field[], const uint32_t reg_field_count)
{
	int i = 0;
	char *last_reg_name = NULL;
	if (ra_info == NULL || ra_info->reg_name == NULL) {
		printf("ra_info or ra_info->reg_name is invalid\n");
		return -1;
	}

	for (i = 0; i < reg_field_count; i++) {
		const reg_field_s *pfield = &reg_field[i];
		if (last_reg_name == pfield->reg_name || pfield->reg_name == NULL)
			continue;

		if (stristr(pfield->reg_name, ra_info->reg_name) != NULL) {
			printf("Find register:  offset=0x%08x, name='%s'\n",
				pfield->offset, pfield->reg_name);
		}
		last_reg_name = pfield->reg_name;
	}
	return 0;
}

void _dump_ra_info(reg_analyzer_info_s *ra_info)
{
	printf("input params: [-m %s] [-o 0x%08X] [-r %s] [-v 0x%08x], op=%d\n",
		ra_info->module_name, ra_info->reg_offset, ra_info->reg_name,
		ra_info->reg_value, ra_info->operate);
}

char *strlwr(char *str)
{
	char *orign = str;
	for (; *str != '\0'; str++)
		*str = tolower(*str);
	return orign;
}

void str_replace(char * cp, int n, char * str)
{
	int lenofstr;
	char * tmp;
	lenofstr = strlen(str);
	//str3比str2短，往前移动
	if(lenofstr < n)
	{
		tmp = cp+n;
		while(*tmp)
		{
			*(tmp-(n-lenofstr)) = *tmp; //n-lenofstr是移动的距离
			tmp++;
		}
		*(tmp-(n-lenofstr)) = *tmp; //move '\0'
	}
	else
	        //str3比str2长，往后移动
		if(lenofstr > n)
		{
			tmp = cp;
			while(*tmp) tmp++;
			while(tmp>=cp+n)
			{
				*(tmp+(lenofstr-n)) = *tmp;
				tmp--;
			}
		}
	strncpy(cp,str,lenofstr);
}

// compare string Ignore case
int stricmp(const char* p1, const char* p2)
{
	while (*p1 && *p2)
	{
		char c1 = *p1;
		char c2 = *p2;
		if (c1 >= 'A' && c1 <= 'Z')
		{
			c1 += 'a' - 'A';
		}
		if (c2 >= 'A' && c2 <= 'Z')
		{
			c2 += 'a' - 'A';
		}
		if (c1 != c2)
		{
			return c1 - c2;
		}
		p1++;
		p2++;
	}
	return *p1 - *p2;
}

// find string Ignore case
char* stristr(const char* haystack, const char* needle)
{
	char* char1 = NULL;
	char* char2  = NULL;
	if ((haystack  == NULL) || (needle  == NULL) || (strlen(haystack) < strlen(needle))){
		return NULL;
	}

	for (char1  = (char*)haystack; (*char1) != '\0'; ++char1) {
		char* char3  = char1;
		for (char2  = (char*)needle; (*char2) != '\0' && (*char1) != '\0'; ++char2, ++char1)	{
			char c1  = (*char1) & 0xDF;
			char c2  = (*char2) & 0xDF;
			if ((c1  != c2) || (((c1 > 0x5A) || (c1 < 0x41)) && (*char1  != *char2)))
				break;
		}

		if ((*char2) == '\0')
			return char3;

		char1 = char3;
	}
	return NULL;
}

int ra_search_reg_by_offset(reg_analyzer_info_s *ra_info)
{
	if (ra_info->reg_offset == INVALID_MAGIC32) {
		return -1;
	}

	return 0;
}

int ra_search_reg_by_name(reg_analyzer_info_s *ra_info)
{
	if (strlen(ra_info->module_name) == 0) {
		return -1;
	}

	return 0;
}

