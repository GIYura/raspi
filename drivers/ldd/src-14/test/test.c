#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "../driver/ioctl_ex.h"

int main()
{
    int answer;
    struct mystruct test = { 4, "Jura" };

    int fd = open("/dev/dummy", O_RDWR);
    if (fd == -1)
    {
        printf("Opening was not possible!\n");
        return -1;
    }

    ioctl(fd, RD_VALUE, &answer);
    printf("The answer is %d\n", answer);

    answer = 123;

    ioctl(fd, WR_VALUE, &answer);
    ioctl(fd, RD_VALUE, &answer);
    printf("The answer is  now %d\n", answer);

    ioctl(fd, GREETER, &test);

    printf("Opening was successfull!\n");

    close(fd);
    return 0;
}
