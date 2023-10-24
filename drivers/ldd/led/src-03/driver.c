#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/leds.h>
#include <linux/slab.h>

#define DRIVER_NAME         "led-driver"

#define LED_OFF             0
#define LED_ON              1

#define BCM2711_PERI_BASE   0xfe000000
#define GPIO_BASE           (BCM2711_PERI_BASE + 0x200000) /* GPIO controller */
#define GPIO_SIZE           9

#define GPIO_REG_DIR        0x08
#define GPIO_REG_SET        0x1c
#define GPIO_REG_CLR        0x28
#define GPIO_BIT            (1 << 3)

typedef struct
{
    void __iomem *regBase;
    struct led_classdev led_cdev;
} ledData_t;

static ledData_t* m_led;

static void LedSetDirection(void)
{
    u32 val;

    /* read value */
    val = readl(m_led->regBase + GPIO_REG_DIR);
    
    /* modify (set bit) */
    val |= GPIO_BIT;

    /* write value */
    writel(val, m_led->regBase + GPIO_REG_DIR);
}

static void LedClear(void)
{
    u32 val;

    /* read value */
    val = readl(m_led->regBase + GPIO_REG_CLR);

    /* modify (clear bit) */
    val &= ~GPIO_BIT;
    
    /* write value */
    writel(val, m_led->regBase + GPIO_REG_CLR);
}

static void LedSet(void)
{
    u32 val;

    /* read value */
    val = readl(m_led->regBase + GPIO_REG_SET);

    /* modify (set bit) */
    val |= GPIO_BIT;
    
    /* write value */
    writel(val, m_led->regBase + GPIO_REG_SET);
}

static void LedChangeState(struct led_classdev *led_cdev, enum led_brightness brightness)
{
    if (brightness)
        LedSet();
    else
        LedClear();
}

static int __init LedInit(void)
{
    int result = 0;

    m_led = kzalloc(sizeof(*m_led), GFP_KERNEL);
    if (!m_led)
    {
        result = -ENOMEM;
        goto ret_err_kzalloc;
    }

    m_led->regBase = ioremap(GPIO_BASE, GPIO_SIZE);
    if (!m_led->regBase)
    {
        printk("%s: Error mapping I/O!\n", DRIVER_NAME);
        result = -ENOMEM;
        goto err_ioremap;
    }

    m_led->led_cdev.name = "ipe:blue:user";
    m_led->led_cdev.brightness_set = LedChangeState;

    result = led_classdev_register(NULL, &m_led->led_cdev);
    if (result)
    {
        printk("%s: Error registering led\n", DRIVER_NAME);
        goto ret_err_led_classdev_register;
    }

    LedSetDirection();
        
    LedClear();

    printk("%s: initialized.\n", DRIVER_NAME);
    goto ret_ok;

ret_err_led_classdev_register:
    iounmap(m_led->regBase);
err_ioremap:
    release_mem_region(GPIO_BASE, GPIO_SIZE);
    kfree(m_led);
ret_err_kzalloc:
ret_ok:
    return result;
}

static void __exit LedExit(void)
{
    led_classdev_unregister(&m_led->led_cdev);
    iounmap(m_led->regBase);
    kfree(m_led);
    printk("%s: exiting.\n", DRIVER_NAME);
}

module_init(LedInit);
module_exit(LedExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_VERSION("1.0");

