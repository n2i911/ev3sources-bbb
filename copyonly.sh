#!/bin/bash

rm -rf ~/working/beaglebone/tftpboot/*
cp ./bbb/out/bin/dtbs/am335x-boneblack.dtb ~/working/beaglebone/tftpboot/
cp ./bbb/out/bin/zImage ~/working/beaglebone/tftpboot/

sudo cp -ra ./bbb/out/bin/modules/lib/*  ~/working/beaglebone/rootfs/lib/
sudo cp -ra ./bbb/out/bin/lms2012/lib/modules/*.ko ~/working/beaglebone/rootfs/lib/modules/
sudo cp -ra ./bbb/out/bin/lms2012/usr/* ~/working/beaglebone/rootfs/usr/
