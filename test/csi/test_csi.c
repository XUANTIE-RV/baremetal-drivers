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

#include "bm_csi_ioctl.h"

#define DEV_PATHNAME "/dev/bm_csi0"

typedef struct _csi_ctx
{
	int dev_fd;
} csi_ctx_t;
static csi_ctx_t csi_ctx;

static int csi_init(csi_ctx_t *ctx)
{
	int fd;
	memset(&csi_ctx, 0, sizeof(csi_ctx));

	fd = open(DEV_PATHNAME, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		printf("%s:Can't open %s: %d(%s)\n", __func__,
			DEV_PATHNAME, errno, strerror(errno));
		return -1;
	}
	ctx->dev_fd = fd;

	return 0;
}

static int csi_deinit(csi_ctx_t *ctx)
{
	if (ctx->dev_fd >= 0) {
		close(ctx->dev_fd);
		ctx->dev_fd = -1;
	}

	return 0;
}

static int csi_ioctl(csi_ctx_t *ctx, unsigned int cmd, void *args)
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
	struct bm_csi_reg_t reg;
    reg.offset = 0;

	ret = csi_init(&csi_ctx);
	if (ret != 0)
		exit(ret);
    int cmd = BMCSI_IOC_S_RESET;
    while(cmd < BMCSI_IOC_MAX) {
	    ret = csi_ioctl(&csi_ctx, cmd, &reg);
	    if (ret != 0) {
	    	printf("!!!!!!!!!!!!!!!!!!!error\n");
	    }
        cmd++;
    }

	ret = csi_deinit(&csi_ctx);
	if (ret != 0)
		exit(ret);

	exit(0);
}

