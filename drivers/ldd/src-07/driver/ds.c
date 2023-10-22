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
MODULE_DESCRIPTION("ds18b20 driver");

static dev_t deviceNumber;
static struct class* devClass;
static struct cdev device;

#define DRIVER_NAME "ds18b20"
#define DRIVER_CLASS "DS-Class"

#define DS18B20_PIN	16

unsigned int gpio_num = DS18B20_PIN;

typedef struct
{
	uint8_t low;
	uint8_t high
} Temperature_t;

#define DS18B20_IN gpio_direction_input(gpio_num)
#define DS18B20_OUT gpio_direction_output(gpio_num, 1)
#define DS18B20_HIGH gpio_set_value(gpio_num, 1)
#define DS18B20_LOW gpio_set_value(gpio_num, 0)
#define DS18B20_GET_DATA gpio_get_value(gpio_num)

/**/
static uint8_t ds18b20_reset(void)
{
	uint8_t ret;

	DS18B20_OUT;
	DS18B20_LOW;
	udelay(480);

	DS18B20_HIGH;
	DS18B20_IN;
	udelay(60);

	ret = DS18B20_GET_DATA;
	udelay(420);

	return ret;
}

/**/
static void ds18b20_write_bit(uint8_t bit)
{
	DS18B20_OUT;
	DS18B20_LOW;
	udelay(1);

	if (bit)
	{
		DS18B20_IN;
	}
	
	udelay(60);
	DS18B20_IN;
}

/**/
static uint8_t ds18b20_read_bit(void)
{
	uint8_t bit;

	DS18B20_OUT;
	DS18B20_LOW;
	udelay(1);

	DS18B20_IN;
	udelay(14);

	bit = DS18B20_GET_DATA;

	udelay(45);
	return bit;
}

/**/
static uint8_t ds18b20_read_byte(void)
{
	uint8_t i, byte;
	i = 8;
	byte = 0;

	while (i--)
	{
		byte >>= 1;
		byte |= (ds18b20_read_bit() << 7);
	}
	return byte;
}

/**/
static void ds18b20_write_byte(uint8_t byte)
{
	uint8_t i;
	i = 8;
	while (i--)
	{
		ds18b20_write_bit(byte & 1);
		byte >>= 1;
	}
}

/**/
static void ds18b20_read_data(Temperature_t* value)
{
    if (ds18b20_reset())
    {
        return;
    }
    ds18b20_write_byte(0xCC);
    ds18b20_write_byte(0x44);

    mdelay(375);

    if (ds18b20_reset())
    {
        return;
    }
    ds18b20_write_byte(0xCC);
    ds18b20_write_byte(0xBE);

    value->low = ds18b20_read_byte();
    value->high = ds18b20_read_byte();
}

/**
 * @brief Read data
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
	Temperature_t temp;
    ds18b20_read_data(&temp);
	return copy_to_user(user_buffer, &temp, sizeof(temp));
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance)
{
    uint8_t reset;
	printk("driver_open called!\n");
 	reset = ds18b20_reset();
    if (reset & 0x01)
	{
        return -1;
	}
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
	.read = driver_read,
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) 
{
	printk("Hello, Kernel!\n");
	
	if(alloc_chrdev_region(&deviceNumber, 0, 1, DRIVER_NAME) < 0)
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
	
	/* DS18B20 PIN init */
	if (gpio_request(DS18B20_PIN, "ds-gpio"))
	{
		printk("Can not allocate DS18D20 GPIO\n");
		goto DS18B20_Error;
	}

	return 0;

DS18B20_Error:
	gpio_free(DS18B20_PIN);
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
	gpio_set_value(DS18B20_PIN, 0);
	gpio_free(DS18B20_PIN);

	cdev_del(&device);
	device_destroy(devClass, deviceNumber);
	class_destroy(devClass);
	unregister_chrdev_region(deviceNumber, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

