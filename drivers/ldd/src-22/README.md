#### Device tree usage

Device tree is a description of the HW.

File **.dts** is a device tree source file. It should be compiled with device tree compiler

```
dtc -@ -I dts -O dtb -o file-name.dtbo file-name.dts
```

To apply device tree:

```
sudo dtoverlay file-name.dtbo
```

Verify:

```
sudo ls /proc/device-tree
```

Test:

```
echo 1 > /proc/my-led
echo 0 > /proc/my-led
```

