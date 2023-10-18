#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>

#define DEBOUNCE_TIMEOUT    200
#define LED_GPIO            21
#define BUTTON_GPIO         17 

static void ledLight(void)
{
    printf("Inside interrupt handler\n");

    static unsigned int lastIsr = 0;
    unsigned int currentTime = millis();
    
    if (currentTime - lastIsr > DEBOUNCE_TIMEOUT)
    {  
        digitalWrite(LED_GPIO, HIGH);
    }

    lastIsr = currentTime;
}

int main(int argc, char* argv[])
{
    wiringPiSetupGpio();
    pinMode(LED_GPIO, OUTPUT);
    pinMode(BUTTON_GPIO, INPUT);

    wiringPiISR(BUTTON_GPIO, INT_EDGE_RISING, &ledLight);

    while (1)
    {   
        sleep(1); 
        digitalWrite(LED_GPIO, LOW);
    }

    return 0;
}
