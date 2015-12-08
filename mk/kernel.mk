build-kernel: $(target_out_kernel)/include/generated/utsrelease.h
	mkdir -p $(target_out_bin)
	mkdir -p $(target_out_kernel)
	mkdir -p $(target_out_kernel_firmware)
	mkdir -p $(target_out_kernel_dtbs)
	cp $(kernel_config) $(target_out_kernel)/.config
	make -C $(kernel_dir) -j4 ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) V=$(VERVOSE) zImage dtbs modules
	$(eval kernel_uts := $(shell cat $(target_out_kernel)/include/generated/utsrelease.h | awk '{print $$3}' | sed 's/\"//g' ))
	make -C $(kernel_dir) -j4 ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) V=$(VERVOSE) INSTALL_MOD_PATH=$(target_out_kernel_modules) modules_install
	make -C $(kernel_dir) -j4 ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) V=$(VERVOSE) INSTALL_FW_PATH=$(target_out_kernel_firmware) firmware_install
	make -C $(kernel_dir) -j4 ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) V=$(VERVOSE) INSTALL_DTBS_PATH=$(target_out_kernel_dtbs) dtbs_install
	cd $(target_out_kernel_modules) && tar czf ../$(kernel_uts)-modules.tar.gz * && cd -
	cd $(target_out_kernel_firmware) && tar czf ../$(kernel_uts)-firmware.tar.gz * && cd -
	cd $(target_out_kernel_dtbs) && tar czf ../$(kernel_uts)-dtbs.tar.gz * && cd -
	cp $(target_out_kernel)/arch/$(ARCH)/boot/$(target_kernel) $(target_out_bin)
	cp $(target_out_kernel)/arch/$(ARCH)/boot/$(target_kernel) $(target_out_bin)/$(kernel_uts).$(target_kernel)

$(target_out_kernel)/include/generated/utsrelease.h:
	mkdir -p $(target_out_bin)
	mkdir -p $(target_out_kernel)
	mkdir -p $(target_out_kernel_firmware)
	mkdir -p $(target_out_kernel_dtbs)
	cp $(kernel_config) $(target_out_kernel)/.config
	make -C $(kernel_dir) -j4 ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) V=$(VERVOSE) modules_prepare

kernel-clean:
	make -C $(kernel_dir) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) clean
