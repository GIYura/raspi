obj-m += ioctl_ex.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod ioctl_ex.ko
	sudo dmesg -c

unload:
	sudo rmmod ioctl_ex.ko
	sudo dmesg -c
	
