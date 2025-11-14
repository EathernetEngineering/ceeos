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

SRCS := entry.c arch/x86/multiboot.S arch/x86/start.S arch/x86/boot_gdt.S
OBJS := $(addprefix $(objtree)/,$(addsuffix .o,$(SRCS)))
LIBS := libk

.PHONY: all ceek
all: ceek.x86_64
ceek: ceek.x86_64

ceek.x86_64: $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-Wl,-T,$(srctree)/ceek/link.ld,-z,max-page-size=4096 -o $@ $(OBJS)

$(objtree)/%.c.o: $(srctree)/ceek/%.c
	@mkdir -p $(dir $@)
	$(CC) -ffreestanding $(CFLAGS) -c -o $@ $<

$(objtree)/%.S.o: $(srctree)/ceek/%.S
	@mkdir -p $(dir $@)
	$(CC) -ffreestanding $(CFLAGS) -c -o $@ $<

.PHONY: clean mrproper
clean:
	rm -rf $(OBJS) ceek

mrproper: clean

