/*
 *  os.asm - initial assembler file executed by grub bootloader
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


#######
# Multiboot-Standard - Kernel
######

# refer to: http://www.lowlevel.eu/wiki/Teil_4_-_Hello_World

# multiboot specification: http://www.gnu.org/software/grub/manual/multiboot/multiboot.htm

.section .text

.space 4 #why??
.int 0x1BADB002		#magic number for multiboot
.int 0x00			#flags 
.int -0x1BADB002	#checksum = -flags - magic (must be zero when added to 
#flags and magic number!)

.extern init
.extern irq1_handler
.global lidt_asm
.global lgdt_asm

.global _start

.global outl
.global inl
.global outb
.global inb

.global default_isr
.global default_isr_no_error
.global idt32

_start: 
	mov $kernel_stack, %esp
		
	mov $'A', %eax
	mov %eax, 0xb8000;
	mov $0x07, %eax
	mov %eax, 0xb8001;

# init keyboard controller
# refer to: http://www.lowlevel.eu/wiki/Keyboard_Controller under
# initalisierung
#repeat:
#	inb 0x64, %al
#	and $0x01, %al
#	cmp %al, $0x00
#	je cont1 
#	inb 0x60, %al
#	jmp repeat:
#cont1:G
#	inb 0x64, %al
#	and $0x02, %al
#	cmp %al, $0x00
#	jne cont1
#	outb $0xF4, 0x60

	call init

	

_stop:
	cli
	hlt

	jmp _stop

# mixing c and assembler: https://courses.engr.illinois.edu/ece390/books/labmanual/c-prog-mixing.html
outl:
	pushl %ebp
	movl %esp, %ebp
	movw 8(%ebp), %dx # first parameter (parameter: data) is accessed with ebp+8! (always 4 offset because of return address!)
	movl 12(%ebp), %eax # (parameter: address) it is a short int (16 bit?) but gcc stores it with pushl -> pushes 32-bit
	outl %eax, %dx
	popl %ebp
	ret

inl:
	pushl %ebp
	movl %esp, %ebp
	movw 8(%ebp), %dx # it is a short int (16 bit?) but gcc stores it with pushl -> pushes 32-bit
	inl %dx, %eax
	popl %ebp
	ret

outb:
	pushl %ebp
	movl %esp, %ebp
	movw 8(%ebp), %dx
	movb 12(%ebp), %al
	outb %al, %dx
	popl %ebp
	ret

inb:
	pushl %ebp
	movl %esp, %ebp
	movw 8(%ebp), %dx
	inb %dx, %al
	and $0xFF, %eax
	popl %ebp
	ret

default_isr:
	add $0x04, %esp
default_isr_no_error:
	# see: http://www.lowlevel.eu/wiki/ISR for example common isr!
	iret	

idt32:
	pushal
	pushfl
	call irq1_handler
	pushl $0x02
	call pic_send_eoi
	add $0x04, %esp
	popfl
	popal
	iret

lidt_asm:
	pushl %ebp
	movl %esp, %ebp
	movl 8(%ebp), %eax
	lidt (%eax)
	popl %ebp
	ret

lgdt_asm:
	lgdt (%eax)
	pushl %eax
	mov $0x10, %ax
	mov %ax, %ds # load segment register!
    	mov %ax, %ss
    	mov %ax, %es
	jmp	$0x08, $flush_cs #$0x100000
flush_cs:
	popl %eax
	ret


.section .bss
.space 8192
kernel_stack:
