ifneq ($(KERNELRELEASE),)

# kbuild part of makefile
obj-m += d_iic.o

else

# normal makefile

TARGET = d_iic.ko

CONF = Linuxmod
ARCH = AM335X

include ../../open_first/rules.mk

endif
