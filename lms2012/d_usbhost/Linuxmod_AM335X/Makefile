ifneq ($(KERNELRELEASE),)

# kbuild part of makefile

obj-m += d_usbhost.o

else

# normal makefile

target = d_usbhost.ko

include $(mk_dir)/module.mk

endif
