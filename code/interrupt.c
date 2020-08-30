/*
 *  interrupt.c - functions for interrupt descriptor table
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

/*
 * Software interrupt register calling: https://en.wikibooks.org/wiki/X86_Assembly/Interfacing_with_Linux
 */

#include "stdint.h"
#include "interrupt.h"
#include "os.h"
#include "pic.h"

#define IDT_ENTRIES 256 /* Maximum 256 */

#define IDT_TRAP_GATE_16_BIT 0b0111
#define IDT_TRAP_GATE_32_BIT 0b1111
#define IDT_TASK_GATE_32_BIT 0b0101
#define IDT_INTERRUPT_GATE_16_BIT 0b0110
#define IDT_INTERRUPT_GATE_32_BIT 0b1110

uint32_t idt[2 * IDT_ENTRIES];

void idt_init()
{
	pic_remap(32);

	/* Make entries for exceptions */
	int i;
	for (i = 0; i < 32; i++)
	{
		if (i == 8 || i == 10 || i == 11 || i == 12 || i == 13 || i == 14 || i == 17 || i == 0x1f)
		{
			/* They save also an error value on stack which has to be deleted from stack */
			idt_set_entry(i, default_isr, 0b1000, IDT_INTERRUPT_GATE_32_BIT, 0, 0, 1);
		}
		else
		{
			idt_set_entry(i, default_isr_no_error, 0b1000, IDT_INTERRUPT_GATE_32_BIT, 0, 0, 1);
		}
		/*
		 * gate_type: IDT_TRAP_GATE_32_BIT: hardware interrupts remain active,
		 * storage_segment: segment 0 without paging yet
		 * descriptor_privilege_level: ring 0 -> kernel mode?
		 * present: 1 -> active
		 */
	}

	idt_set_entry(32, idt32, 0b1000, IDT_INTERRUPT_GATE_32_BIT, 0, 0, 1);

	for (i = 33; i < 128; i++)
	{
		idt_set_entry(i, default_isr_no_error, 0b1000, IDT_INTERRUPT_GATE_32_BIT, 0, 0, 1);
	}

	idt_set_entry(128, idt128, 0b1000, IDT_INTERRUPT_GATE_32_BIT, 0, 0, 1);

	for (i = 129; i < 256; i++)
	{
		idt_set_entry(i, default_isr_no_error, 0b1000, IDT_INTERRUPT_GATE_32_BIT, 0, 0, 1);
	}

	struct
	{
		uint16_t limit;
		void *pointer;
	} __attribute__((packed)) idtp = {
		.limit = IDT_ENTRIES * 8 - 1,
		.pointer = idt,
	};

	lidt_asm(&idtp);

	asm volatile("sti");
}

void idt_set_entry(short int num, unsigned int offset, short int selector, char gate_type,
				   char storage_segment, char descriptor_privilege_level, char present)
{
	idt[2 * num] = 0;
	idt[2 * num + 1] = 0;
	idt[2 * num] |= offset & 0xFFFF;
	idt[2 * num] |= (selector & 0xFFFF) << 16;
	idt[2 * num + 1] |= (gate_type & 0xF) << 8;
	idt[2 * num + 1] |= (storage_segment & 0b1) << 12;
	idt[2 * num + 1] |= (descriptor_privilege_level & 0b11) << 13;
	idt[2 * num + 1] |= (present & 0b1) << 15;
	idt[2 * num + 1] |= (offset & 0xFFFF0000);
}
