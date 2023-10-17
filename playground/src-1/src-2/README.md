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
./blink setup   - /* set up LED and direction */
./blink on      - /* turn on LED */ 
./blink off     - /* turn off LED */
./blink close   - /* deinit LED */
```
