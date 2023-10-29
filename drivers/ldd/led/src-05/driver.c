#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>

#define LED_OFF 0
#define LED_ON  1

typedef struct
{
    struct gpio_desc *desc;
    struct led_classdev led_cdev;
} ledData_t;

static ledData_t* m_led;

static void LedSet(unsigned int status)
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

static int LedProbe(struct platform_device *pdev)
{
    struct device_node *np = pdev->dev.of_node;
    struct device_node *child = NULL;
    int result, gpio;

    child = of_get_next_child(np, NULL);

    m_led = devm_kzalloc(&pdev->dev, sizeof(*m_led), GFP_KERNEL);
    if (!m_led)
        return -ENOMEM;

    gpio = of_get_gpio(child, 0);

    result = devm_gpio_request(&pdev->dev, gpio, pdev->name);
    if (result)
    {
        dev_err(&pdev->dev, "Error requesting GPIO\n");
        return result;
    }

    m_led->desc = gpio_to_desc(gpio);

    m_led->led_cdev.name = of_get_property(child, "label", NULL);
    m_led->led_cdev.brightness_set = OnLedChangeState;

    result = devm_led_classdev_register(&pdev->dev, &m_led->led_cdev);
    if (result)
    {
        dev_err(&pdev->dev, "Error registering led\n");
        return result;
    }

    gpiod_direction_output(m_led->desc, 0);

    dev_info(&pdev->dev, "initialized.\n");

    return 0;
}

static int LedRemove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "exiting.\n");

    return 0;
}

static const struct of_device_id of_drvled_match[] = {
    { .compatible = "led-driver" },
    {},
};

static struct platform_driver drvled_driver = {
    .driver = {
        .name   = "led-driver",
        .owner  = THIS_MODULE,
        .of_match_table = of_drvled_match,
    },
    .probe      = LedProbe,
    .remove     = LedRemove,
};

module_platform_driver(drvled_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_VERSION("1.0");

