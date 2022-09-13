##
 # Copyright (C) 2021 Alibaba Group Holding Limited
 #
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License version 2 as
 # published by the Free Software Foundation.
##

TARGET = bm_csi

KBUILD_EXTRA_SYMBOLS += $(shell pwd)/../visys/Module.symvers

export KBUILD_EXTRA_SYMBOLS

obj-m +=$(TARGET).o
##$(TARGET)-objs += bm_csi_dphy.o
$(TARGET)-objs += bm_csi_driver.o
$(TARGET)-objs += bm_csi_hw.o
$(TARGET)-objs += dw-csi-plat.o
$(TARGET)-objs += dw-csi-sysfs.o
$(TARGET)-objs += dw-mipi-csi.o
$(TARGET)-objs += dw-dphy-plat.o
$(TARGET)-objs += dw-dphy-rx.o
$(TARGET)-objs += dw-dphy-sysfs.o


EXTRA_CFLAGS += -I$(PWD)/./
EXTRA_CFLAGS += -I$(PWD)/../common

#DEFS = -DCONFIG_DWC_MIPI_TC_DPHY_GEN3

ifeq ($(BUILD_TYPE),DEBUG)
	EXTRA_CFLAGS += -DDEBUG
endif

EXTRA_CFLAGS += $(DEFS)

PWD :=$(shell pwd)

all:
	make -C $(KERNEL) M=$(PWD) modules
modules_install:
	make -C $(KERNEL_SRC) M=$(SRC) modules_install
clean:
	rm -rf $($(TARGET)-objs)
	make -C $(KERNEL) M=`pwd` clean
