ifneq ($(KERNELRELEASE),)

# kbuild part of makefile

obj-m += d_sound.o

else

# normal makefile

target = d_sound.ko

include ../../../mk/module.mk

endif
