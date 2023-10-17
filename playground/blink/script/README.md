#### Connection

|LED pin | RPI pin  |    Physical   |
|--------|----------|---------------|
| +      |  GND     | 6/14/20/30/34 |
| -      |  GPIO07  |     26        |

#### HOW-TO run blink service.

1. Copy blink.sh to /usr/local/sbin

```
sudo cp blink.sh /usr/local/sbin
```

2. Copy blink.service to /etc/systemd/system

```
sudo cp blink.service /etc/systemd/system
```

3. Run blink service

```
sudo systemctl enable blink.service
sudo systemctl start blink.service
```

4. Check blink service status

```
systemctl status blink.service
```
**NOTE:** LED should be connected to GPIO7.

