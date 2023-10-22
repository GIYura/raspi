###Serial (UART) Driver

For this driver you need an UART-to-USB Adpter to receive and send characters to the Linux Driver.

1. Run **sudo raspi-config**, go to Interfaces -> Serial and Disable Login Shell over SSH but enable Serial Port.

2. Copy uart-echo-overlay.dtbo to /boot/overlays
```
cp uart-echo-overlay.dtbo /boot/overlays
```

3. In the file /boot/config.txt add the following line:
```
dtoverlays=uart-echo-overlay
```

