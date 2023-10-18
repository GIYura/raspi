obj-m += adxl345-rpi4.o
#obj-m += adxl345-spi.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod adxl345-rpi4.ko
#	sudo insmod adxl345-spi.ko
	sudo dmesg -c

unload:
	sudo rmmod adxl345-rpi4.ko
#	sudo rmmod adxl345-spi.ko
	sudo dmesg -c
	
