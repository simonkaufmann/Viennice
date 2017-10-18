/*
 *  gdt.c - functions for global descriptor table
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


/* refer to: http://www.lowlevel.eu/wiki/Global_Descriptor_Table */

#include "gdt.h"
#include "stdint.h"
#include "os.h"

#define GDT_ENTRIES 	3

#define GDT_ACCESS_BYTE_PRESENT_BIT	7
#define GDT_ACCESS_BYTE_PRIVILEGE	5 /* 6 and 5 */
#define GDT_ACCESS_BYTE_SEGMENT_BIT	4
#define GDT_ACCESS_BYTE_EXECUTABLE_BIT	3
#define GDT_ACCESS_BYTE_DIRECTION_BIT	2
#define GDT_ACCESS_BYTE_CONFORMING_BIT	2
#define GDT_ACCESS_BYTE_READABLE_BIT	1
#define GDT_ACCESS_BYTE_WRITEABLE_BIT	1
#define GDT_ACCESS_BYTE_ACCESSED_BIT	0

//#define GDT_FLAGS_GRANULARITY		7
//#define GDT_FLAGS_SIZE_BIT		6
//#define GDT_LONG_MODE_BIT		5
//#define GDT_AVAILABLE_BIT		4

#define GDT_FLAGS_GRANULARITY		3
#define GDT_FLAGS_SIZE_BIT		2
#define GDT_LONG_MODE_BIT		1
#define GDT_AVAILABLE_BIT		0



//struct GDTDescr	{
//	uint16_t limit;
//	uint16_t base_1;
//	uint8_t base_2;
//	uint8_t access;
//	uint8_t flags_limit;
//	uint8_t base_3;
//};

//struct GDTDescr gdt[GDT_ENTRIES];
uint32_t gdt[2*GDT_ENTRIES];

void gdt_init()
{
	gdt_set_entry(0, 0, 0, 0); /* zero-descriptor */

	int access_byte;
	int flags;

	access_byte = (1 << GDT_ACCESS_BYTE_PRESENT_BIT) | (1 << GDT_ACCESS_BYTE_SEGMENT_BIT) |
			(1 << GDT_ACCESS_BYTE_EXECUTABLE_BIT) | (1 << GDT_ACCESS_BYTE_READABLE_BIT);
	flags = (1 << GDT_FLAGS_GRANULARITY) | (1 << GDT_FLAGS_SIZE_BIT);
	gdt_set_entry(1, 0, 0xFFFFF, access_byte | (flags << 8));

	access_byte = (1 << GDT_ACCESS_BYTE_PRESENT_BIT) | (1 << GDT_ACCESS_BYTE_SEGMENT_BIT) |
			(1 << GDT_ACCESS_BYTE_WRITEABLE_BIT);
	flags = (1 << GDT_FLAGS_GRANULARITY) | (1 << GDT_FLAGS_SIZE_BIT);
	gdt_set_entry(2, 0, 0xFFFFF, access_byte | (flags << 8));
	struct {
	    uint16_t limit;
	    void* pointer;
	} __attribute__((packed)) gdtp = {
	    .limit = GDT_ENTRIES * 8 - 1,
	    .pointer = gdt,
	};
	lgdt_asm(&gdtp);
}


void gdt_set_entry(int i, unsigned int base, unsigned int limit, int flags)
{
	gdt[2*i] = 0;
	gdt[2*i+1] = 0;
	gdt[2*i] |= limit & 0xffffLL;
	gdt[2*i] |= (base & 0xffffLL) << 16;
	gdt[2*i+1] |= (base & 0xff0000LL) >> 16;
	gdt[2*i+1] |= (flags & 0xffLL) << 8;
	gdt[2*i+1] |= ((limit >> 16) & 0xfLL) << 16;
	gdt[2*i+1] |= ((flags >> 8 )& 0xffLL) << 20;
	gdt[2*i+1] |= ((base >> 24) & 0xffLL) << 24;
}
