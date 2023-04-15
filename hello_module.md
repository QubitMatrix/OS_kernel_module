## Introduction to Linux Kernel Module(LKM):
A kernel module is a piece of software that runs in the kernel space.  
**Advantages**:   
- Can access the hardware and kernel directly.
- Load dynamically into kernel and out of it.   

**Disadvantages**:   
- Can't use float variables.
- Smaller stack space than user space.
- Can only access specific header files inside the kernel modules.
> But this is the right place to write the device drivers.

## Creating the kernel module:
> Create the 'hello_module.c' file in the directory which contains the WSL2-Linux-Kernel   
```
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
```
## Creating makefile
> Create the makefile in the same place as the module   
```
vi Makefile
```
Content:
```
obj-m += hello_module.o

all:
		make -C $(shell pwd)/WSL2-Linux-Kernel M=$(shell pwd) modules
clean:
		make -C $(shell pwd)/WSL2-Linux-Kernel M=$(shell pwd) clean
```

> Execute the makefile   
```
make
```

## Loading the kernel module into kernel
- insmod stands for insert module   
```
sudo insmod hello_module.ko
```   
- check if module is there in the list of loaded modules   
```
lsmod | grep hello_module
```
- to check what is printed   
```
dmesg | tail
```
## Unloading the kernel module from the kernel
- rmmod stand for remove module   
```
sudo rmmod hello_module.ko
```
> lsmod now should not show this module   

- to check print   
```
dmesg | tail
```
**Hello World module is successfully loaded and unloaded!**

