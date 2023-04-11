#include <linux/init.h>
#include <linux/module.h>

static int __init my_module_init(void)
{
    printk(KERN_INFO "Hello, world!\n");
    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "Goodbye, world!\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zuhayr Elahi");
MODULE_DESCRIPTION("A simple Linux kernel module.");
MODULE_VERSION("0.01");
