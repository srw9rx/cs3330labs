//sophia walton, srw9rx
//min_benchmarks.c
#include <stdlib.h>
#include <limits.h>  /* for USHRT_MAX */

#include <immintrin.h>

#include "min.h"
/* reference implementation in C */
short min_C(long size, short * a) {
    short result = SHRT_MAX;
    for (int i = 0; i < size; ++i) {
        if (a[i] < result)
            result = a[i];
    }
    return result;
}
short min_AVX(long size, short * a) {
    __m256i lowest_each = _mm256_loadu_si256((__m256i*) &a[0]); //take inital minimum for first 16 items
    if (size >16) {
        for (int i = 16; i < size; i+=16) {
            __m256i compare = _mm256_loadu_si256((__m256i*) &a[i]); //if there are more items, compare the first set to second set
            lowest_each = _mm256_min_epi16(lowest_each, compare); // save minimum of sets
        }
    }
    unsigned short result[16]; //convert vector into array
    unsigned short min = SHRT_MAX;
    _mm256_storeu_si256((__m256i*) &result, lowest_each);
    for (int i = 0; i < 16; i++) {
        if (result[i] < min)
            min = result[i]; //loop through final 16 to see the minimum val
    }
    return min;
}

/* This is the list of functions to test */
function_info functions[] = {
    {min_C, "C (local)"},
    {min_AVX, "minimum w avx vals"},
    // add entries here!
    {NULL, NULL},
};
