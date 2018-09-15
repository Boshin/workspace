	.file	"main_4_3.cpp"
	.text
.Ltext0:
	.section	.text._ZN7Point2d3FooEv,"axG",@progbits,Point2d::Foo(),comdat
	.align 2
	.weak	Point2d::Foo()
	.type	Point2d::Foo(), @function
Point2d::Foo():
.LFB0:
	.file 1 "main_4_3.cpp"
	.loc 1 3 0
	.cfi_startproc
	pushq	%rbp
.LCFI0:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI1:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	.loc 1 3 0
	popq	%rbp
.LCFI2:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	Point2d::Foo(), .-Point2d::Foo()
	.section	.text._ZN7Point2d3BooEv,"axG",@progbits,Point2d::Boo(),comdat
	.align 2
	.weak	Point2d::Boo()
	.type	Point2d::Boo(), @function
Point2d::Boo():
.LFB1:
	.loc 1 4 0
	.cfi_startproc
	pushq	%rbp
.LCFI3:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI4:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	.loc 1 4 0
	popq	%rbp
.LCFI5:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	Point2d::Boo(), .-Point2d::Boo()
	.section	.text._ZN7Point2d13non_overwriteEv,"axG",@progbits,Point2d::non_overwrite(),comdat
	.align 2
	.weak	Point2d::non_overwrite()
	.type	Point2d::non_overwrite(), @function
Point2d::non_overwrite():
.LFB2:
	.loc 1 5 0
	.cfi_startproc
	pushq	%rbp
.LCFI6:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI7:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	.loc 1 5 0
	popq	%rbp
.LCFI8:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	Point2d::non_overwrite(), .-Point2d::non_overwrite()
	.section	.text._ZN6Vertex3FooEv,"axG",@progbits,Vertex::Foo(),comdat
	.align 2
	.weak	Vertex::Foo()
	.type	Vertex::Foo(), @function
Vertex::Foo():
.LFB5:
	.loc 1 17 0
	.cfi_startproc
	pushq	%rbp
.LCFI9:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI10:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	.loc 1 17 0
	popq	%rbp
.LCFI11:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	Vertex::Foo(), .-Vertex::Foo()
	.set	.LTHUNK0,Vertex::Foo()
	.weak	virtual thunk to Vertex::Foo()
	.type	virtual thunk to Vertex::Foo(), @function
virtual thunk to Vertex::Foo():
.LFB22:
	.cfi_startproc
	movq	(%rdi), %r10
	addq	-24(%r10), %rdi
	jmp	.LTHUNK0
	.cfi_endproc
.LFE22:
	.size	virtual thunk to Vertex::Foo(), .-virtual thunk to Vertex::Foo()
	.section	.text._ZN6Vertex6BooVerEv,"axG",@progbits,Vertex::BooVer(),comdat
	.align 2
	.weak	Vertex::BooVer()
	.type	Vertex::BooVer(), @function
Vertex::BooVer():
.LFB6:
	.loc 1 18 0
	.cfi_startproc
	pushq	%rbp
.LCFI12:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI13:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	.loc 1 18 0
	popq	%rbp
.LCFI14:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	Vertex::BooVer(), .-Vertex::BooVer()
	.section	.text._ZN7Point3d5Boo3dEv,"axG",@progbits,Point3d::Boo3d(),comdat
	.align 2
	.weak	Point3d::Boo3d()
	.type	Point3d::Boo3d(), @function
Point3d::Boo3d():
.LFB7:
	.loc 1 25 0
	.cfi_startproc
	pushq	%rbp
.LCFI15:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI16:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	.loc 1 25 0
	popq	%rbp
.LCFI17:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	Point3d::Boo3d(), .-Point3d::Boo3d()
	.section	.text._ZN8Vertex3d4testEv,"axG",@progbits,Vertex3d::test(),comdat
	.align 2
	.weak	Vertex3d::test()
	.type	Vertex3d::test(), @function
Vertex3d::test():
.LFB8:
	.loc 1 32 0
	.cfi_startproc
	pushq	%rbp
.LCFI18:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI19:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	.loc 1 32 0
	popq	%rbp
.LCFI20:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	Vertex3d::test(), .-Vertex3d::test()
	.section	.text._ZN7Point2dC2Ev,"axG",@progbits,Point2d::Point2d(),comdat
	.align 2
	.weak	Point2d::Point2d()
	.type	Point2d::Point2d(), @function
Point2d::Point2d():
.LFB12:
	.loc 1 1 0
	.cfi_startproc
	pushq	%rbp
.LCFI21:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI22:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
.LBB2:
	.loc 1 1 0
	movq	-8(%rbp), %rax
	movq	vtable for Point2d+16, (%rax)
.LBE2:
	popq	%rbp
.LCFI23:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	Point2d::Point2d(), .-Point2d::Point2d()
	.weak	Point2d::Point2d()
	.set	Point2d::Point2d(),Point2d::Point2d()
	.section	.text._ZN6VertexC2Ev,"axG",@progbits,Vertex::Vertex(),comdat
	.align 2
	.weak	Vertex::Vertex()
	.type	Vertex::Vertex(), @function
Vertex::Vertex():
.LFB15:
	.loc 1 15 0
	.cfi_startproc
	pushq	%rbp
.LCFI24:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI25:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
.LBB3:
	.loc 1 15 0
	movq	-16(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	subq	$24, %rax
	movq	(%rax), %rax
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	addq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, (%rdx)
.LBE3:
	popq	%rbp
.LCFI26:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	Vertex::Vertex(), .-Vertex::Vertex()
	.section	.text._ZN7Point3dC2Ev,"axG",@progbits,Point3d::Point3d(),comdat
	.align 2
	.weak	Point3d::Point3d()
	.type	Point3d::Point3d(), @function
Point3d::Point3d():
.LFB18:
	.loc 1 23 0
	.cfi_startproc
	pushq	%rbp
.LCFI27:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI28:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
.LBB4:
	.loc 1 23 0
	movq	-16(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	subq	$24, %rax
	movq	(%rax), %rax
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	addq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, (%rdx)
.LBE4:
	popq	%rbp
.LCFI29:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	Point3d::Point3d(), .-Point3d::Point3d()
	.section	.text._ZN8Vertex3dC1Ev,"axG",@progbits,Vertex3d::Vertex3d(),comdat
	.align 2
	.weak	Vertex3d::Vertex3d()
	.type	Vertex3d::Vertex3d(), @function
Vertex3d::Vertex3d():
.LFB21:
	.loc 1 30 0
	.cfi_startproc
	pushq	%rbp
.LCFI30:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI31:
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
.LBB5:
	.loc 1 30 0
	movq	-8(%rbp), %rax
	addq	$32, %rax
	movq	%rax, %rdi
	call	Point2d::Point2d()
	movl	VTT for Vertex3d+8, %edx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	Vertex::Vertex()
	movl	VTT for Vertex3d+24, %eax
	movq	-8(%rbp), %rdx
	addq	$16, %rdx
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	Point3d::Point3d()
	movl	vtable for Vertex3d+24, %edx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movl	$32, %edx
	movq	-8(%rbp), %rax
	addq	%rax, %rdx
	movl	vtable for Vertex3d+112, %eax
	movq	%rax, (%rdx)
	movl	vtable for Vertex3d+64, %edx
	movq	-8(%rbp), %rax
	movq	%rdx, 16(%rax)
.LBE5:
	leave
.LCFI32:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	Vertex3d::Vertex3d(), .-Vertex3d::Vertex3d()
	.text
	.globl	main
	.type	main, @function
main:
.LFB9:
	.loc 1 38 0
	.cfi_startproc
	pushq	%rbp
.LCFI33:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI34:
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	%edi, -52(%rbp)
	movq	%rsi, -64(%rbp)
.LBB6:
	.loc 1 39 0
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	Vertex3d::Vertex3d()
	.loc 1 40 0
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	Vertex3d::test()
	.loc 1 41 0
	movl	$0, %eax
.LBE6:
	.loc 1 42 0
	leave
.LCFI35:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	main, .-main
	.weak	vtable for Vertex3d
	.section	.rodata._ZTV8Vertex3d,"aG",@progbits,vtable for Vertex3d,comdat
	.align 32
	.type	vtable for Vertex3d, @object
	.size	vtable for Vertex3d, 136
vtable for Vertex3d:
	.quad	32
	.quad	0
	.quad	typeinfo for Vertex3d
	.quad	Vertex::Foo()
	.quad	Vertex::BooVer()
	.quad	16
	.quad	-16
	.quad	typeinfo for Vertex3d
	.quad	Point3d::Boo3d()
	.quad	0
	.quad	0
	.quad	-32
	.quad	-32
	.quad	typeinfo for Vertex3d
	.quad	virtual thunk to Vertex::Foo()
	.quad	Point2d::Boo()
	.quad	Point2d::non_overwrite()
	.weak	VTT for Vertex3d
	.section	.rodata._ZTT8Vertex3d,"aG",@progbits,vtable for Vertex3d,comdat
	.align 32
	.type	VTT for Vertex3d, @object
	.size	VTT for Vertex3d, 56
VTT for Vertex3d:
	.quad	vtable for Vertex3d+24
	.quad	construction vtable for Vertex-in-Vertex3d+24
	.quad	construction vtable for Vertex-in-Vertex3d+80
	.quad	construction vtable for Point3d-in-Vertex3d+24
	.quad	construction vtable for Point3d-in-Vertex3d+72
	.quad	vtable for Vertex3d+112
	.quad	vtable for Vertex3d+64
	.weak	construction vtable for Vertex-in-Vertex3d
	.section	.rodata._ZTC8Vertex3d0_6Vertex,"aG",@progbits,vtable for Vertex3d,comdat
	.align 32
	.type	construction vtable for Vertex-in-Vertex3d, @object
	.size	construction vtable for Vertex-in-Vertex3d, 104
construction vtable for Vertex-in-Vertex3d:
	.quad	32
	.quad	0
	.quad	typeinfo for Vertex
	.quad	Vertex::Foo()
	.quad	Vertex::BooVer()
	.quad	0
	.quad	0
	.quad	-32
	.quad	-32
	.quad	typeinfo for Vertex
	.quad	virtual thunk to Vertex::Foo()
	.quad	Point2d::Boo()
	.quad	Point2d::non_overwrite()
	.weak	construction vtable for Point3d-in-Vertex3d
	.section	.rodata._ZTC8Vertex3d16_7Point3d,"aG",@progbits,vtable for Vertex3d,comdat
	.align 32
	.type	construction vtable for Point3d-in-Vertex3d, @object
	.size	construction vtable for Point3d-in-Vertex3d, 96
construction vtable for Point3d-in-Vertex3d:
	.quad	16
	.quad	0
	.quad	typeinfo for Point3d
	.quad	Point3d::Boo3d()
	.quad	0
	.quad	0
	.quad	0
	.quad	-16
	.quad	typeinfo for Point3d
	.quad	Point2d::Foo()
	.quad	Point2d::Boo()
	.quad	Point2d::non_overwrite()
	.weak	vtable for Point2d
	.section	.rodata._ZTV7Point2d,"aG",@progbits,vtable for Point2d,comdat
	.align 32
	.type	vtable for Point2d, @object
	.size	vtable for Point2d, 40
vtable for Point2d:
	.quad	0
	.quad	typeinfo for Point2d
	.quad	Point2d::Foo()
	.quad	Point2d::Boo()
	.quad	Point2d::non_overwrite()
	.weak	typeinfo name for Vertex3d
	.section	.rodata._ZTS8Vertex3d,"aG",@progbits,typeinfo name for Vertex3d,comdat
	.type	typeinfo name for Vertex3d, @object
	.size	typeinfo name for Vertex3d, 10
typeinfo name for Vertex3d:
	.string	"8Vertex3d"
	.weak	typeinfo for Vertex3d
	.section	.rodata._ZTI8Vertex3d,"aG",@progbits,typeinfo for Vertex3d,comdat
	.align 32
	.type	typeinfo for Vertex3d, @object
	.size	typeinfo for Vertex3d, 56
typeinfo for Vertex3d:
	.quad	vtable for __cxxabiv1::__vmi_class_type_info+16
	.quad	typeinfo name for Vertex3d
	.long	2
	.long	2
	.quad	typeinfo for Vertex
	.quad	2
	.quad	typeinfo for Point3d
	.quad	4098
	.weak	typeinfo name for Point3d
	.section	.rodata._ZTS7Point3d,"aG",@progbits,typeinfo name for Point3d,comdat
	.type	typeinfo name for Point3d, @object
	.size	typeinfo name for Point3d, 9
typeinfo name for Point3d:
	.string	"7Point3d"
	.weak	typeinfo for Point3d
	.section	.rodata._ZTI7Point3d,"aG",@progbits,typeinfo for Point3d,comdat
	.align 32
	.type	typeinfo for Point3d, @object
	.size	typeinfo for Point3d, 40
typeinfo for Point3d:
	.quad	vtable for __cxxabiv1::__vmi_class_type_info+16
	.quad	typeinfo name for Point3d
	.long	0
	.long	1
	.quad	typeinfo for Point2d
	.quad	-6141
	.weak	typeinfo name for Vertex
	.section	.rodata._ZTS6Vertex,"aG",@progbits,typeinfo name for Vertex,comdat
	.type	typeinfo name for Vertex, @object
	.size	typeinfo name for Vertex, 8
typeinfo name for Vertex:
	.string	"6Vertex"
	.weak	typeinfo for Vertex
	.section	.rodata._ZTI6Vertex,"aG",@progbits,typeinfo for Vertex,comdat
	.align 32
	.type	typeinfo for Vertex, @object
	.size	typeinfo for Vertex, 40
typeinfo for Vertex:
	.quad	vtable for __cxxabiv1::__vmi_class_type_info+16
	.quad	typeinfo name for Vertex
	.long	0
	.long	1
	.quad	typeinfo for Point2d
	.quad	-6141
	.weak	typeinfo name for Point2d
	.section	.rodata._ZTS7Point2d,"aG",@progbits,typeinfo name for Point2d,comdat
	.type	typeinfo name for Point2d, @object
	.size	typeinfo name for Point2d, 9
typeinfo name for Point2d:
	.string	"7Point2d"
	.weak	typeinfo for Point2d
	.section	.rodata._ZTI7Point2d,"aG",@progbits,typeinfo for Point2d,comdat
	.align 16
	.type	typeinfo for Point2d, @object
	.size	typeinfo for Point2d, 16
typeinfo for Point2d:
	.quad	vtable for __cxxabiv1::__class_type_info+16
	.quad	typeinfo name for Point2d
	.text
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x72e
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF29
	.byte	0x4
	.long	.LASF30
	.long	.LASF31
	.long	.Ldebug_ranges0+0
	.quad	0
	.quad	0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF0
	.byte	0x30
	.byte	0x1
	.byte	0x1e
	.long	0xc6
	.long	0xc6
	.uleb128 0x3
	.long	0xc6
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.byte	0x1
	.uleb128 0x3
	.long	0x190
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.byte	0x1
	.uleb128 0x4
	.long	.LASF2
	.byte	0x1
	.byte	0x22
	.long	0x235
	.byte	0x2
	.byte	0x23
	.uleb128 0x1c
	.byte	0x2
	.uleb128 0x5
	.byte	0x1
	.long	.LASF0
	.byte	0x1
	.byte	0x1
	.long	0x72
	.long	0x8a
	.uleb128 0x6
	.long	0x23c
	.byte	0x1
	.uleb128 0x6
	.long	0x242
	.byte	0x1
	.uleb128 0x6
	.long	0x249
	.byte	0x1
	.uleb128 0x7
	.long	0x256
	.byte	0
	.uleb128 0x5
	.byte	0x1
	.long	.LASF0
	.byte	0x1
	.byte	0x1
	.long	0x9a
	.long	0xad
	.uleb128 0x6
	.long	0x23c
	.byte	0x1
	.uleb128 0x6
	.long	0x242
	.byte	0x1
	.uleb128 0x6
	.long	0x249
	.byte	0x1
	.byte	0
	.uleb128 0x8
	.byte	0x1
	.long	.LASF32
	.byte	0x1
	.byte	0x20
	.long	.LASF33
	.byte	0x1
	.long	0xbe
	.uleb128 0x6
	.long	0x23c
	.byte	0x1
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF1
	.byte	0x20
	.byte	0x1
	.byte	0xf
	.long	0xc6
	.long	0x190
	.uleb128 0x9
	.long	0x261
	.byte	0x6
	.byte	0x12
	.byte	0x6
	.byte	0x48
	.byte	0x1c
	.byte	0x6
	.byte	0x22
	.byte	0x1
	.byte	0x1
	.uleb128 0xa
	.long	.LASF5
	.long	0x37b
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.byte	0x1
	.uleb128 0x4
	.long	.LASF3
	.byte	0x1
	.byte	0x14
	.long	0x39c
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.byte	0x2
	.uleb128 0x5
	.byte	0x1
	.long	.LASF1
	.byte	0x1
	.byte	0x1
	.long	0x110
	.long	0x128
	.uleb128 0x6
	.long	0x3a2
	.byte	0x1
	.uleb128 0x6
	.long	0x242
	.byte	0x1
	.uleb128 0x6
	.long	0x249
	.byte	0x1
	.uleb128 0x7
	.long	0x3a8
	.byte	0
	.uleb128 0x5
	.byte	0x1
	.long	.LASF1
	.byte	0x1
	.byte	0x1
	.long	0x138
	.long	0x14b
	.uleb128 0x6
	.long	0x3a2
	.byte	0x1
	.uleb128 0x6
	.long	0x242
	.byte	0x1
	.uleb128 0x6
	.long	0x249
	.byte	0x1
	.byte	0
	.uleb128 0xb
	.byte	0x1
	.string	"Foo"
	.byte	0x1
	.byte	0x11
	.long	.LASF14
	.byte	0x1
	.byte	0x2
	.byte	0x10
	.uleb128 0
	.long	0xc6
	.byte	0x1
	.long	0x168
	.long	0x16f
	.uleb128 0x6
	.long	0x3a2
	.byte	0x1
	.byte	0
	.uleb128 0xc
	.byte	0x1
	.long	.LASF7
	.byte	0x1
	.byte	0x12
	.long	.LASF9
	.byte	0x1
	.byte	0x2
	.byte	0x10
	.uleb128 0x1
	.long	0xc6
	.byte	0x1
	.long	0x188
	.uleb128 0x6
	.long	0x3a2
	.byte	0x1
	.byte	0
	.byte	0
	.uleb128 0x2
	.long	.LASF4
	.byte	0x20
	.byte	0x1
	.byte	0x17
	.long	0x190
	.long	0x235
	.uleb128 0x9
	.long	0x261
	.byte	0x6
	.byte	0x12
	.byte	0x6
	.byte	0x48
	.byte	0x1c
	.byte	0x6
	.byte	0x22
	.byte	0x1
	.byte	0x1
	.uleb128 0xa
	.long	.LASF6
	.long	0x37b
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.byte	0x1
	.uleb128 0xd
	.string	"_z"
	.byte	0x1
	.byte	0x1b
	.long	0x235
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.byte	0x2
	.uleb128 0x5
	.byte	0x1
	.long	.LASF4
	.byte	0x1
	.byte	0x1
	.long	0x1d9
	.long	0x1f1
	.uleb128 0x6
	.long	0x38b
	.byte	0x1
	.uleb128 0x6
	.long	0x242
	.byte	0x1
	.uleb128 0x6
	.long	0x249
	.byte	0x1
	.uleb128 0x7
	.long	0x391
	.byte	0
	.uleb128 0x5
	.byte	0x1
	.long	.LASF4
	.byte	0x1
	.byte	0x1
	.long	0x201
	.long	0x214
	.uleb128 0x6
	.long	0x38b
	.byte	0x1
	.uleb128 0x6
	.long	0x242
	.byte	0x1
	.uleb128 0x6
	.long	0x249
	.byte	0x1
	.byte	0
	.uleb128 0xc
	.byte	0x1
	.long	.LASF8
	.byte	0x1
	.byte	0x19
	.long	.LASF10
	.byte	0x1
	.byte	0x2
	.byte	0x10
	.uleb128 0
	.long	0x190
	.byte	0x1
	.long	0x22d
	.uleb128 0x6
	.long	0x38b
	.byte	0x1
	.byte	0
	.byte	0
	.uleb128 0xe
	.byte	0x4
	.byte	0x4
	.long	.LASF11
	.uleb128 0xf
	.byte	0x8
	.long	0x31
	.uleb128 0x10
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0xf
	.byte	0x8
	.long	0x24f
	.uleb128 0xf
	.byte	0x8
	.long	0x255
	.uleb128 0x11
	.uleb128 0x12
	.byte	0x8
	.long	0x25c
	.uleb128 0x13
	.long	0x31
	.uleb128 0x2
	.long	.LASF12
	.byte	0x10
	.byte	0x1
	.byte	0x1
	.long	0x261
	.long	0x370
	.uleb128 0xa
	.long	.LASF13
	.long	0x37b
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.byte	0x1
	.uleb128 0xd
	.string	"_x"
	.byte	0x1
	.byte	0xc
	.long	0x235
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.byte	0x2
	.uleb128 0xd
	.string	"_y"
	.byte	0x1
	.byte	0xc
	.long	0x235
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.byte	0x2
	.uleb128 0x5
	.byte	0x1
	.long	.LASF12
	.byte	0x1
	.byte	0x1
	.long	0x2aa
	.long	0x2b6
	.uleb128 0x6
	.long	0x3b3
	.byte	0x1
	.uleb128 0x7
	.long	0x3b9
	.byte	0
	.uleb128 0x5
	.byte	0x1
	.long	.LASF12
	.byte	0x1
	.byte	0x1
	.long	0x2c6
	.long	0x2cd
	.uleb128 0x6
	.long	0x3b3
	.byte	0x1
	.byte	0
	.uleb128 0xb
	.byte	0x1
	.string	"Foo"
	.byte	0x1
	.byte	0x3
	.long	.LASF15
	.byte	0x1
	.byte	0x2
	.byte	0x10
	.uleb128 0
	.long	0x261
	.byte	0x1
	.long	0x2ea
	.long	0x2f1
	.uleb128 0x6
	.long	0x3b3
	.byte	0x1
	.byte	0
	.uleb128 0xb
	.byte	0x1
	.string	"Boo"
	.byte	0x1
	.byte	0x4
	.long	.LASF16
	.byte	0x1
	.byte	0x2
	.byte	0x10
	.uleb128 0x1
	.long	0x261
	.byte	0x1
	.long	0x30e
	.long	0x315
	.uleb128 0x6
	.long	0x3b3
	.byte	0x1
	.byte	0
	.uleb128 0x14
	.byte	0x1
	.long	.LASF17
	.byte	0x1
	.byte	0x5
	.long	.LASF34
	.byte	0x1
	.byte	0x2
	.byte	0x10
	.uleb128 0x2
	.long	0x261
	.byte	0x1
	.long	0x332
	.long	0x339
	.uleb128 0x6
	.long	0x3b3
	.byte	0x1
	.byte	0
	.uleb128 0x15
	.byte	0x1
	.long	.LASF35
	.byte	0x1
	.byte	0x8
	.long	.LASF36
	.byte	0x3
	.byte	0x1
	.long	0x34f
	.long	0x356
	.uleb128 0x6
	.long	0x3b3
	.byte	0x1
	.byte	0
	.uleb128 0x16
	.byte	0x1
	.long	.LASF37
	.byte	0x1
	.byte	0xb
	.long	.LASF38
	.byte	0x2
	.byte	0x1
	.long	0x368
	.uleb128 0x6
	.long	0x3b3
	.byte	0x1
	.byte	0
	.byte	0
	.uleb128 0x17
	.long	0x242
	.long	0x37b
	.uleb128 0x18
	.byte	0
	.uleb128 0xf
	.byte	0x8
	.long	0x381
	.uleb128 0x19
	.byte	0x8
	.long	.LASF39
	.long	0x370
	.uleb128 0xf
	.byte	0x8
	.long	0x190
	.uleb128 0x12
	.byte	0x8
	.long	0x397
	.uleb128 0x13
	.long	0x190
	.uleb128 0xf
	.byte	0x8
	.long	0xc6
	.uleb128 0xf
	.byte	0x8
	.long	0xc6
	.uleb128 0x12
	.byte	0x8
	.long	0x3ae
	.uleb128 0x13
	.long	0xc6
	.uleb128 0xf
	.byte	0x8
	.long	0x261
	.uleb128 0x12
	.byte	0x8
	.long	0x3bf
	.uleb128 0x13
	.long	0x261
	.uleb128 0x1a
	.long	0x2cd
	.quad	.LFB0
	.quad	.LFE0
	.long	.LLST0
	.long	0x3e6
	.byte	0x1
	.long	0x3f4
	.uleb128 0x1b
	.long	.LASF18
	.long	0x3f4
	.byte	0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x13
	.long	0x3b3
	.uleb128 0x1a
	.long	0x2f1
	.quad	.LFB1
	.quad	.LFE1
	.long	.LLST1
	.long	0x41b
	.byte	0x1
	.long	0x429
	.uleb128 0x1b
	.long	.LASF18
	.long	0x3f4
	.byte	0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1a
	.long	0x315
	.quad	.LFB2
	.quad	.LFE2
	.long	.LLST2
	.long	0x44b
	.byte	0x1
	.long	0x459
	.uleb128 0x1b
	.long	.LASF18
	.long	0x3f4
	.byte	0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1a
	.long	0x14b
	.quad	.LFB5
	.quad	.LFE5
	.long	.LLST3
	.long	0x47b
	.byte	0x1
	.long	0x489
	.uleb128 0x1b
	.long	.LASF18
	.long	0x489
	.byte	0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x13
	.long	0x3a2
	.uleb128 0x1a
	.long	0x16f
	.quad	.LFB6
	.quad	.LFE6
	.long	.LLST4
	.long	0x4b0
	.byte	0x1
	.long	0x4be
	.uleb128 0x1b
	.long	.LASF18
	.long	0x489
	.byte	0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1a
	.long	0x214
	.quad	.LFB7
	.quad	.LFE7
	.long	.LLST5
	.long	0x4e0
	.byte	0x1
	.long	0x4ee
	.uleb128 0x1b
	.long	.LASF18
	.long	0x4ee
	.byte	0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x13
	.long	0x38b
	.uleb128 0x1a
	.long	0xad
	.quad	.LFB8
	.quad	.LFE8
	.long	.LLST6
	.long	0x515
	.byte	0x1
	.long	0x523
	.uleb128 0x1b
	.long	.LASF18
	.long	0x523
	.byte	0x1
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x13
	.long	0x23c
	.uleb128 0x1c
	.long	0x2b6
	.byte	0x1
	.byte	0x1
	.byte	0x2
	.long	0x538
	.long	0x543
	.uleb128 0x1d
	.long	.LASF18
	.long	0x3f4
	.byte	0x1
	.byte	0
	.uleb128 0x1e
	.long	0x528
	.long	.LASF21
	.quad	.LFB12
	.quad	.LFE12
	.long	.LLST7
	.long	0x569
	.byte	0x1
	.long	0x572
	.uleb128 0x1f
	.long	0x538
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1c
	.long	0x128
	.byte	0x1
	.byte	0xf
	.byte	0x2
	.long	0x582
	.long	0x5a1
	.uleb128 0x1d
	.long	.LASF18
	.long	0x489
	.byte	0x1
	.uleb128 0x1d
	.long	.LASF19
	.long	0x5a1
	.byte	0x1
	.uleb128 0x1d
	.long	.LASF20
	.long	0x5a6
	.byte	0x1
	.byte	0
	.uleb128 0x13
	.long	0x242
	.uleb128 0x13
	.long	0x249
	.uleb128 0x1e
	.long	0x572
	.long	.LASF22
	.quad	.LFB15
	.quad	.LFE15
	.long	.LLST8
	.long	0x5d1
	.byte	0x1
	.long	0x5e2
	.uleb128 0x1f
	.long	0x582
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1f
	.long	0x596
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x1c
	.long	0x1f1
	.byte	0x1
	.byte	0x17
	.byte	0x2
	.long	0x5f2
	.long	0x611
	.uleb128 0x1d
	.long	.LASF18
	.long	0x4ee
	.byte	0x1
	.uleb128 0x1d
	.long	.LASF19
	.long	0x611
	.byte	0x1
	.uleb128 0x1d
	.long	.LASF20
	.long	0x616
	.byte	0x1
	.byte	0
	.uleb128 0x13
	.long	0x242
	.uleb128 0x13
	.long	0x249
	.uleb128 0x1e
	.long	0x5e2
	.long	.LASF23
	.quad	.LFB18
	.quad	.LFE18
	.long	.LLST9
	.long	0x641
	.byte	0x1
	.long	0x652
	.uleb128 0x1f
	.long	0x5f2
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1f
	.long	0x606
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x1c
	.long	0x8a
	.byte	0x1
	.byte	0x1e
	.byte	0x2
	.long	0x662
	.long	0x681
	.uleb128 0x1d
	.long	.LASF18
	.long	0x523
	.byte	0x1
	.uleb128 0x1d
	.long	.LASF19
	.long	0x681
	.byte	0x1
	.uleb128 0x1d
	.long	.LASF20
	.long	0x686
	.byte	0x1
	.byte	0
	.uleb128 0x13
	.long	0x242
	.uleb128 0x13
	.long	0x249
	.uleb128 0x20
	.long	0x652
	.long	.LASF24
	.quad	.LFB21
	.quad	.LFE21
	.long	.LLST10
	.long	0x6b1
	.byte	0x1
	.long	0x6ba
	.uleb128 0x1f
	.long	0x662
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x21
	.byte	0x1
	.long	.LASF25
	.byte	0x1
	.byte	0x25
	.long	0x242
	.quad	.LFB9
	.quad	.LFE9
	.long	.LLST11
	.byte	0x1
	.long	0x71e
	.uleb128 0x22
	.long	.LASF26
	.byte	0x1
	.byte	0x25
	.long	0x242
	.byte	0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x22
	.long	.LASF27
	.byte	0x1
	.byte	0x25
	.long	0x71e
	.byte	0x3
	.byte	0x91
	.sleb128 -80
	.uleb128 0x23
	.quad	.LBB6
	.quad	.LBE6
	.uleb128 0x24
	.string	"obj"
	.byte	0x1
	.byte	0x27
	.long	0x31
	.byte	0x2
	.byte	0x91
	.sleb128 -64
	.byte	0
	.byte	0
	.uleb128 0xf
	.byte	0x8
	.long	0x724
	.uleb128 0xf
	.byte	0x8
	.long	0x72a
	.uleb128 0xe
	.byte	0x1
	.byte	0x6
	.long	.LASF28
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
	.uleb128 0x1d
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x1c
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.uleb128 0x32
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x4
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
	.uleb128 0x5
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
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
	.uleb128 0x9
	.uleb128 0x1c
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.uleb128 0x4c
	.uleb128 0xb
	.uleb128 0x32
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.uleb128 0x34
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x4c
	.uleb128 0xb
	.uleb128 0x4d
	.uleb128 0xa
	.uleb128 0x1d
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
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
	.uleb128 0x4c
	.uleb128 0xb
	.uleb128 0x4d
	.uleb128 0xa
	.uleb128 0x1d
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
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
	.uleb128 0xe
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
	.uleb128 0xf
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
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
	.uleb128 0x11
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x10
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
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
	.uleb128 0x4c
	.uleb128 0xb
	.uleb128 0x4d
	.uleb128 0xa
	.uleb128 0x1d
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
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
	.uleb128 0x32
	.uleb128 0xb
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
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
	.uleb128 0x32
	.uleb128 0xb
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
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
	.uleb128 0x1b
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
	.uleb128 0x1c
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
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
	.uleb128 0x1e
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
	.uleb128 0x1f
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x20
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
	.uleb128 0x2116
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x21
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
	.uleb128 0x22
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
	.uleb128 0x23
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0
	.byte	0
	.uleb128 0x24
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
	.uleb128 0xa
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.quad	.LFB0
	.quad	.LCFI0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0
	.quad	.LCFI1
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI1
	.quad	.LCFI2
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI2
	.quad	.LFE0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST1:
	.quad	.LFB1
	.quad	.LCFI3
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI3
	.quad	.LCFI4
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI4
	.quad	.LCFI5
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI5
	.quad	.LFE1
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST2:
	.quad	.LFB2
	.quad	.LCFI6
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI6
	.quad	.LCFI7
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI7
	.quad	.LCFI8
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI8
	.quad	.LFE2
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST3:
	.quad	.LFB5
	.quad	.LCFI9
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI9
	.quad	.LCFI10
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI10
	.quad	.LCFI11
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI11
	.quad	.LFE5
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST4:
	.quad	.LFB6
	.quad	.LCFI12
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI12
	.quad	.LCFI13
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI13
	.quad	.LCFI14
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI14
	.quad	.LFE6
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST5:
	.quad	.LFB7
	.quad	.LCFI15
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI15
	.quad	.LCFI16
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI16
	.quad	.LCFI17
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI17
	.quad	.LFE7
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST6:
	.quad	.LFB8
	.quad	.LCFI18
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI18
	.quad	.LCFI19
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI19
	.quad	.LCFI20
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI20
	.quad	.LFE8
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST7:
	.quad	.LFB12
	.quad	.LCFI21
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI21
	.quad	.LCFI22
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI22
	.quad	.LCFI23
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI23
	.quad	.LFE12
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST8:
	.quad	.LFB15
	.quad	.LCFI24
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI24
	.quad	.LCFI25
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI25
	.quad	.LCFI26
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI26
	.quad	.LFE15
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST9:
	.quad	.LFB18
	.quad	.LCFI27
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI27
	.quad	.LCFI28
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI28
	.quad	.LCFI29
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI29
	.quad	.LFE18
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST10:
	.quad	.LFB21
	.quad	.LCFI30
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI30
	.quad	.LCFI31
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI31
	.quad	.LCFI32
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI32
	.quad	.LFE21
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST11:
	.quad	.LFB9
	.quad	.LCFI33
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI33
	.quad	.LCFI34
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI34
	.quad	.LCFI35
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI35
	.quad	.LFE9
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
	.section	.debug_aranges,"",@progbits
	.long	0xec
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.quad	.LFB1
	.quad	.LFE1-.LFB1
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.quad	.LFB5
	.quad	.LFE5-.LFB5
	.quad	.LFB22
	.quad	.LFE22-.LFB22
	.quad	.LFB6
	.quad	.LFE6-.LFB6
	.quad	.LFB7
	.quad	.LFE7-.LFB7
	.quad	.LFB8
	.quad	.LFE8-.LFB8
	.quad	.LFB12
	.quad	.LFE12-.LFB12
	.quad	.LFB15
	.quad	.LFE15-.LFB15
	.quad	.LFB18
	.quad	.LFE18-.LFB18
	.quad	.LFB21
	.quad	.LFE21-.LFB21
	.quad	0
	.quad	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.quad	.Ltext0
	.quad	.Letext0
	.quad	.LFB0
	.quad	.LFE0
	.quad	.LFB1
	.quad	.LFE1
	.quad	.LFB2
	.quad	.LFE2
	.quad	.LFB5
	.quad	.LFE5
	.quad	.LFB22
	.quad	.LFE22
	.quad	.LFB6
	.quad	.LFE6
	.quad	.LFB7
	.quad	.LFE7
	.quad	.LFB8
	.quad	.LFE8
	.quad	.LFB12
	.quad	.LFE12
	.quad	.LFB15
	.quad	.LFE15
	.quad	.LFB18
	.quad	.LFE18
	.quad	.LFB21
	.quad	.LFE21
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF36:
	.string	"Point2d::priTest()"
.LASF2:
	.string	"mumble"
.LASF3:
	.string	"next"
.LASF6:
	.string	"_vptr.Point3d"
.LASF27:
	.string	"argv"
.LASF15:
	.string	"Point2d::Foo()"
.LASF39:
	.string	"__vtbl_ptr_type"
.LASF4:
	.string	"Point3d"
.LASF0:
	.string	"Vertex3d"
.LASF11:
	.string	"float"
.LASF21:
	.string	"Point2d::Point2d()"
.LASF31:
	.string	"/home/boshin/boshin/github/test/linuxdebugging"
.LASF24:
	.string	"Vertex3d::Vertex3d()"
.LASF35:
	.string	"priTest"
.LASF18:
	.string	"this"
.LASF33:
	.string	"Vertex3d::test()"
.LASF10:
	.string	"Point3d::Boo3d()"
.LASF5:
	.string	"_vptr.Vertex"
.LASF17:
	.string	"non_overwrite"
.LASF25:
	.string	"main"
.LASF37:
	.string	"proTest"
.LASF13:
	.string	"_vptr.Point2d"
.LASF20:
	.string	"__vtt_parm"
.LASF32:
	.string	"test"
.LASF38:
	.string	"Point2d::proTest()"
.LASF28:
	.string	"char"
.LASF1:
	.string	"Vertex"
.LASF29:
	.string	"GNU C++ 4.7.4"
.LASF7:
	.string	"BooVer"
.LASF22:
	.string	"Vertex::Vertex()"
.LASF14:
	.string	"Vertex::Foo()"
.LASF9:
	.string	"Vertex::BooVer()"
.LASF8:
	.string	"Boo3d"
.LASF19:
	.string	"__in_chrg"
.LASF26:
	.string	"argc"
.LASF12:
	.string	"Point2d"
.LASF30:
	.string	"main_4_3.cpp"
.LASF23:
	.string	"Point3d::Point3d()"
.LASF34:
	.string	"Point2d::non_overwrite()"
.LASF16:
	.string	"Point2d::Boo()"
	.ident	"GCC: (Ubuntu/Linaro 4.7.4-3ubuntu12) 4.7.4"
	.section	.note.GNU-stack,"",@progbits
