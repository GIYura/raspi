### This LKM is for button IRQ test
#### Connection
One button terminal should be connected to 3.3V, another one to GPIO.

**NOTE:** GPIO17 is used as button terminal

Setup:
1. Compile the driver. 
```    
make
```
2. Load **.ko** file into the kernel.
```
make load
```
3. Create interface to the driver (device file).
```
sudo mknod /dev/irq-sig c 64 0
```
4. Compile test.c
```
gcc test.c -o test
```
5. Run test
```
./test
```
6. Press the button

7. Unload the driver.
```
make unload
```
8. Delete device file
```
rm /dev/irq-sig
```

