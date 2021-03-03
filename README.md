# GuyOS
My attempt in making my own operating system from scratch.

I am building GuyOS in order to learn more about operating systems, aswell as to practice low-level programming.


## Requirements
* Linux Host System(Tested on ubuntu 20.04)

```bash
sudo apt-get install g++ binutils libc6-dev-i386
sudo apt-get install VirtualBox grub-legacy xorriso
```
* Make

## Installation
To create an ISO image, simply run:
```bash
make clean
make install
make run
```
