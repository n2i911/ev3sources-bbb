#
# Common
#
ARCH := arm
CROSS := arm-linux-gnueabihf-
VERVOSE := 0
export ARCH
export CROSS
export VERVOSE

# This is speific to u-boot
BOARD := am335x_boneblack_defconfig

# source directory
src_dir := $(root_dir)/bbb/src
uboot_dir := $(src_dir)/$(uboot_version)
kernel_dir := $(src_dir)/$(kernel_version)
export kernel_dir

mk_dir := $(root_dir)/mk
export mk_dir

# lms2012 directory
lms_dir := $(root_dir)/lms2012
first_dir := $(lms_dir)/open_first

# output directory
target_out := $(root_dir)/bbb/out
target_out_uboot := $(target_out)/$(uboot_version)
target_out_kernel := $(target_out)/$(kernel_version)
target_out_bin := $(target_out)/bin
target_out_kernel_firmware := $(target_out_bin)/firmware
target_out_kernel_dtbs := $(target_out_bin)/dtbs
target_out_kernel_modules := $(target_out_bin)/modules
target_out_modules := $(lms_dir)/lms2012/Linux_AM335X/sys/mod
export target_out_kernel
export target_out_modules

# config directory
config_dir := $(root_dir)/bbb/config

kernel_config := $(config_dir)/kernel/bb.org_defconfig

# bin files
target_kernel := zImage
target_uboot := u-boot.bin
target_uboot_spl := MLO
