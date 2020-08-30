/*
 *  ps2.c - ps2 driver for os Copyright (C) 2016  Simon Kaufmann, HeKa
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

/* Refer to: http://wiki.osdev.org/%228042%22_PS/2_Controller#Overview
 *
 * Scancodes see:
 * http://www.computer-engineering.org/ps2keyboard/scancodes2.html on osdev.org:
 * http://wiki.osdev.org/PS/2_Keyboard it is scancode set 2.
 */

#include "os.h"
#include "print.h"
#include "ps2.h"
#include "main.h"

#define PS2_DATA 0x60
#define PS2_STATUS 0x64	 /* For reading */
#define PS2_COMMAND 0x64 /* Same address as PS2_STATUS but for writing */

#define PS2_CMD_DISABLE_SECOND 0xA7
#define PS2_CMD_ENABLE_SECOND 0xA8
#define PS2_CMD_TEST_SECOND 0xA9
#define PS2_CMD_DISABLE_FIRST 0xAD
#define PS2_CMD_ENABLE_FIRST 0xAE
#define PS2_CMD_TEST_FIRST 0xAB

#define PS2_CMD_READ_CTRL_BYTE 0x20
#define PS2_CMD_WRITE_CTRL_BYTE 0x60
#define PS2_CMD_TEST 0xAA

#define PS2_CMD_WRITE_SECOND 0xD4

/* Bits in ps2 controller configuration byte */
#define PS2_CTRL_BYTE_FIRST_INTERRUPT 0
#define PS2_CTRL_BYTE_SECOND_INTERRUPT 1
#define PS2_CTRL_BYTE_FIRST_TRANSLATION 6

#define PS2_SCANCODE_MAKE 0
#define PS2_SCANCODE_BREAK1 1 /* Break with f0 */
#define PS2_SCANCODE_BREAK2 2 /* Break with e0 */
#define PS2_SCANCODE_BREAK3 3 /* Break with e0 f0 */
#define PS2_SCANCODE_BREAK4 4 /* Break with e1 */

/* Character codes For ascii_charblock if first 4 bytes (two dimensional array)
 * are 0x01 (state change), then this code is in the second four bytes to
 * indicate which state has changed 0x80 and 0x04 is released.
 */
#define CAPS 0x00
#define SHIFT 0x01
#define ALT 0x02
#define ALTGR 0x03
#define CTRL 0x04
#define SHIFT_RIGHT 0x05
#define SUPER 0x06
#define SUPER_RIGHT 0x07
#define MENU 0x08
#define CTRL_RIGHT 0x09

/* Keycodes: First byte: 80 -> pressed, 04 -> released Second byte: 00 -> left
 * keyboard, 01 -> keypad, 02 -> middle pad (arrow keys, delete key...) Third
 * byte: number of row starting with 1 Fourth byte: number of column starting
 * with 1
 *
 * Keys are given as comment in us-layout
 *
 * Reference keyboard for character block is ISO-keyboard:
 * https://en.wikipedia.org/wiki/File:KB_United_Kingdom.svg
 *
 * Reference keyboard for number block and f-keys is: ANSI-keyboard:
 * https://en.wikipedia.org/wiki/Keyboard_layout#/media/File:ANSI_Keyboard_Layout_Diagram_with_Form_Factor.svg
 */

int keycode_make[] = /* All with no prefix */
	{
		0x00000000, /* No key for scancode 00 */
		0x8000010a, /* F9 pressed for scancode 01 */
		0x00000000, /* No key for scancode 02 */
		0x80000106, /* F5 pressed for scancode 03 */
		0x80000104, /* F3 pressed for scancode 04 */
		0x80000102, /* F1 pressed for scancode 05 */
		0x80000103, /* F2 pressed for scancode 06 */
		0x8000010d, /* F13 pressed for scancode 07 */
		0x00000000, /* No key for scancode 08 */
		0x8000010b, /* F10 pressed for scancode 09 */
		0x80000109, /* F8 pressed for scancode 0a */
		0x80000107, /* F6 pressed for scancode 0b */
		0x80000105, /* F4 pressed for scancode 0c */
		0x80000301, /* Tab pressed for scancode 0d */
		0x80000201, /* ` pressed for scancode 0e */
		0x00000000, /* No key for scancode 0f */
		0x00000000, /* No key for scancode 10 */
		0x80000603, /* Left alt pressed for scancode 11 */
		0x80000501, /* Left shift pressed for scancode 12 */
		0x00000000, /* No key for scancode 13 */
		0x80000601, /* Left ctrl pressed for scancode 14 */
		0x80000302, /* q pressed for scancode 15 */
		0x80000202, /* 1 pressed for scancode 16 */
		0x00000000, /* No key for scancode 17 */
		0x00000000, /* No key for scancode 18 */
		0x00000000, /* No key for scancode 19 */
		0x80000503, /* z pressed for scancode 1a */
		0x80000403, /* s pressed for scancode 1b */
		0x80000402, /* a pressed for scancode 1c */
		0x80000303, /* w pressed for scancode 1d */
		0x80000203, /* 2 pressed for scancode 1e */
		0x00000000, /* No key for scancode 1f */
		0x00000000, /* No key for scancode 20 */
		0x80000505, /* c pressed for scancode 21 */
		0x80000504, /* x pressed for scancode 22 */
		0x80000404, /* d pressed for scancode 23 */
		0x80000304, /* e pressed for scancode 24 */
		0x80000205, /* 4 pressed for scancode 25 */
		0x80000204, /* 3 pressed for scancode 26 */
		0x00000000, /* No key for scancode 27 */
		0x00000000, /* No key for scancode 28 */
		0x80000604, /* Space pressed for scancode 29 */
		0x80000506, /* v pressed for scancode 2a */
		0x80000405, /* f pressed for scancode 2b */
		0x80000306, /* t pressed for scancode 2c */
		0x80000305, /* r pressed for scancode 2d */
		0x80000206, /* 5 pressed for scancode 2e */
		0x00000000, /* No key for scancode 2f */
		0x00000000, /* No key for scancode 30 */
		0x80000508, /* n pressed for scancode 31 */
		0x80000507, /* b pressed for scancode 32 */
		0x80000407, /* h pressed for scancode 33 */
		0x80000406, /* g pressed for scancode 34 */
		0x80000307, /* y pressed for scancode 35 */
		0x80000207, /* 6 pressed for scancode 36 */
		0x00000000, /* No key for scancode 37 */
		0x00000000, /* No key for scancode 38 */
		0x00000000, /* No key for scancode 39 */
		0x80000509, /* m pressed for scancode 3a */
		0x80000408, /* j pressed for scancode 3b */
		0x80000308, /* u pressed for scancode 3c */
		0x80000208, /* 7 pressed for scancode 3d */
		0x80000209, /* 8 pressed for scancode 3e */
		0x00000000, /* No key for scancode 3f */
		0x00000000, /* No key for scancode 40 */
		0x8000050a, /* , pressed for scancode 41 */
		0x80000409, /* k pressed for scancode 42 */
		0x80000309, /* i pressed for scancode 43 */
		0x8000030a, /* o pressed for scancode 44 */
		0x8000020b, /* 0 pressed for scancode 45 */
		0x8000020a, /* 9 pressed for scancode 46 */
		0x00000000, /* No key for scancode 47 */
		0x00000000, /* No key for scancode 48 */
		0x8000050b, /* . pressed for scancode 49 */
		0x8000050c, /* / pressed for scancode 4a */
		0x8000040a, /* l pressed for scancode 4b */
		0x8000040b, /* ; pressed for scancode 4c */
		0x8000030b, /* p pressed for scancode 4d */
		0x8000020c, /* - pressed for scancode 4e */
		0x00000000, /* No key for scancode 4f */
		0x00000000, /* No key for scancode 50 */
		0x00000000, /* No key for scancode 51 */
		0x8000040c, /* ' pressed for scancode 52 */
		0x00000000, /* No key for scancode 53 */
		0x8000030c, /* [ pressed for scancode 54 */
		0x8000020d, /* = pressed for scancode 55 */
		0x00000000, /* No key for scancode 56 */
		0x00000000, /* No key for scancode 57 */
		0x80000401, /* Caps key pressed for scancode 58 */
		0x8000050d, /* Right shift pressed for scancode 59 */
		0x8000040e, /* Enter pressed for scancode 5a */
		0x8000030d, /* ] pressed for scancode 5b */
		0x00000000, /* No key for scancode 5c */
		0x8000040d, /* \ pressed for scancode 5d */
		0x00000000, /* No key for scancode 5e */
		0x00000000, /* No key for scancode 5f */
		0x00000000, /* No key for scancode 60 */
		0x80000502, /* \ key pressed (beside left shift key) for scancode 61 */
		0x00000000, /* No key for scancode 62 */
		0x00000000, /* No key for scancode 63 */
		0x00000000, /* No key for scancode 64 */
		0x00000000, /* No key for scancode 65 */
		0x8000010e, /* Backspace pressed for scancode 66 */
		0x00000000, /* No key for scancode 67 */
		0x00000000, /* No key for scancode 68 */
		0x80010401, /* Keypad 1 pressed for scancode 69 */
		0x00000000, /* No key for scancode 6a */
		0x80010401, /* Keypad 4 pressed for scancode 6b */
		0x80010201, /* Keypad 7 pressed for scancode 6c */
		0x00000000, /* No key for scancode 6d */
		0x00000000, /* No key for scancode 6e */
		0x00000000, /* No key for scancode 6f */
		0x80010501, /* Keypad 0 pressed for scancode 70 */
		0x80010502, /* Keypad . pressed for scancode 71 */
		0x80010402, /* Keypad 2 pressed for scancode 72 */
		0x80010302, /* Keypad 5 pressed for scancode 73 */
		0x80010303, /* Keypad 6 pressed for scancode 74 */
		0x80010202, /* Keypad 8 pressed for scancode 75 */
		0x80000101, /* Esc pressed for scancode 76 */
		0x80010101, /* Num lock pressed for scancode 77 */
		0x8000010c, /* F11 pressed for scancode 78 */
		0x80010304, /* Keypad + pressed for scancode 76 */
		0x00000000, /* No key for scancode 77 */
		0x00000000, /* No key for scancode 78 */
		0x00000000, /* No key for scancode 79 */
		0x80010403, /* Keypad 3 pressed for scancode 7a */
		0x80010104, /* Keypad - pressed for scancode 7b */
		0x80010103, /* Keypad * pressed for scancode 7c */
		0x80010203, /* Keypad 9 pressed for scancode 7d */
		0x80020102, /* Scroll pressed for scancode 7e */
		0x00000000, /* No key for scancode 7f */
		0x00000000, /* No key for scancode 80 */
		0x00000000, /* No key for scancode 81 */
		0x00000000, /* No key for scancode 82 */
		0x80000108	/* F7 pressed for scancode 83 */
};

int keycode_break1[] = /* All with f0 prefix */
	{
		0x00000000, /* No key for scancode 0f 00 */
		0x4000010a, /* F9 released for scancode 0f 01 */
		0x00000000, /* No key for scancode 0f 02 */
		0x40000106, /* F5 released for scancode 0f 03 */
		0x40000104, /* F3 released for scancode 0f 04 */
		0x40000102, /* F1 released for scancode 0f 05 */
		0x40000103, /* F2 released for scancode 0f 06 */
		0x4000010d, /* F13 released for scancode 0f 07 */
		0x00000000, /* No key for scancode 0f 08 */
		0x4000010b, /* F10 released for scancode 0f 09 */
		0x40000109, /* F8 released for scancode 0f 0a */
		0x40000107, /* F6 released for scancode 0f 0b */
		0x40000105, /* F4 released for scancode 0f 0c */
		0x40000301, /* Tab released for scancode 0f 0d */
		0x40000201, /* ` released for scancode 0f 0e */
		0x00000000, /* No key for scancode 0f 0f */
		0x00000000, /* No key for scancode 0f 10 */
		0x40000603, /* Left alt released for scancode 0f 11 */
		0x40000501, /* Left shift released for scancode 0f 12 */
		0x00000000, /* No key for scancode 0f 13 */
		0x40000601, /* Left ctrl released for scancode 0f 14 */
		0x40000302, /* q released for scancode 0f 15 */
		0x40000202, /* 1 released for scancode 0f 16 */
		0x00000000, /* No key for scancode 0f 17 */
		0x00000000, /* No key for scancode 0f 18 */
		0x00000000, /* No key for scancode 0f 19 */
		0x40000503, /* z released for scancode 0f 1a */
		0x40000403, /* s released for scancode 0f 1b */
		0x40000402, /* a released for scancode 0f 1c */
		0x40000303, /* w released for scancode 0f 1d */
		0x40000203, /* 2 released for scancode 0f 1e */
		0x00000000, /* No key for scancode 0f 1f */
		0x00000000, /* No key for scancode 0f 20 */
		0x40000505, /* c released for scancode 0f 21 */
		0x40000504, /* x released for scancode 0f 22 */
		0x40000404, /* d released for scancode 0f 23 */
		0x40000304, /* e released for scancode 0f 24 */
		0x40000205, /* 4 released for scancode 0f 25 */
		0x40000204, /* 3 released for scancode 0f 26 */
		0x00000000, /* No key for scancode 0f 27 */
		0x00000000, /* No key for scancode 0f 28 */
		0x40000604, /* Space released for scancode 0f 29 */
		0x40000506, /* v released for scancode 0f 2a */
		0x40000405, /* f released for scancode 0f 2b */
		0x40000306, /* t released for scancode 0f 2c */
		0x40000305, /* r released for scancode 0f 2d */
		0x40000206, /* 5 released for scancode 0f 2e */
		0x00000000, /* No key for scancode 0f 2f */
		0x00000000, /* No key for scancode 0f 30 */
		0x40000508, /* n released for scancode 0f 31 */
		0x40000507, /* b released for scancode 0f 32 */
		0x40000407, /* h released for scancode 0f 33 */
		0x40000406, /* g released for scancode 0f 34 */
		0x40000307, /* y released for scancode 0f 35 */
		0x40000207, /* 6 released for scancode 0f 36 */
		0x00000000, /* No key for scancode 0f 37 */
		0x00000000, /* No key for scancode 0f 38 */
		0x00000000, /* No key for scancode 0f 39 */
		0x40000509, /* m released for scancode 0f 3a */
		0x40000408, /* j released for scancode 0f 3b */
		0x40000308, /* u released for scancode 0f 3c */
		0x40000208, /* 7 released for scancode 0f 3d */
		0x40000209, /* 8 released for scancode 0f 3e */
		0x00000000, /* No key for scancode 0f 3f */
		0x00000000, /* No key for scancode 0f 40 */
		0x4000050a, /* , released for scancode 0f 41 */
		0x40000409, /* k released for scancode 0f 42 */
		0x40000309, /* i released for scancode 0f 43 */
		0x4000030a, /* o released for scancode 0f 44 */
		0x4000020b, /* 0 released for scancode 0f 45 */
		0x4000020a, /* 9 released for scancode 0f 46 */
		0x00000000, /* No key for scancode 0f 47 */
		0x00000000, /* No key for scancode 0f 48 */
		0x4000050b, /* . released for scancode 0f 49 */
		0x4000050c, /* / released for scancode 0f 4a */
		0x4000040a, /* l released for scancode 0f 4b */
		0x4000040b, /* ; released for scancode 0f 4c */
		0x4000030b, /* p released for scancode 0f 4d */
		0x4000020c, /* - released for scancode 0f 4e */
		0x00000000, /* No key for scancode 0f 4f */
		0x00000000, /* No key for scancode 0f 50 */
		0x00000000, /* No key for scancode 0f 51 */
		0x4000040c, /* ' released for scancode 0f 52 */
		0x00000000, /* No key for scancode 0f 53 */
		0x4000030c, /* [ released for scancode 0f 54 */
		0x4000020d, /* = released for scancode 0f 55 */
		0x00000000, /* No key for scancode 0f 56 */
		0x00000000, /* No key for scancode 0f 57 */
		0x40000401, /* Caps key released for scancode 0f 58 */
		0x4000050d, /* Right shift released for scancode 0f 59 */
		0x4000040e, /* Enter released for scancode 0f 5a */
		0x4000030d, /* ] released for scancode 0f 5b */
		0x00000000, /* No key for scancode 0f 5c */
		0x4000040d, /* \ released for scancode 0f 5d */
		0x00000000, /* No key for scancode 0f 5e */
		0x00000000, /* No key for scancode 0f 5f */
		0x00000000, /* No key for scancode 0f 60 */
		0x40000502, /* \ key released (beside left shift key) for scancode 0f 61 */
		0x00000000, /* No key for scancode 0f 62 */
		0x00000000, /* No key for scancode 0f 63 */
		0x00000000, /* No key for scancode 0f 64 */
		0x00000000, /* No key for scancode 0f 65 */
		0x4000010e, /* Backspace released for scancode 0f 66 */
		0x00000000, /* No key for scancode 0f 67 */
		0x00000000, /* No key for scancode 0f 68 */
		0x40010401, /* Keypad 1 released for scancode 0f 69 */
		0x00000000, /* No key for scancode 0f 6a */
		0x40010401, /* Keypad 4 released for scancode 0f 6b */
		0x40010201, /* Keypad 7 released for scancode 0f 6c */
		0x00000000, /* No key for scancode 0f 6d */
		0x00000000, /* No key for scancode 0f 6e */
		0x00000000, /* No key for scancode 0f 6f */
		0x40010501, /* Keypad 0 released for scancode 0f 70 */
		0x40010502, /* Keypad . released for scancode 0f 71 */
		0x40010402, /* Keypad 2 released for scancode 0f 72 */
		0x40010302, /* Keypad 5 released for scancode 0f 73 */
		0x40010303, /* Keypad 6 released for scancode 0f 74 */
		0x40010202, /* Keypad 8 released for scancode 0f 75 */
		0x40000101, /* Esc released for scancode 0f 76 */
		0x40010101, /* Num lock released for scancode 0f 77 */
		0x4000010c, /* F11 released for scancode 0f 78 */
		0x40010304, /* Keypad + released for scancode 0f 76 */
		0x00000000, /* No key for scancode 0f 77 */
		0x00000000, /* No key for scancode 0f 78 */
		0x00000000, /* No key for scancode 0f 79 */
		0x40010403, /* Keypad 3 released for scancode 0f 7a */
		0x40010104, /* Keypad - released for scancode 0f 7b */
		0x40010103, /* Keypad * released for scancode 0f 7c */
		0x40010203, /* Keypad 9 released for scancode 0f 7d */
		0x40020102, /* Scroll released for scancode 0f 7e */
		0x00000000, /* No key for scancode 0f 7f */
		0x00000000, /* No key for scancode 0f 80 */
		0x00000000, /* No key for scancode 0f 81 */
		0x00000000, /* No key for scancode 0f 82 */
		0x40000108	/* F7 released for scancode 0f 83 */
};

int keycode_break2[] = {
	/* All with prefix e0 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,
	0x80000605, /* right alt key pressed for scancode e0 11 */
	0x80020101, /* Print pressed for scancode e0 12 */
	0,
	0x80000608, /* Right ctrl key pressed for scancode e0 14 (one more right because right windows key is not on asus keyboard) */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0x80000602, /* Left windows key pressed for scancode e0 1f */
	0, 0, 0, 0, 0, 0, 0,
	0x80000608, /* Right windows key pressed for scancode e0 27 */
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0x80020501, /* Left arrow pressed for scancode e0 6b */
	0, 0, 0, 0,
	0,
	0x8000010f, /* Del pressed for scancode e0 71 */
	0x80020502, /* Down arrow pressed for scancode e0 72 */
	0,
	0x80020503, /* Right arrow presed for scancode e0 74 */
	0,
	0x80020401, /* Up arrow pressed for scancode e0 75 */
};

int keycode_break3[] = {
	/* All with prefix e0 f0 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,
	0x40000605, /* Right alt key released for scancode e0 f0 11 */
	0x40020101, /* Print released for scancode e0 f0 12 */
	0,
	0x40000608, /* Right ctrl key released for scancode e0 f0 14 (one more right because right windows key is not on Asus keyboard) */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0x40000602, /* Left windows key released for scancode e0 f0 1f */
	0, 0, 0, 0, 0, 0, 0,
	0x40000608, /* Right windows key released for scancode e0 f0 27 */
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0x40020501, /* Left arrow released for scancode e0 f0 6b */
	0, 0, 0, 0,
	0,
	0x40020301, /* Del released for scancode e0 f0 71 */
	0x40020502, /* Down arrow released for scancode e0 f0 72 */
	0,
	0x40020503, /* Right arrow released for scancode e0 f0 74 */
	0,
	0x40020401, /* Up arrow released for scancode e0 f0 75 */
};

/*
 * Character tables: First four bytes: 0x00 -> real character, 0x01 -> state
 * change (shift and so on) 0x01 -> needs another key press Second four bytes:
 * the character or code for key (utf8)
 */

int ascii_charblock_line1[13 * 2]; /* Only f-keys and so on */

int ascii_charblock_line2[] = {0x00, '`', 0x00, '1', 0x00, '2', 0x00, '3', 0x00, '4', 0x00, '5', 0x00, '6', 0x00, '7', 0x00, '8', 0x00, '9', 0x00, '0', 0x00, '-', 0x00, '='};

int ascii_charblock_line3[] = {0x00, '\t', 0x00, 'q', 0x00, 'w', 0x00, 'e', 0x00, 'r', 0x00, 't', 0x00, 'y', 0x00, 'u', 0x00, 'i', 0x00, 'o', 0x00, 'p', 0x00, '[', 0x00, ']'};

int ascii_charblock_line4[] = {0x01, CAPS, 0x00, 'a', 0x00, 's', 0x00, 'd', 0x00, 'f', 0x00, 'g', 0x00, 'h', 0x00, 'j', 0x00, 'k', 0x00, 'l', 0x00, ';', 0x00, '\'', 0x00, '\\', 0x00, '\n'};

int ascii_charblock_line5[] = {0x01, SHIFT, 0x00, '\\', 0x00, 'z', 0x00, 'x', 0x00, 'c', 0x00, 'v', 0x00, 'b', 0x00, 'n', 0x00, 'm', 0x00, ',', 0x00, '.', 0x00, '/', 0x01, SHIFT_RIGHT};

int ascii_charblock_line6[] = {0x01, CTRL, 0x01, SUPER, 0x01, ALT, 0x00, ' ', 0x01, ALTGR, 0x01, SUPER_RIGHT, 0x01, MENU, 0x01, CTRL_RIGHT};

int *ascii_charblock_line[] =
	{
		ascii_charblock_line1,
		ascii_charblock_line2,
		ascii_charblock_line3,
		ascii_charblock_line4,
		ascii_charblock_line5,
		ascii_charblock_line6};

int *ascii_keypad_line[10];

int *ascii_middle_pad_line[10];

int **ascii_keyboard_uk[] =
	{
		ascii_charblock_line,
		ascii_keypad_line,
		ascii_middle_pad_line};

char channel_ok = 0; /* Bit 1: set -> channel 1 ok, unset -> channel 1 not ok; bit 2: set -> channel 2 ok, unset -> channel 2 not ok */

static char read_controller_config()
{
	outb(PS2_COMMAND, PS2_CMD_TEST);
	char ret = inb(PS2_DATA);
	return ret;
}

static char is_controller1_ok()
{
	if ((channel_ok & (1 << 1)) == 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

static char is_controller2_ok()
{
	if ((channel_ok & (1 << 2)) == 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

static void set_bit_control_configuration_byte(int bitmask)
{
	outb(PS2_COMMAND, PS2_CMD_READ_CTRL_BYTE);
	char tmp = inb(PS2_DATA);
	tmp |= bitmask;
	outb(PS2_COMMAND, PS2_CMD_WRITE_CTRL_BYTE);
	outb(PS2_DATA, tmp);
}

static void clear_bit_control_configuration_byte(int bitmask)
{
	outb(PS2_COMMAND, PS2_CMD_READ_CTRL_BYTE);
	char tmp = inb(PS2_DATA);
	tmp &= ~bitmask;
	outb(PS2_COMMAND, PS2_CMD_WRITE_CTRL_BYTE);
	outb(PS2_DATA, tmp);
}

void ps2_init()
{
	kprintf("\nPS2 Beginning\n");
	outb(PS2_COMMAND, PS2_CMD_DISABLE_SECOND);
	outb(PS2_COMMAND, PS2_CMD_DISABLE_FIRST);

	inb(PS2_DATA); /* Clear output buffer */

	int bitmask = (1 << PS2_CTRL_BYTE_FIRST_TRANSLATION) | (1 << PS2_CTRL_BYTE_FIRST_INTERRUPT) | (1 << PS2_CTRL_BYTE_SECOND_INTERRUPT);
	clear_bit_control_configuration_byte(bitmask);

	int ret = 0x00;					   /* Read_controller_config(); */
	kprintf("PS2-Reply: 0x%x\n", ret); /* Should be 0x55 */

	/* Test if there is a second channel and test both channels */
	outb(PS2_COMMAND, PS2_CMD_ENABLE_SECOND);
	ret = read_controller_config();
	if ((ret & (1 << 5)) == 0)
	{
		outb(PS2_COMMAND, PS2_CMD_DISABLE_SECOND);

		outb(PS2_COMMAND, PS2_CMD_TEST_FIRST);
		ret = inb(PS2_DATA);
		kprintf("Test PS2 Channel 2: 0x%x\n", ret);
		if (ret == 0x00)
		{
			kprintf("Test PS2 Channel 2 passed\n");
			channel_ok |= (1 << 2);
		}
	}
	else
	{
		kprintf("Single PS2-Controller\n");
	}

	outb(PS2_COMMAND, PS2_CMD_ENABLE_FIRST);

	outb(PS2_COMMAND, PS2_CMD_TEST_FIRST);
	ret = inb(PS2_DATA);

	kprintf("Test PS2 Channel 1: 0x%x\n", ret);
	if (ret == 0x00)
	{
		kprintf("Test PS2 Channel 1 passed\n");
		channel_ok |= (1 << 1);
	}

	/* Enable channels */
	if (is_controller1_ok())
	{
		outb(PS2_COMMAND, PS2_CMD_ENABLE_FIRST);
		/* Enable interrupt for first device: */
		set_bit_control_configuration_byte(1 << PS2_CTRL_BYTE_FIRST_INTERRUPT);
	}
	if (is_controller2_ok())
	{
		outb(PS2_COMMAND, PS2_CMD_ENABLE_SECOND);
	}
}

/**
 * scancode_keycode - returns internal keycode to keyboard scancode
 * @scancode: pointer to maximum 3 byte scancode
 * @length: length of array given by scancode
 *
 * Return: internal keycode if scancode is known, zero otherwise
 */
int scancode_keycode(int *scancode, int length)
{
	int release = PS2_SCANCODE_MAKE;
	int count = 0;
	if (scancode[count] == 0xf0)
	{
		count++;
		release = PS2_SCANCODE_BREAK1;
	}
	else if (scancode[count] == 0xe0)
	{
		count++;
		if (scancode[count] == 0xf0)
		{
			count++;
			release = PS2_SCANCODE_BREAK3;
		}
		else
		{
			release = PS2_SCANCODE_BREAK2;
		}
	}
	else if (scancode[count] == 0xe1)
	{
		/* Contains always of 3 bytes -> count + 2 */
		count++;
		count++;
		release = PS2_SCANCODE_BREAK4;
	}
	switch (release)
	{
	case PS2_SCANCODE_MAKE:
		return keycode_make[scancode[count]];
	case PS2_SCANCODE_BREAK1:
		return keycode_break1[scancode[count]];
	case PS2_SCANCODE_BREAK2:
		return keycode_break2[scancode[count]];
	case PS2_SCANCODE_BREAK3:
		return keycode_break3[scancode[count]];
	}
	return 0x0;
}

/**
 * keycode_char - convert keycode to char
 * @keycode: give keycode
 *
 * Return: Integer variable, byte0 is char in ascii code, byte3 is 0x80 if key
 * is pressed and 0x40 if key is released.
 */
int keycode_char(int keycode)
{
	int x1, x2, x3, x4;
	x4 = keycode & 0xff;
	keycode = keycode >> 8;
	x3 = keycode & 0xff;
	keycode = keycode >> 8;
	x2 = keycode & 0xff;
	keycode = keycode >> 8;
	x1 = keycode & 0xff;
	char c = ascii_keyboard_uk[x2][x3 - 1][2 * (x4 - 1) + 1]; // Write comment here why we can't use the last array index, but have to do the multiplication!
	if (ascii_keyboard_uk[x2][x3 - 1][2 * (x4 - 1)] == 0x01)
	{
		kprintf("Special Key!");
	}
	return ((x1 << 24) | c);
}

void irq1_handler()
{
	static int val[3];
	static int count = 0;
	if ((inb(PS2_STATUS) & (1 << 0)) == 0)
	{
		/* Look in status register if new bytes are in data-register */
		/* Why is this necessary -> we have interrupt for that? */
		return;
	}
	val[count] = inb(PS2_DATA);
	if (val[count] != 0xf0 && val[count] != 0xe0)
	{
#ifdef DEBUG_PRINT_SCANCODE
		int test;
		printf(" scancode: ");
		for (test = 0; test < count; test++)
		{
			printf("0x%x ", val[count]);
		}
#endif
		int keycode = scancode_keycode(val, count);
		int charcode = keycode_char(keycode);
		if ((charcode & 0xff000000) == 0x80000000)
		{
			/* Key is pressed */
			kprintf("%c", charcode & 0xff);
		}
		count = 0;
	}
	else
	{
		count++;
		if (count >= 3)
		{
			count = 2;
		}
	}
}