obj-m += test.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: 
	sudo insmod test.ko
	sudo dmesg -c

unload:
	sudo rmmod test.ko
	sudo dmesg -c

