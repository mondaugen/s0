/*
Binary operators on buffers. These take two buffers as input and give a single
buffer as output.

Examples of binary operators

+, -, /, *, %, &, |, ^, >, <, >=, <=, ==, 

To keep the number of possibilities down, both buffers must have the same type

first buffer has m0 values, n0 channels
second buffer has m1 values, n1 channels
9 possibilities:

a 1< b means a == 1 and a < b

Note on diagrams: all the values of a channel form a column.

m0 1<  m1, n0 1< n1
    |_| x |_|_|_|
          |_|_|_|
          |_|_|_|

m0 ==  m1, n0 1< n1
    |_| x |_|_|_|
    |_|   |_|_|_|
    |_|   |_|_|_|

m0 >1  m1, n0 1< n1
    first buffer has many values and the second has 1, first buffer has one
    channel and second has many, so the outer product is formed
    |_| x |_|_| -> |_|_|
    |_|            |_|_|
    |_|            |_|_|

m0 1<  m1, n0 == n1
    |_|_|_| x |_|_|_|
              |_|_|_|
              |_|_|_|

m0 ==  m1, n0 == n1
    -> operator applied to each pair of elements
    -> also in this case, the output is put in the right buffer, which means no
    new buffer is made for the output buffer
    |_|_| x |_|_|
    |_|_|   |_|_|
    |_|_|   |_|_|

m0 >1  m1, n0 == n1
    same as the opposite configuration for commutative operators
    |_|_|_| x |_|_|_| 
    |_|_|_|   
    |_|_|_|   

m0 1<  m1,  n0 >1 n1
    same as the opposite configuration for commutative operators
    |_|_| x |_| -> |_|_|
            |_|    |_|_|
            |_|    |_|_|

m0 ==  m1,  n0 >1 n1
    same as the opposite configuration for commutative operators
    |_|_|_| x |_|
    |_|_|_|   |_|
    |_|_|_|   |_|

m0 >1  m1,  n0 >1 n0
    same as the opposite configuration for commutative operators
    |_|_|_| x |_|
    |_|_|_|   
    |_|_|_|   
*/

#include "aproc.h"

typedef struct {
    s0_aproc_inst_t super;
    s0_buf_t *input_bufs[2];
    s0_buf_t *output_buf;
    /*
    This aux_output_buf is used only if the number of values and channels of
    the input buffers don't match. In that case, this buffer is allocated to
    store the result.
    */
    s0_buf_t aux_output_buf;
    /* The function that carries out the operation. */
    s0_buf_bin_op_fun op_fun;
} buf_bin_op_t;

static s0_err_t
set_input_buffer(s0_aproc_t *instance_, unsigned int index, s0_buf_t *buf)
{
    buf_bin_op_t *instance = (buf_bin_op_t*)instance_;
    if (index >= 2) { return s0_err_BADINDEX; }
    instance->input_bufs[index] = buf;
    return s0_SUCCESS;
}

static s0_buf_t *
get_output_buffer(s0_aproc_t *instance_, unsigned int index)
{
    buf_bin_op_t *instance = (buf_bin_op_t*)instance_;
    if (index >= 1) { return NULL; }
    return instance->output_buf;
}

static s0_err_t
process(s0_aproc_t *instance_)
{
    buf_bin_op_t *instance = (buf_bin_op_t*)instance_;
    void *ptr_o = instance->output_buf->data,
         *ptr_i0 = instance->input_bufs[0]->data,
         *ptr_i1 = instance->input_bufs[1]->data;
    unsigned int n_frames = instance->output_buf->n_frames,
                 n_channels = instance->output_buf->n_channels;
    instance->op_fun(ptr_i0,ptr_i1,ptr_o,n_frames,n_channels);
    return s0_SUCCESS;
}

/* Looks up the function table index based on the input arguments */
static int
nval_func_selector (unsigned int n0, unsigned int n1)
{
    return 1 - (2 - (n0 == 1)) * (n0 < n1) - (2 - (n1 == 1)) * (n0 > n1);
}

static int
is_outer_product(s0_buf_bin_op_fun op_fun, union s0_buf_bin_op_fun_table *fun_table)
{
    return (op_fun == fun_table->m0_1lt_m1_n0_gt1_n1) || (op_fun == fun_table->m0_gt1_m1_n0_1lt_n1);
}

/*
assumes is_outer_product has already determined if this should be an in-place
operation or not, this simply returns the index of the in-place buffer
*/
static unsigned int
inplace_buf(s0_buf_bin_op_fun op_fun, union s0_buf_bin_op_fun_table *fun_table)
{
    /* assumes in1 and out point to the same memory */
    if (op_fun == fun_table->m0_1lt_m1_n0_1lt_n1) { return 1; }
    /* assumes in1 and out point to the same memory */
    if (op_fun == fun_table->m0_1lt_m1_n0_eq_n1) { return 1; }
    /* assumes in1 and out point to the same memory */
    if (op_fun == fun_table->m0_eq_m1_n0_1lt_n1) { return 1; }
    /* assumes in0 and out point to the same memory */
    if (op_fun == fun_table->m0_eq_m1_n0_eq_n1) { return 0; } 
    /* assumes in0 and out point to the same memory */
    if (op_fun == fun_table->m0_eq_m1_n0_gt1_n1) { return 0; }
    /* assumes in0 and out point to the same memory */
    if (op_fun == fun_table->m0_gt1_m1_n0_eq_n1) { return 0; }
    /* assumes in0 and out point to the same memory */
    if (op_fun == fun_table->m0_gt1_m1_n0_gt1_n1) { return 0; }
}
    
static s0_err_t
initialize(s0_aproc_t *instance_, s0_aproc_init_t *args)
{
    buf_bin_op_t *instance = (buf_bin_op_t*)instance_;
    s0_buf_t *in0_buf = instance->input_bufs[0],
             *in1_buf = instance->input_bufs[1];
    s0_dtype_t dtype;
    if (in0_buf->info.dtype != in1_buf->info.dtype) {
        /* types must match */
        return s0_err_BADTYPE;
    }
    dtype = in0_buf->info.dtype;
    int nframes_fun_idx = nval_func_selector(
            in0_buf->info.n_frames,
            in1_buf->info.n_frames),
        nchans_fun_idx = nval_func_selector(
            in0_buf->info.n_channels,
            in1_buf->input_bufs[1]->info.n_channels);
    if ((nchans_fun_idx < 0)
        || (nchans_fun_idx >= S0_BUF_BIN_OP_N_CHAN_COMB)) {
        return s0_err_BADCHANCOMB;
    }
    if ((nframes_fun_idx < 0)
        || (nframes_fun_idx >= S0_BUF_BIN_OP_N_FRAME_COMB)) {
        return s0_err_BADFRAMECOMB;
    }
    /* Look up the operator function based on the sizes of the input buffers */
    union s0_buf_bin_op_fun_table *fun_table =
        &(*(s0_buf_bin_op_interface_t**)instance)->fun_table[dtype];
    instance->op_fun = fun_table->functions[nframes_fun_idx][nchans_fun_idx];
    if (!instance->op_fun) 
        /* Operator not implemented for these input types */
        return s0_err_NOTIMP;
    }
    /* Allocate an output buffer if an outer product is being performed */
    if (is_outer_product(instance->op_fun,fun_table)) {
        instance->aux_output_buf.info = (s0_buf_info_t) {
            .n_frames = MAX(in0_buf->info.n_frames,in1_buf->info.n_frames),
            .n_channels = MAX(in0_buf->info.n_channels,in1_buf->info.n_channels),
            .dtype = in0_buf->info.dtype
        };
        s0_err_t err = s0_buf_init(&instance->aux_output_buf);
        if (err != s0_SUCCESS) { return err; }
        instance->output_buf = &instance->aux_output_buf;
    } else {
        instance->output_buf = instance->input_bufs[inplace_buf(instance->op_fun,fun_table)];
    }
    return s0_SUCCESS;
}

void free_inst(s0_aproc_t *instance)
{
    buf_bin_op_t *instance = (buf_bin_op_t*)instance_;
    s0_buf_destroy(instance->aux_output_buf);
    free(instance);
}

s0_aproc_methods_t s0_buf_bin_op_methods = {
    .set_input_buffer = set_input_buffer,
    .get_output_buffer = get_output_buffer,
    .process = process,
    .initialize = initialize,
    .free = free_inst
};

