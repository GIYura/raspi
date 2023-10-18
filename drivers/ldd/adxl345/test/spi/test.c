#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define BUFF_SIZE	20

static int AccelIdGet(const char* const dev, char* const buff, int buffSize);

int main(int argc, char* argv[])
{
    char accelBuff[BUFF_SIZE];

    if (argc < 2)
    {
        printf("Invalid argument\n");
        return -1;
    }

    if (AccelIdGet(argv[1], accelBuff, sizeof(accelBuff)) == 0)
    {
        printf("Accel ID: %#x\n", accelBuff[0]);
    }
    else
    {
        printf("File %s does not exist\n", argv[1]);
        return -1;
    }

    return 0;
}

static int AccelIdGet(const char* const dev, char* const buff, int buffSize)
{
    int fd; 
    int ret = -1;

    fd = open(dev, O_RDONLY);
    if (fd >= 0)
    {
        if (read(fd, buff, buffSize) > 0)
        {
            ret = 0;
        }
        close (fd);
    }

    return ret;
}
