#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>
#include <linux/input.h>

#define DRIVER_NAME     "adxl345-spi"
#define DRIVER_CLASS    "adxl345Class-spi"

#define ADXL_ID_ADDR    0x00

#define SPI_BUS_NUM     0

#define ADXL345_CMD_READ    (1 << 7)
#define ADXL345_READCMD(reg)    (ADXL345_CMD_READ | (reg & 0x3F))

/* FIFO_CTL Bits */
#define FIFO_MODE(x)    (((x) & 0x3) << 6)
#define FIFO_BYPASS 0
#define FIFO_FIFO   1
#define FIFO_STREAM 2
#define SAMPLES(x)  ((x) & 0x1F)

#define DEVID       0x00    /* R   Device ID */

#if 0
/* Register information about your slave device */
struct spi_board_info adxl_spi_board_info = 
{
  .modalias     = "adxl345-spi-driver",
  .max_speed_hz = 5000000,              // speed your device (slave) can handle
  .bus_num      = SPI_BUS_NUM,          // SPI 0
  .chip_select  = 0,                    // /*TODO  */Use 0 Chip select (GPIO 18)
  .mode         = SPI_MODE_3            // SPI mode 3
};

static struct spi_master* master = NULL;

static struct device adxl_device;
#endif
static struct spi_device* adxl_spi_device;

static int adxl345_spi_probe(struct spi_device *spi);
static void adxl345_spi_remove(struct spi_device *spi);

static struct spi_driver adxl_spi_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
    },
    .probe   = adxl345_spi_probe,
    .remove  = adxl345_spi_remove,
};


/* Meta Information */
MODULE_AUTHOR("Jura");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver for reading out ID ADXL345 Accelerometer");

/* Variables for Device and Deviceclass*/
static dev_t deviceNumber;
static struct class* devClass;
static struct cdev device;

/* Macros to do SPI operations */
#define AC_READ(ac, reg)    ((ac)->bops->read((ac)->dev, reg))
#define AC_WRITE(ac, reg, val)  ((ac)->bops->write((ac)->dev, reg, val))

struct adxl345_bus_ops
{
    u16 bustype;
    int (*read)(struct device *, unsigned char);
    //int (*read_block)(struct device *, unsigned char, int, void *);
    //int (*write)(struct device *, unsigned char, unsigned char);
};

struct adxl345_platform_data
{

     /*
      * low_power_mode:
      * A '0' = Normal operation and a '1' = Reduced
      * power operation with somewhat higher noise.
      */

     u8 low_power_mode;

    /*
     * tap_threshold:
     * holds the threshold value for tap detection/interrupts.
     * The data format is unsigned. The scale factor is 62.5 mg/LSB
     * (i.e. 0xFF = +16 g). A zero value may result in undesirable
     * behavior if Tap/Double Tap is enabled.
     */

    u8 tap_threshold;

    /*
     * tap_duration:
     * is an unsigned time value representing the maximum
     * time that an event must be above the tap_threshold threshold
     * to qualify as a tap event. The scale factor is 625 us/LSB. A zero
     * value will prevent Tap/Double Tap functions from working.
     */

    u8 tap_duration;

    /*
     * TAP_X/Y/Z Enable: Setting TAP_X, Y, or Z Enable enables X,
     * Y, or Z participation in Tap detection. A '0' excludes the
     * selected axis from participation in Tap detection.
     * Setting the SUPPRESS bit suppresses Double Tap detection if
     * acceleration greater than tap_threshold is present during the
     * tap_latency period, i.e. after the first tap but before the
     * opening of the second tap window.
     */

#define ADXL_TAP_X_EN   (1 << 2)
#define ADXL_TAP_Y_EN   (1 << 1)
#define ADXL_TAP_Z_EN   (1 << 0)

    u8 tap_axis_control;

    /*
     * data_rate:
     * Selects device bandwidth and output data rate.
     * RATE = 3200 Hz / (2^(15 - x)). Default value is 0x0A, or 100 Hz
     * Output Data Rate. An Output Data Rate should be selected that
     * is appropriate for the communication protocol and frequency
     * selected. Selecting too high of an Output Data Rate with a low
     * communication speed will result in samples being discarded.
     */

    u8 data_rate;

    /*
     * data_range:
     * FULL_RES: When this bit is set with the device is
     * in Full-Resolution Mode, where the output resolution increases
     * with RANGE to maintain a 4 mg/LSB scale factor. When this
     * bit is cleared the device is in 10-bit Mode and RANGE determine the
     * maximum g-Range and scale factor.
    */

#define ADXL_FULL_RES       (1 << 3)
#define ADXL_RANGE_PM_2g    0
#define ADXL_RANGE_PM_4g    1
#define ADXL_RANGE_PM_8g    2
#define ADXL_RANGE_PM_16g   3

    u8 data_range;

    /*
     * A valid BTN or KEY Code; use tap_axis_control to disable
     * event reporting
     */

    u32 ev_code_tap[3]; /* EV_KEY {X-Axis, Y-Axis, Z-Axis} */

    /*
     * fifo_mode:
     * BYPASS The FIFO is bypassed
     * FIFO   FIFO collects up to 32 values then stops collecting data
     * STREAM FIFO holds the last 32 data values. Once full, the FIFO's
     *        oldest data is lost as it is replaced with newer data
     *
     * DEFAULT should be FIFO_STREAM
     */

    u8 fifo_mode;

    /*
     * watermark:
     * The Watermark feature can be used to reduce the interrupt load
     * of the system. The FIFO fills up to the value stored in watermark
     * [1..32] and then generates an interrupt.
     * A '0' disables the watermark feature.
     */

    u8 watermark;

};

/* Set initial adxl345 register values */
static const struct adxl345_platform_data adxl345_default_init = {
    .tap_threshold = 50,
    .tap_duration = 3,
    //.tap_axis_control = ADXL_TAP_X_EN | ADXL_TAP_Y_EN | ADXL_TAP_Z_EN,
    .tap_axis_control = ADXL_TAP_Z_EN,
    .data_rate = 8,
    .data_range = ADXL_FULL_RES,
    .ev_code_tap = {BTN_TOUCH, BTN_TOUCH, BTN_TOUCH}, /* EV_KEY {x,y,z} */
    //.fifo_mode = ADXL_FIFO_STREAM,
    .fifo_mode = FIFO_BYPASS,
    .watermark = 0,
};

/* Create private data structure */
struct adxl345
{
    struct gpio_desc *gpio;
    struct device *dev;
    struct input_dev *input;
    struct adxl345_platform_data pdata;
//    struct axis_triple saved;
    u8 phys[32];
    int irq;
    u32 model;
    u32 int_mask;
    const struct adxl345_bus_ops *bops;
};

struct adxl345 *adxl345_probe(struct device *dev, const struct adxl345_bus_ops *bops)
{
    struct adxl345 *ac; /* declare our private structure */
    struct input_dev *input_dev;
    const struct adxl345_platform_data *pdata;
    int err;
    u8 revid;

    /* Allocate private structure*/
    ac = devm_kzalloc(dev, sizeof(*ac), GFP_KERNEL);
    if (!ac) {
        dev_err(dev, "Failed to allocate memory\n");
        err = -ENOMEM;
        goto err_out;
    }

    /* Allocate the input_dev structure */
    input_dev = devm_input_allocate_device(dev);
    if (!ac || !input_dev) {
        dev_err(dev, "failed to allocate input device\n");
        err = -ENOMEM;
        goto err_out;
    }

    /* Initialize our private structure */

    /*
     * Store the previously initialized platform data
     * in our private structure
     */
    pdata = &adxl345_default_init;
    ac->pdata = *pdata;
    pdata = &ac->pdata;

    ac->input = input_dev;
    ac->dev = dev;

    /* Store the SPI operations in our private structure */
    ac->bops = bops;

    input_dev->name = "ADXL345 accelerometer";
    revid = AC_READ(ac, DEVID);
    dev_info(dev, "DEVID: %d\n", revid);

    if (revid == 0xE5)
    {
        dev_info(dev, "ADXL345 is found");
    }
    else
    {
        dev_err(dev, "Failed to probe %s\n", input_dev->name);
        err = -ENODEV;
        goto err_out;
    }

#if 0
    snprintf(ac->phys, sizeof(ac->phys), "%s/input0", dev_name(dev));

    /* Initialize the input device */
    input_dev->phys = ac->phys;
    input_dev->dev.parent = dev;
    input_dev->id.product = ac->model;
    input_dev->id.bustype = bops->bustype;

    /* Attach the input device and the private structure */
    input_set_drvdata(input_dev, ac);

    /* 
     * Set the different event types.
     * EV_KEY type events, with BTN_TOUCH events code
     * when the single tap interrupt is triggered
     */
    __set_bit(EV_KEY, input_dev->evbit);
    __set_bit(pdata->ev_code_tap[ADXL_X_AXIS], input_dev->keybit);
    __set_bit(pdata->ev_code_tap[ADXL_Y_AXIS], input_dev->keybit);
    __set_bit(pdata->ev_code_tap[ADXL_Z_AXIS], input_dev->keybit);

    /* 
     * Check if any of the axis has been enabled
     * and set the interrupt mask
     * In this driver only SINGLE_TAP interrupt
     */
    if (pdata->tap_axis_control & (TAP_X_EN | TAP_Y_EN | TAP_Z_EN))
        ac->int_mask |= SINGLE_TAP;

    ac->gpio = devm_gpiod_get_index(dev, ADXL345_GPIO_NAME, 0, GPIOD_IN);
    if (IS_ERR(ac->gpio)) {
        dev_err(dev, "gpio get index failed\n");
        err = PTR_ERR(ac->gpio); /* PTR_ERR return an int from a pointer */
        goto err_out;
    }
    
    ac->irq = gpiod_to_irq(ac->gpio);
    if (ac->irq < 0) {
        dev_err(dev, "gpio get irq failed\n");
        err = ac->irq;
        goto err_out;
    }
    dev_info(dev, "The IRQ number is: %d\n", ac->irq);

    /* Request threaded interrupt */
    err = devm_request_threaded_irq(input_dev->dev.parent, ac->irq, NULL,
            adxl345_irq, IRQF_TRIGGER_HIGH | IRQF_ONESHOT, dev_name(dev), ac);
    if (err)
            goto err_out;

    err = sysfs_create_group(&dev->kobj, &adxl345_attr_group);
    if (err)
        goto err_out;

    /* Register the input device to the input core */
    err = input_register_device(input_dev);
    if (err)
        goto err_remove_attr;

    /* Initialize the ADXL345 registers */

    /* Set the tap threshold and duration */
    AC_WRITE(ac, THRESH_TAP, pdata->tap_threshold);
    AC_WRITE(ac, DUR, pdata->tap_duration);

    /* set the axis where the tap will be detected */
    AC_WRITE(ac, TAP_AXES, pdata->tap_axis_control);

    /* set the data rate and the axis reading power
     * mode, less or higher noise reducing power
     */
    AC_WRITE(ac, BW_RATE, RATE(ac->pdata.data_rate) |
         (pdata->low_power_mode ? LOW_POWER : 0));

    /* 13-bit full resolution right justified */
    AC_WRITE(ac, DATA_FORMAT, pdata->data_range);

    /* Set the FIFO mode, no FIFO by default */
    AC_WRITE(ac, FIFO_CTL, FIFO_MODE(pdata->fifo_mode) |
            SAMPLES(pdata->watermark));

    /* Map all INTs to INT1 pin */
    AC_WRITE(ac, INT_MAP, 0);

    /* Enables interrupts */
    AC_WRITE(ac, INT_ENABLE, ac->int_mask);

    /* Set RUN mode */
    AC_WRITE(ac, POWER_CTL, PCTL_MEASURE);
#endif
    return ac;

 //err_remove_attr:
   // sysfs_remove_group(&dev->kobj, &adxl345_attr_group);

/* 
 * this function returns a pointer
 * to a struct ac or an err pointer
 */
 err_out:
    return ERR_PTR(err);
}


static int adxl345_spi_read(struct device *dev, unsigned char reg)
{
    struct spi_device *spi = to_spi_device(dev);
    u8 cmd;

    cmd = ADXL345_READCMD(reg);

    return spi_w8r8(spi, cmd);
}

static const struct adxl345_bus_ops adxl345_spi_bops = {
    .bustype    = BUS_SPI,
   // .write      = adxl345_spi_write,
    .read       = adxl345_spi_read,
  //  .read_block = adxl345_spi_read_block,
};

static int adxl345_spi_probe(struct spi_device *spi)
{
    struct adxl345 *ac;

#if 1
    /* send the spi operations */
    ac = adxl345_probe(&spi->dev, &adxl345_spi_bops);

    if (IS_ERR(ac))
        return PTR_ERR(ac);

    /* Attach the SPI device to the private structure */
    spi_set_drvdata(spi, ac);
#endif
    return 0;
}

static void adxl345_spi_remove(struct spi_device *spi)
{
#if 0 
    struct adxl345 *ac = spi_get_drvdata(spi);
    dev_info(ac->dev, "my_remove() function is called.\n");
    sysfs_remove_group(&ac->dev->kobj, &adxl345_attr_group);
    input_unregister_device(ac->input);
    AC_WRITE(ac, POWER_CTL, PCTL_STANDBY);
    dev_info(ac->dev, "unregistered accelerometer\n");
#endif
}

#if 1
static u8 readId(void)
{
    u8 cmd = ADXL345_READCMD(ADXL_ID_ADDR);
    u8 id = spi_w8r8(adxl_spi_device, cmd);
    return id;
}
#endif

#if 0
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
#endif

/**
 * @brief Get data out of buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
    int to_copy, not_copied, delta;
    u8 outBuff[20];
#if 1
    /* Get amount of bytes to copy */
    to_copy = min(sizeof(outBuff), count);

    /* read ID */
    outBuff[0] = readId();

    /* Copy Data to user */
    not_copied = copy_to_user(user_buffer, outBuff, to_copy);

    /* Calculate delta */
    delta = to_copy - not_copied;
#endif
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
#if 0
    .write = driver_write,
#endif
};

/**
 * @brief function, which is called after loading module to kernel, do initialization there
 */
static int __init ModuleInit(void)
{
    int ret = 0;
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

    //spi_register_driver(&adxl_spi_driver);

#if 0
/* TODO: */
 /*
    1. alloc master
    2. register master
    3. new device
    4. register driver
    5. spi setup
  */
    /*1*/
    master = spi_alloc_master(&adxl_device, 0);
    if (master == NULL)
    {
        printk("Error alloc master\n");
        return -1;
    }
    /* TODO:
    master->bus_num = ;
     */

    /*
    ret = spi_register_master(master);
    if (ret < 0)
    {
        spi_master_put(master);
    }
    */
#endif
#if 0
    /* create a new slave device, given the master and device info */
    adxl_spi_device = spi_new_device(master, &adxl_spi_board_info);
    if (adxl_spi_device == NULL) 
    {
        printk("FAILED to create slave.\n");
        return -1;
    }

    /* 8-bits in a word */
    adxl_spi_device->bits_per_word = 8;

    /* setup the SPI slave device */
    ret = spi_setup(adxl_spi_device);
    if (ret < 0)
    {
        printk("FAILED to setup slave.\n");
        spi_unregister_device(adxl_spi_device);
        return -1;
    }
#endif    
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

    //spi_unregister_master(master);
    spi_unregister_device(adxl_spi_device);
    spi_unregister_driver(&adxl_spi_driver);
    cdev_del(&device);
    device_destroy(devClass, deviceNumber);
    class_destroy(devClass);
    unregister_chrdev_region(deviceNumber, 1);
}

//module_spi_driver(adxl_spi_driver);

module_init(ModuleInit);
module_exit(ModuleExit);

