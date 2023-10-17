#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GPIO_NUMBER "21"
#define GPIO21_PATH "/sys/class/gpio/gpio21/"
#define GPIO_SYSFS "/sys/class/gpio/"

void writeGPIO(char filename[], char value[])
{
    FILE* fp;                           // create a file pointer fp
    printf("%s\n", filename);
    fp = fopen(filename, "w+");         // open file for writing
    fprintf(fp, "%s", value);           // send the value to the file
    fclose(fp);                         // close the file using fp
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {                        // program name is argument 1
        printf("Invalid number of arguments\n");
        return 2;                        // invalid number of arguments
    }
    printf("Starting the makeLED program\n");

    if (strcmp(argv[1],"setup") == 0)
    {
        printf("Setting up the LED on the GPIO\n");
        writeGPIO(GPIO_SYSFS "export", GPIO_NUMBER);
        usleep(100000);                  // sleep for 100ms
        writeGPIO(GPIO21_PATH "direction", "out");
    }
    else if (strcmp(argv[1],"close") == 0)
    {
        printf("Closing the LED on the GPIO\n");
        writeGPIO(GPIO_SYSFS "unexport", GPIO_NUMBER);
    }
    else if (strcmp(argv[1],"on") == 0)
    {
        printf("Turning the LED on\n");
        writeGPIO(GPIO21_PATH "value", "1");
    }
    else if (strcmp(argv[1],"off") == 0)
    {
        printf("Turning the LED off\n");
        writeGPIO(GPIO21_PATH "value", "0");
    }
    else
    {
        printf("Invalid command!\n");
    }

    printf("Finished the makeLED Program\n");

    return 0;
}

