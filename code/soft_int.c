/*
 *  soft_int.c - Functions for Software interrupts
 *  Copyright (C) 2020  Simon Kaufmann, HeKa
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

#include "soft_int.h"
#include "string.h"
#include "print.h"
#include "os.h"
#include "char_buffer.h"

static int write_software_interrupt(int fd, int buf, int count);
static int read_software_interrupt(int fd, int buf, int count);

int software_interrupt_80h(int syscall, int first, int second, int third, int fourth)
{
    if (syscall == SYSCALL_WRITE)
    {
        return write_software_interrupt(first, second, third);
    }
    if (syscall == SYSCALL_READ)
    {
        return read_software_interrupt(first, second, third);
    }
    return SYSCALL_NOT_RECOGNIZED;
}

static int write_software_interrupt(int fd, int buf, int count)
{
    char char_buffer[count + 1];
    memcpy(char_buffer, (void *)buf, count);
    char_buffer[count] = 0;

    kprintf(char_buffer);
    return 0;
}

static int read_software_interrupt(int fd, int buf, int count)
{
    return buffer_read_characters((void *)buf, count);
}