#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include "ioctl_ex.h"

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("IOCTL example");

#define MYMAJOR 64

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance)
{
	printk("driver_open called!\n");
	return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance)
{
	printk("driver_close called!\n");
	return 0;
}

int32_t answer = 45;

static long int ioctl_example(struct file* file, unsigned cmd, unsigned long arg)
{
    struct mystruct test;
    switch(cmd)
    {
        case WR_VALUE:
            if (copy_from_user(&answer, (int32_t *) arg, sizeof(answer))) 
                printk("ioctl_example - Error copying data from user!\n");
            else
                printk("ioctl_example - Update the answer to %d\n", answer);
        break;
    
        case RD_VALUE:
            if (copy_to_user((int32_t *) arg, &answer, sizeof(answer))) 
                printk("ioctl_example - Error copying data to user!\n");
            else
                printk("ioctl_example - The answer was copied!\n");
            break;

        case GREETER:
            if (copy_from_user(&test, (struct mystruct *) arg, sizeof(test))) 
                printk("ioctl_example - Error copying data from user!\n");
            else
                printk("ioctl_example - %d greets to %s\n", test.repeat, test.name);
            break;  
    }
    return 0;
}

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
    .unlocked_ioctl = ioctl_example
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) 
{
	int retval;
	printk("Hello, Kernel!\n");
	/* register device nr. */
	retval = register_chrdev(MYMAJOR, "my_dev", &fops);
	if (retval == 0)
	{
		printk("dev - registered Device number Major: %d, Minor: %d\n", MYMAJOR, 0);
	}
	else if (retval > 0)
	{
		printk("dev - registered Device number Major: %d, Minor: %d\n", retval>>20, retval&0xfffff);
	}
	else
	{
		printk("Could not register device number!\n");
		return -1;
	}
	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void)
{
	unregister_chrdev(MYMAJOR, "my_dev");
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

