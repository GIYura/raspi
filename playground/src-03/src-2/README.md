#### Connection

|LED pin | RPI pin  |    Physical   |
|--------|----------|---------------|
| +      |  GND     | 6/14/20/30/34 |
| -      |  GPIO21  |     40        |

#### Compile

```
make all
```

#### Usage

```
./test setup   - /* set up LED and direction */
./test on      - /* turn on LED */ 
./test off     - /* turn off LED */
./test close   - /* deinit LED */
```
