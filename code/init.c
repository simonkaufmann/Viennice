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

/* assemble the file with the CFLAGS from Makefile and -S init.c */

/* refer to: http://www.lowlevel.eu/wiki/CMOS */
#define CMOS_PORT_ADDRESS		0x70 /* io-space address for the cmos-offset-addresses */
#define CMOS_PORT_DATA			0x71 /* io-space address for the cmos-data */

/* function declarations */
void print(const char* string);
void println(const char* string);
void print_int(int number);

void init()	{
	char Message[] = "Operating System - VIENNICE";
	println(Message);

	gdt_init();
	idt_init();
	ps2_init();

	printf("\nPCI-Bus Reading:\n");
	int device, bus;
	for (bus = 0; bus < 3; bus++)	{
		for (device = 0; device < 10; device++)	{
			int pci_ret = pci_config_readl(bus, device, 0, 0);
			int vendor_id = pci_ret & 0xFFFF;
			if (vendor_id != 0xFFFF)	{
				/* it is a device */
				pci_ret = pci_config_readl(bus, device, 0, 0x008);
				int class_id = (pci_ret >> 8) & 0xFFFFFF;
				printf("Bus: %d Dev: %d Vendor ID: 0x%x ClassId: %x\n", bus, device, vendor_id, class_id);
			}
		}
	}
}



