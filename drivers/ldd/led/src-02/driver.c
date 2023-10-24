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

#define BCM2711_PERI_BASE   0xfe000000
#define GPIO_BASE           (BCM2711_PERI_BASE + 0x200000) /* GPIO controller */
#define GPIO_SIZE           9

#define GPIO_REG_DIR        0x08
#define GPIO_REG_SET        0x1c
#define GPIO_REG_CLR        0x28
#define GPIO_BIT            (1 << 3)

typedef struct
{
    dev_t deviceNumber;
    struct cdev cdev;
    unsigned char status;
    void __iomem *regBase;
} ledData_t;

static ledData_t m_led;

static void LedClear(void)
{
    u32 val;

    /* read value */
    val = readl(m_led.regBase + GPIO_REG_CLR);

    /* modify (clear bit) */
    val &= ~GPIO_BIT;
    
    /* write value */
    writel(val, m_led.regBase + GPIO_REG_CLR);

    /* update status */
    m_led.status = LED_OFF;
}

static void LedSet(void)
{
    u32 val;

    /* read value */
    val = readl(m_led.regBase + GPIO_REG_SET);

    /* modify (set bit) */
    val |= GPIO_BIT;
    
    /* write value */
    writel(val, m_led.regBase + GPIO_REG_SET);

    /* update status */
    m_led.status = LED_ON;
}

static void LedSetDirection(void)
{
    u32 val;

    /* read value */
    val = readl(m_led.regBase + GPIO_REG_DIR);

    /* modify (set bit) */
    val |= GPIO_BIT;

    /* write value */
    writel(val, m_led.regBase + GPIO_REG_DIR);
}

static ssize_t LedRead(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    static const char* const msg[] = { "OFF\n", "ON\n" };
    int size;

    /* check if EOF */
    if (*ppos > 0)
        return 0;

    size = strlen(msg[m_led.status]);
    if (size > count)
        size = count;

    if (copy_to_user(buf, msg[m_led.status], size))
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
        LedSet();
    } 
    else if (kbuf == '0')
    {
        LedClear();
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

    m_led.regBase = ioremap(GPIO_BASE, GPIO_SIZE);
    if (!m_led.regBase)
    {
        printk("%s: Error mapping I/O!\n", DRIVER_NAME);
        result = -ENOMEM;
        goto err_ioremap;
    }

    result = alloc_chrdev_region(&m_led.deviceNumber, 0, 1, DRIVER_NAME);
    if (result)
    {
        printk("%s: Failed to allocate device number!\n", DRIVER_NAME);
        goto ret_err_alloc_chrdev_region;
    }

    cdev_init(&m_led.cdev, &LedOperations);

    result = cdev_add(&m_led.cdev, m_led.deviceNumber, 1);
    if (result)
    {
        printk("%s: Char device registration failed!\n", DRIVER_NAME);
        goto ret_err_cdev_add;
    }

    /* setup gpio LED direction */
    LedSetDirection();

    /* turn off LED */
    LedClear();

    printk("%s: initialized.\n", DRIVER_NAME);
    goto ret_ok;

ret_err_cdev_add:
    unregister_chrdev_region(m_led.deviceNumber, 1);
ret_err_alloc_chrdev_region:
    iounmap(m_led.regBase);
err_ioremap:
    release_mem_region(GPIO_BASE, GPIO_SIZE);
ret_ok:
    return result;
}

static void __exit LedExit(void)
{
    cdev_del(&m_led.cdev);
    unregister_chrdev_region(m_led.deviceNumber, 1);
    iounmap(m_led.regBase);
    printk("%s: exiting.\n", DRIVER_NAME);
}

module_init(LedInit);
module_exit(LedExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_VERSION("1.0");

