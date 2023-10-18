#include <wiringPi.h>
#include <stdio.h> 
#include <stdlib.h> 

#define LED_GPIO    21  /* BCM = gpio21; wiringPi = gpio29 */

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Invalid argument\n");
        return 1;
    }

    unsigned int blinkPeriod = atoi(argv[1]);

    if (blinkPeriod == 0)
    {
        printf("Invalid blink period\n");
        return 2;
    }

    wiringPiSetupGpio();
    pinMode(LED_GPIO, OUTPUT);

    printf("LED blinking ...\n");

    while (1)
    {
        digitalWrite(LED_GPIO, HIGH);

        delayMicroseconds(blinkPeriod * 1000);

        digitalWrite(LED_GPIO, LOW);

        delayMicroseconds(blinkPeriod * 1000);
    }
    
    return 0;
}
