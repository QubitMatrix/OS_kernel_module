## Building a custom kernel   
1. Install basic pre-requisites:   

   ```
   sudo apt install build-essential flex bison libssl-dev libelf-dev git dwarves
   ```   
   > `build-essential` - contains basic tools needed to compile and build softare from source code. (needed for make)   
   > `libssl-dev` - development files for OpenSSL cryptography library.(needed if kernel module uses SSL/TLS(Secure Socket Layer and Transport Layer Security) encryption.   
   > `libelf-dev` - development files for the ELF(Executable and Linkable Format) object file format.(needed if kernel module interacts with ELF files)   
   > `dwarves` - set of debugging tools that can analyze and debug the ELF format files.   
   > `git` -to host kernel module on GitHub.   

   These two packages may come handy if the kernel module needs to parse a configuration file.(but this may increase the kernel module's size)
   - `flex` - generates scanners(lexical analyzers) for programming languages.
   - `bison` - generates parsers for programming languages.   
<br/>   

2. Clone the microsoft version of kernel for wsl2:      
   ```
   git clone https://github.com/microsoft/WSL2-Linux-Kernel.git
   ```
   > The repo is about 2.3 GB
<br/>   

3. Create a copy of original config file before proceding   
   ```
   cd WSL2-Linux-Kernel
   cp .config original_config
   ```
   Now copy this config file to the existing wsl config   
   ```
   cp Microsoft/config_wsl .config
   ```   

4. Update the configurations with new configs:   
   ```
   make prepare
   ```
   > /bin/sh: 1: bc: not found
make[1]:  [Kbuild:24: include/generated/timeconst.h] Error 127
make:  [Makefile:1254: prepare0] Error 2   

   > To fix this error install the bc package and re-run the `make` command   
   ```
   sudo apt-get install bc   
   ```

5. Compile kernel   
   ```
   make -j$(nproc)
   ```
   > -j indicates how many processes can run at a time, nprocs give the max number of processes that can be supported   

   > This takes quite a bit of time   
`Kernel: arch/x86/boot/bzImage is ready` indicates successful compilation
<br/>   

6. Copy the image into a folder in windows   
   ```
   mkdir /mnt/c/Users/<username>/wslkernel   
   cp arch/x86/boot/bzImage /mnt/c/Users/<username>/wslkernel/kernel_bzImage
   ```

7. Reference the new custom kernel in the .wslconfig
   ```
   vi /mnt/c/Users/<username>/.wslconfig
   ```
   Content:
   ```
   [wsl2]
   kernel = c:\\Users\\<username>\\wslkernel\\kernel_bzImage
   swap = 0
   localhostForwarding = true
   ```

8. Shutdown the wsl(in powershell) to set the new configurations
   ```
   wsl --shutdown
   ```
   
9. Check if the kernel got updated
   ```
   uname -a
   ```
   > `uname`- Unix NAME (-a stands for all), it shows all details about the kernel and the system.   
   > '+' along with the kernel build release (`uname -r`) indicates it is a custom build.   
   >  `uname -v` gives the kernel version, this will show the time when kernel was last changed.   
   
   > Note: If there are multiple distros on wsl, all of them use the same kernel

<br/>   


## Reverting the kernel back to its default build   
The custom kernel is built on the WSL2 kernel from the GitHub repo and while both the kernels are derivatives of the upstream Linux kernel they have different specifications.   
While the default kernel gets regular security patches and other updates, the custom kernel allows more functionality and features but at the same time it may not be the most stable version of kernel. This new custom kernel that we built might lead to performance issues in WSL and it is best to revert the kernel back to its default build when the kernel module exercise is complete.   
> Note: The kernel modules cannot be compiled or loaded into the kernel once it is reverted.   

### Steps to revert   
- make a copy of the `.wslconfig` file in the C drive to revert back to the custom build later.
- remove `.wslconfig` from the C drive in windows 
- `wsl --shutdown` (in powershell)
- open the WSL distro and run `uname -a` to check if the kernel got reverted(if it got reverted the date will go back to the last update of your kernel and the '+' sign will not exist next to the build name)
> Fun fact: WSL identifies the need for a custom build when the `.wslconfig` file is in the c drive. It then uses the image specified in that file to boot that kernel build. If the file is not found it just uses the default kernel with which WSL comes.

