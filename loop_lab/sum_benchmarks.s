	.file	"sum_benchmarks.c"
	.text
	.p2align 4,,15
	.globl	sum_C
	.type	sum_C, @function
sum_C:
.LFB4741:
	.cfi_startproc
	testq	%rdi, %rdi
	jle	.L4
	leaq	(%rsi,%rdi,2), %rdx
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L3:
	addw	(%rsi), %ax
	addq	$2, %rsi
	cmpq	%rdx, %rsi
	jne	.L3
	rep ret
	.p2align 4,,10
	.p2align 3
.L4:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE4741:
	.size	sum_C, .-sum_C
	.p2align 4,,15
	.globl	sum_multiple_accum_C
	.type	sum_multiple_accum_C, @function
sum_multiple_accum_C:
.LFB4742:
	.cfi_startproc
	testq	%rdi, %rdi
	jle	.L11
	subq	$1, %rdi
	xorl	%eax, %eax
	xorl	%edx, %edx
	shrq	$2, %rdi
	xorl	%ecx, %ecx
	leaq	8(%rsi,%rdi,8), %r8
	xorl	%edi, %edi
	.p2align 4,,10
	.p2align 3
.L10:
	addw	(%rsi), %cx
	addw	2(%rsi), %di
	addq	$8, %rsi
	addw	-4(%rsi), %dx
	addw	-2(%rsi), %ax
	cmpq	%rsi, %r8
	jne	.L10
	addl	%edx, %eax
	addl	%edi, %eax
	addl	%ecx, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L11:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE4742:
	.size	sum_multiple_accum_C, .-sum_multiple_accum_C
	.globl	functions
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"sum_clang6_O: simple C compiled with clang 6 -O -mavx2"
	.align 8
.LC1:
	.string	"sum_gcc7_O3: simple C compiled with GCC7 -O3 -mavx2"
	.align 8
.LC2:
	.string	"sum_C: simple C compiled on this machine with settings in Makefile"
	.align 8
.LC3:
	.string	"sum_simple: simple ASM implementation"
	.align 8
.LC4:
	.string	"sum_unrolled2: with unrolling to 2 at once"
	.align 8
.LC5:
	.string	"sum_unrolled4: with unrolling to 4 at once"
	.align 8
.LC6:
	.string	"sum_multiple_accum: with unrolling to 4 at once and multiple accums"
	.align 8
.LC7:
	.string	"sum_multiple_accum_C: C code for multiple accumulators"
	.data
	.align 32
	.type	functions, @object
	.size	functions, 144
functions:
	.quad	sum_clang6_O
	.quad	.LC0
	.quad	sum_gcc7_O3
	.quad	.LC1
	.quad	sum_C
	.quad	.LC2
	.quad	sum_simple
	.quad	.LC3
	.quad	sum_unrolled2
	.quad	.LC4
	.quad	sum_unrolled4
	.quad	.LC5
	.quad	sum_multiple_accum
	.quad	.LC6
	.quad	sum_multiple_accum_C
	.quad	.LC7
	.quad	0
	.quad	0
	.ident	"GCC: (GNU) 7.1.0"
	.section	.note.GNU-stack,"",@progbits
