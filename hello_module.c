#include <linux/init.h>
#include <linux/module.h>

//Meta informations
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Preethi");
MODULE_DESCRIPTION("Hello linux kernel module");

//Function that is executed when the module is loaded into the kernel (return int to know if successfully loaded(0) or not(-1))
static int __init InitModule(void) 
{
	printk("Hello, module loaded\n");
	//printf prints to the command line but no command line is available in kernel, so printk is used where it prints to the linux kernel locks
	return 0;//to say successfully loaded
}

//Function that is executed when the module is removed from the kernel
static void __exit ExitModule(void)
{
	printk("Module unloaded\n");
}

module_init(InitModule);//defines which function to load at init
module_exit(ExitModule);//defines which function to load at exit









