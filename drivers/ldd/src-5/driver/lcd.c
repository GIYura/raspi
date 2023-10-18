#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("LCD driver");

static dev_t deviceNumber;
static struct class* devClass;
static struct cdev device;

static const char text[] = "Hello Linux!";

#define DRIVER_NAME "lcd-driver"
#define DRIVER_CLASS "LCD-Class"

/* LCD char buffer */
static char lcdBuffer[17];

/* Pinout for LCD Display */
unsigned int lcdPins[] = {
	3,  /* Enable Pin */
	2,  /* Register Select Pin */
	4,  /* Data Pin 0*/
	17, /* Data Pin 1*/
	27, /* Data Pin 2*/
	22, /* Data Pin 3*/
	10, /* Data Pin 4*/
	9,  /* Data Pin 5*/
	11, /* Data Pin 6*/
	5,  /* Data Pin 7*/
};

#define REGISTER_SELECT lcdPins[1]

/**
 * @brief generates a pulse on the enable signal
 */
void lcdEnable(void)
{
	gpio_set_value(lcdPins[0], 1);
	msleep(5);
	gpio_set_value(lcdPins[0], 0);
}

/**
 * @brief set the 8 bit data bus
 * @param data: Data to set
 */
void lcdSendByte(char data)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		gpio_set_value(lcdPins[i + 2], ((data) & (1 << i)) >> i);
	}
	lcdEnable();
	msleep(5);
}

/**
 * @brief send a command to the LCD
 *
 * @param data: command to send
 */
void lcdCommand(uint8_t data)
{
 	gpio_set_value(REGISTER_SELECT, 0);	/* RS to Instruction */
	lcdSendByte(data);
}

/**
 * @brief send a data to the LCD
 *
 * @param data: command to send
 */
void lcdData(uint8_t data)
{
 	gpio_set_value(REGISTER_SELECT, 1);	/* RS to data */
	lcdSendByte(data);
}

/**
 * @brief Write data to buffer
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
	int to_copy, not_copied, delta, i;

	/* Get amount of data to copy */
	to_copy = min(count, (sizeof(lcdBuffer) - 1));

	/* Copy data to user */
	not_copied = copy_from_user(lcdBuffer, user_buffer, to_copy);

	/* Calculate data */
	delta = to_copy - not_copied;

	/* Set the new data to the display */
	lcdCommand(0x1);

	for( i = 0; i < (to_copy - 1); i++)
	{
		lcdData(lcdBuffer[i]);
	}
	printk("Write called\n");
	return delta;
}

 
/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance)
{
	printk("driver_open called!\n");
	return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance)
{
	printk("driver_close called!\n");
	return 0;
}

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.write = driver_write
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) 
{
	int i;
	char *gpioNames[] = {"ENABLE_PIN", "REGISTER_SELECT", "DATA_PIN0", "DATA_PIN1", "DATA_PIN2", "DATA_PIN3", "DATA_PIN4", "DATA_PIN5", "DATA_PIN6", "DATA_PIN7"};

	printk("Hello, Kernel!\n");
	
	if (alloc_chrdev_region(&deviceNumber, 0, 1, DRIVER_NAME) < 0)
	{
		printk("Device Number could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device number Major: %d, Minor: %d was registered!\n", deviceNumber >> 20, deviceNumber && 0xfffff);

	/* Create device class */
	if ((devClass = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
	{
		printk("Device class can not be created!\n");
		goto ClassError;
	}

	/* create device file */
	if (device_create(devClass, NULL, deviceNumber, NULL, DRIVER_NAME) == NULL)
	{
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&device, &fops);

	/* Regisering device to kernel */
	if (cdev_add(&device, deviceNumber, 1) == -1)
	{
		printk("Registering of device to kernel failed!\n");
		goto AddError;
	}
	
	/* Initialize GPIOs */
	printk("lcd-driver - GPIO Init\n");
	for (i = 0; i < 10; i++)
	{
		if (gpio_request(lcdPins[i], gpioNames[i]))
		{
			printk("lcd-driver - Error Init GPIO %d\n", lcdPins[i]);
			goto GpioInitError;
		}
	}

	printk("lcd-driver - Set GPIOs to output\n");
	for (i = 0; i < 10; i++)
	{
		if (gpio_direction_output(lcdPins[i], 0))
		{
			printk("lcd-driver - Error setting GPIO %d to output\n", i);
			goto GpioDirectionError;
		}
	}

	/* Init the display */
	lcdCommand(0x30);	/* Set the display for 8 bit data interface */

	lcdCommand(0xf);	/* Turn display on, turn cursor on, set cursor blinking */

	lcdCommand(0x1);

	for (i = 0; i < sizeof(text) - 1; i++)
	{
		lcdData(text[i]);
	}

	return 0;

GpioDirectionError:
	i = 9;

GpioInitError:
	for(; i >= 0; i--)
	{
		gpio_free(lcdPins[i]);
	}	

AddError:
	device_destroy(devClass, deviceNumber);

FileError:
	class_destroy(devClass);

ClassError:
	unregister_chrdev_region(deviceNumber, 1);

	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void)
{
	int i;
	lcdCommand(0x1);	/* Clear the display */

	for (i = 0; i < 10; i++)
	{
		gpio_set_value(lcdPins[i], 0);
		gpio_free(lcdPins[i]);
	}
	cdev_del(&device);
	device_destroy(devClass, deviceNumber);
	class_destroy(devClass);
	unregister_chrdev_region(deviceNumber, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

