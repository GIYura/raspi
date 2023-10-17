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
    unsigned char val;

    fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0)
    {
        printf("Can't open file\n");
        return 1;
    }

    ioctl(fd, I2C_SLAVE, BMP280_ADDRESS);

    val = i2c_smbus_read_word_data(fd, BMP280_REVISION_REG);
    
    printf("BMP280 ID: %#x\n", val);

    close (fd);

    return 0;
}
