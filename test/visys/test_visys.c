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

#include "bm_visys_ioctl.h"

#define DEV_PATHNAME "/dev/bm_visys0"
typedef struct _visys_ctx
{
	int dev_fd;
} visys_ctx_t;
static visys_ctx_t visys_ctx;

static int visys_init(visys_ctx_t *ctx)
{
	int fd;
	memset(&visys_ctx, 0, sizeof(visys_ctx));

	fd = open(DEV_PATHNAME, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		printf("%s:Can't open %s: %d(%s)\n", __func__,
			DEV_PATHNAME, errno, strerror(errno));
		return -1;
	}
	ctx->dev_fd = fd;

	return 0;
}

static int visys_deinit(visys_ctx_t *ctx)
{
	if (ctx->dev_fd >= 0) {
		close(ctx->dev_fd);
		ctx->dev_fd = -1;
	}

	return 0;
}

static int visys_ioctl(visys_ctx_t *ctx, unsigned int cmd, void *args)
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

int main(void)
{
	int ret;
	struct bm_visys_reg_t reg;

	ret = visys_init(&visys_ctx);
	if (ret != 0)
		exit(ret);

	reg.offset = 0x4;	// should be 0x5000008b in reset
	ret = visys_ioctl(&visys_ctx, BMVISYSIOC_READ_REG, &reg);
	if (ret == 0) {
		printf("%s:Read reg OK, offset=0x%08x, value=0x%08x\n", __func__,
			reg.offset, reg.value);
	}

	ret = visys_deinit(&visys_ctx);
	if (ret != 0)
		exit(ret);

	exit(0);
}

