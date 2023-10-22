obj-m += wait-queue.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod wait-queue.ko
	sudo dmesg -c

unload:
	sudo rmmod wait-queue.ko
	sudo dmesg -c
	
