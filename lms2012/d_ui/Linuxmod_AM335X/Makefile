ifneq ($(KERNELRELEASE),)

# kbuild part of makefile

obj-m += d_ui.o

else

# normal makefile

target = d_ui.ko

include $(mk_dir)/module.mk

endif
