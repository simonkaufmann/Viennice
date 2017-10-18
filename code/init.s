	.file	"init.c"
	.text
.Ltext0:
	.section	.rodata
.LC0:
	.string	"Zahl: "
.LC1:
	.string	""
.LC2:
	.string	"Zeit: "
	.text
	.globl	init
	.type	init, @function
init:
.LFB0:
	.file 1 "init.c"
	.loc 1 16 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	.loc 1 17 0
	movl	$1919250511, -37(%ebp)
	movl	$1852404833, -33(%ebp)
	movl	$2035490919, -29(%ebp)
	movl	$1835365491, -25(%ebp)
	movl	$1444949280, -21(%ebp)
	movl	$1313752393, -17(%ebp)
	movl	$4539209, -13(%ebp)
	.loc 1 18 0
	subl	$12, %esp
	leal	-37(%ebp), %eax
	pushl	%eax
	call	println
	addl	$16, %esp
	.loc 1 19 0
	subl	$12, %esp
	pushl	$.LC0
	call	print
	addl	$16, %esp
	.loc 1 20 0
	subl	$12, %esp
	pushl	$125
	call	print_int
	addl	$16, %esp
	.loc 1 21 0
	subl	$12, %esp
	pushl	$.LC1
	call	println
	addl	$16, %esp
.L2:
	.loc 1 24 0 discriminator 1
	subl	$12, %esp
	pushl	$112
	call	inb
	addl	$16, %esp
	andl	$-128, %eax
	movb	%al, -9(%ebp)
	.loc 1 26 0 discriminator 1
	movsbl	-9(%ebp), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$112
	call	outb
	addl	$16, %esp
	.loc 1 27 0 discriminator 1
	subl	$12, %esp
	pushl	$113
	call	inb
	addl	$16, %esp
	movb	%al, -9(%ebp)
	.loc 1 29 0 discriminator 1
	subl	$12, %esp
	pushl	$.LC2
	call	print
	addl	$16, %esp
	.loc 1 30 0 discriminator 1
	movsbl	-9(%ebp), %eax
	subl	$12, %esp
	pushl	%eax
	call	print_int
	addl	$16, %esp
	.loc 1 31 0 discriminator 1
	call	reset_line
	.loc 1 32 0 discriminator 1
	jmp	.L2
	.cfi_endproc
.LFE0:
	.size	init, .-init
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x70
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF3
	.byte	0x1
	.long	.LASF4
	.long	.LASF5
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF6
	.byte	0x1
	.byte	0x10
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0x55
	.uleb128 0x3
	.long	.LASF0
	.byte	0x1
	.byte	0x11
	.long	0x55
	.uleb128 0x2
	.byte	0x91
	.sleb128 -45
	.uleb128 0x4
	.string	"x"
	.byte	0x1
	.byte	0x16
	.long	0x6c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.byte	0
	.uleb128 0x5
	.long	0x6c
	.long	0x65
	.uleb128 0x6
	.long	0x65
	.byte	0x1b
	.byte	0
	.uleb128 0x7
	.byte	0x4
	.byte	0x7
	.long	.LASF1
	.uleb128 0x7
	.byte	0x1
	.byte	0x6
	.long	.LASF2
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
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x34
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
	.uleb128 0x4
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
	.uleb128 0x5
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x7
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
.LASF3:
	.string	"GNU C 4.9.3 -m32 -mtune=generic -march=i586 -g -fno-stack-protector"
.LASF0:
	.string	"Message"
.LASF1:
	.string	"sizetype"
.LASF4:
	.string	"init.c"
.LASF2:
	.string	"char"
.LASF6:
	.string	"init"
.LASF5:
	.string	"/home/simon/os/multiboot_grub"
	.ident	"GCC: (Debian 4.9.3-4) 4.9.3"
	.section	.note.GNU-stack,"",@progbits
