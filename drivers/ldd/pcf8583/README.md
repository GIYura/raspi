#### HOW-TO start

**NOTE:** The PCF8583 is a clock and calendar chip. 

##### Connection

| PCF pin     | Raspberry pin |
| :---        | :---          |
| VDD         | +3.3V         |
| GND         | GND           |
| SCL         | SCL1          |
| SDA         | SDA1          |

##### Driver

- Enter driver dir;
- Run make to build pcf driver;

```
make
```
- Load **.ko** file
```
make load
```

##### Test

- Enter test dir;
- Run make to build executable
```
make
```
- Run executable
```
sudo ./test /dev/pcf8583 SET
sudo ./test /dev/pcf8583 GET
```

**NOTE:** SET allows write system time to RTC

