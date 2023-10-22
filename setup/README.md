#### Connection

|  FTDI | Raspberry  |
|-------|------------|
|  TX   |  RXD0      |
|  RX   |  TXD0      |
|  GND  |  GND       |

UART settings: 
- baudrate: 115200
- format: 8N1

Run the following command to find out which USB to use:
```
ls /dev/tty*
```
Run the following command to connect to the corresponding USB:
```
minicom -D /dev/ttyUSBx
```

#### Initial Raspberry setup

**NOTE:** default login/password: **pi/raspberry**.

**NOTE:** default credentials not suitable for all images.

Follow the link for more details [link](https://forums.raspberrypi.com/viewtopic.php?t=332712)

1. Write OS **.img** to flash. Can be downloaded [here](https://www.raspberrypi.com/software/operating-systems/)

2. To write **.img** balena [Etcher](https://etcher.balena.io/) can be used.

3. Enable uart

- Open **/boot/config.txt** add the line: 
```   
enable_uart=1
```
- Option. If necessary change **console=serial2 to console=serial0** in /boot/cmdline.txt

4. Put flash card to the raspberry and power cycle it.

5. Run the following command:
```
passwd
```
Follow instructions 

6. Add user

```
sudo adduser <user-name>
```
- Enter **new** password and follow instructions.

7. Add new user to sudo group

```
sudo usermod -a -G <group-to-add> <user-name>
```
Example: sudo usermod -a -G sudo jura

8. Add sudo access
   
- Log in as pi;
- sudo /etc/sudoers add **new** user
- reboot and login as a **new** user

9. Configuration

 ```
sudo raspi-config
```
Enable Wi-Fi connection and SSH server to get remote access.

10. Basic installation

Run the following commands:

```
sudo apt-get update
sudo apt-get install vim
sudo apt-get install git
sudo apt install build-essential
```

