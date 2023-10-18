obj-m += uart-echo.o

all: module dt
	@echo 'Builded Device Tree Overlay and kernel module'

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

dt: uart-echo-overlay.dts
	dtc -@ -I dts -O dtb -o uart-echo-overlay.dtbo uart-echo-overlay.dts

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf uart-echo-overlay.dtbo

load:
	sudo insmod uart-echo.ko
	sudo dmesg -c

unload:
	sudo rmmod uart-echo.ko
	sudo dmesg -c
	
