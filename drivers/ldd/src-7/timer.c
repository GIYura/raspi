#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("PWM");

#define LED_PIN	17

/* variable for timer */
static struct timer_list m_timer;

static void timer_callback(struct timer_list * data)
{
	int led_state = gpio_get_value(LED_PIN);
	if (led_state)
	{
		gpio_set_value(LED_PIN, 0);
	}
	else
	{	
		gpio_set_value(LED_PIN, 1);
	}
}

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init moduleInit(void)
{
	printk("Hello, Kernel!\n");

	/* GPIO 17 init */
	if (gpio_request(LED_PIN, "rpi-gpio"))
	{
		printk("Can not allocate GPIO %d\n", LED_PIN);
		return -1;
	}

	if (gpio_direction_output(LED_PIN, 0))
	{
		printk("Can not set GPIO %d to output!\n", LED_PIN);
		gpio_free(LED_PIN);
		return -1;
	}

	/* Turn LED on */
	gpio_set_value(LED_PIN, 1);

	/* Initialize timer */
	timer_setup(&m_timer, timer_callback, 0);
	mod_timer(&m_timer, jiffies + msecs_to_jiffies(1000));	

	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit moduleExit(void)
{
	printk("Goodbye, Kernel\n");
	gpio_free(LED_PIN);
	del_timer(&m_timer);
	printk("Goodbye, Kernel\n");
}

module_init(moduleInit);
module_exit(moduleExit);

