/*
 *  pic.c - functions for pic interrupt controller
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

/* Refer to: http://www.lowlevel.eu/wiki/PIC_Tutorial */

#include "stdint.h"
#include "os.h"
#include "pic.h"
#include "print.h"
#include "ps2.h"

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_MASTER_IMR 0x21
#define PIC_SLAVE_COMMAND 0xA0
#define PIC_SLAVE_DATA 0xA1
#define PIC_SLAVE_IMR 0xA1

#define PIC_EOI 0x20

void pic_remap(int interrupt_num)
{
	/* Send initialisation code */
	outb(PIC_MASTER_COMMAND, 0x11);
	outb(PIC_SLAVE_COMMAND, 0x11);

	/*
	 * Send the offset in IDT -> IRQ0 is in interrupt descriptor
	 * table at entry with interrupt_num, usual 32 is taken
	 * because it is right after exceptions.
	 * value must be divisible without remainder by 8
	 */
	outb(PIC_MASTER_DATA, interrupt_num);
	outb(PIC_SLAVE_DATA, interrupt_num + 8); /* Second pic has the upper 8 interrupts */

	/* The two pics communicate via IRQ2 */
	outb(PIC_MASTER_DATA, (1 << 2)); /* Send to master with bitmask */
	outb(PIC_SLAVE_DATA, 2);		 /* Send to slave as binary value */

	/* Indicates that processor is in 8086 mode */
	outb(PIC_MASTER_DATA, 0x01);
	outb(PIC_SLAVE_DATA, 0x01);
}

void pic_mask_irqs(uint16_t mask)
{
	outb(PIC_MASTER_IMR, (uint8_t)mask);
	outb(PIC_SLAVE_IMR, (uint8_t)(mask >> 8));
}

void pic_send_eoi(int irq_num)
{
	outb(PIC_MASTER_COMMAND, PIC_EOI);
	if (irq_num > 7)
	{
		outb(PIC_SLAVE_COMMAND, PIC_EOI);
	}
}
