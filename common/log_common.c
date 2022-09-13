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

#define LOG_COLOR_RED_YELLO_BACK "\033[1;31;43m"
#define LOG_COLOR_RED            "\033[2;31;49m"
#define LOG_COLOR_YELLOW         "\033[2;33;49m"
#define LOG_COLOR_GREEN          "\033[2;32;49m"
#define LOG_COLOR_GRAY           "\033[1;30m"
#define LOG_COLOR_RESET          "\033[0m"

static char sprint_green_buf[1024]=LOG_COLOR_GREEN;
int printf_green(char *fmt, ...)
{
	va_list args;
	int n;

	int color_len = strlen(LOG_COLOR_GREEN);

	va_start(args, fmt);
	n = vsprintf(sprint_green_buf+color_len, fmt, args);
	va_end(args);

	memcpy(sprint_green_buf+color_len+n, LOG_COLOR_RESET, strlen(LOG_COLOR_RESET)+1);

	printf("%s", sprint_green_buf);
	return n;
}

static char sprint_red_buf[1024]=LOG_COLOR_RED;
int printf_red(char *fmt, ...)
{
	va_list args;
	int n;

	int color_len = strlen(LOG_COLOR_RED);

	va_start(args, fmt);
	n = vsprintf(sprint_red_buf+color_len, fmt, args);
	va_end(args);

	memcpy(sprint_red_buf+color_len+n, LOG_COLOR_RESET, strlen(LOG_COLOR_RESET));

	printf("%s", sprint_red_buf);
	return n;
}

