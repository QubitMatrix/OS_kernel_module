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


char* get_state_string(long state)
{
    switch (state)
    {
        case TASK_RUNNING:
            return "Running";
        case TASK_INTERRUPTIBLE:
            return "Sleeping (Interruptible)";
        case TASK_UNINTERRUPTIBLE:
            return "Sleeping (Uninterruptible)";
        case __TASK_STOPPED:
            return "Stopped";
        case __TASK_TRACED:
            return "Tracing";
        case TASK_DEAD:
            return "Dead(Zombie)";
        case TASK_WAKEKILL:
            return "Wakekill";
        case TASK_WAKING:
            return "Waking";
        case TASK_PARKED:
            return "Parked";
        case TASK_NOLOAD:
            return "Noload";
        case TASK_NEW:
            return "New";
        case 1026://1024 OR 2
            return "New (Interruptible)";
        case 1028://1024 OR 4
            return "New (Uninterruptible)";
        default:
            return "Unknown";
    }
}


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
        printk(KERN_CONT "|_%s(PID:%d, PPID:%d) ---> %s\n",process->comm,process->pid,(process->parent)->pid,get_state_string(process->__state));
        list_for_each(list, &process->children)
        {
                next_process=list_entry(list,struct task_struct,sibling);
                traverse_process(next_process,count+1);
        }
}


static int __init InitModule(void)
{
        printk("Loading Kernel Module\n");
        printk("Process name, PID, PPID, State\n");
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
1. All the three MODULE declarations are compulsary, doesn't compile without those.  
  
2. The kernel functions are defined as static for the following reasons   
   - It is not needed outside of this file, so it is provides a good encapsulation.
   - Avoids name collisions and reduces the size of kernel binary.
   - Additionally, declaring it as static prevents it from being used in the global space.   

3. **GPL** -GNU General Public License   
It is a widely used open source software license created by the Free Software Foundation(FSF), it is a copyleft license which grants other users the rights to modify and redistribute the software. It is also used in Linux Kernel and GCC.   

4. `__init` is a macro that indicates that this portion of the code will only be used at initialization and nowhere else. This helps in reducing the memory footprint of the kernel as it lets it know it can be discarded after initialization hence giving it a improved performance.   
Similarly `__exit` is used only at time of exiting the module.
   > It is important to include these macros in the functions as without these the functions may have undefined behaviour or cause system crashes when called after initialization as the function may depend on resources that are available only during the initialization process.   

5. States of a process in Linux:
A process can be in various states and the task_struct has about 10 states. They can be categorized into 5 main states.
   - Running
   - Waiting(can be interruptable or uninterruptible)
   - Sleeping
   - Stopped
   - Zombie   

   The `task_struct` structure has a function to get the state `<process variable name>->__state`, but it gives it as a numerical value, however the header file has defined the states as these corresponding numbers so by using a simple switch case the states can be retrieved in the form of words. Some states are formed by the logical OR of two or more states and the code above takes care of two such states.    
<br/>   

## Explanation of structures and functions used in the code:   
1. **&init_task**   
It is a global variable and member of task_struct. In a linux system the `init` process is the root process(PID 1). It is the first process that is started on booting and it can start and stop other processes.   
   `init_task` is a global variable defined in the kernel source code so it cannot be used in user space modules. However it can be referenced in kernel modules using its address i.e &init_task   
   > this variable is defined in the header file `linux/sched.h`, in later versions this header was renamed into `linux/sched/task.h`

2. **task_struct**   
It is a data structure which contains details about the processes. It has various data fields the main ones being:
   - `comm` provides name of executable file that the process is running
   - `pid` provides the process ID of the process
   - `parent` provides the task_struct of the process' parent
   - `children` gives the linked list containing the process' child processes
   - `sibling` gives a linked list containing the processes with same parent
   - `__state` gives the state of the process (a number which is a power of 2 or logical OR of some powers of 2)   

3. **list_for_each**   
It is a macro in linux kernel that is used to iterate through a doubly linked list.   
It takes a pointer to a `list_head` structure and a pointer to a loop variable to store it.   
The loop body is executed once for each node in the list.   
This macro is a more low-level way to iterate the linked list than using for and while, but it provides more efficiency and safety.   

4. **list_entry**   
Again a macro in linux kernel. This can be used to obtain the next process in this case. It takes a pointer to the `list_head` structure which holds the node. It takes the type of data structure of the node. Name of the list_head field used in the data structure.   
By using list_entry, a pointer to `task_struct` is obtained without knowing offset of list_head field.   

5. **fix_display**   
To get the output in the form of a tree structure a for loop was needed, but in the kernel module for loop is not allowed.   
`fix_display` is a recursive function which does the job of aligning so that a tree structure is seen.   
The cursor is moved to nextline by default so `KERN_CONT` is used to continue it on same line.   

6. **traverse_process**   
It is used to traverse through the processes running in the system in a DFS(Depth-First-Search) fashion.   

<br/>   

## Headers used   
1. `linux/init.h`   
This header contains the macros for the initialization function `module_init(function)` and cleanup function `module_exit(function)`. This allows the module to be dynamically loaded and unloaded.   

2. `linux/module.h`   
   - Module Metadata:
This header contains the macros for the metadata information like `MODULE_LICENSE`, `MODULE_AUTHOR`, `MODULE_DESCRIPTION`.
   - Memory Management:
It also contains functions for dynamic memory allocation and freeing of memory like `kmalloc`, `kzalloc`, `kfree`.
   - Symbol Exporting:
It has functions for exporting symbols from one module to others.`EXPORT_SYMBOL`, `EXPORT_SYMBOL_GPL`   
It also uses the macros defined in init.h for initialization and cleanup functions.   

3. `linux/kernel.h`   
It provides the necessary definitions and macros for working with kernel-level code.
   > printk(to view log in dmesg), LIST_HEAD need this header   

4. `linux/sched/task.h`   
This header provides all the functionalities required for working with tasks(processes) and scheduling in the kernel.   
`task_struct`, `get_current`, `schedule`, kernel thread structures are some of the functions supported by this header.
   > It was initially `linux/task.h` which was updated into `linux/sched/task.h` in recent versions   
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

### Explanation of the makefile   
`make` command can be used to build exectubles from source code.   
`obj-m += list_process.o` is used to add the list_process.o object file into the list of executables that is built to load kernel modules.   
> `obj-m` is a special variable used by the kernel build system to identify the object files that has to be built as loadable kernel modules.   

- `all` is used as a default rule, this is used to build the module.   
- `make -C $(shell pwd)/WSL2-Linux-Kernel M=$(shell pwd) modules`.   
- `make -C` specifies the directory where the Makefile exists.
- `$(shell pwd)` is used to run the command `pwd`(present working directory) as a shell script and place the answer in the makefile.
- `M=` is used to specify where the source files mentioned in the Makefile exist, the command `make modules` is executed in this location. The .ko files along with other necessary files get created.
- `clean` is used as a rule which helps in cleaning up the build artifacts. This deletes all the files that were created by running `make`.   
<br/>   

## Loading the kernel module into kernel
- insmod stands for insert module
   ```
   sudo insmod hello_module.ko
   ```
- check if module is there in the list of loaded modules
   ```
   lsmod | grep hello_module
   ```
- check what is printed
   ```
   dmesg | tail
   ```

## Unloading the kernel module from the kernel

- rmmod stand for remove module
   ```
   sudo rmmod hello_module.ko
   ```

   >lsmod now should not show this module

- check print log 
   ```
   dmesg | tail
   ```
<br/>   

**Linux Kernel Module to display all processes is successfully loaded!**
