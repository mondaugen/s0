#ifndef BUF_BIN_OP_H
#define BUF_BIN_OP_H 

#include "dtypes.h"

#define S0_BUF_BIN_OP_N_FRAME_COMB 3
#define S0_BUF_BIN_OP_N_CHAN_COMB 3

typedef void (*s0_buf_bin_op_fun)(
const void *in0,
const void *in1,
void *out,
unsigned int n_frames,
unsigned n_channels);

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
All the binary operators use the same aproc methods but a different fun_table.
So this is to initialize the methods of the particular binary operator's
interface (the fun_table is initialized elsewhere).  Alternatively, each
instance could point to its own table, and then you wouldn't have to copy the
interface methods to each binary operator's interface, but most likely we'll
have more instances than operators, so it is more memory efficient to do it this
way. Also conceptually the fun_table is a bunch of class methods and not instance
methods (the instance method is the method chosen when the sizes of the buffers
are known).
*/
extern s0_aproc_methods_t s0_buf_bin_op_methods;

#endif /* BUF_BIN_OP_H */
