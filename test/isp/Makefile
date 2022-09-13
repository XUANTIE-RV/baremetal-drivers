##
 # Copyright (C) 2021 Alibaba Group Holding Limited
 #
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License version 2 as
 # published by the Free Software Foundation.
##

# To support host build in current path
ifeq ($(CC),)
  CC:=gcc
endif

CFLAGS = -O0 -Wall -g -lm
INCLUDES = -I../../driver/isp -I../../common

EXEC1 = output/isp_run_bm_script


all: $(EXEC1)

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

prepare:
	mkdir -p output

SOURCES_COMMON = ../../common/log_common.c
OBJECTS_COMMON = log_common.o
common: $(SOURCES_COMMON)
	$(CC) $(CFLAGS) -c $<

SOURCES1 = isp_run_bm_script.c
OBJECTS1 = $(SOURCES1:.c=.o)
$(EXEC1): prepare common $(OBJECTS1)
	@echo OBJECTS1=$(OBJECTS1)
	$(CC) $(CFLAGS) -o $(EXEC1) $(OBJECTS_COMMON) $(OBJECTS1)
	cp -r isp_bm_scripts ./output/

clean:
	rm -f *.o
	rm -rf output

install:

.PHONY: clean all prepare common

