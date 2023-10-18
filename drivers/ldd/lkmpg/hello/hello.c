#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("Hello world Linux Kernel Module");

static int __init moduleInit(void)
{
	pr_info("Hello, Kernel!\n");
	return 0;
}

static void __exit moduleExit(void)
{
	pr_info("Goodbye, Kernel\n");
}

module_init(moduleInit);
module_exit(moduleExit);

