# GuyOS
GuyOS is an operating system written from scratch,
i am working on this project to learn more about kernel development,
further improve my low-level programming. And learn more about computers in general.

Pull requests are welcome!
## Screenshots

![Image of the OS with interrupts, keyboard&mouse](screenshot.png)

## Requirements

* Linux Host System(Tested on Ubuntu 20.04)

* Make(apt-get install make)
* g++
* xorriso
* grub-pc-bin
* VirtualBox
* binutils
* libc6-dev-i386

## Installation
Install required packages:
```bash
sudo apt-get install g++ binutils libc6-dev-i386
sudo apt-get install VirtualBox grub-pc-bin xorriso
```
Create a virtual machine without a hard drive.

To create a bootable ISO image, simply run:
```bash
make clean
make
```
Link the Virtual machine you created to the ISO image


## Features
* 32 bit Protected mode
* Dynamic drivers support, to add more drivers in the future
* Keyboard and mouse support
* Basic terminal
* Dynamic

## Current Goals
Here are the goals for the future.

* Implement Networking
* GUI
* Simple programs.
* extended libc

## Project Structure

Self explaintory, project layout.

### /include
The include folder has all the header files.

#### /drivers
header files for drivers are here

#### /hardware
includes interrupt header files, hardware ports, PCI.

#### /common
Has the types.h header.

### /src
Contains all the source code for the project.

#### /hardware
Source code for hardware management, (interrupts, PCI, and more)

#### kernel.cpp
The main kernel function, here everything is initialized and the main routine kernelMain

#### gdt.cpp
Code for the GDT(Global descriptor table)

#### /drivers
Source code for the drivers

## Known issues
- When building in other operating systems then ubuntu, sometimes grub-pc-bin creates an empty/malfunctioning ISO image.
- QEMU/VMware sometimes boot into the OS and sometimes not.