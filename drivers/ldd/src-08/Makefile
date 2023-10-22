obj-m += timer.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: 
	sudo insmod timer.ko
	sudo dmesg -c

unload:
	sudo rmmod timer.ko
	sudo dmesg -c

