#ifndef DTYPES_H
#define DTYPES_H 

#include <stdio.h>
#include <stdint.h>

/* Datatypes */
typedef enum {
    s0_FLOAT32 = 0,
    s0_INT32,
    s0_N_DTYPES
} s0_dtype_t;

typedef float   s0_f32;
typedef int32_t s0_s32;

unsigned int
s0_dtype_size(s0_dtype_t t);

void
s0_dtype_fprint(FILE *stream, s0_dtype_t dtype, void *dat);

#endif /* DTYPES_H */
