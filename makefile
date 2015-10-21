#
# This makefile would help to build the kernel, bootloader, lms2012, modules, etc.
#

root_dir := $(shell pwd)
export root_dir

include mk/version.mk
include mk/defs.mk
include mk/download.mk

.PHONY: all clean distclean help doc doc.view doc.clean \
	programs programs.clean lms2012 lms2012.clean stamp-modules

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

# doc
doc:
	make -C $(first_dir) -f Makefile $@

doc.view:
	make -C $(first_dir) -f Makefile $@

doc.clean:
	make -C $(first_dir) -f Makefile $@

# programs
programs:
	make -C $(first_dir) -f Makefile $@

programs.clean:
	make -C $(first_dir) -f Makefile $@

# lms2012 program

lms2012:
	make -C $(first_dir) -f Makefile $@

lms2012.clean:
	make -C $(first_dir) -f Makefile $@

# lms2012 kernel modules
include mk/modules.mk
stamp-modules:
	mkdir -p $(target_out_bin)
	mkdir -p $(target_out_kernel)
	cp $(kernel_config) $(target_out_kernel)/.config
	make -C $(kernel_dir) -j4 ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) V=$(VERVOSE) oldconfig modules_prepare
	make modules

clean: distclean-kernel distclean-uboot doc.clean programs.clean lms2012.clean
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
	@echo "# modules:"
	@echo "  make stamp-modules"
	@echo ""
	@echo "# rebuild all:"
	@echo "  make clean; make"
	@echo ""
	@echo "# remove source and rebuild all:"
	@echo "  make distclean; make"
	@echo ""
	@make -C $(first_dir) -f Makefile help
	@echo ""
	@echo "# display help:"
	@echo "  make hlep"
	@echo ""
