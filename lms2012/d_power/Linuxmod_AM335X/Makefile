ifneq ($(KERNELRELEASE),)

# kbuild part of makefile

obj-m += d_power.o

else

# normal makefile

target = d_power.ko

include ../../../mk/module.mk

endif
