obj-m += gpio-irq.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod gpio-irq.ko
	sudo dmesg -c

unload:
	sudo rmmod gpio-irq.ko
	sudo dmesg -c
	
