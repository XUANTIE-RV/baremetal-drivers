##
 # Copyright (C) 2021 Alibaba Group Holding Limited
 #
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License version 2 as
 # published by the Free Software Foundation.
##

TARGET = bm_isp


obj-m +=$(TARGET).o
$(TARGET)-objs += bm_isp_driver.o

EXTRA_CFLAGS += -I$(PWD)/./
EXTRA_CFLAGS += -I$(PWD)/../common
KBUILD_CFLAGS += -O0
ifeq ($(BUILD_TYPE),DEBUG)
	EXTRA_CFLAGS += -DDEBUG
endif

PWD :=$(shell pwd)

all:
	make -C $(KERNEL) M=$(PWD) modules
modules_install:
	make -C $(KERNEL_SRC) M=$(SRC) modules_install
clean:
	rm -rf $($(TARGET)-objs)
	if [ -d "$(KERNEL)" ]; then \
		make -C $(KERNEL) M=`pwd` clean; \
	fi
