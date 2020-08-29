/*
 *  interrupt.h - declarations for interrupt descriptor table
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

#ifndef INTERRUPT_H
#define INTERRUPT_H

void idt_init();
void idt_set_entry(short int num, unsigned int offset, short int selector, char gate_type,
				   char storage_segment, char descriptor_privilege_level, char present);

#endif
