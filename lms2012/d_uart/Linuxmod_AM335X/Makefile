ifneq ($(KERNELRELEASE),)

# kbuild part of makefile

obj-m += d_uart.o

else

# normal makefile

target = d_uart.ko

include $(mk_dir)/module.mk

endif
