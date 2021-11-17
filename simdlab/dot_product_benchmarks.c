//sophia walton, srw9rx
//dot_product_benchmarks.c -calculate dot product with benchmarks
#include <stdlib.h>

#include <immintrin.h>

#include "dot_product.h"
/* reference implementation in C */
unsigned int dot_product_C(long size, unsigned short * a, unsigned short *b) {
    unsigned int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}
unsigned int dot_product_AVX(long size, unsigned short * a, unsigned short *b) {
    //unsigned int sum = 0;
    __m256i all_dot_sums = _mm256_setzero_si256(); //declare the partial sums as a vector
    for (int i = 0; i < size; i+=8) {
        //initialize row vectors 
        __m128i ainit = _mm_loadu_si128((__m128i*) &a[i]);
        __m128i binit = _mm_loadu_si128((__m128i*) &b[i]);
        //convert to 256 bit
        __m256i avals = _mm256_cvtepu16_epi32(ainit);
        __m256i bvals = _mm256_cvtepu16_epi32(binit);
        //take the multiply of each part
        __m256i partial_dot = _mm256_mullo_epi32(avals, bvals);
        //add the dot products to the accumulator
        all_dot_sums = _mm256_add_epi32(partial_dot, all_dot_sums);
    }
    unsigned int extracted_dot_prod[8]; //declare extraction array
    _mm256_storeu_si256((__m256i*) &extracted_dot_prod, all_dot_sums); //extract
    //return sum of all vector vals
    return extracted_dot_prod[0]+extracted_dot_prod[1]+extracted_dot_prod[2]+extracted_dot_prod[3]+extracted_dot_prod[4]+extracted_dot_prod[5]+extracted_dot_prod[6]+extracted_dot_prod[7];
}
unsigned int dot_product_AVX_ma2(long size, unsigned short * a, unsigned short *b) {
    //initialize sum vectors 
    __m256i all_dot_sums_1 = _mm256_setzero_si256(); //declare the partial sums as a vector
    __m256i all_dot_sums_2 = _mm256_setzero_si256(); //declare the partial sums as a vector
    for (int i = 0; i < size; i+=16) {
        // first part of items
        __m128i ainit1 = _mm_loadu_si128((__m128i*) &a[i]); //init a
        __m128i binit1 = _mm_loadu_si128((__m128i*) &b[i]); // init b

        //convert to 256 bit
        __m256i avals1 = _mm256_cvtepu16_epi32(ainit1); 
        __m256i bvals1 = _mm256_cvtepu16_epi32(binit1);

        __m256i partial_dot1 = _mm256_mullo_epi32(avals1, bvals1); //multiply together values
        all_dot_sums_1 = _mm256_add_epi32(partial_dot1, all_dot_sums_1); //add the vals to suma

        //perform same thing but +8 for unrolling 
        //initialize vectors
        __m128i ainit2 = _mm_loadu_si128((__m128i*) &a[i+8]);
        __m128i binit2 = _mm_loadu_si128((__m128i*) &b[i+8]);

        //convert to 256 bit
        __m256i avals2 = _mm256_cvtepu16_epi32(ainit2);
        __m256i bvals2 = _mm256_cvtepu16_epi32(binit2);

        __m256i partial_dot2 = _mm256_mullo_epi32(avals2, bvals2); //multiply values
        all_dot_sums_2 = _mm256_add_epi32(partial_dot2, all_dot_sums_2); //add to final dot product

    }
    all_dot_sums_1 =  _mm256_add_epi32(all_dot_sums_1, all_dot_sums_2); // add final sums together
    unsigned int extracted_dot_prod[8];
    _mm256_storeu_si256((__m256i*) &extracted_dot_prod, all_dot_sums_1);//store to temporary array
    //add together final vals
    return extracted_dot_prod[0]+extracted_dot_prod[1]+extracted_dot_prod[2]+extracted_dot_prod[3]+extracted_dot_prod[4]+extracted_dot_prod[5]+extracted_dot_prod[6]+extracted_dot_prod[7];
}
unsigned int dot_product_AVX_ma4(long size, unsigned short * a, unsigned short *b) {
    //unsigned int sum = 0;
    __m256i all_dot_sums_1 = _mm256_setzero_si256(); //declare the partial sums as a vector
    __m256i all_dot_sums_2 = _mm256_setzero_si256(); //declare the partial sums as a vector
    __m256i all_dot_sums_3 = _mm256_setzero_si256(); //declare the partial sums as a vector
    __m256i all_dot_sums_4 = _mm256_setzero_si256(); //declare the partial sums as a vector
    for (int i = 0; i < size; i+=32) {
        __m128i ainit1 = _mm_loadu_si128((__m128i*) &a[i]);
        __m128i binit1 = _mm_loadu_si128((__m128i*) &b[i]);
        __m256i avals1 = _mm256_cvtepu16_epi32(ainit1);
        __m256i bvals1 = _mm256_cvtepu16_epi32(binit1);
        __m256i partial_dot1 = _mm256_mullo_epi32(avals1, bvals1);
        all_dot_sums_1 = _mm256_add_epi32(partial_dot1, all_dot_sums_1);

        __m128i ainit2 = _mm_loadu_si128((__m128i*) &a[i+8]);
        __m128i binit2 = _mm_loadu_si128((__m128i*) &b[i+8]);
        __m256i avals2 = _mm256_cvtepu16_epi32(ainit2);
        __m256i bvals2 = _mm256_cvtepu16_epi32(binit2);
        __m256i partial_dot2 = _mm256_mullo_epi32(avals2, bvals2);
        all_dot_sums_2 = _mm256_add_epi32(partial_dot2, all_dot_sums_2);
        if (i+16 < size) {
            __m128i ainit3 = _mm_loadu_si128((__m128i*) &a[i+16]);
            __m128i binit3 = _mm_loadu_si128((__m128i*) &b[i+16]);
            __m256i avals3 = _mm256_cvtepu16_epi32(ainit3);
            __m256i bvals3 = _mm256_cvtepu16_epi32(binit3);
            __m256i partial_dot3 = _mm256_mullo_epi32(avals3, bvals3);
            all_dot_sums_3 = _mm256_add_epi32(partial_dot3, all_dot_sums_3);

            __m128i ainit4 = _mm_loadu_si128((__m128i*) &a[i+24]);
            __m128i binit4 = _mm_loadu_si128((__m128i*) &b[i+24]);
            __m256i avals4 = _mm256_cvtepu16_epi32(ainit4);
            __m256i bvals4 = _mm256_cvtepu16_epi32(binit4);
            __m256i partial_dot4 = _mm256_mullo_epi32(avals4, bvals4);
            all_dot_sums_4 = _mm256_add_epi32(partial_dot4, all_dot_sums_4);
        }

    }
    all_dot_sums_1 =  _mm256_add_epi32(all_dot_sums_1, all_dot_sums_2);
    all_dot_sums_3 =  _mm256_add_epi32(all_dot_sums_3, all_dot_sums_4);
    __m256i all_dot_sums =  _mm256_add_epi32(all_dot_sums_1, all_dot_sums_3);
    unsigned int extracted_dot_prod[8];
    _mm256_storeu_si256((__m256i*) &extracted_dot_prod, all_dot_sums);
    return extracted_dot_prod[0]+extracted_dot_prod[1]+extracted_dot_prod[2]+extracted_dot_prod[3]+extracted_dot_prod[4]+extracted_dot_prod[5]+extracted_dot_prod[6]+extracted_dot_prod[7];
}
// add prototypes here!
extern unsigned int dot_product_gcc7_O3(long size, unsigned short * a, unsigned short *b);

/* This is the list of functions to test */
function_info functions[] = {
    {dot_product_C, "C (local)"},
    {dot_product_AVX, "AVX function"},
    {dot_product_gcc7_O3, "C (compiled with GCC7.2 -O3 -mavx2)"},
    {dot_product_AVX_ma2, "multiple accumulators: 2 w unrolling"},
    {dot_product_AVX_ma4, "multiple accumulators: 4 w unrolling"},
    // add entries here!
    {NULL, NULL},
};
