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
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <poll.h>
#include <stdint.h>
#include <ctype.h>

#include "bm_isp_ioctl.h"
#include "log_common.h"

#define DEV_PATHNAME "/dev/bm_isp0"
#define TOOL_NAME "isp_run_bm_script"
#define INVALID_MAGIC32 0xDEADBEEF

typedef enum {
	ISP_BM_SCRIPT_OP_REG_SET,
	ISP_BM_SCRIPT_OP_REG_GET,
	ISP_BM_SCRIPT_OP_MSLEEP,
} isp_bm_script_op;

typedef struct _isp_ctx {
	int dev_fd;
} isp_ctx_t;
static isp_ctx_t isp_ctx;

typedef struct {
	isp_bm_script_op	op;
	struct bm_isp_reg_t	reg;
	unsigned int		expect;
	unsigned int 		msleep;
} isp_bm_script_info;

typedef struct {
	uint32_t begin_line;	// default:0
	uint32_t end_line;	// default:-1, -1 means no limit
	char device_name[256];
	char bm_script_name[256];
	uint32_t simulate;
} cmd_param_s;

static int isp_init(isp_ctx_t *ctx, char *device_name)
{
	int fd;

	if (!device_name) {
		printf("%s:device name is invalid\n", __func__);
		return -1;
	}

	memset(&isp_ctx, 0, sizeof(isp_ctx));

	fd = open(device_name, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		printf("%s:Can't open %s: %d(%s)\n", __func__,
			device_name, errno, strerror(errno));
		return -1;
	}
	ctx->dev_fd = fd;

	return 0;
}

static int isp_deinit(isp_ctx_t *ctx)
{
	if (ctx->dev_fd >= 0) {
		close(ctx->dev_fd);
		ctx->dev_fd = -1;
	}

	return 0;
}

static int isp_ioctl(isp_ctx_t *ctx, unsigned int cmd, void *args)
{
	int ret;

	if (ctx == NULL || ctx->dev_fd < 0) {
		printf("%s:ctx is invalid\n", __func__);
		return -1;
	}

	ret = ioctl(ctx->dev_fd, cmd, args);
	if (ret != 0) {
		printf("%s:ioctl failed, ret=%d(%s)\n", __func__,
			ret, strerror(ret * -1));
		return ret;
	}

	return 0;
}

static int parse_script_line(isp_bm_script_info *script_info, char *script_line)
{
	int ret;
	char op_str[64]="";
	//printf("script_line=%s", script_line);
	ret = sscanf(script_line, "%s %x %x", op_str, &script_info->reg.offset, &script_info->reg.value);
	if (ret == 3 && (strcmp(op_str, "set") == 0 || strcmp(op_str, "0") == 0)) {
		script_info->op = ISP_BM_SCRIPT_OP_REG_SET;
		return 0;
	} else if (ret >= 2 && (strcmp(op_str, "get") == 0 || strcmp(op_str, "1") == 0)) {
		script_info->op = ISP_BM_SCRIPT_OP_REG_GET;
		if (ret == 2)
			script_info->expect = INVALID_MAGIC32;
		else
			script_info->expect = script_info->reg.value;
		return 0;
	} else if (ret == 2 && strcmp(op_str, "msleep") == 0) {
		ret = sscanf(script_line, "%s %d", op_str, &script_info->msleep);
		script_info->op = ISP_BM_SCRIPT_OP_MSLEEP;
		return 0;
	} else {
		printf_red("script_line invalid:'%s'\n", script_line);
		return -1;
	}
}

static void help(void)
{
	printf("usage: %s [-d device] [-f script_file] [-s simulate]\n", TOOL_NAME);
	printf("  example: %s -d /dev/isp0 -f isp_bm_scripts/isp_v4_mcm.txt -b 0 -e 9 -s 0\n", TOOL_NAME);
}

static void _dump_cmd_param(cmd_param_s *cmd_param)
{
	printf("%s -d %s -f %s -b %d -e %d -s %d\n",
		TOOL_NAME,
		cmd_param->device_name,
		cmd_param->bm_script_name,
		cmd_param->begin_line,
		cmd_param->end_line,
		cmd_param->simulate);
}

static int parse_cmd_params(int argc,char *argv[], cmd_param_s *cmd_param)
{
	int param_cnt = 1;
	memset(cmd_param, 0, sizeof(*cmd_param));
	cmd_param->begin_line = 0;
	cmd_param->end_line = -1;

	if (argc == 1) {
		help();
		return -1;
	}

	while (param_cnt < argc) {
		if (0 == strncmp (argv[param_cnt], "-d", 2)) {
			strncpy(cmd_param->device_name, argv[param_cnt+1], sizeof(cmd_param->bm_script_name));
			param_cnt += 2;
		} else if (0 == strncmp (argv[param_cnt], "-f", 2)) {
			strncpy(cmd_param->bm_script_name, argv[param_cnt+1], sizeof(cmd_param->bm_script_name));
			param_cnt += 2;
		} else if (0 == strncmp (argv[param_cnt], "-b", 2)) {
			cmd_param->begin_line = atoi(argv[param_cnt+1]);
			param_cnt += 2;
		} else if (0 == strncmp (argv[param_cnt], "-e", 2)) {
			cmd_param->end_line = atoi(argv[param_cnt+1]);
			param_cnt += 2;
		} else if (0 == strncmp (argv[param_cnt], "-s", 2)) {
			cmd_param->simulate = atoi(argv[param_cnt+1]);
			param_cnt += 2;
		} else {
			printf("Invalid syntax.\n");
			_dump_cmd_param(cmd_param);
			help();
			return -1;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int32_t ret = 0;
	uint32_t current_line = 0;
	uint32_t run_lines = 0;
	FILE *stream;
	char string[1024];

	cmd_param_s cmd_param;
	ret = parse_cmd_params(argc, argv, &cmd_param);
	if (ret != 0) {
		return ret;
	}
	_dump_cmd_param(&cmd_param);

	if (cmd_param.simulate == 0) {
		ret = isp_init(&isp_ctx, &(cmd_param.device_name[0]));
		if (ret != 0) {
			printf("isp_init failed\n");
			exit(ret);
		}
	}
	stream = fopen(cmd_param.bm_script_name, "r");
	if(stream == NULL) {
		printf("Open script file:'%s' failed\n", cmd_param.bm_script_name);
		return -1;
	}

	printf(">>> Below result is run under %s:\n",
		cmd_param.simulate ? "Simulation" : "Hardware");

	fseek(stream, 0, SEEK_SET);
	while (current_line <= cmd_param.end_line) {
		if (fgets(string, sizeof(string), stream) == NULL) {
			//printf("fgets script file:'%s' failed\n", cmd_param.bm_script_name);
			break;
		}

		if (current_line >= cmd_param.begin_line &&
		    current_line <= cmd_param.end_line) {
			//struct bm_isp_reg_t reg;
			isp_bm_script_info script_info;
			ret = parse_script_line(&script_info, string);
			if (ret != 0) {
				//printf_red("Line-%03d is invalid:%s", current_line+1, string);
				current_line++;
				continue;
			}

			//printf("[line-%03d] offset=0x%08x, value=0x%08x\n",
			//	current_line, reg.offset, reg.value);

			if (cmd_param.simulate == 0) {
				if (script_info.op == ISP_BM_SCRIPT_OP_REG_SET) {
					ret = isp_ioctl(&isp_ctx, BMISPIOC_WRITE_REG, &script_info.reg);
					if (ret == 0) {
						printf("line%03d: Write reg OK, offset=0x%08x, value=0x%08x\n",
							current_line+1, script_info.reg.offset, script_info.reg.value);
					} else {
						printf("line%03d: Write reg failed\n", current_line+1);
						ret = -1;
						break;
					}
				} else if (script_info.op == ISP_BM_SCRIPT_OP_REG_GET) {
					ret = isp_ioctl(&isp_ctx, BMISPIOC_READ_REG, &script_info.reg);
					if (ret == 0) {
						printf("line%03d: Read  reg OK, offset=0x%08x, value=0x%08x\n",
							current_line+1, script_info.reg.offset, script_info.reg.value);
					} else {
						printf("line%03d: Read  reg failed\n", current_line+1);
						ret = -1;
						break;
					}
					if (script_info.expect != INVALID_MAGIC32 &&
					    script_info.reg.value != script_info.expect) {
						printf_red("         Read value(0x%08x) != expect(0x%08x)\n",
							script_info.reg.value, script_info.expect);
					}
				} else if (script_info.op == ISP_BM_SCRIPT_OP_MSLEEP) {
					usleep(script_info.msleep * 1000);
					printf("line%03d: sleep %ums finished\n", current_line+1, script_info.msleep);
				}
			} else {
				if (script_info.op == ISP_BM_SCRIPT_OP_REG_SET) {
					printf("line%03d: Write reg OK, offset=0x%08x, value=0x%08x\n",
						current_line+1, script_info.reg.offset, script_info.reg.value);
				} else if (script_info.op == ISP_BM_SCRIPT_OP_REG_GET) {
					printf("line%03d: Read  reg OK, offset=0x%08x, value=0x%08x\n",
						current_line+1, script_info.reg.offset,script_info.reg.value);
					if (script_info.expect != INVALID_MAGIC32 &&
					    script_info.reg.value != script_info.expect) {
						printf_red("         Read value(0x%08x) != expect(0x%08x)\n",
							script_info.reg.value, script_info.expect);
					}
				} else if (script_info.op == ISP_BM_SCRIPT_OP_MSLEEP) {
					usleep(script_info.msleep * 1000);
					printf("line%03d: sleep %ums finished\n", current_line+1, script_info.msleep);
				}
			}
		}
		run_lines++;
		current_line++;
	}
	printf("<<< %d script lines are run\n", run_lines);

	if (cmd_param.simulate == 0) {
		isp_deinit(&isp_ctx);
	}
	fclose(stream);
	return 0;
}
