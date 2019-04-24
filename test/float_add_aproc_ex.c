/*
An example implementation of an aproc.  This takes two input buffers of the
same size containing floats and adds them together, putting the result in an
output buffer.  This example is contrived because we actually implement addition
as a particular instance of a binary operator aproc, which handles different
types and sizes. This is just a basic example to show how to implement new aproc
subclasses.
*/

#include <stdlib.h>
#include "s0.h"

typedef struct {
    s0_aproc_inst_t super;
    s0_buf_t *input_bufs[2];
    s0_buf_t output_buf;
} float_add_aproc_ex_t;

static s0_err_t
set_input_buffer(s0_aproc_t *instance_, unsigned int index, s0_buf_t *buf)
{
    /*
    Notice that we don't check any characteristics of buf (e.g., n_channels
    etc.), because it probably hasn't been initialized yet.
    */
    float_add_aproc_ex_t *instance = (float_add_aproc_ex_t*)instance_;
    if (index >= 2) { return s0_err_BADINDEX; }
    instance->input_bufs[index] = buf;
    return s0_SUCCESS;
}

static s0_buf_t *
get_output_buffer(s0_aproc_t *instance_, unsigned int index)
{
    float_add_aproc_ex_t *instance = (float_add_aproc_ex_t*)instance_;
    if (index >= 1) { return NULL; }
    return &instance->output_buf;
}

static s0_err_t
process(s0_aproc_t *instance_)
{
    float_add_aproc_ex_t *instance = (float_add_aproc_ex_t*)instance_;
    unsigned int n = instance->output_buf.info.n_frames;
    s0_f32 *ptr_o = instance->output_buf.data,
           *ptr_i0 = instance->input_bufs[0]->data,
           *ptr_i1 = instance->input_bufs[1]->data;
    while (n-- > 0) {
        *ptr_o++ = *ptr_i0++ + *ptr_i1++;
    }
    return s0_SUCCESS;
}
    
static s0_err_t
initialize(s0_aproc_t *instance_, s0_aproc_init_t *args)
{
    float_add_aproc_ex_t *instance = (float_add_aproc_ex_t*)instance_;
    s0_err_t err = s0_buf_chk_match(instance->input_bufs[0],instance->input_bufs[1]);
    if (err != s0_SUCCESS) { return err; }
    if (instance->input_bufs[0]->info.dtype != s0_FLOAT32) { return s0_err_BADTYPE; }
    instance->output_buf.info = instance->input_bufs[0]->info;
    err = s0_buf_init(&instance->output_buf);
    return err;
}

static void
float_add_aproc_ex_free(s0_aproc_t *instance_)
{
    float_add_aproc_ex_t *instance = (float_add_aproc_ex_t*)instance_;
    s0_buf_destroy(&instance->output_buf);
    free(instance_);
}

s0_aproc_methods_t _methods = {
    .set_input_buffer = set_input_buffer,
    .get_output_buffer = get_output_buffer,
    .process = process,
    .initialize = initialize,
    .free = float_add_aproc_ex_free
};

s0_aproc_t *
float_add_aproc_ex_new(s0_aproc_new_args_t *args_)
{
    /* This doesn't need the initialization args */
    float_add_aproc_ex_t *ret = calloc(1,sizeof(float_add_aproc_ex_t));
    if (!ret) { return NULL; }
    s0_aproc_inst_init((s0_aproc_inst_t *)ret, &(s0_aproc_inst_init_t){.methods = &_methods});
    return (s0_aproc_t *)ret;
}

