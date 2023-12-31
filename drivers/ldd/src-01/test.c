/*
Simple kernel module
*/
#include <linux/module.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("Hello world for Linux Kernel Module");

/* This function is called, when the module is loaded into the kernel */
static int __init moduleInit(void)
{
    printk("Hello, Kernel!\n");
    return 0;
}

/* This function is called, when the module is removed from the kernel */
static void __exit moduleExit(void)
{
    printk("Goodbye, Kernel\n");
}

module_init(moduleInit);
module_exit(moduleExit);

