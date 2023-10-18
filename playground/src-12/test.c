#include <bcm2835.h>
#include <stdio.h>

#define LED_PIN RPI_BPLUS_GPIO_J8_37

int main(int argc, char** argv)
{
	if (!bcm2835_init())
	{
		printf("bcm2835 init FAILED\r\n");
		return 1;
	}
	
	bcm2835_gpio_fsel(LED_PIN, BCM2835_GPIO_FSEL_OUTP);

	while (1)
	{
		bcm2835_gpio_write(LED_PIN, HIGH);
		bcm2835_delay(100);
		bcm2835_gpio_write(LED_PIN, LOW);
		bcm2835_delay(100);
	}

	bcm2835_close();

	return 0;
}
