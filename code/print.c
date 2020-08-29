/*
 *  print.c - functions for printing on console
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

#include "print.h"
#include "stdarg.h"
#include "string.h"
#include "main.h"

#define SCREEN_RAM 0xb8000
#define SCREEN_MAX_ADDRESS (SCREEN_RAM + 2 * LINES * ROWS) /* 2 because two bytes for each character (character + style) */
#define LINES 25
#define ROWS 80

char *screen_pointer = (char *)SCREEN_RAM;

/* Internal functions */

static void check_screen_pointer()
{
	if (screen_pointer >= SCREEN_MAX_ADDRESS)
	{
		shift_screen_up(1);
	}
}

/* Printing */

void printf(const char *string, ...)
{
	va_list va;
	int i = 0;
	while (string[i++] != 0)
		;
	va_start(va, string + i);

	i = 0;
	while (string[i] != 0)
	{
		switch (string[i])
		{
		case '\n':
			println("");
			break;
		case '%':
			i++;
			if (string[i] == 'd')
			{
				print_int(va_arg(va, int));
			}
			else if (string[i] == 'x')
			{
				print_hex(va_arg(va, int));
			}
			else if (string[i] == 'c')
			{
				print_char(va_arg(va, int)); /* Also here the type int! otherwise it would not work as tests have shown */
			}
			else if (string[i] == 's')
			{
				print(va_arg(va, char *));
			}
			else if (string[i] == '%')
			{
				print_char('%');
			}
			break;
		default:
			print_char(string[i]);
		}
		i++;
	}
	va_end(va);
}

void print_char(char c)
{
	*screen_pointer = c;
	screen_pointer++;
	check_screen_pointer(); /* Check if screen is at end -> shift upwards */
	*screen_pointer = 0x07; /* Gray with black background */
	screen_pointer++;
	check_screen_pointer();
}

void print_intern(const char *string, int newline)
{
	int i = 0;
	while (string[i] != 0)
	{
		print_char(string[i]);
		i++;
	}
	if (newline != FALSE)
	{
		int temp = ((int)(screen_pointer - SCREEN_RAM)) % (2 * ROWS);
		screen_pointer += (2 * ROWS) - temp;
		check_screen_pointer();
	}
}

void print(const char *string)
{
	print_intern(string, FALSE);
}

void println(const char *string)
{
	print_intern(string, TRUE);
}

/* Accessing the screen */

void reset_line()
{
	int temp = ((int)(screen_pointer - SCREEN_RAM)) % (2 * ROWS);
	screen_pointer -= temp;
	int i;
	for (i = 0; i < ROWS; i++)
	{
		screen_pointer[i] = 0;
	}
}

void shift_screen_up(int lines)
{
	char *tmp_screen_pointer = SCREEN_RAM;
	char *tmp_screen_pointer_up = tmp_screen_pointer + lines * ROWS * 2;
	while (tmp_screen_pointer_up < SCREEN_MAX_ADDRESS)
	{
		memcpy(tmp_screen_pointer, tmp_screen_pointer_up, ROWS * 2);
		tmp_screen_pointer += ROWS * 2;
		tmp_screen_pointer_up += ROWS * 2;
	}
	/* clear that was not copied any more! */
	while (tmp_screen_pointer < SCREEN_MAX_ADDRESS)
	{
		*tmp_screen_pointer = 0;
		tmp_screen_pointer++;
	}
	screen_pointer -= lines * ROWS * 2;
	if (screen_pointer < SCREEN_RAM)
	{
		screen_pointer = SCREEN_RAM;
	}
}

void clear_screen()
{
	screen_pointer = SCREEN_RAM;
	int line;
	int row;
	for (row = 0; row < ROWS; row++)
	{
		for (line = 0; line < LINES; line++)
		{
			*screen_pointer = 0;
			screen_pointer++;
			*screen_pointer = 0;
			screen_pointer++;
		}
	}
	screen_pointer = SCREEN_RAM;
}

void set_cursor(int line, int row)
{
	if (line >= LINES)
	{
		line = LINES - 1;
	}
	if (row >= ROWS)
	{
		row = ROWS - 1;
	}
	screen_pointer = SCREEN_RAM + (line * ROWS + row) * 2;
}

/* Printing numbers */

void print_bcd(int bcd_number)
{
	int binary = ((bcd_number / 16) * 10) + (bcd_number & 0xf);
	print_int(binary);
}

void print_hex(int number)
{
	int was_negative = FALSE;

	if (number < 0)
	{
		number = -number;
		was_negative = TRUE;
	}

	static char string[11];
	string[10] = 0;

	int i = 9;
	string[i] = (number % 16);
	if (string[i] >= 0 && string[i] < 10)
	{
		string[i] += '0';
	}
	else
	{
		string[i] += 'a' - 10;
	}

	number /= 16;
	while (number > 0)
	{
		string[--i] = (number % 16);

		if (string[i] >= 0 && string[i] < 10)
		{
			string[i] += '0';
		}
		else
		{
			string[i] += 'a' - 10;
		}

		number /= 16;

		if (i == 1)
		{
			break;
		}
	}

	if (was_negative != FALSE)
	{
		string[--i] = '-';
	}

	print(string + i);
}

void print_int(int number)
{
	int was_negative = FALSE;

	if (number < 0)
	{
		number = -number;
		was_negative = TRUE;
	}

	static char string[11];
	string[10] = 0;

	int i = 9;
	string[i] = (number % 10) + '0'; /* That also number = 0 gets printed */
	number /= 10;
	while (number > 0)
	{
		string[--i] = (number % 10) + '0';
		number /= 10;
		if (i == 1)
		{
			/* Break with 1 so that there is also space for a '-' if number was negative */
			break;
		}
	}

	if (was_negative != FALSE)
	{
		string[--i] = '-';
	}

	print(string + i);
}
