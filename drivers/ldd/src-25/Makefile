obj-m += dt-iio.o

all: module dt
	@echo 'Builded Device Tree Overlay and kernel module'

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

dt: dt-iio-overlay.dts
	dtc -@ -I dts -O dtb -o dt-iio-overlay.dtbo dt-iio-overlay.dts

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf dt-iio-overlay.dtbo

load:
	sudo insmod dt-iio.ko
	sudo dmesg -c

unload:
	sudo rmmod dt-iio.ko
	sudo dmesg -c
	
