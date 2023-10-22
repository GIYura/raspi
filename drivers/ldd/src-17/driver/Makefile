obj-m += gpio-irq-poll.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod gpio-irq-poll.ko
	sudo dmesg -c

unload:
	sudo rmmod gpio-irq-poll.ko
	sudo dmesg -c
	
