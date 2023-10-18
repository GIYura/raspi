#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/kernel.h>

#define DRIVER_NAME         "pcf8583"
#define DRIVER_CLASS        "pcf8583Class"

#define PCF_SECONDS_ADDR    0x02
#define PCF_MINUTES_ADDR    0x03
#define PCF_HOURS_ADDR      0x04

#define DEC_TO_BIN(x)       (((x / 10) << 4) | (x % 10))
#define BIN_TO_DEC(x)       ((((x & 0xF0) >> 4) * 10) + (x & 0x0F))

static struct i2c_adapter* pcf8583_i2c_adapter = NULL;
static struct i2c_client* pcf8583_i2c_client = NULL;

/* Meta Information */
MODULE_AUTHOR("Jura");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver for reading out PCF8583 RTC");

/* Defines for device identification */ 
#define I2C_BUS_AVAILABLE		1			/* The I2C Bus available on the raspberry */
#define SLAVE_DEVICE_NAME		"PCF8583"	/* Device and Driver Name */
#define PCF8583_SLAVE_ADDRESS	0x50		/* PCF8583 I2C address */

static struct i2c_driver pcf8583_driver = {
    .driver = {
        .name = SLAVE_DEVICE_NAME,
        .owner = THIS_MODULE
    }
};

static struct i2c_board_info pcf_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, PCF8583_SLAVE_ADDRESS)
};

/* Variables for Device and Deviceclass*/
static dev_t deviceNumber;
static struct class* devClass;
static struct cdev device;
static char pcfBuffer[20];

static u8 readSeconds(void)
{
    u8 seconds = i2c_smbus_read_byte_data(pcf8583_i2c_client, PCF_SECONDS_ADDR);
    return BIN_TO_DEC(seconds);
}

static u8 readMinutes(void)
{
    u8 minutes = i2c_smbus_read_byte_data(pcf8583_i2c_client, PCF_MINUTES_ADDR);
    return BIN_TO_DEC(minutes);
}

static u8 readHours(void)
{
    u8 hours = i2c_smbus_read_byte_data(pcf8583_i2c_client, PCF_HOURS_ADDR);
    return BIN_TO_DEC(hours);
}

static s32 writeSeconds(u8 seconds)
{
    u8 secs = DEC_TO_BIN(seconds);
    s32 ret = i2c_smbus_write_byte_data(pcf8583_i2c_client, PCF_SECONDS_ADDR, secs);
    return (ret < 0) ? ret : 0;
}

static s32 writeMinutes(u8 minutes)
{
    u8 mins = DEC_TO_BIN(minutes);
    s32 ret = i2c_smbus_write_byte_data(pcf8583_i2c_client, PCF_MINUTES_ADDR, mins);
    return (ret < 0) ? ret : 0;
}

static s32 writeHours(u8 hours)
{
    u8 hrs = DEC_TO_BIN(hours);
    s32 ret = i2c_smbus_write_byte_data(pcf8583_i2c_client, PCF_HOURS_ADDR, hrs);
    return (ret < 0) ? ret : 0;
}
/**/
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
    int to_copy, not_copied, delta;

    /* Get amount of data to copy */
    to_copy = min(count, (sizeof(pcfBuffer) - 1));

    /* Copy data to user */
    not_copied = copy_from_user(pcfBuffer, user_buffer, to_copy);

    /* Calculate data */
    delta = to_copy - not_copied;

    /* set time */
    if (writeSeconds(pcfBuffer[2]) < 0)
        printk("Failed to set seconds\n");

    if (writeMinutes(pcfBuffer[1]) < 0)
        printk("Failed to set minutes\n");

    if (writeHours(pcfBuffer[0]) < 0)
        printk("Failed to set houts\n");

    return delta; 

}

/**
 * @brief Get data out of buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
    int to_copy, not_copied, delta;
    u8 outBuff[20];

    /* Get amount of bytes to copy */
    to_copy = min(sizeof(outBuff), count);

    /* Get time */
    outBuff[0] = readHours();
    outBuff[1] = readMinutes();
    outBuff[2] = readSeconds();

    /* Copy Data to user */
    not_copied = copy_to_user(user_buffer, outBuff, to_copy);

    /* Calculate delta */
    delta = to_copy - not_copied;

    return delta;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *deviceFile, struct file *instance)
{
    printk("PCF8583 DeviceDriver -  Open was called\n");
    return 0;
}

/**
 * @brief This function is called, when the device file is close
 */
static int driver_close(struct inode *deviceFile, struct file *instance)
{
    printk("PCF8583 DeviceDriver -  Close was called\n");
    return 0;
}

/* Map the file operations */
static struct file_operations devOps = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
    .write = driver_write,
};

/**
 * @brief function, which is called after loading module to kernel, do initialization there
 */
static int __init ModuleInit(void)
{
    int ret = -1;
    printk("PCF8583 DeviceDriver - Hello Kernel\n");

    /* Allocate Device Nr */
    if (alloc_chrdev_region(&deviceNumber, 0, 1, DRIVER_NAME) < 0)
    {
        printk("PCF8583 Device Number could not be allocated!\n");
        return ret;
    }
    printk("PCF8583 DeviceDriver - Device Number %d was registered\n", deviceNumber);

    /* Create Device Class */
    if ((devClass = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
    {
        printk("PCF8583 Device Class can not be created!\n");
        goto ClassError;
    }

    /* Create Device file */
    if (device_create(devClass, NULL, deviceNumber, NULL, DRIVER_NAME) == NULL)
    {
        printk("PCF8583 Can not create device file!\n");
        goto FileError;
    }

    /* Initialize Device file */
    cdev_init(&device, &devOps);

    /* register device to kernel */
    if (cdev_add(&device, deviceNumber, 1) == -1)
    {
        printk("PCF8583 Registering of device to kernel failed!\n");
        goto AddError;
    }

    pcf8583_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

    if (pcf8583_i2c_adapter != NULL)
    {
        pcf8583_i2c_client = i2c_new_client_device(pcf8583_i2c_adapter, &pcf_i2c_board_info);
        if (pcf8583_i2c_client != NULL)
        {
            if (i2c_add_driver(&pcf8583_driver) != -1)
                ret = 0;
            else
                printk("PCF8583 Can't add driver...\n");
        }
        i2c_put_adapter(pcf8583_i2c_adapter);
    }
    printk("PCF8583 Driver added!\n");

    return ret;

AddError:
    device_destroy(devClass, deviceNumber);
FileError:
    class_destroy(devClass);
ClassError:
    unregister_chrdev(deviceNumber, DRIVER_NAME); 
    return -1;
}

/**
 * @brief function, which is called when removing module from kernel
 * free alocated resources
 */
static void __exit ModuleExit(void)
{
    printk("PCF8583 DeviceDriver - Goodbye, Kernel!\n");

    i2c_unregister_device(pcf8583_i2c_client);
    i2c_del_driver(&pcf8583_driver);
    cdev_del(&device);
    device_destroy(devClass, deviceNumber);
    class_destroy(devClass);
    unregister_chrdev_region(deviceNumber, 1);
}

module_init(ModuleInit);
module_exit(ModuleExit);

