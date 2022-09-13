##
 # Copyright (C) 2021 Alibaba Group Holding Limited
 #
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License version 2 as
 # published by the Free Software Foundation.
##

TARGET = bm_demo

obj-m +=$(TARGET).o
$(TARGET)-objs += bm_demo_driver.o

EXTRA_CFLAGS += -I$(PWD)/./
EXTRA_CFLAGS += -I$(PWD)/../common/include


PWD :=$(shell pwd)

all:
	make   -C $(KERNEL) M=$(PWD)  modules
modules_install:
	make -C $(KERNEL_SRC) M=$(SRC) modules_install
clean:
	rm -rf $($(TARGET)-objs)
	if [ -d "$(KERNEL)" ]; then \
		make -C $(KERNEL) M=`pwd` clean; \
	fi
