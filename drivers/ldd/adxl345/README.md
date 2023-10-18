#### HOW-TO start

**NOTE:** ADXL345 is 3-axis accelerometer
with high resolution (13-bit) measurement at up to ±16 g.
It supports 2 interfaces:

- I2C;
- SPI;

##### I2C

1. Enter directory '/adxl345/driver/i2c/';

2. Run make to compile kernel module;

```
make
```
3. Run make load to load **.ko** file to the kernel;

```
make load
```
**NOTE:** to unload module run:
```
make unload
```

4. Verify device file:

```
ls /dev/adxl345 -la
```

##### TEST

1. Enter directory '/adxl345/test/';

2. Run make to compile application:

```
make
```
3. Run excutable:

```
sudo ./text /dev/adxl345
```

4. Application should read accelerometer ID and print it.

**NOTE:** ID is: 0xE5;

##### SPI

**Connection**

| ADXL345 pin | Raspi pin |
| ----------- | --------- |
| SDL         | SPICLK    |
| SDA         | MOSI      |
| SDO         | MISO      |
| CS          | SPICEO    |

1. Enter /adxl345/test/spi/
2. Run make to compile

```
make
```
3. Run executable 

```
./test
```
