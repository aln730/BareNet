# GNUmakefile for RayNet Kernel Project

MAKEFLAGS += -rR
.SUFFIXES:

override OUTPUT := RayNet

CC := cc

CFLAGS := -g -O2 -pipe

CPPFLAGS :=

NASMFLAGS := -F dwarf -g

# Linker flags (user-defined)
LDFLAGS :=

override CC_IS_CLANG := $(shell ! $(CC) --version 2>/dev/null | grep 'clang' >/dev/null 2>&1; echo $$?)

ifeq ($(CC_IS_CLANG),1)
    override CC += \
        -target x86_64-unknown-none
endif

# Required flags for kernel development (do not remove)
override CFLAGS += \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-PIC \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel

# Internal preprocessor flags for Limine + header deps
override CPPFLAGS := \
    -I src \
    $(CPPFLAGS) \
    -DLIMINE_API_REVISION=3 \
    -MMD \
    -MP

# Extra internal NASM flags (for 64-bit ELF output)
override NASMFLAGS += \
    -Wall \
    -f elf64

# Linker flags (must include linker.ld and disable standard linking)
override LDFLAGS += \
    -Wl,-m,elf_x86_64 \
    -Wl,--build-id=none \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -T linker.ld

# Gather all source files in src/ directory
override SRCFILES := $(shell cd src && find -L * -type f | LC_ALL=C sort)
override CFILES := $(filter %.c,$(SRCFILES))
override ASFILES := $(filter %.S,$(SRCFILES))
override NASMFILES := $(filter %.asm,$(SRCFILES))
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

# Default target: build final kernel ELF
.PHONY: all
all: bin/$(OUTPUT)

# Include header dependencies
-include $(HEADER_DEPS)

# Link final kernel ELF from object files
bin/$(OUTPUT): GNUmakefile linker.ld $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

# Compile C files
obj/%.c.o: src/%.c GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compile assembly files (.S)
obj/%.S.o: src/%.S GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Assemble NASM files (.asm)
obj/%.asm.o: src/%.asm GNUmakefile
	mkdir -p "$$(dirname $@)"
	nasm $(NASMFLAGS) $< -o $@

# Clean all build artifacts
.PHONY: clean
clean:
	rm -rf bin obj isodir RayNet.iso

# Build ISO image using Limine
ISO_ROOT := isodir
ISO_NAME := RayNet.iso

.PHONY: iso
iso: bin/$(OUTPUT)
	rm -rf $(ISO_ROOT)
	mkdir -p $(ISO_ROOT)/boot
	cp bin/$(OUTPUT) $(ISO_ROOT)/boot/RayNet
	cp limine.cfg limine.sys limine-cd.bin limine-eltorito-efi.bin $(ISO_ROOT)/boot/
	xorriso -as mkisofs \
		-b boot/limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot boot/limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_ROOT) -o $(ISO_NAME)

	./limine-deploy $(ISO_NAME)

# Run ISO in QEMU
.PHONY: run
run: iso
	qemu-system-x86_64 -cdrom $(ISO_NAME)