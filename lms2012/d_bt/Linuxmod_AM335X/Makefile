ifneq ($(KERNELRELEASE),)

# kbuild part of makefile

obj-m += d_bt.o

else

# normal makefile

target = d_bt.ko

include $(mk_dir)/module.mk

endif
