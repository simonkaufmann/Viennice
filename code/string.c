/*
 *  string.c - string functions like in libc
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


#include "string.h"

void *memcpy(void *dest, const void *src, size_t n)
{
	unsigned int i;
	for (i = 0; i < n; i++)	{
		((char*)dest)[i] = ((char*)src)[i]; 
	}
	return dest;
}
