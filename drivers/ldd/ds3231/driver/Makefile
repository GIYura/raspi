obj-m += ds3231.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod ds3231.ko
	sudo dmesg -c

unload:
	sudo rmmod ds3231.ko
	sudo dmesg -c
	
