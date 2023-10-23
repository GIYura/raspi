#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DRIVER_NAME  "led-driver"

#define LED_OFF 0
#define LED_ON  1

typedef struct
{
    dev_t deviceNumber;
    struct cdev cdev;
    unsigned char status;
} ledData_t;

static ledData_t m_ledData;

static void LedSet(unsigned int status)
{
    m_ledData.status = status;
}

/* read the state of LED */
static ssize_t LedRead(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    static const char* const msg[] = { "OFF\n", "ON\n" };
    int size;

    /* check if EOF */
    if (*ppos > 0)
    {
        return 0;
    }

    size = strlen(msg[m_ledData.status]);
    if (size > count)
    {
        size = count;
    }

    if (copy_to_user(buf, msg[m_ledData.status], size))
    {
        return -EFAULT;
    }

    *ppos += size;

    return size;
}

static ssize_t LedWrite(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    char kbuf = 0;

    if (copy_from_user(&kbuf, buf, 1))
    {
        return -EFAULT;
    }

    if (kbuf == '1')
    {
        LedSet(LED_ON);
        printk("LED ON!\n");
    } 
    else if (kbuf == '0')
    {
        LedSet(LED_OFF);
        printk("LED OFF!\n");
    }

    return count;
}

static const struct file_operations drvled_fops =
{
    .owner = THIS_MODULE,
    .read = LedRead,
    .write = LedWrite,
};

static int __init drvled_init(void)
{
    int result;

    result = alloc_chrdev_region(&m_ledData.deviceNumber, 0, 1, DRIVER_NAME);
    if (result)
    {
        printk("%s: Failed to allocate device number!\n", DRIVER_NAME);
        return result;
    }

    cdev_init(&m_ledData.cdev, &drvled_fops);

    result = cdev_add(&m_ledData.cdev, m_ledData.deviceNumber, 1);
    if (result)
    {
        printk("%s: Char device registration failed!\n", DRIVER_NAME);
        unregister_chrdev_region(m_ledData.deviceNumber, 1);
        return result;
    }

    LedSet(LED_OFF);

    printk("%s: initialized.\n", DRIVER_NAME);
    
    return 0;
}

static void __exit drvled_exit(void)
{
    cdev_del(&m_ledData.cdev);
    unregister_chrdev_region(m_ledData.deviceNumber, 1);
    printk("%s: exiting.\n", DRIVER_NAME);
}

module_init(drvled_init);
module_exit(drvled_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_VERSION("1.0");

