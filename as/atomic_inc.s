	.section	__TEXT,__text,regular,pure_instructions
	.section	__TEXT,__StaticInit,regular,pure_instructions
	.align	4, 0x90
__GLOBAL__I_counter:
Leh_func_begin1:
	pushq	%rbp
Ltmp0:
	movq	%rsp, %rbp
Ltmp1:
	movl	$1, %eax
	movl	$65535, %ecx
	movl	%eax, %edi
	movl	%ecx, %esi
	callq	__Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	ret
Leh_func_end1:

	.align	4, 0x90
__Z41__static_initialization_and_destruction_0ii:
Leh_func_begin2:
	pushq	%rbp
Ltmp2:
	movq	%rsp, %rbp
Ltmp3:
	subq	$16, %rsp
Ltmp4:
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %eax
	cmpl	$1, %eax
	jne	LBB2_3
	movl	-8(%rbp), %eax
	cmpl	$65535, %eax
	jne	LBB2_3
	leaq	__ZStL8__ioinit(%rip), %rax
	movq	%rax, %rdi
	callq	__ZNSt8ios_base4InitC1Ev
	leaq	___tcf_0(%rip), %rax
	movabsq	$0, %rcx
	movq	___dso_handle@GOTPCREL(%rip), %rdx
	leaq	(%rdx), %rdx
	movq	%rax, %rdi
	movq	%rcx, %rsi
	callq	___cxa_atexit
LBB2_3:
	addq	$16, %rsp
	popq	%rbp
	ret
Leh_func_end2:

	.section	__TEXT,__text,regular,pure_instructions
	.align	4, 0x90
___tcf_0:
Leh_func_begin3:
	pushq	%rbp
Ltmp5:
	movq	%rsp, %rbp
Ltmp6:
	subq	$16, %rsp
Ltmp7:
	movq	%rdi, -8(%rbp)
	leaq	__ZStL8__ioinit(%rip), %rax
	movq	%rax, %rdi
	callq	__ZNSt8ios_base4InitD1Ev
	addq	$16, %rsp
	popq	%rbp
	ret
Leh_func_end3:

	.globl	_main
	.align	4, 0x90
_main:
Leh_func_begin4:
	pushq	%rbp
Ltmp43:
	movq	%rsp, %rbp
Ltmp44:
	subq	$288, %rsp
Ltmp45:
	movl	%edi, %eax
	movq	___stack_chk_guard@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rcx
	movq	%rcx, -8(%rbp)
	movl	%eax, -12(%rbp)
	movq	%rsi, -24(%rbp)
	movq	%rsp, %rax
	movq	%rax, -48(%rbp)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	callq	_atoi
	movl	%eax, %ecx
	movl	%ecx, -68(%rbp)
	movl	-68(%rbp), %ecx
	movl	-68(%rbp), %ecx
	movl	-68(%rbp), %ecx
	movabsq	$8, %rcx
	movl	-68(%rbp), %edx
	movl	-68(%rbp), %edx
	movslq	%edx, %rdx
	imulq	%rcx, %rdx
	movl	%edx, %ecx
	movl	%ecx, %ecx
	leaq	15(%rcx), %rcx
	andq	$-16, %rcx
	movq	%rsp, %rdx
	subq	%rcx, %rdx
	movq	%rdx, %rsp
	movq	%rdx, -64(%rbp)
	movl	$0, -140(%rbp)
	jmp	LBB4_3
LBB4_1:
	movl	-140(%rbp), %eax
	movq	-64(%rbp), %rcx
	movslq	%eax, %rax
	movabsq	$8, %rdx
	imulq	%rdx, %rax
	addq	%rax, %rcx
Ltmp8:
	xorl	%eax, %eax
	leaq	__Z8run_xaddPv(%rip), %rdx
	movq	%rcx, %rdi
	movq	%rax, %rsi
	movq	%rax, %rcx
	callq	_pthread_create
Ltmp9:
	jmp	LBB4_2
LBB4_2:
	movl	-140(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -140(%rbp)
LBB4_3:
	movl	-140(%rbp), %eax
	movl	-68(%rbp), %ecx
	cmpl	%ecx, %eax
	jl	LBB4_1
	movl	$0, -144(%rbp)
	jmp	LBB4_7
LBB4_5:
	movl	-144(%rbp), %eax
	movq	-64(%rbp), %rcx
	movslq	%eax, %rax
	movq	(%rcx,%rax,8), %rax
Ltmp10:
	leaq	-80(%rbp), %rcx
	movq	%rax, %rdi
	movq	%rcx, %rsi
	callq	_pthread_join
Ltmp11:
	jmp	LBB4_6
LBB4_6:
	movl	-144(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -144(%rbp)
LBB4_7:
	movl	-144(%rbp), %eax
	movl	-68(%rbp), %ecx
	cmpl	%ecx, %eax
	jl	LBB4_5
	movq	-96(%rbp), %rax
	movabsq	$1000000000, %rcx
	imulq	%rcx, %rax
	movq	-88(%rbp), %rdx
	addq	%rdx, %rax
	movq	%rax, -120(%rbp)
	movq	-112(%rbp), %rax
	imulq	%rcx, %rax
	movq	-104(%rbp), %rcx
	addq	%rcx, %rax
	movq	%rax, -128(%rbp)
	movq	-128(%rbp), %rax
	movq	-120(%rbp), %rcx
	subq	%rcx, %rax
	movq	%rax, -136(%rbp)
Ltmp12:
	movq	__ZSt4cout@GOTPCREL(%rip), %rax
	leaq	L_.str(%rip), %rcx
	movq	%rax, %rdi
	movq	%rcx, %rsi
	callq	__ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
Ltmp13:
	movq	%rax, -168(%rbp)
	jmp	LBB4_9
LBB4_9:
	movl	-68(%rbp), %eax
Ltmp14:
	movq	-168(%rbp), %rdi
	movl	%eax, %esi
	callq	__ZNSolsEi
Ltmp15:
	movq	%rax, -176(%rbp)
	jmp	LBB4_10
LBB4_10:
Ltmp16:
	movq	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GOTPCREL(%rip), %rax
	movq	-176(%rbp), %rdi
	movq	%rax, %rsi
	callq	__ZNSolsEPFRSoS_E
Ltmp17:
	jmp	LBB4_11
LBB4_11:
Ltmp18:
	movq	__ZSt4cout@GOTPCREL(%rip), %rax
	leaq	L_.str1(%rip), %rcx
	movq	%rax, %rdi
	movq	%rcx, %rsi
	callq	__ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
Ltmp19:
	movq	%rax, -184(%rbp)
	jmp	LBB4_12
LBB4_12:
	movq	-136(%rbp), %rax
Ltmp20:
	movq	-184(%rbp), %rdi
	movq	%rax, %rsi
	callq	__ZNSolsEy
Ltmp21:
	movq	%rax, -192(%rbp)
	jmp	LBB4_13
LBB4_13:
Ltmp22:
	movq	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GOTPCREL(%rip), %rax
	movq	-192(%rbp), %rdi
	movq	%rax, %rsi
	callq	__ZNSolsEPFRSoS_E
Ltmp23:
	jmp	LBB4_14
LBB4_14:
	movq	-136(%rbp), %rax
	movabsq	$1360296554856532783, %rcx
	movq	%rax, -200(%rbp)
	mulq	%rcx
	movq	-200(%rbp), %rax
	subq	%rdx, %rax
	shrq	%rax
	leaq	(%rax,%rdx), %rax
	shrq	$29, %rax
Ltmp24:
	movq	__ZSt4cout@GOTPCREL(%rip), %rcx
	leaq	L_.str2(%rip), %rdx
	movq	%rcx, %rdi
	movq	%rdx, %rsi
	movq	%rax, -208(%rbp)
	callq	__ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
Ltmp25:
	movq	%rax, -216(%rbp)
	jmp	LBB4_15
LBB4_15:
Ltmp26:
	movq	-216(%rbp), %rdi
	movq	-208(%rbp), %rsi
	callq	__ZNSolsEy
Ltmp27:
	movq	%rax, -224(%rbp)
	jmp	LBB4_16
LBB4_16:
Ltmp28:
	movq	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GOTPCREL(%rip), %rax
	movq	-224(%rbp), %rdi
	movq	%rax, %rsi
	callq	__ZNSolsEPFRSoS_E
Ltmp29:
	jmp	LBB4_17
LBB4_17:
	movq	-136(%rbp), %rax
	movabsq	$1000000000000000000, %rcx
	xorl	%edx, %edx
	movq	%rax, -232(%rbp)
	movq	%rcx, %rax
	movq	-232(%rbp), %rcx
	divq	%rcx
Ltmp30:
	movq	__ZSt4cout@GOTPCREL(%rip), %rcx
	leaq	L_.str3(%rip), %rdx
	movq	%rcx, %rdi
	movq	%rdx, %rsi
	movq	%rax, -240(%rbp)
	callq	__ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
Ltmp31:
	movq	%rax, -248(%rbp)
	jmp	LBB4_18
LBB4_18:
Ltmp32:
	movq	-248(%rbp), %rdi
	movq	-240(%rbp), %rsi
	callq	__ZNSolsEy
Ltmp33:
	movq	%rax, -256(%rbp)
	jmp	LBB4_19
LBB4_19:
Ltmp34:
	movq	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GOTPCREL(%rip), %rax
	movq	-256(%rbp), %rdi
	movq	%rax, %rsi
	callq	__ZNSolsEPFRSoS_E
Ltmp35:
	jmp	LBB4_20
LBB4_20:
	movq	_counter(%rip), %rax
Ltmp36:
	movq	__ZSt4cout@GOTPCREL(%rip), %rcx
	leaq	L_.str4(%rip), %rdx
	movq	%rcx, %rdi
	movq	%rdx, %rsi
	movq	%rax, -264(%rbp)
	callq	__ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
Ltmp37:
	movq	%rax, -272(%rbp)
	jmp	LBB4_21
LBB4_21:
Ltmp38:
	movq	-272(%rbp), %rdi
	movq	-264(%rbp), %rsi
	callq	__ZNSolsEy
Ltmp39:
	movq	%rax, -280(%rbp)
	jmp	LBB4_22
LBB4_22:
Ltmp40:
	movq	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GOTPCREL(%rip), %rax
	movq	-280(%rbp), %rdi
	movq	%rax, %rsi
	callq	__ZNSolsEPFRSoS_E
Ltmp41:
	jmp	LBB4_23
LBB4_23:
	movl	$0, -52(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, %rsp
	movl	-52(%rbp), %eax
	movl	%eax, -28(%rbp)
	jmp	LBB4_25
LBB4_24:
	movl	-156(%rbp), %eax
	movl	%eax, -32(%rbp)
	movq	-152(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, %rsp
	movq	-40(%rbp), %rax
	movq	%rax, -152(%rbp)
	movl	-32(%rbp), %eax
	movl	%eax, -156(%rbp)
	jmp	LBB4_29
LBB4_25:
	movl	-28(%rbp), %eax
	movq	___stack_chk_guard@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rcx
	movq	-8(%rbp), %rdx
	cmpq	%rdx, %rcx
	movl	%eax, -284(%rbp)
	jne	LBB4_30
	movl	-284(%rbp), %eax
	movq	%rbp, %rsp
	popq	%rbp
	ret
LBB4_27:
Ltmp42:
	movq	%rax, -152(%rbp)
	movq	-152(%rbp), %rax
	movl	%edx, %eax
	movl	%eax, -156(%rbp)
	jmp	LBB4_24
LBB4_29:
	movq	-152(%rbp), %rax
	movq	%rax, %rdi
	callq	__Unwind_Resume_or_Rethrow
LBB4_30:
	callq	___stack_chk_fail
Leh_func_end4:
	.section	__TEXT,__gcc_except_tab
	.align	2
GCC_except_table4:
Lexception4:
	.byte	255
	.byte	155
	.byte	156
	.space	1
	.byte	3
	.byte	26
Lset0 = Ltmp8-Leh_func_begin4
	.long	Lset0
Lset1 = Ltmp41-Ltmp8
	.long	Lset1
Lset2 = Ltmp42-Leh_func_begin4
	.long	Lset2
	.byte	0
Lset3 = Ltmp41-Leh_func_begin4
	.long	Lset3
Lset4 = Leh_func_end4-Ltmp41
	.long	Lset4
	.long	0
	.byte	0
	.align	2

	.section	__TEXT,__text,regular,pure_instructions
	.globl	__Z8run_xaddPv
	.align	4, 0x90
__Z8run_xaddPv:
Leh_func_begin5:
	pushq	%rbp
Ltmp46:
	movq	%rsp, %rbp
Ltmp47:
	movq	%rdi, -8(%rbp)
	movq	_counter(%rip), %rax
	movq	%rax, -24(%rbp)
	jmp	LBB5_2
LBB5_1:
	mfence
	movl	$1, %eax
	lock
	xadd	%rax, _counter(%rip)
	mfence
	movabsq	$1, %rcx
	addq	%rcx, %rax
	movq	%rax, -24(%rbp)
LBB5_2:
	movq	-24(%rbp), %rax
	cmpq	$499999999, %rax
	jbe	LBB5_1
	movq	-16(%rbp), %rax
	popq	%rbp
	ret
Leh_func_end5:

	.globl	__Z7run_casPv
	.align	4, 0x90
__Z7run_casPv:
Leh_func_begin6:
	pushq	%rbp
Ltmp48:
	movq	%rsp, %rbp
Ltmp49:
	movq	%rdi, -8(%rbp)
	movq	$0, -32(%rbp)
	jmp	LBB6_2
LBB6_1:
	movq	_counter(%rip), %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movabsq	$1, %rcx
	addq	%rcx, %rax
	movq	-32(%rbp), %rcx
	mfence
	movq	%rax, -40(%rbp)
	movq	%rcx, %rax
	movq	-40(%rbp), %rdx
	lock
	cmpxchgq	%rdx,_counter(%rip)
	mfence
	cmpq	%rcx, %rax
	sete	%al
	andb	$1, %al
	cmpb	$0, %al
	setne	%al
	xorb	$1, %al
	andb	$1, %al
	movb	%al, -17(%rbp)
	movb	-17(%rbp), %al
	cmpb	$0, %al
	jne	LBB6_1
LBB6_2:
	movq	-32(%rbp), %rax
	cmpq	$499999999, %rax
	jbe	LBB6_1
	movq	-16(%rbp), %rax
	popq	%rbp
	ret
Leh_func_end6:

	.globl	_counter
.zerofill __DATA,__common,_counter,8,3
.zerofill __DATA,__bss,__ZStL8__ioinit,1,3
	.section	__TEXT,__cstring,cstring_literals
L_.str:
	.asciz	 "threads = "

L_.str1:
	.asciz	 "duration = "

L_.str2:
	.asciz	 "ns per op = "

L_.str3:
	.asciz	 "op/sec = "

L_.str4:
	.asciz	 "counter = "

	.section	__DATA,__mod_init_func,mod_init_funcs
	.align	3
	.quad	__GLOBAL__I_counter
	.section	__TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame0:
Lsection_eh_frame:
Leh_frame_common:
Lset5 = Leh_frame_common_end-Leh_frame_common_begin
	.long	Lset5
Leh_frame_common_begin:
	.long	0
	.byte	1
	.asciz	 "zPLR"
	.byte	1
	.byte	120
	.byte	16
	.byte	7
	.byte	155
	.long	___gxx_personality_v0@GOTPCREL+4
	.byte	16
	.byte	16
	.byte	12
	.byte	7
	.byte	8
	.byte	144
	.byte	1
	.align	3
Leh_frame_common_end:
__GLOBAL__I_counter.eh:
Lset6 = Leh_frame_end1-Leh_frame_begin1
	.long	Lset6
Leh_frame_begin1:
Lset7 = Leh_frame_begin1-Leh_frame_common
	.long	Lset7
Ltmp50:
	.quad	Leh_func_begin1-Ltmp50
Lset8 = Leh_func_end1-Leh_func_begin1
	.quad	Lset8
	.byte	8
	.quad	0
	.byte	4
Lset9 = Ltmp0-Leh_func_begin1
	.long	Lset9
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset10 = Ltmp1-Ltmp0
	.long	Lset10
	.byte	13
	.byte	6
	.align	3
Leh_frame_end1:

__Z41__static_initialization_and_destruction_0ii.eh:
Lset11 = Leh_frame_end2-Leh_frame_begin2
	.long	Lset11
Leh_frame_begin2:
Lset12 = Leh_frame_begin2-Leh_frame_common
	.long	Lset12
Ltmp51:
	.quad	Leh_func_begin2-Ltmp51
Lset13 = Leh_func_end2-Leh_func_begin2
	.quad	Lset13
	.byte	8
	.quad	0
	.byte	4
Lset14 = Ltmp2-Leh_func_begin2
	.long	Lset14
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset15 = Ltmp3-Ltmp2
	.long	Lset15
	.byte	13
	.byte	6
	.align	3
Leh_frame_end2:

___tcf_0.eh:
Lset16 = Leh_frame_end3-Leh_frame_begin3
	.long	Lset16
Leh_frame_begin3:
Lset17 = Leh_frame_begin3-Leh_frame_common
	.long	Lset17
Ltmp52:
	.quad	Leh_func_begin3-Ltmp52
Lset18 = Leh_func_end3-Leh_func_begin3
	.quad	Lset18
	.byte	8
	.quad	0
	.byte	4
Lset19 = Ltmp5-Leh_func_begin3
	.long	Lset19
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset20 = Ltmp6-Ltmp5
	.long	Lset20
	.byte	13
	.byte	6
	.align	3
Leh_frame_end3:

	.globl	_main.eh
_main.eh:
Lset21 = Leh_frame_end4-Leh_frame_begin4
	.long	Lset21
Leh_frame_begin4:
Lset22 = Leh_frame_begin4-Leh_frame_common
	.long	Lset22
Ltmp53:
	.quad	Leh_func_begin4-Ltmp53
Lset23 = Leh_func_end4-Leh_func_begin4
	.quad	Lset23
	.byte	8
Ltmp54:
	.quad	Lexception4-Ltmp54
	.byte	4
Lset24 = Ltmp43-Leh_func_begin4
	.long	Lset24
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset25 = Ltmp44-Ltmp43
	.long	Lset25
	.byte	13
	.byte	6
	.align	3
Leh_frame_end4:

	.globl	__Z8run_xaddPv.eh
__Z8run_xaddPv.eh:
Lset26 = Leh_frame_end5-Leh_frame_begin5
	.long	Lset26
Leh_frame_begin5:
Lset27 = Leh_frame_begin5-Leh_frame_common
	.long	Lset27
Ltmp55:
	.quad	Leh_func_begin5-Ltmp55
Lset28 = Leh_func_end5-Leh_func_begin5
	.quad	Lset28
	.byte	8
	.quad	0
	.byte	4
Lset29 = Ltmp46-Leh_func_begin5
	.long	Lset29
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset30 = Ltmp47-Ltmp46
	.long	Lset30
	.byte	13
	.byte	6
	.align	3
Leh_frame_end5:

	.globl	__Z7run_casPv.eh
__Z7run_casPv.eh:
Lset31 = Leh_frame_end6-Leh_frame_begin6
	.long	Lset31
Leh_frame_begin6:
Lset32 = Leh_frame_begin6-Leh_frame_common
	.long	Lset32
Ltmp56:
	.quad	Leh_func_begin6-Ltmp56
Lset33 = Leh_func_end6-Leh_func_begin6
	.quad	Lset33
	.byte	8
	.quad	0
	.byte	4
Lset34 = Ltmp48-Leh_func_begin6
	.long	Lset34
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset35 = Ltmp49-Ltmp48
	.long	Lset35
	.byte	13
	.byte	6
	.align	3
Leh_frame_end6:


.subsections_via_symbols
