/*
 *  pci.c - functions for accessing pci bus
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

#include "pci.h"

#define PCI_CONFIG_DATA		0x0CFC
#define PCI_CONFIG_ADDRESS	0x0CF8

/* 
 * Refer to:
 * http://www.lowlevel.eu/wiki/Peripheral_Component_Interconnect#Zugriff_auf_den_Konfigurations-Adressraum
 * http://wiki.osdev.org/Pci#Enumerating_PCI_Buses
 */

/* From: http://www.lowlevel.eu/wiki/Peripheral_Component_Interconnect#Zugriff_auf_den_Konfigurations-Adressraum */
int pci_config_readl(int bus, int dev, int func, int offset)
{
	int val;
	int address = 0x80000000 | (bus << 16) | (dev << 11) | (func << 8) | (offset & 0xFC);
	outl(PCI_CONFIG_ADDRESS, address);
	val = inl(PCI_CONFIG_DATA);
	return val;
}

void pci_config_writel(int bus, int dev, int func, int offset, int val)
{
	int address	= 0x80000000 | (bus << 16) | (dev << 11) | (func << 8) | (offset & 0xFC);
	outl(PCI_CONFIG_ADDRESS, address);
	outl(PCI_CONFIG_DATA, val);
}
