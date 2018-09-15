	.file	"main_3_3.cpp"
	.text
.Ltext0:
	.section	.text._ZN4TestC2Ev,"axG",@progbits,Test::Test(),comdat
	.align 2
	.weak	Test::Test()
	.type	Test::Test(), @function
Test::Test():
.LFB1:
	.file 1 "main_3_3.cpp"
	.loc 1 6 0
	.cfi_startproc
	pushl	%ebp
.LCFI0:
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
.LCFI1:
	.cfi_def_cfa_register 5
.LBB2:
	.loc 1 6 0
	movl	8(%ebp), %eax
	movl	$3333, (%eax)
.LBE2:
	popl	%ebp
.LCFI2:
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	Test::Test(), .-Test::Test()
	.weak	Test::Test()
	.set	Test::Test(),Test::Test()
	.section	.text._ZN4Test5func2Eii,"axG",@progbits,Test::func2(int, int),comdat
	.align 2
	.weak	Test::func2(int, int)
	.type	Test::func2(int, int), @function
Test::func2(int, int):
.LFB3:
	.loc 1 7 0
	.cfi_startproc
	pushl	%ebp
.LCFI3:
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
.LCFI4:
	.cfi_def_cfa_register 5
	.loc 1 9 0
	addl	$1, 12(%ebp)
	.loc 1 10 0
	addl	$2, 16(%ebp)
	.loc 1 11 0
	popl	%ebp
.LCFI5:
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	Test::func2(int, int), .-Test::func2(int, int)
	.text
	.globl	main
	.type	main, @function
main:
.LFB4:
	.loc 1 17 0
	.cfi_startproc
	pushl	%ebp
.LCFI6:
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
.LCFI7:
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
.LBB3:
	.loc 1 18 0
	leal	28(%esp), %eax
	movl	%eax, (%esp)
	call	Test::Test()
	.loc 1 19 0
	movl	$2222, 8(%esp)
	movl	$1111, 4(%esp)
	leal	28(%esp), %eax
	movl	%eax, (%esp)
	call	Test::func2(int, int)
	.loc 1 20 0
	movl	$0, %eax
.LBE3:
	.loc 1 21 0
	leave
.LCFI8:
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	main, .-main
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x173
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF5
	.byte	0x4
	.long	.LASF6
	.long	.LASF7
	.long	.Ldebug_ranges0+0
	.long	0
	.long	0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF0
	.byte	0x4
	.byte	0x1
	.byte	0x3
	.long	0x7f
	.uleb128 0x3
	.long	.LASF8
	.byte	0x1
	.byte	0xd
	.long	0x7f
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.byte	0x3
	.uleb128 0x4
	.byte	0x1
	.long	.LASF0
	.byte	0x1
	.byte	0x6
	.byte	0x1
	.long	0x55
	.long	0x5c
	.uleb128 0x5
	.long	0x86
	.byte	0x1
	.byte	0
	.uleb128 0x6
	.byte	0x1
	.long	.LASF1
	.byte	0x1
	.byte	0x7
	.long	.LASF9
	.byte	0x1
	.long	0x6d
	.uleb128 0x5
	.long	0x86
	.byte	0x1
	.uleb128 0x7
	.long	0x7f
	.uleb128 0x7
	.long	0x7f
	.byte	0
	.byte	0
	.uleb128 0x8
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x9
	.byte	0x4
	.long	0x29
	.uleb128 0xa
	.long	0x44
	.byte	0x2
	.long	0x9a
	.long	0xa5
	.uleb128 0xb
	.long	.LASF10
	.long	0xa5
	.byte	0x1
	.byte	0
	.uleb128 0xc
	.long	0x86
	.uleb128 0xd
	.long	0x8c
	.long	.LASF11
	.long	.LFB1
	.long	.LFE1
	.long	.LLST0
	.long	0xc8
	.byte	0x1
	.long	0xd1
	.uleb128 0xe
	.long	0x9a
	.byte	0x2
	.byte	0x91
	.sleb128 0
	.byte	0
	.uleb128 0xf
	.long	0x5c
	.long	.LFB3
	.long	.LFE3
	.long	.LLST1
	.long	0xeb
	.byte	0x1
	.long	0x111
	.uleb128 0x10
	.long	.LASF10
	.long	0xa5
	.byte	0x1
	.byte	0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x11
	.string	"a"
	.byte	0x1
	.byte	0x7
	.long	0x7f
	.byte	0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0x11
	.string	"b"
	.byte	0x1
	.byte	0x7
	.long	0x7f
	.byte	0x2
	.byte	0x91
	.sleb128 8
	.byte	0
	.uleb128 0x12
	.byte	0x1
	.long	.LASF12
	.byte	0x1
	.byte	0x10
	.long	0x7f
	.long	.LFB4
	.long	.LFE4
	.long	.LLST2
	.byte	0x1
	.long	0x163
	.uleb128 0x13
	.long	.LASF2
	.byte	0x1
	.byte	0x10
	.long	0x7f
	.byte	0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x13
	.long	.LASF3
	.byte	0x1
	.byte	0x10
	.long	0x163
	.byte	0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0x14
	.long	.LBB3
	.long	.LBE3
	.uleb128 0x15
	.long	.LASF13
	.byte	0x1
	.byte	0x12
	.long	0x29
	.byte	0x2
	.byte	0x74
	.sleb128 28
	.byte	0
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.long	0x169
	.uleb128 0x9
	.byte	0x4
	.long	0x16f
	.uleb128 0x16
	.byte	0x1
	.byte	0x6
	.long	.LASF4
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
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x2
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.uleb128 0x32
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
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
	.uleb128 0x9
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x5
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
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x2116
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
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
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0
	.byte	0
	.uleb128 0x15
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
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x16
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
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.long	.LFB1
	.long	.LCFI0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI0
	.long	.LCFI1
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI1
	.long	.LCFI2
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LCFI2
	.long	.LFE1
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	0
	.long	0
.LLST1:
	.long	.LFB3
	.long	.LCFI3
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI3
	.long	.LCFI4
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI4
	.long	.LCFI5
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LCFI5
	.long	.LFE3
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	0
	.long	0
.LLST2:
	.long	.LFB4
	.long	.LCFI6
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI6
	.long	.LCFI7
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI7
	.long	.LCFI8
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LCFI8
	.long	.LFE4
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	0
	.long	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.value	0
	.value	0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.LFB1
	.long	.LFE1-.LFB1
	.long	.LFB3
	.long	.LFE3-.LFB3
	.long	0
	.long	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.Ltext0
	.long	.Letext0
	.long	.LFB1
	.long	.LFE1
	.long	.LFB3
	.long	.LFE3
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF7:
	.string	"/home/boshin/boshin/github/test/linuxdebugging"
.LASF11:
	.string	"Test::Test()"
.LASF10:
	.string	"this"
.LASF13:
	.string	"tInst"
.LASF8:
	.string	"number"
.LASF6:
	.string	"main_3_3.cpp"
.LASF2:
	.string	"argc"
.LASF12:
	.string	"main"
.LASF5:
	.string	"GNU C++ 4.7.4"
.LASF9:
	.string	"Test::func2(int, int)"
.LASF1:
	.string	"func2"
.LASF4:
	.string	"char"
.LASF0:
	.string	"Test"
.LASF3:
	.string	"argv"
	.ident	"GCC: (Ubuntu/Linaro 4.7.4-3ubuntu12) 4.7.4"
	.section	.note.GNU-stack,"",@progbits
