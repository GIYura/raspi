obj-m += bmp280-spi.o

KERNEL_VERSION := $(shell uname -r)

KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod bmp280-spi.ko
	sudo dmesg -c

unload:
	sudo rmmod bmp280-spi.ko
	sudo dmesg -c
	
