#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

static const char* adxl345 = "/dev/spidev0.0";

static int AdxlSpiInit(uint8_t file, uint8_t* mode, uint8_t* bits, uint32_t* speed);
static int AdxlReadId(int file, struct spi_ioc_transfer* tr);

static unsigned char send[2] = { (0x80 | 0x00), 0x00 };
static unsigned char receive[2];
static uint8_t mode = 3; 
static uint8_t bits = 8;
static uint32_t speed = 5000000;
static int file;

int main(int argc, char* argv[])
{
    struct spi_ioc_transfer transfer = 
    {
        .tx_buf = (unsigned long)send,
        .rx_buf = (unsigned long)receive,
        .len = 2,
    };

    if ((file = open(adxl345, O_RDWR)) < 0)
    {
        printf("SPI: Can't open device\n");
        return -1;
    }

    if (AdxlSpiInit(file, &mode, &bits, &speed) == 0)
    {
        printf ("SPI settings:\n");
        printf("Mode: %d\n", mode);
        printf("Bits per word: %d\n", bits);
        printf("Speed: %d Hz\n", speed);
    }
    
    if (AdxlReadId(file, &transfer) > 0)
    {
        printf("ADXL345 ID: %#0x\n", receive[1]);
    }

    close(file);

    return 0;
}

static int AdxlSpiInit(uint8_t file, uint8_t* mode, uint8_t* bits, uint32_t* speed)
{
    if (ioctl(file, SPI_IOC_WR_MODE, mode) == -1)
    {
        printf("SPI: Can't set SPI mode\n");
        return -1;
    }

    if (ioctl(file, SPI_IOC_RD_MODE, mode) == -1)
    {
        printf("SPI: Can't get SPI mode\n");
        return -1;
    }

    if (ioctl(file, SPI_IOC_WR_BITS_PER_WORD, bits) == -1)
    {
        printf("SPI: Can't set bits per word\n");
        return -1;
    }

    if (ioctl(file, SPI_IOC_RD_BITS_PER_WORD, bits) == -1)
    {
        printf("SPI: Can't get bits per word\n");
        return -1;
    }

    if (ioctl(file, SPI_IOC_WR_MAX_SPEED_HZ, speed) == -1)
    {
        printf("SPI: Can't set max speed Hz\n");
        return -1;
    }

    if (ioctl(file, SPI_IOC_RD_MAX_SPEED_HZ, speed) == -1)
    {
        printf("SPI: Can't get max speed Hz\n");
        return -1;
    }
    return 0;
}

static int AdxlReadId(int file, struct spi_ioc_transfer* tr)
{
    if (ioctl(file, SPI_IOC_MESSAGE(1), tr) < 0)
    {
        printf("Failed to send SPI message\n");
        return -1;
    }
    return receive[1];
}

