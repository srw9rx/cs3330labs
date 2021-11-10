//sophia walton, srw9rx, sum_multiple_accum.s
// This is assembly is roughly equivalent to the following C code:
// unsigned short sum_C(long size, unsigned short * a) {
//    unsigned short sum1 = 0;
//    unsigned short sum2 = 0;
//    unsigned short sum3 = 0;
//    unsigned short sum4 = 0;
//    for (int i = 0; i < size; i+=4) {
//        sum1 += a[i];
//        sum2 += a[i+1];      
//        sum3 += a[i+2];
//        sum4 += a[i+3];
//    }
//    sum1 = sum1+sum2;
//    sum4 = sum3+sum4;
//    return sum1+sum4;
//}

// This implementation follows the Linux x86-64 calling convention:
//    %rdi contains the size
//    %rsi contains the pointer a
// and
//    %ax needs to contain the result when the function returns
// in addition, this code uses
//    %rcx to store i

// the '.global' directive indicates to the assembler to make this symbol 
// available to other files.
.global sum_multiple_accum
sum_multiple_accum:
    // set sum (%ax) to 0
    xor %eax, %eax
    xor %r9d, %r9d
    xor %r8d, %r8d
    xor %r10d, %r10d
    // return immediately; special case if size (%rdi) == 0
    test %rdi, %rdi
    je .L_done
    // store i = 0 in rcx
    movq $0, %rcx

// labels starting with '.L' are local to this file
.L_loop:
    // sum (%ax) += a[i]
    addw (%rsi,%rcx,2), %ax 
    // sum (%r8w) += a[i+1]
    addw 2(%rsi, %rcx, 2), %r8w 
    // sum (%r9w) += a[i+2]
    addw 4(%rsi, %rcx, 2), %r9w
    // sum (%r10w) += a[i+3]
    addw 6(%rsi, %rcx, 2), %r10w
    // i += 4
    addq $4, %rcx
    // i < end?
    cmpq %rdi, %rcx
    jl .L_loop
    //combine together the values from the multiple accumulators
    addw %r8w,%ax
    addw %r9w, %r10w
    addw %r10w, %ax
.L_done:
    retq
