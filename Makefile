##
 # Copyright (C) 2021 Alibaba Group Holding Limited
 #
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License version 2 as
 # published by the Free Software Foundation.
##

test = $(shell if [ -f "../.param" ]; then echo "exist"; else echo "noexist"; fi)
ifeq ("$(test)", "exist")
  include ../.param
endif

CONFIG_DRIVER_BUILD_PARAMS=KERNEL=$(LINUX_DIR) CROSS=$(CROSS_COMPILE) ARCH=$(ARCH) BOARD_NAME=$(BOARD_NAME)
CONFIG_LIB_BUILD_PARAMS=CROSS=$(CROSS_COMPILE) ARCH=$(ARCH) BOARD_NAME=$(BOARD_NAME)
CONFIG_TEST_BUILD_PARAMS=CROSS=$(CROSS_COMPILE) ARCH=$(ARCH) BOARD_NAME=$(BOARD_NAME)

MODULE_NAME=BAREMETAL
BUILD_LOG_START="\033[47;30m>>> $(MODULE_NAME) $@ begin\033[0m"
BUILD_LOG_END  ="\033[47;30m<<< $(MODULE_NAME) $@ end\033[0m"

DIR_TARGET_BASE=bsp/baremetal
DIR_TARGET_LIB =bsp/baremetal/lib
DIR_TARGET_KO  =bsp/baremetal/ko
DIR_TARGET_TEST=bsp/baremetal/test
DIR_TARGET_TOOLS=bsp/baremetal/tools

#
# Do a parallel build with multiple jobs, based on the number of CPUs online
# in this system: 'make -j8' on a 8-CPU system, etc.
#
# (To override it, run 'make JOBS=1' and similar.)
#
ifeq ($(JOBS),)
  JOBS := $(shell grep -c ^processor /proc/cpuinfo 2>/dev/null)
  ifeq ($(JOBS),)
    JOBS := 1
  endif
endif

all:    info lib driver test tools install_local_output install_rootfs
.PHONY: info driver lib test tools install_local_output install_rootfs \
        clean_driver clean_lib clean_test clean_tools clean_output clean

info:
	@echo $(BUILD_LOG_START)
	@echo "  ====== Build Info from repo project ======"
	@echo "    BUILDROOT_DIR="$(BUILDROOT_DIR)
	@echo "    CROSS_COMPILE="$(CROSS_COMPILE)
	@echo "    LINUX_DIR="$(LINUX_DIR)
	@echo "    ARCH="$(ARCH)
	@echo "    BOARD_NAME="$(BOARD_NAME)
	@echo "    KERNEL_ID="$(KERNELVERSION)
	@echo "    KERNEL_DIR="$(LINUX_DIR)
	@echo "    INSTALL_DIR_ROOTFS="$(INSTALL_DIR_ROOTFS)
	@echo "    INSTALL_DIR_SDK="$(INSTALL_DIR_SDK)
	@echo "    CONFIG_DRIVER_BUILD_PARAMS="$(CONFIG_DRIVER_BUILD_PARAMS)
	@echo "    CONFIG_LIB_BUILD_PARAMS="$(CONFIG_LIB_BUILD_PARAMS)
	@echo "    CONFIG_TEST_BUILD_PARAMS="$(CONFIG_TEST_BUILD_PARAMS)
	@echo $(BUILD_LOG_END)

driver:
	@echo $(BUILD_LOG_START)
	make -C driver/demo  $(CONFIG_DRIVER_BUILD_PARAMS)
	make -C driver/visys $(CONFIG_DRIVER_BUILD_PARAMS) #BUILD_TYPE=DEBUG
	make -C driver/isp $(CONFIG_DRIVER_BUILD_PARAMS)
	make -C driver/csi   $(CONFIG_DRIVER_BUILD_PARAMS) BUILD_TYPE=DEBUG
	@echo $(BUILD_LOG_END)

clean_driver:
	@echo $(BUILD_LOG_START)
	make -C driver/demo  $(CONFIG_DRIVER_BUILD_PARAMS) clean
	make -C driver/visys $(CONFIG_DRIVER_BUILD_PARAMS) clean
	make -C driver/isp $(CONFIG_DRIVER_BUILD_PARAMS) clean
	make -C driver/csi   $(CONFIG_DRIVER_BUILD_PARAMS) clean
	@echo $(BUILD_LOG_END)

lib:
	@echo $(BUILD_LOG_START)
	@echo $(BUILD_LOG_END)

clean_lib:
	@echo $(BUILD_LOG_START)
	@echo $(BUILD_LOG_END)

test: lib driver
	@echo $(BUILD_LOG_START)
	make -C test/visys
	make -C test/csi
	make -C test/isp
	@echo $(BUILD_LOG_END)

clean_test:
	@echo $(BUILD_LOG_START)
	make -C test/visys clean
	make -C test/isp clean
	@echo $(BUILD_LOG_END)

tools:
	@echo $(BUILD_LOG_START)
	make -C tools/reg_analyzer -j $(JOBS)
	@echo $(BUILD_LOG_END)

clean_tools:
	@echo $(BUILD_LOG_START)
	make -C tools/reg_analyzer clean
	make -C test/csi clean
	make -C test/isp clean
	@echo $(BUILD_LOG_END)

install_local_output: driver lib test
	@echo $(BUILD_LOG_START)
	# driver files
	mkdir -p ./output/rootfs/$(DIR_TARGET_KO)
	cp -f ./driver/demo/*.ko ./output/rootfs/$(DIR_TARGET_KO)
	cp -f ./driver/visys/*.ko ./output/rootfs/$(DIR_TARGET_KO)
	cp -f ./driver/csi/*.ko ./output/rootfs/$(DIR_TARGET_KO)
	cp -f ./driver/isp/*.ko ./output/rootfs/$(DIR_TARGET_KO)
	# lib files
	mkdir -p ./output/rootfs/$(DIR_TARGET_LIB)
	# test files
	mkdir -p ./output/rootfs/$(DIR_TARGET_TEST)
	cp -rf ./test/visys/output/* ./output/rootfs/$(DIR_TARGET_TEST)
	cp -rf ./test/isp/output/* ./output/rootfs/$(DIR_TARGET_TEST)
	# tools files
	mkdir -p ./output/rootfs/$(DIR_TARGET_TOOLS)
	cp -f ./tools/reg_analyzer/output/* ./output/rootfs/$(DIR_TARGET_TOOLS)
	cp -f ./test/csi/output/* ./output/rootfs/$(DIR_TARGET_TEST)
	@if [ `command -v tree` != "" ]; then \
	    tree ./output/rootfs -I 'sdk' | grep -v "\.json"; \
	    echo "INFO: The files above, has filter out the sdk folder and .json files"; \
	fi
	@echo $(BUILD_LOG_END)

install_rootfs: install_local_output
	@echo $(BUILD_LOG_START)
#	cp -rf output/rootfs/* $(INSTALL_DIR_ROOTFS)
	@echo $(BUILD_LOG_END)

clean_output:
	@echo $(BUILD_LOG_START)
	rm -rf ./output
	rm -rf $(INSTALL_DIR_ROOTFS)/$(DIR_TARGET_BASE)
	@echo $(BUILD_LOG_END)

clean: clean_output clean_driver clean_lib clean_test clean_tools

