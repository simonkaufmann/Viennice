# Viennice - Small x86-OS project

## Run the project in emulator

If you would like to run the project in qemu emulator, create os.img file
according to the manual in doc/manual "Grub installation to Image file".

Then go to code directory and execute make run (root might be needed because
the make run tries to automatically mount the image file os.img to copy the kernel file created from c-files)

## Create bootable USB flash drive with the OS

Look at doc/manual for instructions to create a bootable GRUB flash drive which can boot the OS.

## What can the OS do?

Only very basic stuff. A small keyboard driver and interrupts are implemented. Apart from that extensions are appreciated.
