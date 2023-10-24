obj-m += driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: 
	sudo insmod driver.ko
	sudo dmesg -c

unload:
	sudo rmmod driver.ko
	sudo dmesg -c

