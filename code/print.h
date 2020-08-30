/*
 *  print.h - declarations for printing
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

#ifndef PRINT_H
#define PRINT_H

void print(const char *string);
void kprintf(const char *string, ...);
void print_char(char c);
void println(const char* string);
void print_int(int number);	
void print_hex(int number);
void print_bcd(int bcd_number);

void clear_screen();
void reset_line(); /* Clears the line and set cursor to beginning of current line */
void shift_screen_up(int lines);
void set_cursor(int line, int row);

#endif
