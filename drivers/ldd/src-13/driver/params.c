#include <linux/module.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("Pass params to kernel module");

static unsigned int gpioNumber = 12;
static char* deviceName = "test-device";

module_param(gpioNumber, uint, S_IRUGO);
module_param(deviceName, charp, S_IRUGO);

MODULE_PARM_DESC(gpioNumber, "Number of GPIO to use in this LKM");
MODULE_PARM_DESC(deviceName, "Device name to use in this LKM");

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init moduleInit(void)
{
    printk("gpio-number = %u\n", gpioNumber);
    printk("device-name = %s\n", deviceName);
	printk("Hello, Kernel!\n");
	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit moduleExit(void)
{
	printk("Goodbye, Kernel\n");
}

module_init(moduleInit);
module_exit(moduleExit);

