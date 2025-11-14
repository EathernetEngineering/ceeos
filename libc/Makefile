# SPDX-License-Identifier: MIT
# Copyright (c) 2025 Chloe Eather

ifneq ("$(origin O)","command line")
$(error Output directory not provided)
endif

ifneq ("$(origin srctree)","command line")
$(error srctree not provided)
endif

objtree := $(abspath $(O))

TARGET          ?= x86_64-elf
CROSS_COMPILE   ?= x86_64-elf-
BUILD_TYPE      ?= Release

COMMON_SOURCES := string.c
LIBC_SOURCES := $(COMMON_SOURCES)
LIBK_SOURCES := $(COMMON_SOURCES)

LIBC_OBJECTS := $(addprefix $(objtree)/,$(addsuffix .o,$(LIBC_SOURCES)))
LIBK_OBJECTS := $(addprefix $(objtree)/,$(addsuffix .o,$(LIBK_SOURCES)))

.PHONY: all libc libk
all: libc.a libk.a
libc: libc.a
libk: libk.a

libc.a: $(LIBC_OBJECTS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $(LIBC_OBJECTS)
libk.a: $(LIBK_OBJECTS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $(LIBK_OBJECTS)

$(objtree)/%.o: $(srctree)/libc/%
	@mkdir -p $(dir $@)
	$(CC) -ffreestanding -c $(CFLAGS) $< -o $@

.PHONY: clean mrproper
clean:
	rm -rf $(LIBC_OBJECTS) $(LIBK_OBJECTS) libc.a libk.a

mrproper: clean
	
