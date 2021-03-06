/*
 *  init.c - first executed c file of os
 *  Copyright (C) 2016  Simon Kaufmann, HeKa
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "os.h"
#include "print.h"
#include "stdarg.h"
#include "pci.h"
#include "util.h"
#include "ps2.h"
#include "main.h"
#include "interrupt.h"
#include "gdt.h"
#include "syscall.h"
#include "char_buffer.h"

/* 
 * Assemble the file with the CFLAGS from Makefile and -S init.c
 *
 * Refer to: http://www.lowlevel.eu/wiki/CMOS
 */

#define CMOS_PORT_ADDRESS 0x70 /* IO-space address for the cmos-offset-addresses */
#define CMOS_PORT_DATA 0x71	   /* IO-space address for the cmos-data */

void init()
{
	char Message[] =
		"\n----------------------------------------------------------------------------\n"
		"\n"
		"                                    VIENNICE\n"
		"\n"
		"\n"
		"     Experimental operating system for x86 systems\n"
		"\n"
		"     (c) Simon Kaufmann, 2020\n"
		"\n"
		"----------------------------------------------------------------------------\n";

	kprintf(Message);

	gdt_init();
	idt_init();
	ps2_init();

	kprintf("\nPCI-Bus Reading:\n");
	int device, bus;
	for (bus = 0; bus < 3; bus++)
	{
		for (device = 0; device < 10; device++)
		{
			int pci_ret = pci_config_readl(bus, device, 0, 0);
			int vendor_id = pci_ret & 0xFFFF;
			if (vendor_id != 0xFFFF)
			{
				/* It is a device */
				pci_ret = pci_config_readl(bus, device, 0, 0x008);
				int class_id = (pci_ret >> 8) & 0xFFFFFF;
				kprintf("Bus: %d Dev: %d Vendor ID: 0x%x ClassId: %x\n", bus, device, vendor_id, class_id);
			}
		}
	}

	kprintf("\n");

	char hallo[] = "Syscall Write Tested\n";
	write(0, hallo, sizeof(hallo));

	while (1)
	{
		char buffer[1];
		if (read(0, buffer, 1) > 0)
		{
			kprintf("%c", buffer[0]);
		}
	}
}
