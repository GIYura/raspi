obj-m += sys-fs.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod sys-fs.ko
	sudo dmesg -c

unload:
	sudo rmmod sys-fs.ko
	sudo dmesg -c
	
