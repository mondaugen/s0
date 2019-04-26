#ifndef BUF_BIN_OP_H
#define BUF_BIN_OP_H 

#include "aproc.h"
#include "buf.h"
#include "dtypes.h"

#define S0_BUF_BIN_OP_N_FRAME_COMB 3
#define S0_BUF_BIN_OP_N_CHAN_COMB 3

typedef void (*s0_buf_bin_op_fun)(
const s0_buf_t *in0,
const s0_buf_t *in1,
s0_buf_t *out);

/*
A table of binary operator functions, one for each possible pair of input buffer sizes.
In the notation, m0 and m1 are the number of values in the first and second
buffers resp. and n0 and n1 the number of channels.
Each implemented binary operator must fill this table appropriately.
*/
union s0_buf_bin_op_fun_table {
    s0_buf_bin_op_fun functions[S0_BUF_BIN_OP_N_FRAME_COMB][S0_BUF_BIN_OP_N_CHAN_COMB];
    struct {
        /* assumes in1 and out point to the same memory */
        s0_buf_bin_op_fun m0_1lt_m1_n0_1lt_n1;
        /* assumes in1 and out point to the same memory */
        s0_buf_bin_op_fun m0_1lt_m1_n0_eq_n1;
        /* outer product */
        s0_buf_bin_op_fun m0_1lt_m1_n0_gt1_n1;
        /* assumes in1 and out point to the same memory */
        s0_buf_bin_op_fun m0_eq_m1_n0_1lt_n1;
        /* assumes in0 and out point to the same memory */
        s0_buf_bin_op_fun m0_eq_m1_n0_eq_n1; 
        /* assumes in0 and out point to the same memory */
        s0_buf_bin_op_fun m0_eq_m1_n0_gt1_n1;
        /* outer product */
        s0_buf_bin_op_fun m0_gt1_m1_n0_1lt_n1;
        /* assumes in0 and out point to the same memory */
        s0_buf_bin_op_fun m0_gt1_m1_n0_eq_n1;
        /* assumes in0 and out point to the same memory */
        s0_buf_bin_op_fun m0_gt1_m1_n0_gt1_n1;
    };
};

typedef struct {
    s0_aproc_methods_t methods;
    union s0_buf_bin_op_fun_table fun_table[s0_N_DTYPES];
} s0_buf_bin_op_interface_t;

/*
Generate all the functions for a particular operator and type.
Assumes the following functions have been defined:

// Applies operator on 2 vectors of the same size
s0_vmath_<op>_<type>_vv(type *in0, type *in1, type *out, unsigned int len)
// Apply operator on 1 vector and a constant
s0_vmath_<op>_<type>_vc(type *in0, type in1, type *out, unsigned int len)
s0_vmath_<op>_<type>_cv(type in0, type *in1, type *out, unsigned int len)
*/

#define s0_buf_bin_op_gen_funs(op,type)\
/* Done this way because the channel values might not be contiguous */\
static void op ## _ ## type ## _m0_1lt_m1_n0_1lt_n1(const s0_buf_t *in0, const s0_buf_t *in1, s0_buf_t *out)\
{\
    type in0_val = *(type *)in0->data,\
         *in1_ptr, *out_ptr; \
    unsigned int n_channels = out->info.n_channels,\
                 n_frames = out->info.n_frames;\
    while (n_channels-- > 0) {\
        out_ptr = s0_buf_get_channel(out,n_channels);\
        in1_ptr = s0_buf_get_channel(in1,n_channels);\
        s0_vmath_ ## op ## _ ## type ## _cv(in0_val,in1_ptr,out_ptr,n_frames);\
    }\
}\
static void op ## _ ## type ## _m0_1lt_m1_n0_eq_n1(const s0_buf_t *in0, const s0_buf_t *in1, s0_buf_t *out)\
{\
    type in0_val, *in1_ptr, *out_ptr; \
    unsigned int n_channels = out->info.n_channels,\
                 n_frames = out->info.n_frames;\
    while (n_channels-- > 0) {\
        in0_val = *(type *)s0_buf_get_channel(in0,n_channels);\
        out_ptr = s0_buf_get_channel(out,n_channels);\
        in1_ptr = s0_buf_get_channel(in1,n_channels);\
        s0_vmath_ ## op ## _ ## type ## _cv(in0_val,in1_ptr,out_ptr,n_frames);\
    }\
}\
static void op ## _ ## type ## _m0_1lt_m1_n0_gt1_n1(const s0_buf_t *in0, const s0_buf_t *in1, s0_buf_t *out)\
{\
    type in0_val, *in1_ptr, *out_ptr; \
    unsigned int n_channels = out->info.n_channels,\
                 n_frames = out->info.n_frames;\
    in1_ptr = s0_buf_get_channel(in1,0);\
    while (n_channels-- > 0) {\
        in0_val = *(type *)s0_buf_get_channel(in0,n_channels);\
        out_ptr = s0_buf_get_channel(out,n_channels);\
        s0_vmath_ ## op ## _ ## type ## _cv(in0_val,in1_ptr,out_ptr,n_frames);\
    }\
}\
static void op ## _ ## type ## _m0_eq_m1_n0_1lt_n1(const s0_buf_t *in0, const s0_buf_t *in1, s0_buf_t *out)\
{\
    type *in0_ptr, *in1_ptr, *out_ptr; \
    unsigned int n_channels = out->info.n_channels,\
                 n_frames = out->info.n_frames;\
    in0_ptr = s0_buf_get_channel(in0,0);\
    while (n_channels-- > 0) {\
        in1_ptr = s0_buf_get_channel(in1,n_channels);\
        out_ptr = s0_buf_get_channel(out,n_channels);\
        s0_vmath_ ## op ## _ ## type ## _vv(in0_ptr,in1_ptr,out_ptr,n_frames);\
    }\
}\
static void op ## _ ## type ## _m0_eq_m1_n0_eq_n1(const s0_buf_t *in0, const s0_buf_t *in1, s0_buf_t *out)\
{\
    type *in0_ptr, *in1_ptr, *out_ptr; \
    unsigned int n_channels = out->info.n_channels,\
                 n_frames = out->info.n_frames;\
    while (n_channels-- > 0) {\
        in0_ptr = s0_buf_get_channel(in0,n_channels);\
        in1_ptr = s0_buf_get_channel(in1,n_channels);\
        out_ptr = s0_buf_get_channel(out,n_channels);\
        s0_vmath_ ## op ## _ ## type ## _vv(in0_ptr,in1_ptr,out_ptr,n_frames);\
    }\
}\
static void op ## _ ## type ## _m0_eq_m1_n0_gt1_n1(const s0_buf_t *in0, const s0_buf_t *in1, s0_buf_t *out)\
{\
    type *in0_ptr, *in1_ptr, *out_ptr; \
    unsigned int n_channels = out->info.n_channels,\
                 n_frames = out->info.n_frames;\
    in1_ptr = s0_buf_get_channel(in1,0);\
    while (n_channels-- > 0) {\
        in0_ptr = s0_buf_get_channel(in0,n_channels);\
        out_ptr = s0_buf_get_channel(out,n_channels);\
        s0_vmath_ ## op ## _ ## type ## _vv(in0_ptr,in1_ptr,out_ptr,n_frames);\
    }\
}\
static void op ## _ ## type ## _m0_gt1_m1_n0_1lt_n1(const s0_buf_t *in0, const s0_buf_t *in1, s0_buf_t *out)\
{\
    type *in0_ptr, in1_val, *out_ptr; \
    unsigned int n_channels = out->info.n_channels,\
                 n_frames = out->info.n_frames;\
    in0_ptr = s0_buf_get_channel(in0,0);\
    while (n_channels-- > 0) {\
        in1_val = *(type *)s0_buf_get_channel(in1,n_channels);\
        out_ptr = s0_buf_get_channel(out,n_channels);\
        s0_vmath_ ## op ## _ ## type ## _vc(in0_ptr,in1_val,out_ptr,n_frames);\
    }\
}\
static void op ## _ ## type ## _m0_gt1_m1_n0_eq_n1(const s0_buf_t *in0, const s0_buf_t *in1, s0_buf_t *out)\
{\
    type *in0_ptr, in1_val, *out_ptr; \
    unsigned int n_channels = out->info.n_channels,\
                 n_frames = out->info.n_frames;\
    while (n_channels-- > 0) {\
        in0_ptr = s0_buf_get_channel(in0,n_channels);\
        in1_val = *(type *)s0_buf_get_channel(in1,n_channels);\
        out_ptr = s0_buf_get_channel(out,n_channels);\
        s0_vmath_ ## op ## _ ## type ## _vc(in0_ptr,in1_val,out_ptr,n_frames);\
    }\
}\
static void op ## _ ## type ## _m0_gt1_m1_n0_gt1_n1(const s0_buf_t *in0, const s0_buf_t *in1, s0_buf_t *out)\
{\
    type *in0_ptr, in1_val, *out_ptr; \
    unsigned int n_channels = out->info.n_channels,\
                 n_frames = out->info.n_frames;\
    in1_val = *(type *)s0_buf_get_channel(in1,0);\
    while (n_channels-- > 0) {\
        in0_ptr = s0_buf_get_channel(in0,n_channels);\
        out_ptr = s0_buf_get_channel(out,n_channels);\
        s0_vmath_ ## op ## _ ## type ## _vc(in0_ptr,in1_val,out_ptr,n_frames);\
    }\
}

/* Generate a function table for this operator and type assuming you have called
s0_buf_bin_op_gen_funs to define the functions. */
#define s0_buf_bin_op_gen_fun_table(op,type)\
(union s0_buf_bin_op_fun_table) {\
    .m0_1lt_m1_n0_1lt_n1 = op ## _ ## type ##_m0_1lt_m1_n0_1lt_n1,\
    .m0_1lt_m1_n0_eq_n1 = op ## _ ## type ##_m0_1lt_m1_n0_eq_n1,\
    .m0_1lt_m1_n0_gt1_n1 = op ## _ ## type ##_m0_1lt_m1_n0_gt1_n1,\
    .m0_eq_m1_n0_1lt_n1 = op ## _ ## type ##_m0_eq_m1_n0_1lt_n1,\
    .m0_eq_m1_n0_eq_n1 = op ## _ ## type ##_m0_eq_m1_n0_eq_n1,\
    .m0_eq_m1_n0_gt1_n1 = op ## _ ## type ##_m0_eq_m1_n0_gt1_n1,\
    .m0_gt1_m1_n0_1lt_n1 = op ## _ ## type ##_m0_gt1_m1_n0_1lt_n1,\
    .m0_gt1_m1_n0_eq_n1 = op ## _ ## type ##_m0_gt1_m1_n0_eq_n1,\
    .m0_gt1_m1_n0_gt1_n1 = op ## _ ## type ##_m0_gt1_m1_n0_gt1_n1,\
}

typedef struct buf_bin_op_t buf_bin_op_t;

/* This should only be called by subclassers */
buf_bin_op_t *
s0_buf_bin_op_new(s0_buf_bin_op_interface_t *interface);

#endif /* BUF_BIN_OP_H */
