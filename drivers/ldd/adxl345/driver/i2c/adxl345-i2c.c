#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/kernel.h>

#define DRIVER_NAME     "adxl345-i2c"
#define DRIVER_CLASS    "adxl345Class-i2c"

#define ADXL_ID_ADDR    0x00

/* adapter = master */
static struct i2c_adapter* adxl345_i2c_adapter = NULL;   // I2C Adapter Structure

/* client = slave */
static struct i2c_client* adxl345_i2c_client = NULL;     // I2C Cient Structure

/* Meta Information */
MODULE_AUTHOR("Jura");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver for reading out ID ADXL345 Accelerometer");

/* Defines for device identification */ 
#define I2C_BUS_AVAILABLE		1           /* The I2C Bus available on the raspberry */
#define SLAVE_DEVICE_NAME		"ADXL345"   /* Device and Driver Name */
#define ADXL345_SLAVE_ADDRESS	0x53        /* ADXL345 I2C address */

/* драйвер устройства на шине 
I2C driver Structure that has to be added to linux
*/
static struct i2c_driver adxl345_driver = {
    .driver = {
        .name = SLAVE_DEVICE_NAME,
        .owner = THIS_MODULE
    }
};

/* I2C Board Info strucutre */
static struct i2c_board_info adxl345_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, ADXL345_SLAVE_ADDRESS)
};

/* Variables for Device and Deviceclass*/
static dev_t deviceNumber;
static struct class* devClass;
static struct cdev device;
static char adxl345Buffer[20];

static u8 readId(void)
{
    u8 id = i2c_smbus_read_byte_data(adxl345_i2c_client, ADXL_ID_ADDR);
    return id;
}

/**/
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
    int to_copy, not_copied, delta;

    /* Get amount of data to copy */
    to_copy = min(count, (sizeof(adxl345Buffer) - 1));

    /* Copy data to user */
    not_copied = copy_from_user(adxl345Buffer, user_buffer, to_copy);

    /* Calculate data */
    delta = to_copy - not_copied;

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

    /* read ID */
    outBuff[0] = readId();

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
    printk("ADXL345 DeviceDriver -  Open was called\n");
    return 0;
}

/**
 * @brief This function is called, when the device file is close
 */
static int driver_close(struct inode *deviceFile, struct file *instance)
{
    printk("ADXL345 DeviceDriver -  Close was called\n");
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
    printk("ADXL345 DeviceDriver - Hello Kernel\n");

    /* Allocate Device Nr */
    if (alloc_chrdev_region(&deviceNumber, 0, 1, DRIVER_NAME) < 0)
    {
        printk("ADXL345 Device Number could not be allocated!\n");
        return ret;
    }
    printk("ADXL345 DeviceDriver - Device Number %d was registered\n", deviceNumber);

    /* Create Device Class */
    if ((devClass = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
    {
        printk("ADXL345 Device Class can not be created!\n");
        goto ClassError;
    }

    /* Create Device file */
    if (device_create(devClass, NULL, deviceNumber, NULL, DRIVER_NAME) == NULL)
    {
        printk("ADXL345 Can not create device file!\n");
        goto FileError;
    }

    /* Initialize Device file */
    cdev_init(&device, &devOps);

    /* register device to kernel */
    if (cdev_add(&device, deviceNumber, 1) == -1)
    {
        printk("ADXL345 Registering of device to kernel failed!\n");
        goto AddError;
    }

    /* Get I2C controller
    доступен ли на шине 1 i2c контроллер
    */
    adxl345_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

    if (adxl345_i2c_adapter != NULL)
    {
        /* The interface is used to instantiate an I2C client device */
        adxl345_i2c_client = i2c_new_client_device(adxl345_i2c_adapter, &adxl345_i2c_board_info);
        if (adxl345_i2c_client != NULL)
        {
            /* add driver to subsystem */
            if (i2c_add_driver(&adxl345_driver) != -1)
            {
                ret = 0;
            }
            else
            {
                printk("ADXL345 Can't add driver...\n");
            }
        }
        /* must call i2c_put_adapter() when done with returned i2c_adapter device */
        i2c_put_adapter(adxl345_i2c_adapter);
    }
    printk("ADXL345 Driver added!\n");

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
    printk("ADXL345 DeviceDriver - Goodbye, Kernel!\n");

    i2c_unregister_device(adxl345_i2c_client);
    i2c_del_driver(&adxl345_driver);
    cdev_del(&device);
    device_destroy(devClass, deviceNumber);
    class_destroy(devClass);
    unregister_chrdev_region(deviceNumber, 1);
}

module_init(ModuleInit);
module_exit(ModuleExit);

