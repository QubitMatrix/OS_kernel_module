1. Install basic pre-requisites:   

```
sudo apt install build-essential flex bison libssl-dev libelf-dev git dwarves
```
2. Clone the microsoft version of kernel for wsl2:      

```
git clone https://github.com/microsoft/WSL2-Linux-Kernel.git
```
> The repo is about 2.3 GB

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
> This will show the time when kernel was last changed

> Note: If there are multiple distros on wsl, all of them use the same kernel

