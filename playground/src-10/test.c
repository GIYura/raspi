#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>

#define BUFF_MAX 1000   /* bytes */
#define OFFSET  10

static char writeBuff[] = "This is UART loop-back test.";
static char readBuff[BUFF_MAX] = "";

int main(int argc, char* argv[])
{
    printf("UART Loop-back test\n");
    printf("Baud rate: 9600\n");
    printf("Format: 8N1\n");

    int file = open("/dev/serial0", O_RDWR | O_NONBLOCK);
    if (file < 0)
    {
        printf("Can't open file\n");
        return -1;
    }

    struct termios options;
    /* read existing settings */
    tcgetattr(file, &options);

    /* modify settings */
    options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;

    /* apply settings */
    tcsetattr(file, TCSANOW, &options);

    write(file, writeBuff, sizeof(writeBuff) / sizeof(writeBuff[0]));
    
    unsigned int delayUs = (strlen(writeBuff) + OFFSET) * 1000;

    usleep(delayUs);

    int rd = read(file, readBuff, sizeof(readBuff) / sizeof(readBuff[0]));

    if (rd < 0)
    {
        printf("Can't read data, probably the loop is broken\n");
        close(file);
        return -2;
    }

    printf("Message: %s\n", readBuff);

    close(file);

    return 0;
}

