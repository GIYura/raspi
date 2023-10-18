obj-m += adxl345-i2c.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod adxl345-i2c.ko
	sudo dmesg -c

unload:
	sudo rmmod adxl345-i2c.ko
	sudo dmesg -c
	
