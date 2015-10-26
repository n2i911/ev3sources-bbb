ifneq ($(KERNELRELEASE),)

# kbuild part of makefile

obj-m += d_pwm.o

else

# normal makefile

target = d_pwm.ko

include $(mk_dir)/module.mk

endif
