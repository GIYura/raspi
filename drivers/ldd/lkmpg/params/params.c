#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("Example shows how to pass value to the module");

static short int valueShortInt = 11;
static int valueInt = 22;
static long int valueLongInt = 33;
static char *valueString = "Hello";
static int valueBuffer[2] = { 555, 555 };
static int bufferSize = 0;

module_param(valueShortInt, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(valueShortInt, "A short integer");

module_param(valueInt, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(valueInt, "An integer");

module_param(valueLongInt, long, S_IRUSR);
MODULE_PARM_DESC(valueLongInt, "A long integer");

module_param(valueString, charp, 0000);
MODULE_PARM_DESC(valueString, "A character string");

module_param_array(valueBuffer, int, &bufferSize, 0000);
MODULE_PARM_DESC(valueBuffer, "An array of integers");

static int __init moduleInit(void)
{
    int i;
	pr_info("Hello, Kernel!\n");

    pr_info("valueShortInt is a short integer: %hd\n", valueShortInt);
    pr_info("valueInt is an integer: %d\n", valueInt);
    pr_info("valueLongInt is a long integer: %ld\n", valueLongInt);
    pr_info("valueString is a string: %s\n", valueString);
    for (i = 0; i < ARRAY_SIZE(valueBuffer); i++)
    {
        pr_info("valueBuffer[%d] = %d\n", i, valueBuffer[i]);
    }
    pr_info("got %d arguments for buffer size.\n", bufferSize);
	return 0;
}

static void __exit moduleExit(void)
{
	pr_info("Goodbye, Kernel\n");
}

module_init(moduleInit);
module_exit(moduleExit);

