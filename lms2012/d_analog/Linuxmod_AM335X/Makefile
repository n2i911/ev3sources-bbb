ifneq ($(KERNELRELEASE),)

# kbuild part of makefile

obj-m += d_analog.o

else

# normal makefile

target = d_analog.ko

include $(mk_dir)/module.mk

endif
