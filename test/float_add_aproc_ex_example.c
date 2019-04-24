/*
Here we just fill buffers with something and add them using float_add_aproc_ex_t
to show how the machinery works.
*/

#include <string.h>
#include <stdio.h>
#include "aproc.h"
#include "float_add_aproc_ex.h"
#include "buf.h"

s0_f32 in0_vals[] = {1,2,3,4,5,6,7,8,9,0},
       in1_vals[] = {0,9,8,7,6,5,4,3,2,1};

static void
print_vals(s0_f32 *vals, unsigned int nvals)
{
    while (nvals-- > 0) {
        printf("%1.1f\t", *vals++);
    }
    printf("\n");
}

int main (void)
{
    s0_buf_info_t info = {
        .n_frames = 10,
        .n_channels = 1,
        .dtype = s0_FLOAT32
    };
    s0_buf_t in0 = { .info = info },
             in1 = { .info = info },
             *out;
    s0_aproc_t *adder = float_add_aproc_ex_new(NULL);
    /* Normally you'd check the returned error */
    (*adder)->set_input_buffer(adder,0,&in0);
    (*adder)->set_input_buffer(adder,1,&in1);
    out = (*adder)->get_output_buffer(adder,0);
    /* Initialize the buffers. Just showing that the input buffers can be set on
    an aproc instance before they are initialized. This is handy for aprocs like
    in the example that allocate their output buffers based on the size of the
    input buffers.*/
    s0_buf_init(&in0);
    s0_buf_init(&in1);
    /* Put some values in them so its obvious the adder works */
    memcpy(in0.data,in0_vals,sizeof(in0_vals));
    memcpy(in1.data,in1_vals,sizeof(in1_vals));
    /* our example doesn't need this but you would fill this structure as
    follows in an application that needed these parameters. You'd put in the
    samplerate and block_n_frames for your application. */
    s0_aproc_init_t aproc_init = {
        .samplerate = 44100,
        .block_n_frames = 64,
        .hw = NULL /* Would point to some hardware instance */
    };
    /* Normally you'd check the returned error */
    (*adder)->initialize(adder,&aproc_init);
    /* Now it should be okay to process */
    (*adder)->process(adder);
    /* Check the output */
    print_vals(in0.data,in0.info.n_frames);
    print_vals(in1.data,in1.info.n_frames);
    print_vals(out->data,out->info.n_frames);
    (*adder)->free(adder);
    s0_buf_destroy(&in0);
    s0_buf_destroy(&in1);
    return 0;
}
