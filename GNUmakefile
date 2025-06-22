MAKEFLAGS += -rR
.SUFFIXES:

OUTPUT := RayNet
CC := cc
CFLAGS := -g -O2 -pipe
CPPFLAGS :=
NASMFLAGS := -F dwarf -g
LDFLAGS :=

CC_IS_CLANG := $(shell ! $(CC) --version 2>/dev/null | grep 'clang' >/dev/null 2>&1; echo $$?)
ifeq ($(CC_IS_CLANG),1)
    CC += -target x86_64-unknown-none
endif

CFLAGS += -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-PIC \
          -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel
CPPFLAGS += -I src -DLIMINE_API_REVISION=3 -MMD -MP
NASMFLAGS += -Wall -f elf64
LDFLAGS += -Wl,-m,elf_x86_64 -Wl,--build-id=none -nostdlib -static -z max-page-size=0x1000 -T linker.ld

SRCFILES := $(shell cd src && find -L * -type f | LC_ALL=C sort)
CFILES := $(filter %.c,$(SRCFILES))
ASFILES := $(filter %.S,$(SRCFILES))
NASMFILES := $(filter %.asm,$(SRCFILES))
OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

ISO_ROOT := iso_root
ISO_NAME := image.iso
LIMINE_DIR := limine

.PHONY: all
all: bin/$(OUTPUT)

-include $(HEADER_DEPS)

bin/$(OUTPUT): linker.ld $(OBJ)
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

obj/%.c.o: src/%.c
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

obj/%.S.o: src/%.S
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

obj/%.asm.o: src/%.asm
	mkdir -p $$(dirname $@)
	nasm $(NASMFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf bin obj $(ISO_ROOT) $(ISO_NAME)

.PHONY: iso
iso: bin/$(OUTPUT)
	rm -rf $(ISO_ROOT)
	mkdir -p $(ISO_ROOT)/boot/limine
	mkdir -p $(ISO_ROOT)/EFI/BOOT
	cp bin/$(OUTPUT) $(ISO_ROOT)/boot/
	cp limine.conf $(ISO_ROOT)/
	cp $(LIMINE_DIR)/limine-bios.sys $(ISO_ROOT)/boot/limine/
	cp $(LIMINE_DIR)/limine-bios-cd.bin $(ISO_ROOT)/boot/limine/
	cp $(LIMINE_DIR)/limine-uefi-cd.bin $(ISO_ROOT)/boot/limine/
	cp $(LIMINE_DIR)/BOOTX64.EFI $(ISO_ROOT)/EFI/BOOT/
	cp $(LIMINE_DIR)/BOOTIA32.EFI $(ISO_ROOT)/EFI/BOOT/

	xorriso -as mkisofs -R -r -J \
		-b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_ROOT) -o $(ISO_NAME)

	./$(LIMINE_DIR)/limine bios-install $(ISO_NAME)

.PHONY: run
run: iso
	qemu-system-x86_64 -enable-kvm -m 512 -cdrom $(ISO_NAME) -boot d
