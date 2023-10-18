obj-m += dt-gpio.o

all: module dt
	@echo 'Builded Device Tree Overlay and kernel module'

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

dt: testoverlay.dts
	dtc -@ -I dts -O dtb -o testoverlay.dtbo testoverlay.dts

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf testoverlay.dtbo

load:
	sudo insmod dt-gpio.ko
	sudo dmesg -c

unload:
	sudo rmmod dt-gpio.ko
	sudo dmesg -c
	
