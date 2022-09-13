##
 # Copyright (C) 2021 Alibaba Group Holding Limited
 #
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License version 2 as
 # published by the Free Software Foundation.
##

SOURCES = test_csi.c
OBJECTS := $(SOURCES:.c=.o)
EXEC = output/test_csi
CFLAGS = -O0 -Wall -g -lm -I../../driver/csi

all: $(EXEC)

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

$(EXEC): $(OBJECTS)
	mkdir -p output
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS) -pthread

clean:
	rm -f *.o $(EXEC)
	rm -rf output

install:

.PHONY: clean all

