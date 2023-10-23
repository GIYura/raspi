#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioport.h>
#include <linux/io.h>

#define DRIVER_NAME  "led-driver"

#define LED_OFF 0
#define LED_ON  1

#define GPIO1_BASE      0x0209C000
#define GPIO1_SIZE      8

#define GPIO1_REG_DATA  0
#define GPIO1_REG_DIR   4

#define GPIO_BIT        (1 << 9)

typedef struct
{
    dev_t devnum;
    struct cdev cdev;
    unsigned char status;
    void __iomem *regBase;
} ledData_t;

static ledData_t m_ledData;

static void LedSet(unsigned char status)
{
    u32 val;

    /* set value */
    val = readl(m_ledData.regBase + GPIO1_REG_DATA);
        
    if (status == LED_ON)
        val |= GPIO_BIT;
    else if (status == LED_OFF)
        val &= ~GPIO_BIT;
    
    writel(val, m_ledData.regBase + GPIO1_REG_DATA);

    /* update status */
    m_ledData.status = status;
}

static void LedSetDirection(void)
{
    u32 val;

    val = readl(m_ledData.regBase + GPIO1_REG_DIR);
    val |= GPIO_BIT;
    writel(val, m_ledData.regBase + GPIO1_REG_DIR);
}

static ssize_t LedRead(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    static const char* const msg[] = { "OFF\n", "ON\n" };
    int size;

    /* check if EOF */
    if (*ppos > 0)
        return 0;

    size = strlen(msg[m_ledData.status]);
    if (size > count)
        size = count;

    if (copy_to_user(buf, msg[m_ledData.status], size))
        return -EFAULT;

    *ppos += size;
    
    return size;
}

static ssize_t LedWrite(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    char kbuf = 0;

    if (copy_from_user(&kbuf, buf, 1))
        return -EFAULT;

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

static const struct file_operations LedOperations =
{
    .owner = THIS_MODULE,
    .write = LedWrite,
    .read = LedRead,
};

static int __init LedInit(void)
{
    int result = 0;

    if (!request_mem_region(GPIO1_BASE, GPIO1_SIZE, DRIVER_NAME))
    {
        printk("%s: Error requesting I/O!\n", DRIVER_NAME);
        result = -EBUSY;
        goto ret_err_request_mem_region;
    }

    m_ledData.regBase = ioremap(GPIO1_BASE, GPIO1_SIZE);
    if (!m_ledData.regBase)
    {
        printk("%s: Error mapping I/O!\n", DRIVER_NAME);
        result = -ENOMEM;
        goto err_ioremap;
    }

    result = alloc_chrdev_region(&m_ledData.devnum, 0, 1, DRIVER_NAME);
    if (result)
    {
        printk("%s: Failed to allocate device number!\n", DRIVER_NAME);
        goto ret_err_alloc_chrdev_region;
    }

    cdev_init(&m_ledData.cdev, &LedOperations);

    result = cdev_add(&m_ledData.cdev, m_ledData.devnum, 1);
    if (result)
    {
        printk("%s: Char device registration failed!\n", DRIVER_NAME);
        goto ret_err_cdev_add;
    }

    LedSetDirection();

    LedSet(LED_OFF);

    printk("%s: initialized.\n", DRIVER_NAME);
    goto ret_ok;

ret_err_cdev_add:
    unregister_chrdev_region(m_ledData.devnum, 1);
ret_err_alloc_chrdev_region:
    iounmap(m_ledData.regBase);
err_ioremap:
    release_mem_region(GPIO1_BASE, GPIO1_SIZE);
ret_err_request_mem_region:
ret_ok:
    return result;
}

static void __exit LedExit(void)
{
    cdev_del(&m_ledData.cdev);
    unregister_chrdev_region(m_ledData.devnum, 1);
    iounmap(m_ledData.regBase);
    release_mem_region(GPIO1_BASE, GPIO1_SIZE);
    printk("%s: exiting.\n", DRIVER_NAME);
}

module_init(LedInit);
module_exit(LedExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_VERSION("1.0");

