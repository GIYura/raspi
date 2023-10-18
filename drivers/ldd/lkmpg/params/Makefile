TARGET=params
obj-m += $(TARGET).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: 
	@sudo insmod $(TARGET).ko
	@sudo dmesg -c

unload:
	@sudo rmmod $(TARGET).ko
	@sudo dmesg -c

