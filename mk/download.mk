# Create source and out directory
$(shell mkdir -p $(src_dir))
$(shell mkdir -p $(target_out))

# Check cross toolchain
filepath := $(shell which $(CROSS)gcc 2> /dev/null)
ifeq ($(strip $(filepath)),)
$(error No Toolchain $(CROSS)gcc)
endif

# Check kernel source
$(info check kernel source: $(kernel_dir))
filepath := $(shell ls $(kernel_dir) 2> /dev/null)
ifeq ($(strip $(filepath)),)
$(info No kernel source, download now...)
$(info $(shell $(fetch_kernel_cmd) $(kernel_dir)))
endif

# Check u-boot source
$(info check u-boot source: $(uboot_dir))
filepath := $(shell ls $(uboot_dir) 2> /dev/null)
ifeq ($(strip $(filepath)),)
$(info No u-boot source, download now...)
$(info $(shell $(fetch_uboot_cmd) $(uboot_dir)))
endif
