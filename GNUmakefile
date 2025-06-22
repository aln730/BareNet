# GNUmakefile for RayNet Kernel Project (UEFI with Limine v9.x)

MAKEFLAGS += -rR
.SUFFIXES:

# Kernel output name
OUTPUT := RayNet

# Compiler and flags
CC := cc
CFLAGS := -g -O2 -pipe
CPPFLAGS :=
NASMFLAGS := -F dwarf -g
LDFLAGS :=

# Detect Clang and set target
CC_IS_CLANG := $(shell ! $(CC) --version 2>/dev/null | grep 'clang' >/dev/null 2>&1; echo $$?)
ifeq ($(CC_IS_CLANG),1)
    CC += -target x86_64-unknown-none
endif

# Kernel-specific flags
CFLAGS += \
    -Wall -Wextra \
    -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC \
    -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone \
    -mcmodel=kernel

CPPFLAGS += -I src -DLIMINE_API_REVISION=3 -MMD -MP
NASMFLAGS += -Wall -f elf64
LDFLAGS += \
    -Wl,-m,elf_x86_64 \
    -Wl,--build-id=none \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -T linker.ld

# Discover source files
SRCFILES := $(shell cd src && find -L * -type f | LC_ALL=C sort)
CFILES := $(filter %.c,$(SRCFILES))
ASFILES := $(filter %.S,$(SRCFILES))
NASMFILES := $(filter %.asm,$(SRCFILES))
OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

# Limine, ISO paths
LIMINE_DIR := limine
ISO_ROOT := isodir
ISO_NAME := RayNet.iso

# Default target
.PHONY: all
all: bin/$(OUTPUT)

# Auto dependency inclusion
-include $(HEADER_DEPS)

# Link final ELF kernel
bin/$(OUTPUT): GNUmakefile linker.ld $(OBJ)
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

# Compile C
obj/%.c.o: src/%.c
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compile .S
obj/%.S.o: src/%.S
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Assemble .asm
obj/%.asm.o: src/%.asm
	mkdir -p $$(dirname $@)
	nasm $(NASMFLAGS) $< -o $@

# Clean
.PHONY: clean
clean:
	rm -rf bin obj $(ISO_ROOT) $(ISO_NAME)

# ISO image
.PHONY: iso
iso: bin/$(OUTPUT)
	rm -rf $(ISO_ROOT)
	mkdir -p $(ISO_ROOT)/boot
	mkdir -p $(ISO_ROOT)/EFI/BOOT
	cp bin/$(OUTPUT) $(ISO_ROOT)/boot/RayNet
	cp limine.cfg $(ISO_ROOT)/
	cp $(LIMINE_DIR)/limine-bios.sys $(ISO_ROOT)/boot/limine.sys
	cp $(LIMINE_DIR)/BOOTX64.EFI $(ISO_ROOT)/EFI/BOOT/
	cp $(LIMINE_DIR)/limine-uefi-cd.bin $(ISO_ROOT)/boot/

	xorriso -as mkisofs \
		-b boot/limine-uefi-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		$(ISO_ROOT) -o $(ISO_NAME)

	$(LIMINE_DIR)/limine bios-install $(ISO_NAME)

# Run on QEMU
.PHONY: run
run: iso
	qemu-system-x86_64 -cdrom $(ISO_NAME)
