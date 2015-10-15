build-uboot:
	mkdir -p $(target_out_bin)
	mkdir -p $(target_out_uboot)
	make -C $(uboot_dir) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_uboot) $(BOARD)
	make -C $(uboot_dir) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_uboot) V=$(VERVOSE)
	cp $(target_out_uboot)/$(target_uboot) $(target_out_bin)
	cp $(target_out_uboot)/$(target_uboot_spl) $(target_out_bin)

uboot-clean:
	make -C $(uboot_dir) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_uboot) clean
