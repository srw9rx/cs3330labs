	.file	"dot_product_benchmarks.c"
	.text
	.globl	dot_product_C
	.type	dot_product_C, @function
dot_product_C:
.LFB4741:
	.cfi_startproc
	testq	%rdi, %rdi
	jle	.L4
	movl	$0, %ecx
	movl	$0, %eax
.L3:
	movslq	%ecx, %r9
	movzwl	(%rsi,%r9,2), %r8d
	movzwl	(%rdx,%r9,2), %r9d
	imull	%r9d, %r8d
	addl	%r8d, %eax
	addl	$1, %ecx
	movslq	%ecx, %r8
	cmpq	%rdi, %r8
	jl	.L3
	ret
.L4:
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE4741:
	.size	dot_product_C, .-dot_product_C
	.globl	dot_product_AVX
	.type	dot_product_AVX, @function
dot_product_AVX:
.LFB4742:
	.cfi_startproc
	testq	%rdi, %rdi
	jle	.L9
	movl	$0, %eax
	vpxor	%xmm1, %xmm1, %xmm1
.L8:
	movslq	%eax, %rcx
	vpmovzxwd	(%rsi,%rcx,2), %ymm2
	vpmovzxwd	(%rdx,%rcx,2), %ymm0
	vpmulld	%ymm2, %ymm0, %ymm0
	vpaddd	%ymm1, %ymm0, %ymm1
	addl	$8, %eax
	movslq	%eax, %rcx
	cmpq	%rdi, %rcx
	jl	.L8
.L7:
	vmovd	%xmm1, %eax
	vpextrd	$1, %xmm1, %edx
	addl	%edx, %eax
	vpextrd	$2, %xmm1, %edx
	addl	%edx, %eax
	vpextrd	$3, %xmm1, %edx
	addl	%edx, %eax
	vextracti128	$0x1, %ymm1, %xmm1
	vmovd	%xmm1, %edx
	addl	%edx, %eax
	vpextrd	$1, %xmm1, %edx
	addl	%edx, %eax
	vpextrd	$2, %xmm1, %edx
	addl	%eax, %edx
	vpextrd	$3, %xmm1, %eax
	addl	%edx, %eax
	ret
.L9:
	vpxor	%xmm1, %xmm1, %xmm1
	jmp	.L7
	.cfi_endproc
.LFE4742:
	.size	dot_product_AVX, .-dot_product_AVX
	.globl	dot_product_AVX_ma2
	.type	dot_product_AVX_ma2, @function
dot_product_AVX_ma2:
.LFB4743:
	.cfi_startproc
	testq	%rdi, %rdi
	jle	.L14
	movl	$0, %eax
	vpxor	%xmm2, %xmm2, %xmm2
	vmovdqa	%ymm2, %ymm0
.L13:
	movslq	%eax, %rcx
	leaq	(%rcx,%rcx), %r8
	vpmovzxwd	(%rsi,%rcx,2), %ymm1
	vpmovzxwd	(%rdx,%rcx,2), %ymm3
	vpmulld	%ymm3, %ymm1, %ymm1
	vpaddd	%ymm0, %ymm1, %ymm0
	vpmovzxwd	16(%rsi,%r8), %ymm3
	vpmovzxwd	16(%rdx,%r8), %ymm1
	vpmulld	%ymm3, %ymm1, %ymm1
	vpaddd	%ymm2, %ymm1, %ymm2
	addl	$16, %eax
	movslq	%eax, %rcx
	cmpq	%rdi, %rcx
	jl	.L13
.L12:
	vpaddd	%ymm2, %ymm0, %ymm0
	vmovd	%xmm0, %eax
	vpextrd	$1, %xmm0, %edx
	addl	%edx, %eax
	vpextrd	$2, %xmm0, %edx
	addl	%edx, %eax
	vpextrd	$3, %xmm0, %edx
	addl	%edx, %eax
	vextracti128	$0x1, %ymm0, %xmm0
	vmovd	%xmm0, %edx
	addl	%edx, %eax
	vpextrd	$1, %xmm0, %edx
	addl	%edx, %eax
	vpextrd	$2, %xmm0, %edx
	addl	%eax, %edx
	vpextrd	$3, %xmm0, %eax
	addl	%edx, %eax
	ret
.L14:
	vpxor	%xmm2, %xmm2, %xmm2
	vmovdqa	%ymm2, %ymm0
	jmp	.L12
	.cfi_endproc
.LFE4743:
	.size	dot_product_AVX_ma2, .-dot_product_AVX_ma2
	.globl	dot_product_AVX_ma4
	.type	dot_product_AVX_ma4, @function
dot_product_AVX_ma4:
.LFB4744:
	.cfi_startproc
	testq	%rdi, %rdi
	jle	.L20
	movl	$0, %eax
	vpxor	%xmm5, %xmm5, %xmm5
	vmovdqa	%ymm5, %ymm4
	vmovdqa	%ymm5, %ymm2
	vmovdqa	%ymm5, %ymm0
	jmp	.L19
.L18:
	addl	$32, %eax
	movslq	%eax, %rcx
	cmpq	%rdi, %rcx
	jge	.L17
.L19:
	movslq	%eax, %r8
	leaq	(%r8,%r8), %rcx
	vpmovzxwd	(%rsi,%r8,2), %ymm3
	vpmovzxwd	(%rdx,%r8,2), %ymm1
	vpmulld	%ymm3, %ymm1, %ymm1
	vpaddd	%ymm0, %ymm1, %ymm0
	vpmovzxwd	16(%rsi,%rcx), %ymm3
	vpmovzxwd	16(%rdx,%rcx), %ymm1
	vpmulld	%ymm3, %ymm1, %ymm1
	vpaddd	%ymm2, %ymm1, %ymm2
	leal	16(%rax), %r8d
	movslq	%r8d, %r8
	cmpq	%rdi, %r8
	jge	.L18
	vpmovzxwd	32(%rsi,%rcx), %ymm1
	vpmovzxwd	32(%rdx,%rcx), %ymm3
	vpmulld	%ymm3, %ymm1, %ymm1
	vpaddd	%ymm4, %ymm1, %ymm4
	vpmovzxwd	48(%rsi,%rcx), %ymm1
	vpmovzxwd	48(%rdx,%rcx), %ymm3
	vpmulld	%ymm3, %ymm1, %ymm1
	vpaddd	%ymm5, %ymm1, %ymm5
	jmp	.L18
.L20:
	vpxor	%xmm5, %xmm5, %xmm5
	vmovdqa	%ymm5, %ymm4
	vmovdqa	%ymm5, %ymm2
	vmovdqa	%ymm5, %ymm0
.L17:
	vpaddd	%ymm2, %ymm0, %ymm0
	vpaddd	%ymm5, %ymm4, %ymm4
	vpaddd	%ymm4, %ymm0, %ymm0
	vmovd	%xmm0, %eax
	vpextrd	$1, %xmm0, %edx
	addl	%edx, %eax
	vpextrd	$2, %xmm0, %edx
	addl	%edx, %eax
	vpextrd	$3, %xmm0, %edx
	addl	%edx, %eax
	vextracti128	$0x1, %ymm0, %xmm0
	vmovd	%xmm0, %edx
	addl	%edx, %eax
	vpextrd	$1, %xmm0, %edx
	addl	%edx, %eax
	vpextrd	$2, %xmm0, %edx
	addl	%eax, %edx
	vpextrd	$3, %xmm0, %eax
	addl	%edx, %eax
	ret
	.cfi_endproc
.LFE4744:
	.size	dot_product_AVX_ma4, .-dot_product_AVX_ma4
	.globl	functions
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"C (local)"
.LC1:
	.string	"AVX function"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"C (compiled with GCC7.2 -O3 -mavx2)"
	.align 8
.LC3:
	.string	"multiple accumulators: 2 w unrolling"
	.align 8
.LC4:
	.string	"multiple accumulators: 4 w unrolling"
	.data
	.align 32
	.type	functions, @object
	.size	functions, 96
functions:
	.quad	dot_product_C
	.quad	.LC0
	.quad	dot_product_AVX
	.quad	.LC1
	.quad	dot_product_gcc7_O3
	.quad	.LC2
	.quad	dot_product_AVX_ma2
	.quad	.LC3
	.quad	dot_product_AVX_ma4
	.quad	.LC4
	.quad	0
	.quad	0
	.ident	"GCC: (GNU) 7.1.0"
	.section	.note.GNU-stack,"",@progbits
