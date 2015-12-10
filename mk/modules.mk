#
# kernel modules
#
all_modules = d_analog d_bt d_iic d_power d_pwm d_sound d_uart \
			  d_ui d_usbdev d_usbhost

modules: $(all_modules)

$(all_modules):
	make -C $(lms_dir)/$@/Linuxmod_AM335X

modules.clean: $(all_modules:%=%.clean)
	rm -rf stamp-modules

$(all_modules:%=%.clean): %.clean:
	make -C $(lms_dir)/$*/Linuxmod_AM335X clean uninstall

.PHONY: modules $(all_modules) modules.clean $(all_modules:%=%.clean)
