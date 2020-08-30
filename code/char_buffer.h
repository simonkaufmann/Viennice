/*
 *  char_buffer.h - Declarations for character input buffer
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

#ifndef CHARBUFFER_H
#define CHARBUFFER_H

#define ERROR_CHAR_BUFFER_OVERFLOW -1
#define CHAR_BUFFER_SUCCESS 0

int buffer_add_character(char character);
int buffer_read_characters(char *buffer, int count);
int buffer_characters_available();

#endif