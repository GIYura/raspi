/*
NOTE: This example is to poll push button.
Init and de-init is done by bash-script.
Poll and read is done by the current .c file.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>

int main(int argc, char* argv[])
{
    static int counter = 0;
    int f;
    struct pollfd poll_fds[1];
    int ret;
    char value[4];
    int n;

    f = open("/sys/class/gpio/gpio17/value", O_RDONLY);
    if (f < 0 )
    {
        printf("Can't open file\n");
        return 1;
    }
	
    poll_fds[0].fd = f;
    poll_fds[0].events = POLLERR | POLLPRI;

    while (1)
    {
        printf("Press the button ...\n");
		
        ret = poll(poll_fds, 1, -1);
        if (ret > 0)
        {
            n = read(f, &value, sizeof(value));
            printf("%d Button pressed: read %d bytes, value = %c\n", ++counter, n, value[0]);
        }
    }

    return 0;
}

