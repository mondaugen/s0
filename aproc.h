#ifndef APROC_H
#define APROC_H 

#include "hw.h"
#include "err.h"
#include "buf.h"

/* Stuff audio processors must implement */ 

typedef struct s0_aproc_methods_t s0_aproc_methods_t;

typedef s0_aproc_methods_t * s0_aproc_t;

typedef struct {
    s0_aproc_t _methods;
} s0_aproc_inst_t;

typedef struct {
    double samplerate;
    unsigned int block_n_frames;
    s0_hw_t *hw;
} s0_aproc_init_t;

struct s0_aproc_methods_t {
    /*
    Sets where this instance gets input from on the channel "index". This
    buffer doesn't yet need to be initialized.
    */
    s0_err_t (*set_input_buffer)(s0_aproc_t *instance, unsigned int index, s0_buf_t *buf);
    /*
    Gets where this instance is writing output to on the channel "index". This
    buffer also doesn't yet need to be initialzed.
    */
    s0_buf_t *(*get_output_buffer)(s0_aproc_t *instance, unsigned int index);
    /*
    Carries out the processing that takes values from the input buffers and
    writes them to the input buffers
    */
    s0_err_t (*process)(s0_aproc_t *instance);
    /*
    Initialize this instance. This is typically called after the input
    buffers have been set. More memory can be allocated to help with processing,
    based on the arguments (sample rate, etc.). This data should be freed when
    the following free method is called.
    If the instance requires the input buffers to be initialized for this
    initialize function to work, then it should be initialized beforehand. In
    fact, the nodes in the graph should be initialized in topological order.
    TODO: Say what helper code does this.
    */
    s0_err_t (*initialize)(s0_aproc_t *instance, s0_aproc_init_t *args);
    /*
    Free this instance.
    */
    void (*free)(s0_aproc_t *instance);
};

typedef struct {
    s0_aproc_methods_t *methods;
} s0_aproc_inst_init_t;

typedef struct {} s0_aproc_new_args_t;

void s0_aproc_inst_init(s0_aproc_inst_t *inst, s0_aproc_inst_init_t *init);

/*
They need to implement a method with this signature

s0_aproc_t *
aproc_name_new(s0_aproc_new_args_t *args);

The implementers can subclass s0_aproc_new_args_t as needed.
See the implementations for examples.
*/

#endif /* APROC_H */
