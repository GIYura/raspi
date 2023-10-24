#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define GPIO_NUMBER "21"
#define GPIO21_PATH "/sys/class/gpio/gpio21/"
#define GPIO_SYSFS "/sys/class/gpio/"

static bool GpioWrite(char* const filename, const char* const value);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Invalid number of arguments\n");
        return 1;
    }

    if (strcmp(argv[1], "setup") == 0)
    {
        printf("Setting up the LED on GPIO\n");
        if (!GpioWrite(GPIO_SYSFS "export", GPIO_NUMBER))
        {
            printf("Can't export LED\n");
            return 2;
        }
        sleep(1);
        if (!GpioWrite(GPIO21_PATH "direction", "out"))
        {
            printf("Can't setup LED direction\n");
            return 3;
        }
    }
    else if (strcmp(argv[1], "close") == 0)
    {
        printf("Closing LED on GPIO\n");
        if (!GpioWrite(GPIO_SYSFS "unexport", GPIO_NUMBER))
        {
            printf("Can't unexport LED\n");
            return 4;
        }
    }
    else if (strcmp(argv[1], "on") == 0)
    {
        printf("Turning the LED on\n");
        if (!GpioWrite(GPIO21_PATH "value", "1"))
        {
            printf("Can't turn on LED\n");
            return 5;
        }
    }
    else if (strcmp(argv[1], "off") == 0)
    {
        printf("Turning the LED off\n");
        if (!GpioWrite(GPIO21_PATH "value", "0"))
        {
            printf("Can't turn off LED\n");
            return 6;
        }
    }
    else
    {
        printf("Unknown command!\n");
        return 7;
    }

    return 0;
}

static bool GpioWrite(char* const filename, const char* const value)
{
    FILE* fp;                           /* create a file pointer fp */

    fp = fopen(filename, "w");          /* open file for writing */
    if (NULL == fp)
    {
        printf("Can't open file: %s\n", filename);
        return false;
    }
    fprintf(fp, "%s", value);           /* write value to the file */
    fclose(fp);                         /* close file using fp */

    return true;
}

