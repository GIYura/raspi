obj-m += dt-probe.o

all: module dt
	@echo 'Builded Device Tree Overlay and kernel module'

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

dt: testoverlay.dts
	dtc -@ -I dts -O dtb -o testoverlay.dtbo testoverlay.dts

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf testoverlay.dtbo

load:
	sudo insmod dt-probe.ko
	sudo dmesg -c

unload:
	sudo rmmod dt-probe.ko
	sudo dmesg -c


#obj-m += sys-fs.o

#KERNEL_VERSION := $(shell uname -r)

#KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

#all:
#    make -C $(KDIR) M=$(PWD) modules

#clean:
#	make -C $(KDIR) M=$(PWD) clean

#load:
#	sudo insmod sys-fs.ko
#	sudo dmesg -c

#unload:
#	sudo rmmod sys-fs.ko
#	sudo dmesg -c
	
