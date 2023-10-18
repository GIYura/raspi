#### HOW-TO start

**NOTE:** The DS3231 is a clock and calendar chip. 

##### Connection

| DS pin      | Raspberry pin |
| :---        | :---          |
| VDD         | +3.3V         |
| GND         | GND           |
| SCL         | SCL1          |
| SDA         | SDA1          |

##### Driver

- Enter driver dir;
- Run make to build ds3231 driver;

```
make
```
- Load **.ko** file
```
make load
```

##### Test

- Enter test dir;
- Run make build executable
```
make
```
- Run executable
```
sudo ./test /dev/ds3231 SET
sudo ./test /dev/ds3231 GET
```

**NOTE:** SET allows write system time to RTC

