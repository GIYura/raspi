#### UART Loop-back test

#### Connection
**/dev/serial0** connected to pin GPIO14(TXD0) short GPIO15(RXD0)

#### Settings

1. Add **enable_uart=1** to /boot/config.txt;
2. Delete **serisl0** in /boot/cmdline.txt;

#### Compile
```
make
```
#### Run
```
./test
```
