#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#define BMP280_ADDRESS      0x76
#define BMP280_REVISION_REG 0xD0

int main(int argc, char* argv[])
{
    int fd;
    unsigned char id;

    fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0)
    {
        printf("Can't open file\n");
        return 1;
    }

    ioctl(fd, I2C_SLAVE, BMP280_ADDRESS);

    id = i2c_smbus_read_word_data(fd, BMP280_REVISION_REG);
    
    if (id == 0x58)
    {
        printf("BMP280 successfully detected: ID = 0x%02hhx\n", id);
    }
    else
    {
        printf("BMP280 not detected\n");
    }

    close (fd);

    return 0;
}
