#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define GPIO_IRQ 17

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("A simple LKM for a gpio interrupt");

/** variable contains pin number o interrupt controller to which GPIO 17 is mapped to */
unsigned int irq_number;

static int gpio_value = 0;

static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
    printk("gpio_irq: Interrupt was triggered and ISR was called!\n");
    gpio_value = gpio_get_value(GPIO_IRQ);
    return (irq_handler_t) IRQ_HANDLED;
}

static ssize_t driver_read(struct file *file, char *user_buffer, size_t count, loff_t *offs)
{
/* TODO:  */

    int to_copy, not_copied, delta = 0;

    copy_to_user(user_buffer, gpio_value, sizeof(gpio_value));

    return delta;
}


static int __init ModuleInit(void)
{
    printk("qpio_irq: Loading module... ");

    /* Setup the gpio */
    if (gpio_request(GPIO_IRQ, "rpi-gpio-17"))
    {
        printk("Error!\nCan not allocate GPIO %d\n", GPIO_IRQ);
        return -1;
    }

    /* Set GPIO 17 direction */
    if (gpio_direction_input(GPIO_IRQ))
    {
        printk("Error!\nCan not set GPIO %d to input!\n", GPIO_IRQ);
        gpio_free(GPIO_IRQ);
        return -1;
    }

    /* Setup the interrupt */
    irq_number = gpio_to_irq(GPIO_IRQ);

    if (request_irq(irq_number, (irq_handler_t) gpio_irq_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0)
    {
        printk("Error!\nCan not request interrupt nr.: %d\n", irq_number);
        gpio_free(GPIO_IRQ);
        return -1;
    }

    printk("Done!\n");
    printk("GPIO %d is mapped to IRQ Nr.: %d\n", GPIO_IRQ, irq_number);
    return 0;
}

static void __exit ModuleExit(void)
{
    printk("gpio_irq: Unloading module...\n");
    free_irq(irq_number, NULL);
    gpio_free(GPIO_IRQ);

}

module_init(ModuleInit);
module_exit(ModuleExit);

