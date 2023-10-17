#### Initial Raspberry setup

**NOTE:** default login/password: **pi/raspberry**

1. Write OS **.img** to flash. Can be downloaded [here](https://www.raspberrypi.com/software/operating-systems/)

2. Enable uart

- Open **/boot/config.txt** add the line: 
```   
enable_uart=1
```
- Option. If necessary change **console=serial2 to console=serial0** in /boot/cmdline.txt

3. Change password

- Open the command line, type **passwd**, and press “Enter.”
- Enter **current** password.
- Enter **new** password.
- Confirm **new** password.

4. Change user

```
sudo adduser <user-name>
```
- Enter **new** password.

5. Add new user to sudo group

```
sudo usermod -a -G <group-to-add> <user-name>
```
Example: sudo usermod -a -G sudo jura

6. Add sudo access
   
- Log in as pi;
- sudo /etc/sudoers add **new** user
- reboot and login as a **new** user

7. Configuration

 ```
sudo raspi-config
```

