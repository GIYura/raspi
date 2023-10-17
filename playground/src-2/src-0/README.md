#### Connection

|LED pin | RPI pin  |    Physical   |
|--------|----------|---------------|
| +      |  GND     | 6/14/20/30/34 |
| -      |  GPIO21  |     40        |


#### Compile

```
make all
```

#### Test

```
sudo ./test <timout-in-milliseconds>
```

**NOTE:** timeout 0 ms invalid

