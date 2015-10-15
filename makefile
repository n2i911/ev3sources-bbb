#
# This makefile would help to build the kernel, bootloader, lms2012, modules, etc.
#

root_dir := $(shell pwd)

include mk/version.mk
include mk/defs.mk
include mk/download.mk

.PHONY: all clean distclean help

all: stamp-uboot stamp-kernel

# u-boot
include mk/uboot.mk
stamp-uboot:
	make build-uboot
	touch $@

clean-uboot:
	make uboot-clean
	rm -rf stamp-uboot

distclean-uboot:
	rm -rf $(target_out_uboot) stamp-uboot

# kernel
include mk/kernel.mk
stamp-kernel:
	make build-kernel
	touch $@

clean-kernel:
	make kernel-clean
	rm -rf stamp-kernel

distclean-kernel:
	rm -rf $(target_out_kernel) stamp-kernel

clean: distclean-kernel distclean-uboot
	rm -rf $(target_out)

distclean: clean
	rm -rf $(src_dir)

help:
	@echo ""
	@echo ""
	@echo "                 ev3-bbb                  "
	@echo "=========================================="
	@echo "# kernel:"
	@echo "  make clearn-kernel; make stamp-kernel"
	@echo ""
	@echo "# uboot:"
	@echo "  make clean-uboot; make stamp-uboot"
	@echo ""
	@echo "# rebuild all:"
	@echo "  make clean; make"
	@echo ""
	@echo "# remove source and rebuild all:"
	@echo "  make distclean; make"
	@echo ""
	@echo "# display help:"
	@echo "  make hlep"
	@echo ""
