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

SRCS := acpi.c kmain.c mmap.c panic.c \
		arch/x86/boot_gdt.S arch/x86/interrupts.c arch/x86/isr_stubs.S \
		arch/x86/load_seg.S arch/x86/multiboot.S arch/x86/segments.c \
		arch/x86/start.S \
		boot/parse_mb_info.c \
		io/kprint.c io/serial.c
HEADERS := acpi.h mmap.c panic.h \
		arch/x86/interrupts.h arch/x86/segments.h \
		boot/multiboot2.h boot/parse_mb_info.h \
		io/kprint.h io/port.h io/serial.h

OBJS := $(addprefix $(this-objtree)/,$(addsuffix .o,$(SRCS)))
LIBS := libk.a
INCLUDES := $(this-srctree) $(srctree)/libc/libk/include

.PHONY: all ceek
all: $(objtree)/ceek.x86_64
ceek: $(objtree)/ceek.x86_64

$(objtree)/ceek.x86_64: $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-Wl,-T,$(this-srctree)/link.ld,-z,max-page-size=4096 -o $@ $(OBJS) $(LIBS)

$(this-objtree)/%.c.o: $(this-srctree)/%.c
	@mkdir -p $(dir $@) $(objtree)/.deps
	$(CC) -ffreestanding $(CFLAGS) \
		-MF '$(objtree)/.deps/$(subst .o,,$(notdir $@)).d' \
		-c $(addprefix -I,$(INCLUDES)) -o $@ $<

$(this-objtree)/%.S.o: $(this-srctree)/%.S
	@mkdir -p $(dir $@) $(objtree)/.deps
	$(CC) -ffreestanding $(CFLAGS) \
		-MF '$(objtree)/.deps/$(subst .o,,$(notdir $@)).d' \
		-c $(addprefix -I,$(INCLUDES)) -o $@ $<

.PHONY: clean mrproper
clean:
	rm -rf $(OBJS) ceek.x86_64

-include $(DEPFILES)

mrproper: clean

