# LKM for listing all processes
In this kernel module we are going to write a code that will display all process starting from the init process.

## Creating the kernel module
> Create the 'list_process.c' file in the directory which contains the WSL2-Linux-Kernel
```
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/sched/task.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Preethi");
MODULE_DESCRIPTION("Linux Kernel Module for listing all processes");


void fix_display(int i,int count) //implementing a for loop as recursion cause for not available in kernel modules
{
        if(i!=count)
        {
                printk(KERN_CONT "| ");
                fix_display(i+1,count);
        }
}

void traverse_process(struct task_struct *process,int count)
{
        struct task_struct *next_process;
        struct list_head *list;
        fix_display(0,count);
        printk(KERN_CONT "|_%s %d %d\n",process->comm,process->pid,(process->parent)->pid);
        list_for_each(list, &process->children)
        {
                next_process=list_entry(list,struct task_struct,sibling);
                traverse_process(next_process,count+1);
        }
}



static int __init InitModule(void)
{
        printk("Loading Kernel Module\n");
        printk("Process name, PID, PPID\n");
        traverse_process(&init_task,0);
        return 0;
}

static void __exit ExitModule(void)
{
        printk("Removing Kernel Module\n");
}

module_init(InitModule);
module_exit(ExitModule);
```

### Extra details about the code
1.**&init_task**

It is a global variable and member of task_struct. In a linux system the `init` process is the root process(PID 1). It is the first process that is started on booting and it can start and stop other processes.

`init_task` is a global variable defined in the kernel source code so it cannot be used in user space modules. However it can be referenced in kernel modules using its address i.e &init_task


> this variable is defined in the header file `linux/sched.h`, in later versions this header was renamed into `linux/sched/task.h`

2.**task_struct**

It is a data structure which contains details about the processes. It has various data fields the main ones being:
- `comm` provides name of executable file that the process is running
- `pid` provides the process ID of the process
- `parent` provides the task_struct of the process' parent
- `children` gives the linked list containing the process' child processes
- `sibling` gives a linked list containing the processes with same parent

3.**list_for_each**

It is a macro in linux kernel that is used to iterate through a doubly linked list.
It takes a pointer to a `list_head` structure and a pointer to a loop variable to store it.
The loop body is executed once for each node in the list.
This macro is a more low-level way to iterate the linked list than using for and while, but it provides more efficiency and safety.


4.**list_entry**

Again a macro in linux kernel. This can be used to obtain the next process in this case. It takes a pointer to the `list_head` structure which holds the node. It takes the type of data structure of the node. Name of the list_head field used in the data structure.
By using list_entry, a pointer to `task_struct` is obtained without knowing offset of list_head field.

5.**fix_display**

To get the output in the form of a tree structure a for loop was needed, but in the kernel module for loop is not allowed.
`fix_display` is a recursive function which does the job of aligning so that a tree structure is seen.
The cursor is moved to nextline by default so `KERN_CONT` is used to continue it on same line.

## Headers used
1. `linux/init.h`   

This header contains the macros for the initialization function `module_init(function)` and cleanup function `module_exit(function)`. This allows the module to be dynamically loaded and unloaded.   
<br/>

2. `linux/module.h`   
- Module Metadata:
This header contains the macros for the metadata information like `MODULE_LICENSE`, `MODULE_AUTHOR`, `MODULE_DESCRIPTION`.
- Memory Management:
It also contains functions for dynamic memory allocation and freeing of memory like `kmalloc`, `kzalloc`, `kfree`.
- Symbol Exporting:
It has functions for exporting symbols from one module to others.`EXPORT_SYMBOL`, `EXPORT_SYMBOL_GPL`   
It also uses the macros defined in init.h for initialization and cleanup functions.
<br/>

3. `linux/kernel.h`

It provides the necessary definitions and macros for working with kernel-level code.
> printk(to view log in dmesg), LIST_HEAD need this header
<br/>

4. `linux/sched/task.h`
> It was initially `linux/task.h` which was updated into `linux/sched/task.h` in recent versions   

This header provides all the functionalities required for working with tasks(processes) and scheduling in the kernel.   
`task_struct`, `get_current`, `schedule`, kernel thread structures are some of the functions supported by this header.
<br/>

## Creating makefile
> Create the makefile in the same place as the module
```
vi Makefile
```
Content:
```

obj-m += list_process.o

all:
                make -C $(shell pwd)/WSL2-Linux-Kernel M=$(shell pwd) modules

clean:
                make -C $(shell pwd)/WSL2-Linux-Kernel M=$(shell pwd) clean
```
> Executing makefile
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

>lsmod now should not show this module

- to check print
```
dmesg | tail
```

**Linux Kernel Module to display all processes is successfully loaded!**
