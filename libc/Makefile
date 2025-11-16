# SPDX-License-Identifier: MIT
# Copyright (c) 2025 Chloe Eather

ifneq ("$(origin O)","command line")
$(error Output directory not provided)
endif

ifneq ("$(origin srctree)","command line")
$(error srctree not provided)
endif

ifneq ("$(origin objtree)","command line")
$(error objtree not provided)
endif

this-objtree := $(objtree)/$(O)
this-srctree := $(srctree)/ceek

TARGET          ?= x86_64-elf
CROSS_COMPILE   ?= x86_64-elf-
BUILD_TYPE      ?= Release

COMMON_SOURCES := string.c
COMMON_HEADERS := string.h stdint.h limits.h
LIBC_SOURCES := $(COMMON_SOURCES)
LIBC_HEADERS := $(COMMON_HEADERS)
LIBK_SOURCES := $(COMMON_SOURCES)
LIBK_HEADERS := $(COMMON_HEADERS)

INCLUDES := $(srctree)/libc

LIBC_OBJECTS := $(addprefix $(this-objtree)/,$(addsuffix .o,$(LIBC_SOURCES)))
LIBK_OBJECTS := $(addprefix $(this-objtree)/,$(addsuffix .o,$(LIBK_SOURCES)))

.PHONY: all libc libk
all: libc.a libk.a
libc: $(objtree)/libc.a
libk: $(objtree)/libk.a

$(objtree)/libc.a: $(LIBC_OBJECTS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $(LIBC_OBJECTS)
$(objtree)/libk.a: $(LIBK_OBJECTS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $(LIBK_OBJECTS)

$(this-objtree)/%.c.o: $(srctree)/libc/%.c
	@mkdir -p $(dir $@) $(objtree)/.deps
	$(CC) -ffreestanding $(CFLAGS) \
		-MF '$(objtree)/.deps/$(subst .o,,$(notdir $@)).d' \
		-c $(addprefix -I,$(INCLUDES)) -o $@ $<

.PHONY: clean mrproper
clean:
	rm -rf $(LIBC_OBJECTS) $(LIBK_OBJECTS) libc.a libk.a

mrproper: clean
	
