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
INCLUDES = -I../../common -I./

SOURCES := ../../common/log_common.c
SOURCES += ra_common.c
SOURCES += ra_light_main.c
SOURCES += ra_reg_demo.c
SOURCES += ra_light_visys.c
SOURCES += ra_light_isp.c
SOURCES += $(wildcard vsi_isp8000/*.c)


OBJECTS := $(SOURCES:.c=.o)
EXEC = output/reg_analyzer
CFLAGS = -O0 -g -Wall -lm

all: $(EXEC)

$(EXEC): $(OBJECTS)
	@mkdir -p output
	$(CC) $(CFLAGS) -o $(EXEC) .obj/*.o

$(OBJECTS):%.o:%.c
	@mkdir -p .obj
	$(CC) -c $(CFLAGS) $(INCLUDES) -o .obj/$(notdir $@) $<

clean:
	rm -rf .obj
	rm -rf output

install:

.PHONY: clean all

