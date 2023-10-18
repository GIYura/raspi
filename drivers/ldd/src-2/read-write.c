#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("Registers a device. and implement some callback functions");

static char buffer[255];
static int bufferIndex;
static dev_t deviceNumber;
static struct class* devClass;
static struct cdev device;

#define DRIVER_NAME "dummydriver"
#define DRIVER_CLASS "MyModuleClass"

/**
 * @brief Read data
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
	int toCopy, notCopied, delta;

	/* get amount of data to copy */
	toCopy = min(count, bufferIndex);

	/* copy data to user  */
	notCopied = copy_to_user(user_buffer, buffer, toCopy);
	
	delta = toCopy - notCopied;
	
	return delta;
}

/**
 * @brief Write data
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
	int toCopy, notCopied, delta;

	/* Get amount of data to copy */
	toCopy = min(count, sizeof(buffer));

	/* Copy data to user */
	notCopied = copy_from_user(buffer, user_buffer, toCopy);
	bufferIndex = toCopy;

	/* Calculate data */
	delta = toCopy - notCopied;

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

	return 0;
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
	cdev_del(&device);
	device_destroy(devClass, deviceNumber);
	class_destroy(devClass);
	unregister_chrdev_region(deviceNumber, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

