#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>

#define PWM0 12 /**/

int main (int argc, char* argv[])
{
    wiringPiSetupGpio();
    pinMode(PWM0, PWM_OUTPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(128);
    pwmSetClock(15);
    
    while (1)
    {
        unsigned int i = 0;
        for (; i < 128; i++)
        {
            pwmWrite(PWM0, i);
            delay(10);
        }


        for (; i > 0; i--)
        {
            pwmWrite(PWM0, i);
            delay(10);
        }
    }

    return 0;
}

