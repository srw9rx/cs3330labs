#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following struct with your name and the name you'd like to appear on the scoreboard
 */
who_t who = {
    "suoerman",           /* Scoreboard name */

    "sophia walton",   /* Full name */
    "srw9rx@virginia.edu",  /* Email address */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    for (int i = 0; i < dim; i++)
	for (int j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}
/* 
 * rotate - Your current working version of rotate
 *          Our supplied version simply calls naive_rotate
 */
char another_rotate_descr[] = "another_rotate: Another version of rotate";
void another_rotate(int dim, pixel *src, pixel *dst) 
{
    naive_rotate(dim, src, dst);
}
char rotate_unroll4_block8_descr[] = "rotate_unrolling: implementation with unrolling 4 + cache blocking by 8";
void rotate_unroll4_block8(int dim, pixel *src, pixel *dst) {
    for (int i = 0; i<dim; i+=8){
        for (int j = 0; j<dim; j+=4) {
            for (int ii = 0; ii < 8; ii++) {
                dst[RIDX(dim-1-j, i+ii, dim)] = src[RIDX(i+ii, j, dim)];
                dst[RIDX(dim-1-(j+1), i+ii, dim)] = src[RIDX(i+ii, (j+1), dim)];
                dst[RIDX(dim-1-(j+2), i+ii, dim)] = src[RIDX(i+ii, (j+2), dim)];
                dst[RIDX(dim-1-(j+3), i+ii, dim)] = src[RIDX(i+ii, (j+3), dim)];
                //dst[RIDX(dim-1-(j+4), i+ii, dim)] = src[RIDX(i+ii, (j+4), dim)];
                //dst[RIDX(dim-1-(j+5), i+ii, dim)] = src[RIDX(i+ii, (j+5), dim)];
                //dst[RIDX(dim-1-(j+6), i+ii, dim)] = src[RIDX(i+ii, (j+6), dim)];
                //dst[RIDX(dim-1-(j+7), i+ii, dim)] = src[RIDX(i+ii, (j+7), dim)];
            }
        }
    }
}
char prev_no_alias_descr[] = "rotate_unrolling remove alias";
void prev_no_alias(int dim, pixel *src, pixel *dst) {
    for (int i = 0; i<dim; i+=8){
        for (int j = 0; j<dim; j+=4) {
            for (int ii = 0; ii < 8; ii++) {
                pixel a = src[RIDX(i+ii, j, dim)];
                pixel b = src[RIDX(i+ii, (j+1), dim)];
                pixel c = src[RIDX(i+ii, (j+2), dim)];
                pixel d = src[RIDX(i+ii, (j+3), dim)];
                dst[RIDX(dim-1-j, i+ii, dim)] = a;
                dst[RIDX(dim-1-(j+1), i+ii, dim)] = b;
                dst[RIDX(dim-1-(j+2), i+ii, dim)] = c;
                dst[RIDX(dim-1-(j+3), i+ii, dim)] = d;
            }
        }
    }
}
char rotate_unroll_inlining_descr[] = "rotate_unrolling with inlining";
void rotate_unroll_inlining(int dim, pixel *src, pixel *dst) {
    rotate_unroll4_block8(dim, src, dst);
}
/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate function by calling the add_rotate_function() for
 *     each test function. When you run the benchmark program, it will
 *     test and report the performance of each registered test
 *     function.  
 *********************************************************************/

void register_rotate_functions() {
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&another_rotate, another_rotate_descr);
    add_rotate_function(&rotate_unroll4_block8, rotate_unroll4_block8_descr);
    add_rotate_function(&rotate_unroll_inlining, rotate_unroll_inlining_descr);
    add_rotate_function(&prev_no_alias, prev_no_alias_descr);
}
