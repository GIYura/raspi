#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define BCM2711_PERI_BASE   0xfe000000
#define GPIO_BASE           (BCM2711_PERI_BASE + 0x200000) /* GPIO controller */
#define GPIO_LED            21
#define BLOCK_SIZE          (4 * 1024)

static volatile unsigned* m_gpio;

static int GpioInit(void);

/* GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) */
#define INP_GPIO(g) *(m_gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(m_gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define GPIO_SET *(m_gpio+7)  /* sets   bits which are 1 ignores bits which are 0 */
#define GPIO_CLR *(m_gpio+10) /* clears bits which are 1 ignores bits which are 0 */
 
int main(int argc, char* argv[])
{
    unsigned int blinkTimeout = 0;
    
    if (argc != 2)
    {
        printf("Invalid number of arguments\n");
        return -1;
    }

    blinkTimeout = atoi(argv[1]) * 1000; /*convert to us */

    if (blinkTimeout == 0)
    {
        printf("Invalid timeout\n");
        return -2;
    }
    
    if (GpioInit() == -1) 
    {
        printf("Failed to map physical GPIO registers into the virtual memory space.\n");
        return -3;
    }

    /* must use INP_GPIO before we can use OUT_GPIO */
    INP_GPIO(GPIO_LED);
    OUT_GPIO(GPIO_LED);
 
    for (;;)
    {
        GPIO_CLR = 1 << GPIO_LED;

        usleep(blinkTimeout);
        
        GPIO_SET = 1 << GPIO_LED;

        usleep(blinkTimeout);
    }
    
    printf("Stopped\n");

    return 0;
}

static int GpioInit(void)
{
    int memFd;
    void* gpioMap;

    /* open /dev/mem */
    if ((memFd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
    {
        printf("can't open /dev/mem \n");
        return -1;
    }
    else
    {
        /* mmap GPIO */
        gpioMap = mmap(
            NULL,                   /* Any adddress in our space will do */
            BLOCK_SIZE,             /* Map length */
            PROT_READ|PROT_WRITE,   /* Enable reading & writting to mapped memory */
            MAP_SHARED,             /* Shared with other processes */
            memFd,                  /* File to map */
            GPIO_BASE               /* Offset to GPIO peripheral */   
        );
        
        close(memFd);

        if (gpioMap == MAP_FAILED) 
        {
            printf("mmap error %d\n", *(int*)gpioMap);
            return -2;
        }
    }
    
    /* Always use volatile pointer! */
    m_gpio = (volatile unsigned *)gpioMap;
    printf("%s\n", "Blinking...");

    return 0;
}

