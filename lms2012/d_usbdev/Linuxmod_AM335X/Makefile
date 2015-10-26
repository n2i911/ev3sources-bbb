ifneq ($(KERNELRELEASE),)

# kbuild part of makefile

obj-m += d_usbdev.o

else

# normal makefile

target = d_usbdev.ko

include $(mk_dir)/module.mk

endif
