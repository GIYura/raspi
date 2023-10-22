#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("Simple GPIO driver");

static dev_t deviceNumber;
static struct class* devClass;
static struct cdev device;

#define DRIVER_NAME "my-gpio-driver"
#define DRIVER_CLASS "MyModuleClass"
#define LED_PIN	17

/**
 * @brief Read data
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
	int to_copy, not_copied, delta;
#if 0
	char tmp[3] = " \n";

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(tmp));

	/* Read value of button */
	printk("Value of button: %d\n", gpio_get_value(17));
	tmp[0] = gpio_get_value(17) + '0';

	/* Copy data to user */
	not_copied = copy_to_user(user_buffer, &tmp, to_copy);

	/* Calculate data */
	delta = to_copy - not_copied;
#endif
	return delta;
}

/**
 * @brief Write data
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
	int to_copy, not_copied, delta;
	char value;

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(value));

	/* Copy data to user */
	not_copied = copy_from_user(&value, user_buffer, to_copy);

	/* Setting the LED */
	switch(value) 
	{
		case '0':
			gpio_set_value(LED_PIN, 0);
			break;

		case '1':
			gpio_set_value(LED_PIN, 1);
			break;

		default:
			printk("Invalid Input!\n");
			break;
	}

	/* Calculate data */
	delta = to_copy - not_copied;

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
	.read = driver_read,
	.write = driver_write
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
	
	/* GPIO 17 init */
	if (gpio_request(LED_PIN, "rpi-gpio-17"))
	{
		printk("Can not allocate GPIO 17\n");
		goto AddError;
	}

	/* Set GPIO 17 direction */
	if (gpio_direction_output(LED_PIN, 0))
	{
		printk("Can not set GPIO 17 to output!\n");
		goto Gpio17Error;
	}

	return 0;

Gpio17Error:
	gpio_free(LED_PIN);
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
	gpio_set_value(LED_PIN, 0);
	gpio_free(LED_PIN);

	cdev_del(&device);
	device_destroy(devClass, deviceNumber);
	class_destroy(devClass);
	unregister_chrdev_region(deviceNumber, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

