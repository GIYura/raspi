obj-m += lcd.o
#obj-m += lcd-1.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod lcd.ko
	sudo dmesg -c

unload:
	sudo rmmod lcd.ko
	sudo dmesg -c
	
