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

	


