	.file	"pic.c"
	.text
.Ltext0:
	.globl	pic_remap
	.type	pic_remap, @function
pic_remap:
.LFB0:
	.file 1 "pic.c"
	.loc 1 20 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	.loc 1 22 0
	subl	$8, %esp
	pushl	$17
	pushl	$32
	call	outb
	addl	$16, %esp
	.loc 1 23 0
	subl	$8, %esp
	pushl	$17
	pushl	$160
	call	outb
	addl	$16, %esp
	.loc 1 31 0
	movl	8(%ebp), %eax
	movsbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$33
	call	outb
	addl	$16, %esp
	.loc 1 32 0
	movl	8(%ebp), %eax
	addl	$8, %eax
	movsbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$161
	call	outb
	addl	$16, %esp
	.loc 1 35 0
	subl	$8, %esp
	pushl	$4
	pushl	$33
	call	outb
	addl	$16, %esp
	.loc 1 36 0
	subl	$8, %esp
	pushl	$2
	pushl	$161
	call	outb
	addl	$16, %esp
	.loc 1 39 0
	subl	$8, %esp
	pushl	$1
	pushl	$33
	call	outb
	addl	$16, %esp
	.loc 1 40 0
	subl	$8, %esp
	pushl	$1
	pushl	$161
	call	outb
	addl	$16, %esp
	.loc 1 41 0
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	pic_remap, .-pic_remap
	.globl	pic_mask_irqs
	.type	pic_mask_irqs, @function
pic_mask_irqs:
.LFB1:
	.loc 1 44 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	8(%ebp), %eax
	movw	%ax, -12(%ebp)
	.loc 1 45 0
	movzwl	-12(%ebp), %eax
	movsbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$33
	call	outb
	addl	$16, %esp
	.loc 1 46 0
	movzwl	-12(%ebp), %eax
	shrw	$8, %ax
	movsbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$161
	call	outb
	addl	$16, %esp
	.loc 1 47 0
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	pic_mask_irqs, .-pic_mask_irqs
	.globl	pic_send_eoi
	.type	pic_send_eoi, @function
pic_send_eoi:
.LFB2:
	.loc 1 50 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	8(%ebp), %eax
	movb	%al, -12(%ebp)
	.loc 1 51 0
	subl	$8, %esp
	pushl	$32
	pushl	$32
	call	outb
	addl	$16, %esp
	.loc 1 52 0
	cmpb	$7, -12(%ebp)
	jle	.L5
	.loc 1 53 0
	subl	$8, %esp
	pushl	$32
	pushl	$160
	call	outb
	addl	$16, %esp
.L5:
	.loc 1 55 0
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	pic_send_eoi, .-pic_send_eoi
	.section	.rodata
.LC0:
	.string	"IRQ1 executed: return 0x%x\n"
	.text
	.globl	pic_send_eoi2
	.type	pic_send_eoi2, @function
pic_send_eoi2:
.LFB3:
	.loc 1 58 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	.loc 1 59 0
	call	read_first
	movzbl	%al, %eax
	movl	%eax, -12(%ebp)
	.loc 1 60 0
	subl	$8, %esp
	pushl	-12(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
.L7:
	.loc 1 61 0 discriminator 1
	jmp	.L7
	.cfi_endproc
.LFE3:
	.size	pic_send_eoi2, .-pic_send_eoi2
.Letext0:
	.file 2 "stdint.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x10a
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF17
	.byte	0xc
	.long	.LASF18
	.long	.LASF19
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF2
	.byte	0x2
	.byte	0x4
	.long	0x30
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.long	.LASF0
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF1
	.uleb128 0x2
	.long	.LASF3
	.byte	0x2
	.byte	0x6
	.long	0x49
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.long	.LASF4
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.long	.LASF6
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.long	.LASF7
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF8
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x4
	.long	.LASF10
	.byte	0x1
	.byte	0x13
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0x97
	.uleb128 0x5
	.long	.LASF12
	.byte	0x1
	.byte	0x13
	.long	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.byte	0
	.uleb128 0x6
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x4
	.long	.LASF11
	.byte	0x1
	.byte	0x2b
	.long	.LFB1
	.long	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0xc2
	.uleb128 0x5
	.long	.LASF13
	.byte	0x1
	.byte	0x2b
	.long	0x3e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x4
	.long	.LASF14
	.byte	0x1
	.byte	0x31
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0xe6
	.uleb128 0x5
	.long	.LASF15
	.byte	0x1
	.byte	0x31
	.long	0xe6
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF16
	.uleb128 0x7
	.long	.LASF20
	.byte	0x1
	.byte	0x39
	.long	.LFB3
	.long	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x8
	.string	"ret"
	.byte	0x1
	.byte	0x3b
	.long	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x1c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.value	0
	.value	0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF9:
	.string	"long long int"
.LASF10:
	.string	"pic_remap"
.LASF19:
	.string	"/home/simon/os/multiboot_grub"
.LASF17:
	.string	"GNU C11 5.2.1 20150903 -m32 -mtune=generic -march=i586 -g -g -fno-stack-protector"
.LASF14:
	.string	"pic_send_eoi"
.LASF6:
	.string	"long unsigned int"
.LASF18:
	.string	"pic.c"
.LASF8:
	.string	"long long unsigned int"
.LASF15:
	.string	"irq_num"
.LASF13:
	.string	"mask"
.LASF2:
	.string	"uint8_t"
.LASF0:
	.string	"unsigned char"
.LASF16:
	.string	"char"
.LASF12:
	.string	"interrupt_num"
.LASF7:
	.string	"long int"
.LASF20:
	.string	"pic_send_eoi2"
.LASF4:
	.string	"short unsigned int"
.LASF1:
	.string	"signed char"
.LASF11:
	.string	"pic_mask_irqs"
.LASF3:
	.string	"uint16_t"
.LASF5:
	.string	"short int"
	.ident	"GCC: (Debian 5.2.1-16) 5.2.1 20150903"
	.section	.note.GNU-stack,"",@progbits
