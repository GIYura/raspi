#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>

#define DRIVER_NAME  "led-driver"

#define LED_OFF     0
#define LED_ON      1

#define GPIO_NUM    21

typedef struct
{
    struct gpio_desc *desc;
    struct led_classdev led_cdev;
} ledData_t;

static ledData_t* m_led;

static void LedSet(unsigned char status)
{
    if (status == LED_ON)
        gpiod_set_value(m_led->desc, 1);
    else
        gpiod_set_value(m_led->desc, 0);
}

static void OnLedChangeState(struct led_classdev *led_cdev, enum led_brightness brightness)
{
    if (brightness)
        LedSet(LED_ON);
    else
        LedSet(LED_OFF);
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

    result = gpio_request(GPIO_NUM, DRIVER_NAME);
    if (result)
    {
        printk("%s: Error requesting GPIO\n", DRIVER_NAME);
        goto ret_err_gpio_request;
    }

    m_led->desc = gpio_to_desc(GPIO_NUM);

    m_led->led_cdev.name = "ipe:blue:user";
    m_led->led_cdev.brightness_set = OnLedChangeState;

    result = led_classdev_register(NULL, &m_led->led_cdev);
    if (result)
    {
        printk("%s: Error registering led\n", DRIVER_NAME);
        goto ret_err_led_classdev_register;
    }

    gpiod_direction_output(m_led->desc, 0);

    printk("%s: initialized.\n", DRIVER_NAME);
    goto ret_ok;

ret_err_led_classdev_register:
    gpio_free(GPIO_NUM);
ret_err_gpio_request:
    kfree(m_led);
ret_err_kzalloc:
ret_ok:
    return result;
}

static void __exit LedExit(void)
{
    led_classdev_unregister(&m_led->led_cdev);
    gpio_free(GPIO_NUM);
    kfree(m_led);
    printk("%s: exiting.\n", DRIVER_NAME);
}

module_init(LedInit);
module_exit(LedExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_VERSION("1.0");

