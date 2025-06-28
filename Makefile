ASM=nasm
CXX=g++
LD=ld

CXXFLAGS=-ffreestanding -m32 -O2 -Wall -Wextra -fno-rtti -fno-exceptions
LDFLAGS=-m elf_i386 -Ttext 0x100000

BUILD_DIR=build
ISO_DIR=$(BUILD_DIR)/iso
GRUB_DIR=$(ISO_DIR)/boot/grub

.PHONY: all iso run clean

all: iso

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/boot.o: boot.s | $(BUILD_DIR)
	$(ASM) -f elf32 $< -o $@

# kernel.o will compile kernel.cpp and implicitly include stack.hpp
$(BUILD_DIR)/kernel.o: kernel/kernel.cpp kernel/data_structures/stack.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o
	$(LD) $(LDFLAGS) -o $@ $^

$(GRUB_DIR):
	mkdir -p $@

$(ISO_DIR)/boot/kernel.elf: $(BUILD_DIR)/kernel.elf | $(GRUB_DIR)
	cp $< $@
	cp grub/grub.cfg $(GRUB_DIR)/grub.cfg

iso: $(ISO_DIR)/boot/kernel.elf
	grub2-mkrescue -o $(BUILD_DIR)/myos.iso $(ISO_DIR)

run: iso
	qemu-system-x86_64 -cdrom $(BUILD_DIR)/myos.iso

clean:
	rm -rf build
