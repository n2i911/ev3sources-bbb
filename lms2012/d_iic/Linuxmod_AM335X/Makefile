ifneq ($(KERNELRELEASE),)

# kbuild part of makefile

obj-m += d_iic.o

else

# normal makefile

target  = d_iic.ko

include ../../../mk/module.mk

endif
