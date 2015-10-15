build-kernel:
	mkdir -p $(target_out_bin)
	mkdir -p $(target_out_kernel)
	cp $(kernel_config) $(target_out_kernel)/.config
	make -C $(kernel_dir) -j4 ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) V=$(VERVOSE) zImage dtbs modules
	cp $(target_out_kernel)/arch/$(ARCH)/boot/$(target_kernel) $(target_out_bin)
	cp -ra $(target_out_kernel)/arch/$(ARCH)/boot/dts/ $(target_out_bin)

kernel-clean:
	make -C $(kernel_dir) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) clean
