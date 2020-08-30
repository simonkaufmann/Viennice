/*
 *  char_buffer.c - Input buffer for characters
 *  Copyright (C) 2020  Simon Kaufmann, HeKa
 *
 *  This is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, either version 3 of the License, or (at your option) any later
 *  version.
 *
 *  Foobar is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License. If not,
 *  see <http://www.gnu.org/licenses/>.
 */

#include "char_buffer.h"
#include "print.h"

#define CHAR_BUFFER_SIZE 10000

static void *memcpy_volatile(void *dest, const volatile void *src, int n);

volatile char char_buffer[CHAR_BUFFER_SIZE];
volatile int index_read = 0;  /* Pointer to next character to read */
volatile int index_write = 0; /* Pointer to next empty position in buffer */

int buffer_add_character(char character)
{
    char_buffer[index_write] = character;
    index_write++;
    if (index_write == CHAR_BUFFER_SIZE)
    {
        index_write = 0;
    }
    if (index_write == index_read)
    {
        return ERROR_CHAR_BUFFER_OVERFLOW;
    }
    return CHAR_BUFFER_SUCCESS;
}

int buffer_read_characters(char *buffer, int count)
{
    int characters_to_return = count;
    if (buffer_characters_available() < characters_to_return)
    {
        characters_to_return = buffer_characters_available();
    }

    if (index_read + characters_to_return <= CHAR_BUFFER_SIZE)
    {
        memcpy_volatile(buffer, char_buffer + index_read, characters_to_return);
    }
    else
    {
        int first_copy = CHAR_BUFFER_SIZE - index_read;
        memcpy_volatile(buffer, char_buffer + index_read, first_copy);
        memcpy_volatile(buffer + first_copy, char_buffer, characters_to_return - first_copy);
    }

    index_read += characters_to_return;
    if (index_read == CHAR_BUFFER_SIZE)
    {
        index_read -= CHAR_BUFFER_SIZE;
    }

    return characters_to_return;
}

int buffer_characters_available()
{
    if (index_read == index_write)
    {
        return 0;
    }
    if (index_read < index_write)
    {
        return index_write - index_read;
    }
    return (CHAR_BUFFER_SIZE - index_read) + index_write;
}

static void *memcpy_volatile(void *dest, const volatile void *src, int n)
{
    unsigned int i;
    for (i = 0; i < n; i++)
    {
        ((char *)dest)[i] = ((char *)src)[i];
    }
    return dest;
}