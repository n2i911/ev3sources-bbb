#
# Common
#
ARCH := arm
CROSS := arm-none-linux-gnueabi-
VERVOSE := 0

# This is speific to u-boot
BOARD := am335x_boneblack_defconfig

# source directory
src_dir := $(root_dir)/bbb/src
uboot_dir := $(src_dir)/$(uboot_version)
kernel_dir := $(src_dir)/$(kernel_version)

# lms2012 directory
lms_dir := $(root_dir)/lms2012
first_dir := $(lms_dir)/open_first

# output directory
target_out := $(root_dir)/bbb/out
target_out_uboot := $(target_out)/$(uboot_version)
target_out_kernel := $(target_out)/$(kernel_version)
target_out_bin := $(target_out)/bin

# config directory
config_dir := $(root_dir)/bbb/config

kernel_config := $(config_dir)/kernel/bb.org_defconfig

# bin files
target_kernel := zImage
target_uboot := u-boot.bin
target_uboot_spl := MLO
