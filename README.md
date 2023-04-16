# OS_kernel_module

**Aim**:    
Build a kernel module that can list all the process starting from the init process in a DFS(Depth-First-Search) fashion. Load this into the kernel and see the execution.   
> This process is much easier on the actual Linux OS, however I am doing it on WSL2 and it requires a few extra steps.   

Step 1: Build the custom Linux Kernel   
> The steps to do this is explained in [build_custom_kernel.md](/build_custom_kernel.md)        

Step 2: Test the custom Kernel with a simple hello module
> The steps to do this can be found in [hello_module.md](/hello_module.md)

Step 3: Proceed with the main aim. Build the module to list processes.
> The steps to do this is in [process_list.md](/process_list.md)

Note:   
The `MODULE_LICENSE` describes the license which our module can be used under.   
To check the information about a specific module use `modinfo <filename>.ko`.
