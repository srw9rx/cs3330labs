#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <immintrin.h>

/* 
 * Please fill in the following team struct 
 */
who_t who = {
    "bomb252",           /* Scoreboard name */

    "Sophia Walton",      /* First member full name */
    "srw9rx@virginia.edu",     /* First member email address */
};

/*** UTILITY FUNCTIONS ***/

/* You are free to use these utility functions, or write your own versions
 * of them. */

/* A struct used to compute averaged pixel value */
typedef struct {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
    unsigned short alpha;
    unsigned short num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = sum->alpha = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    //unsigned short pixels[4] = {(unsigned short)p.red, (unsigned short)p.green, (unsigned short)p.blue, (unsigned short)p.alpha};
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->alpha += (int) p.alpha;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    current_pixel->alpha = (unsigned short) (sum.alpha/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(int jj=max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	for(int ii=max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	    accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
 
    return current_pixel;
}



/******************************************************
 * Your different versions of the smooth go here
 ******************************************************/

/* 
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    for (int i = 0; i < dim; i++)
	for (int j = 0; j < dim; j++)
            dst[RIDX(i,j, dim)] = avg(dim, i, j, src);
}
/* 
 * smooth - Your current working version of smooth
 *          Our supplied version simply calls naive_smooth
 */
char another_smooth_descr[] = "another_smooth: Another version of smooth";
void another_smooth(int dim, pixel *src, pixel *dst) 
{
    naive_smooth(dim, src, dst);
}

char step3_descr[] = "step3: add vector for each pixel";
void step3(int dim, pixel *src, pixel *dst) {
    //unsigned short sum[4] = {0,0,0,0};
    for (int i = 1; i < (dim-1); i++) {
        for (int j = 1; j < (dim-1); j++) {
            pixel current_pixel;
            //__m256i pixelsums = _mm256_setzero_si256(); //declare the partial sums as a vector
            //__m128i pixelsums = _mm_loadu_si128((__m128i*) &sum);
            __m128i pixel1 = _mm_setr_epi8(src[RIDX(i-1, j-1, dim)].red,src[RIDX(i-1, j-1, dim)].green,src[RIDX(i-1, j-1, dim)].blue,src[RIDX(i-1, j-1, dim)].alpha,src[RIDX(i-1, j, dim)].red,src[RIDX(i-1, j, dim)].green,src[RIDX(i-1, j, dim)].blue,src[RIDX(i-1, j, dim)].alpha, src[RIDX(i-1, j+1, dim)].red, src[RIDX(i-1, j+1, dim)].green,src[RIDX(i-1, j+1, dim)].blue,src[RIDX(i-1, j+1, dim)].alpha,src[RIDX(i, j-1, dim)].red,src[RIDX(i, j-1, dim)].green,src[RIDX(i, j-1, dim)].blue,src[RIDX(i, j-1, dim)].alpha);
            __m128i pixel2 = _mm_setr_epi8(src[RIDX(i, j+1, dim)].red,src[RIDX(i, j+1, dim)].green,src[RIDX(i, j+1, dim)].blue,src[RIDX(i, j+1, dim)].alpha,src[RIDX(i, j, dim)].red,src[RIDX(i, j, dim)].green,src[RIDX(i, j, dim)].blue,src[RIDX(i, j, dim)].alpha,src[RIDX(i+1, j+1, dim)].red,src[RIDX(i+1, j+1, dim)].green,src[RIDX(i+1, j+1, dim)].blue,src[RIDX(i+1, j+1, dim)].alpha,src[RIDX(i+1, j-1, dim)].red,src[RIDX(i+1, j-1, dim)].green,src[RIDX(i+1, j-1, dim)].blue,src[RIDX(i+1, j-1, dim)].alpha);
            __m256i add1 = _mm256_cvtepu8_epi16(pixel1);
            __m256i add2 = _mm256_cvtepu8_epi16(pixel2);
            add2 = _mm256_add_epi16(add2, add1);
            //pixelsums = _mm256_add_epi16(pixelsums, add2);


            unsigned short extracted_sum[16]; //declare extraction array
            _mm256_storeu_si256((__m256i* ) &extracted_sum, add2); //extract
            extracted_sum[0] = extracted_sum[0]+extracted_sum[4]+extracted_sum[8]+extracted_sum[12]+src[RIDX(i+1, j, dim)].red;
            extracted_sum[1] = extracted_sum[1]+extracted_sum[5]+extracted_sum[9]+extracted_sum[13]+src[RIDX(i+1, j, dim)].green;
            extracted_sum[2] = extracted_sum[2]+extracted_sum[6]+extracted_sum[10]+extracted_sum[14]+src[RIDX(i+1, j, dim)].blue;
            extracted_sum[3] = extracted_sum[3]+extracted_sum[7]+extracted_sum[11]+extracted_sum[15]+src[RIDX(i+1, j, dim)].alpha;
            current_pixel = src[RIDX(i, j, dim)];
            current_pixel.red = (extracted_sum[0]/9);
            current_pixel.green = (extracted_sum[1]/9);
            current_pixel.blue = (extracted_sum[2]/9);
            current_pixel.alpha = (extracted_sum[3]/9);
            dst[RIDX(i, j, dim)] = current_pixel;
        }
    }
    //deal with leftovers
    for (int j = 0; j<dim; ++j) {
        dst[RIDX(0,j, dim)] = avg(dim, 0, j, src); 
        dst[RIDX(dim-1,j, dim)] = avg(dim, dim-1, j, src);
        dst[RIDX(j,0, dim)] = avg(dim, j, 0, src); 
        dst[RIDX(j,dim-1, dim)] = avg(dim, j, dim-1, src);         
    }
}
char step1_descr[] = "step2: unroll";
void step1(int dim, pixel *src, pixel *dst) {
    //unsigned short sum[4] = {0,0,0,0};
    for (int i = 1; i < (dim-1); i++) {
        for (int j = 1; j < (dim-1); j++) {
            pixel_sum sum;
            pixel current_pixel;
            initialize_pixel_sum(&sum);
            accumulate_sum(&sum, src[RIDX(i-1, j-1, dim)]);
            accumulate_sum(&sum, src[RIDX(i-1, j, dim)]);
            accumulate_sum(&sum, src[RIDX(i-1, j+1, dim)]);
            accumulate_sum(&sum, src[RIDX(i+1, j-1, dim)]);
            accumulate_sum(&sum, src[RIDX(i+1, j, dim)]);
            accumulate_sum(&sum, src[RIDX(i+1, j+1, dim)]);
            accumulate_sum(&sum, src[RIDX(i, j+1, dim)]);
            accumulate_sum(&sum, src[RIDX(i, j-1, dim)]);
            accumulate_sum(&sum, src[RIDX(i, j, dim)]);

            current_pixel.red = (unsigned short) (sum.red/9);
            current_pixel.green = (unsigned short) (sum.green/9);
            current_pixel.blue = (unsigned short) (sum.blue/9);
            current_pixel.alpha = (unsigned short) (sum.alpha/9);
            dst[RIDX(i, j, dim)] = current_pixel;
        }
    }
    //still have to deal with edge cases
    for (int j = 0; j<dim; ++j) {
        dst[RIDX(0,j, dim)] = avg(dim, 0, j, src); 
        dst[RIDX(dim-1,j, dim)] = avg(dim, dim-1, j, src);
        dst[RIDX(j,0, dim)] = avg(dim, j, 0, src); 
        dst[RIDX(j,dim-1, dim)] = avg(dim, j, dim-1, src);         
    }
}
char step4a_descr[] = "step4a: add 3 at a time";
void step4a(int dim, pixel *src, pixel *dst) {
    //unsigned short sum[4] = {0,0,0,0};
    for (int i = 1; i < (dim-1); i++) {
        for (int j = 1; j < (dim-1); j+=4) {
            //__m256i pixelsums = _mm256_setzero_si256(); //declare the partial sums as a vector
            __m128i p1 = _mm_loadu_si128((__m128i*) &src[RIDX(i-1, j-1, dim)]);
            __m128i p2 = _mm_loadu_si128((__m128i*) &src[RIDX(i-1, j, dim)]);
            __m128i p3 = _mm_loadu_si128((__m128i*) &src[RIDX(i-1, j+1, dim)]);
            __m128i p4 = _mm_loadu_si128((__m128i*) &src[RIDX(i, j-1, dim)]);
            __m128i p5 = _mm_loadu_si128((__m128i*) &src[RIDX(i, j+1, dim)]);
            __m128i p6 = _mm_loadu_si128((__m128i*) &src[RIDX(i, j, dim)]);
            __m128i p7 = _mm_loadu_si128((__m128i*) &src[RIDX(i+1, j+1, dim)]);
            __m128i p8 = _mm_loadu_si128((__m128i*) &src[RIDX(i+1, j-1, dim)]);
            __m128i p9 = _mm_loadu_si128((__m128i*) &src[RIDX(i+1, j, dim)]);
            __m256i pi1 = _mm256_cvtepu8_epi16(p1);
            __m256i pi2 = _mm256_cvtepu8_epi16(p2);
            __m256i pi3 = _mm256_cvtepu8_epi16(p3);
            __m256i pi4 = _mm256_cvtepu8_epi16(p4);
            __m256i pi5 = _mm256_cvtepu8_epi16(p5);
            __m256i pi6 = _mm256_cvtepu8_epi16(p6);
            __m256i pi7 = _mm256_cvtepu8_epi16(p7);
            __m256i pi8 = _mm256_cvtepu8_epi16(p8);
            __m256i pi9 = _mm256_cvtepu8_epi16(p9);
            __m256i add1 = _mm256_add_epi16(pi1, pi2);
            __m256i add2 = _mm256_add_epi16(pi3, pi4);
            __m256i add3 = _mm256_add_epi16(pi5, pi6);
            __m256i add4 = _mm256_add_epi16(pi7, pi8);
            add1 = _mm256_add_epi16(add1, pi9);
            add2 = _mm256_add_epi16(add2, add3);
            add1 = _mm256_add_epi16(add4, add1);
            add1 = _mm256_add_epi16(add1, add2);

            //pixelsums = _mm256_add_epi16(pixelsums, add2);


            unsigned short extracted_sum[16]; //declare extraction array
            _mm256_storeu_si256((__m256i* ) &extracted_sum, add1); //extract
            pixel a;
            a.red = (extracted_sum[0]*7282) >>16;
            a.green = (extracted_sum[1]*7282) >>16;
            a.blue = (extracted_sum[2]*7282) >>16;
            a.alpha = (extracted_sum[3]*7282) >>16;
            dst[RIDX(i, j, dim)] = a;

            pixel b;
            b.red = (extracted_sum[4]*7282) >>16;
            b.green = (extracted_sum[5]*7282) >>16;
            b.blue = (extracted_sum[6]*7282) >>16;
            b.alpha = (extracted_sum[7]*7282) >>16;
            dst[RIDX(i, j+1, dim)] = b;

            pixel c;
            c.red = (extracted_sum[8]*7282) >>16;
            c.green = (extracted_sum[9]*7282) >>16;
            c.blue = (extracted_sum[10]*7282) >>16;
            c.alpha = (extracted_sum[11]*7282) >>16;
            dst[RIDX(i, j+2, dim)] = c;

            pixel d;
            d.red = (extracted_sum[12] * 7282) >>16;
            d.green = (extracted_sum[13] *7282) >>16;
            d.blue = (extracted_sum[14]*7282) >>16;
            d.alpha = (extracted_sum[15]*7282) >>16;
            dst[RIDX(i, j+3, dim)] = d;
        }
    }

    //deal with leftovers
    for (int j = 0; j<dim; ++j) {
        dst[RIDX(0,j, dim)] = avg(dim, 0, j, src); 
        dst[RIDX(dim-1,j, dim)] = avg(dim, dim-1, j, src);
        dst[RIDX(j,0, dim)] = avg(dim, j, 0, src); 
        dst[RIDX(j,dim-1, dim)] = avg(dim, j, dim-1, src);         
    }
}
char step4ab_descr[] = "step4a and b: add 3 at a time and use vector division";
void step4ab(int dim, pixel *src, pixel *dst) {
    //unsigned short sum[4] = {0,0,0,0};
    for (int i = 1; i < (dim-1); i++) {
        for (int j = 1; j < (dim-1); j+=4) {
            //__m256i pixelsums = _mm256_setzero_si256(); //declare the partial sums as a vector
            __m128i p1 = _mm_loadu_si128((__m128i*) &src[RIDX(i-1, j-1, dim)]);
            __m128i p2 = _mm_loadu_si128((__m128i*) &src[RIDX(i-1, j, dim)]);
            __m128i p3 = _mm_loadu_si128((__m128i*) &src[RIDX(i-1, j+1, dim)]);
            __m128i p4 = _mm_loadu_si128((__m128i*) &src[RIDX(i, j-1, dim)]);
            __m128i p5 = _mm_loadu_si128((__m128i*) &src[RIDX(i, j+1, dim)]);
            __m128i p6 = _mm_loadu_si128((__m128i*) &src[RIDX(i, j, dim)]);
            __m128i p7 = _mm_loadu_si128((__m128i*) &src[RIDX(i+1, j+1, dim)]);
            __m128i p8 = _mm_loadu_si128((__m128i*) &src[RIDX(i+1, j-1, dim)]);
            __m128i p9 = _mm_loadu_si128((__m128i*) &src[RIDX(i+1, j, dim)]);
            __m256i pi1 = _mm256_cvtepu8_epi16(p1);
            __m256i pi2 = _mm256_cvtepu8_epi16(p2);
            __m256i pi3 = _mm256_cvtepu8_epi16(p3);
            __m256i pi4 = _mm256_cvtepu8_epi16(p4);
            __m256i pi5 = _mm256_cvtepu8_epi16(p5);
            __m256i pi6 = _mm256_cvtepu8_epi16(p6);
            __m256i pi7 = _mm256_cvtepu8_epi16(p7);
            __m256i pi8 = _mm256_cvtepu8_epi16(p8);
            __m256i pi9 = _mm256_cvtepu8_epi16(p9);
            __m256i add1 = _mm256_add_epi16(pi1, pi2);
            __m256i add2 = _mm256_add_epi16(pi3, pi4);
            __m256i add3 = _mm256_add_epi16(pi5, pi6);
            __m256i add4 = _mm256_add_epi16(pi7, pi8);
            add1 = _mm256_add_epi16(add1, pi9);
            add2 = _mm256_add_epi16(add2, add3);
            add1 = _mm256_add_epi16(add4, add1);
            add1 = _mm256_add_epi16(add1, add2);

            __m256i all7282 = _mm256_set1_epi16(7282);
            __m256i dividedvals = _mm256_mulhi_epi16(add1, all7282);
            __m256i reverse = _mm256_permute2x128_si256(dividedvals,dividedvals,0x21);
            __m256i div256 = _mm256_packus_epi16(dividedvals, reverse);
            __m128i first = _mm256_extracti128_si256(div256, 0);


        
            //pixelsums = _mm256_add_epi16(pixelsums, add2);


            unsigned char extracted_sum[16]; //declare extraction array
            _mm_storeu_si128((__m128i* ) &extracted_sum, first); //extract
            pixel a;
            a.red = (extracted_sum[0]);
            a.green = (extracted_sum[1]);
            a.blue = (extracted_sum[2]);
            a.alpha = (extracted_sum[3]);
            dst[RIDX(i, j, dim)] = a;

            pixel b;
            b.red = (extracted_sum[4]);
            b.green = (extracted_sum[5]);
            b.blue = (extracted_sum[6]);
            b.alpha = (extracted_sum[7]);
            dst[RIDX(i, j+1, dim)] = b;

            pixel c;
            c.red = (extracted_sum[8]);
            c.green = (extracted_sum[9]);
            c.blue = (extracted_sum[10]);
            c.alpha = (extracted_sum[11]);
            dst[RIDX(i, j+2, dim)] = c;

            pixel d;
            d.red = (extracted_sum[12]);
            d.green = (extracted_sum[13]);
            d.blue = (extracted_sum[14]);
            d.alpha = (extracted_sum[15]);
            dst[RIDX(i, j+3, dim)] = d;
        }
    }

    //deal with leftovers
    for (int j = 0; j<dim; ++j) {
        dst[RIDX(0,j, dim)] = avg(dim, 0, j, src); 
        dst[RIDX(dim-1,j, dim)] = avg(dim, dim-1, j, src);
        dst[RIDX(j,0, dim)] = avg(dim, j, 0, src); 
        dst[RIDX(j,dim-1, dim)] = avg(dim, j, dim-1, src);         
    }
}

/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth function by calling the add_smooth_function() for
 *     each test function. When you run the benchmark program, it will
 *     test and report the performance of each registered test
 *     function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&another_smooth, another_smooth_descr);
    add_smooth_function(&step1, step1_descr);
    add_smooth_function(&step3, step3_descr);
    add_smooth_function(&step4a, step4a_descr);
    add_smooth_function(&step4ab, step4ab_descr);
}
