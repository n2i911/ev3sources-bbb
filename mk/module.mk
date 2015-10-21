#
# kernel module
#
install_target = $(target_out_modules)/$(target)

all: install

$(target):
	make -C $(kernel_dir) -j4 ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) V=$(VERVOSE) M=$$PWD

install: $(install_target)

$(install_target): $(target)
	mkdir -p $(dir $@) && cp $< $@

clean:
	make -C $(kernel_dir) -j4 ARCH=$(ARCH) CROSS_COMPILE=$(CROSS) \
		O=$(target_out_kernel) V=$(VERVOSE) M=$$PWD clean

uninstall:
	rm -f $(install_target)

.PHONY: all install clean uninstall
