#include <stdlib.h>
#include <immintrin.h>  // for future use of SSE

#include "sum.h"
/* sum.h defines function_type. */

/* reference implementation in C */
unsigned short sum_C(long size, unsigned short * a) {
    unsigned short sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += a[i];
    }
    return sum;
}
//c code reference for how the final version should work
unsigned short sum_multiple_accum_C(long size, unsigned short * a) {
    unsigned short sum1 = 0;
    unsigned short sum2 = 0;
    unsigned short sum3 = 0;
    unsigned short sum4 = 0;
    for (int i = 0; i < size; i+=4) {
        sum1 += a[i];
        sum2 += a[i+1];      
        sum3 += a[i+2];
        sum4 += a[i+3];
    }
    sum2 = sum1+sum2;
    sum4 = sum3+sum4;
    return sum2+sum4;
}
/* implementations in assembly */
extern unsigned short sum_clang6_O(long, unsigned short *);
extern unsigned short sum_gcc7_O3(long, unsigned short *);
extern unsigned short sum_simple(long, unsigned short *);
//unroll 2 
extern unsigned short sum_unrolled2(long, unsigned short *);
//unroll 4
extern unsigned short sum_unrolled4(long, unsigned short *);
//multiple accumulators
extern unsigned short sum_multiple_accum(long, unsigned short *);


/* This is the list of functions to test */
function_info functions[] = {
    /* compiled versions from various compilers, each in their own .s file: */ 
    {sum_clang6_O, "sum_clang6_O: simple C compiled with clang 6 -O -mavx2"},
    {sum_gcc7_O3, "sum_gcc7_O3: simple C compiled with GCC7 -O3 -mavx2"}, 

    /* source code for this version is above */
    {sum_C, "sum_C: simple C compiled on this machine with settings in Makefile"},

    /* source code for this version is in sum_simple.s */
    {sum_simple, "sum_simple: simple ASM implementation"},

    // ADD ENTRIES HERE!
    //unroll 2
    {sum_unrolled2, "sum_unrolled2: with unrolling to 2 at once"},

    //unroll 4
    {sum_unrolled4, "sum_unrolled4: with unrolling to 4 at once"},

    //multiple accumulators added
    {sum_multiple_accum, "sum_multiple_accum: with unrolling to 4 at once and multiple accums"},

    //c code for multiple accumulators 
    {sum_multiple_accum_C, "sum_multiple_accum_C: C code for multiple accumulators"}, 

    {NULL, NULL},
};