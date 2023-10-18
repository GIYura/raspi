obj-m += pcf8583.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod pcf8583.ko
	sudo dmesg -c

unload:
	sudo rmmod pcf8583.ko
	sudo dmesg -c
	
