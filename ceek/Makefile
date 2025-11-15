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

SRCS := entry.c io/kprint.c io/serial.c arch/x86/multiboot.S arch/x86/start.S arch/x86/boot_gdt.S
HEADERS := io/kprint.h io/serial.h io/port.h
OBJS := $(addprefix $(objtree)/,$(addsuffix .o,$(SRCS)))
HEADER_DEP := $(addprefix $(srctree)/ceek/,$(HEADERS))
LIBS := libk.a

.PHONY: all ceek
all: ceek.x86_64
ceek: ceek.x86_64

ceek.x86_64: $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-Wl,-T,$(srctree)/ceek/link.ld,-z,max-page-size=4096 -o $@ $(OBJS) $(LIBS)

$(objtree)/%.c.o: $(srctree)/ceek/%.c $(HEADER_DEP) 
	@mkdir -p $(dir $@)
	$(CC) -ffreestanding $(CFLAGS) -c -o $@ $<

$(objtree)/%.S.o: $(srctree)/ceek/%.S $(HEADER_DEP)
	@mkdir -p $(dir $@)
	$(CC) -ffreestanding $(CFLAGS) -c -o $@ $<

.PHONY: clean mrproper
clean:
	rm -rf $(OBJS) ceek

mrproper: clean

