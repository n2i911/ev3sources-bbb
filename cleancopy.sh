#!/bin/bash

rm -rf ~/working/beaglebone/tftpboot/*
cp ./bbb/out/bin/dtbs/am335x-boneblack.dtb ~/working/beaglebone/tftpboot/
cp ./bbb/out/bin/zImage ~/working/beaglebone/tftpboot/

sudo rm -rf ~/working/beaglebone/rootfs
mkdir ~/working/beaglebone/rootfs

#
# debian-based root filesystem
#
#sudo tar xzf /home/sean/working/beaglebone-image-builder/out/bin/debian.rootfs.tgz -C ~/working/beaglebone/rootfs
#sudo chmod -R 644 ~/working/beaglebone/rootfs/etc/shadow*
#sudo chmod -R 644 ~/working/beaglebone/rootfs/etc/sudoers 
#sudo chmod -R 644 ~/working/beaglebone/rootfs/etc/sudoers.d/
#sudo chmod    777 ~/working/beaglebone/rootfs/var/lib/
#sudo chmod -R 777 ~/working/beaglebone/rootfs/var/lib/sudo/
#sudo chmod    755 ~/working/beaglebone/rootfs/root/

#
# scratch root filesystem
#
sudo tar xzf /home/sean/working/beaglebone-image-builder/out/bin/scratch.rootfs.tgz -C ~/working/beaglebone/rootfs
sudo cp -ra ~/working/toolchain/gcc-linaro-4.9-2015.02-3-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib/* ~/working/beaglebone/rootfs/lib/

sudo cp -ra ./bbb/out/bin/modules/lib/*  ~/working/beaglebone/rootfs/lib/
sudo cp -ra ./bbb/out/bin/lms2012/lib/modules/*.ko ~/working/beaglebone/rootfs/lib/modules/
sudo cp -ra ./bbb/out/bin/lms2012/usr/* ~/working/beaglebone/rootfs/usr/
