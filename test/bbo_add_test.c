/*
Here we just fill buffers with something and add them using float_add_aproc_ex_t
to show how the machinery works.
*/

#include <string.h>
#include <stdio.h>
#include "s0.h"

s0_f32 in0_vals[] = {1,2,3,4,5,6,7,8,9,0},
       in1_vals[] = {0,9,8,7,6,5,4,3,2,1};

int test_1 (void)
{
    s0_buf_info_t info = {
        .n_frames = 10,
        .n_channels = 1,
        .dtype = s0_FLOAT32
    };
    s0_buf_t in0 = { .info = info },
             in1 = { .info = info },
             *out;
    s0_aproc_t *adder = s0_bbo_add_new(NULL);
    /* Normally you'd check the returned error */
    (*adder)->set_input_buffer(adder,0,&in0);
    (*adder)->set_input_buffer(adder,1,&in1);
    /* Initialize the buffers. Just showing that the input buffers can be set on
    an aproc instance before they are initialized. This is handy for aprocs like
    in the example that allocate their output buffers based on the size of the
    input buffers.*/
    s0_buf_init(&in0);
    s0_buf_init(&in1);
    /* Put some values in them so its obvious the adder works */
    memcpy(in0.data,in0_vals,sizeof(in0_vals));
    memcpy(in1.data,in1_vals,sizeof(in1_vals));
    s0_aproc_init_t aproc_init = {
        .samplerate = 44100,
        .block_n_frames = 64,
        .hw = NULL /* Would point to some hardware instance */
    };
    /* Normally you'd check the returned error */
    (*adder)->initialize(adder,&aproc_init);
    /* Now you can get the output buffer */
    out = (*adder)->get_output_buffer(adder,0);
    /* Now it should be okay to process */
    (*adder)->process(adder);

    /*
    Check the output
    in0 will be the same as out because they point to the same place as the
    implementation doesn't make a new buffer unless it needs to.
    */
    printf("test_1\n");
    printf("in0\n");
    s0_buf_fprint(stdout,&in0);
    printf("in1\n");
    s0_buf_fprint(stdout,&in1);
    printf("out\n");
    s0_buf_fprint(stdout,out);

    (*adder)->free(adder);
    s0_buf_destroy(&in0);
    s0_buf_destroy(&in1);
    return 0;
}

int test_2 (void)
{
    s0_buf_info_t info1 = {
        .n_frames = 10,
        .n_channels = 1,
        .dtype = s0_FLOAT32
    };
    /*
    TODO If channels cease to be packed contiguously, a memcpy of the raw
    data to fill the channels won't work.
    */
    s0_buf_info_t info2 = {
        .n_frames = 1,
        .n_channels = 10,
        .dtype = s0_FLOAT32
    };
    s0_buf_t in0 = { .info = info1 },
             in1 = { .info = info2 },
             *out;
    s0_aproc_t *adder = s0_bbo_add_new(NULL);
    /* Normally you'd check the returned error */
    (*adder)->set_input_buffer(adder,0,&in0);
    (*adder)->set_input_buffer(adder,1,&in1);
    /* Initialize the buffers. Just showing that the input buffers can be set on
    an aproc instance before they are initialized. This is handy for aprocs like
    in the example that allocate their output buffers based on the size of the
    input buffers.*/
    s0_buf_init(&in0);
    s0_buf_init(&in1);
    /* Put some values in them so its obvious the adder works */
    memcpy(in0.data,in0_vals,sizeof(in0_vals));
    memcpy(in1.data,in1_vals,sizeof(in1_vals));
    s0_aproc_init_t aproc_init = {
        .samplerate = 44100,
        .block_n_frames = 64,
        .hw = NULL /* Would point to some hardware instance */
    };
    /* Normally you'd check the returned error */
    (*adder)->initialize(adder,&aproc_init);
    /* Now you can get the output buffer */
    out = (*adder)->get_output_buffer(adder,0);
    /* Now it should be okay to process */
    (*adder)->process(adder);
    /* Check the output */

    printf("test_1\n");
    printf("in0\n");
    s0_buf_fprint(stdout,&in0);
    printf("in1\n");
    s0_buf_fprint(stdout,&in1);
    printf("out\n");
    s0_buf_fprint(stdout,out);

    (*adder)->free(adder);
    s0_buf_destroy(&in0);
    s0_buf_destroy(&in1);
    return 0;
}

int tester (void *x, void *y, s0_buf_info_t info1, s0_buf_info_t info2, const char *name)
{
    s0_buf_t in0 = { .info = info1 },
             in1 = { .info = info2 },
             *out;
    s0_aproc_t *adder = s0_bbo_add_new(NULL);
    /* Normally you'd check the returned error */
    (*adder)->set_input_buffer(adder,0,&in0);
    (*adder)->set_input_buffer(adder,1,&in1);
    /* Initialize the buffers. Just showing that the input buffers can be set on
    an aproc instance before they are initialized. This is handy for aprocs like
    in the example that allocate their output buffers based on the size of the
    input buffers.*/
    s0_buf_init(&in0);
    s0_buf_init(&in1);
    /* Put some values in them so it's obvious the adder works */
    memcpy(in0.data,x,s0_dtype_size(info1.dtype)*info1.n_frames*info1.n_channels);
    memcpy(in1.data,y,s0_dtype_size(info2.dtype)*info2.n_frames*info2.n_channels);
    s0_aproc_init_t aproc_init = {
        .samplerate = 44100,
        .block_n_frames = 64,
        .hw = NULL /* Would point to some hardware instance */
    };
    puts(name);
    /* Normally you'd check the returned error */
    s0_err_t err = (*adder)->initialize(adder,&aproc_init);
    if (err != s0_SUCCESS) {
        printf("error intializing");
        return -1;
    }
    /* Now you can get the output buffer */
    out = (*adder)->get_output_buffer(adder,0);

    printf("in0\n");
    s0_buf_fprint(stdout,&in0);
    printf("in1\n");
    s0_buf_fprint(stdout,&in1);

    /* Now it should be okay to process */
    err = (*adder)->process(adder);
    if (err != s0_SUCCESS) {
        printf("error processing");
        return -1;
    }
    /* Check the output */

    printf("out\n");
    s0_buf_fprint(stdout,out);

    (*adder)->free(adder);
    s0_buf_destroy(&in0);
    s0_buf_destroy(&in1);
    return 0;
}

int test_3(void)
{

    s0_f32 x[] = {11.},
           y[] = {1,3,2,4,5,7,6,8,7,9};
    s0_buf_info_t info1 = {
        .n_frames = 1,
        .n_channels = 1,
        .dtype = s0_FLOAT32
    };
    /*
    TODO If channels cease to be packed contiguously, a memcpy of the raw
    data to fill the channels won't work.
    */
    s0_buf_info_t info2 = {
        .n_frames = 2,
        .n_channels = 5,
        .dtype = s0_FLOAT32
    };
    return tester (x, y, info1, info2,"test3");
}

int test_4(void)
{

    s0_f32 x[] = {11,12,13,14,15},
           y[] = {1,3,2,4,5,7,6,8,7,9};
    s0_buf_info_t info1 = {
        .n_frames = 1,
        .n_channels = 5,
        .dtype = s0_FLOAT32
    };
    /*
    TODO If channels cease to be packed contiguously, a memcpy of the raw
    data to fill the channels won't work.
    */
    s0_buf_info_t info2 = {
        .n_frames = 2,
        .n_channels = 5,
        .dtype = s0_FLOAT32
    };
    return tester (x, y, info1, info2,"test4");
}

int test_5(void)
{

    s0_f32 x[] = {11,12,13,14,15},
           y[] = {1,3,2,4,5,7,6,8,7,9};
    s0_buf_info_t info1 = {
        .n_frames = 1,
        .n_channels = 5,
        .dtype = s0_FLOAT32
    };
    /*
    TODO If channels cease to be packed contiguously, a memcpy of the raw
    data to fill the channels won't work.
    */
    s0_buf_info_t info2 = {
        .n_frames = 10,
        .n_channels = 1,
        .dtype = s0_FLOAT32
    };
    return tester (x, y, info1, info2,"test5");
}

int test_6(void)
{

    s0_f32 x[] = {11,12,13,14,15},
           y[] = {1,3,2,4,5,7,6,8,7,9};
    s0_buf_info_t info1 = {
        .n_frames = 5,
        .n_channels = 1,
        .dtype = s0_FLOAT32
    };
    /*
    TODO If channels cease to be packed contiguously, a memcpy of the raw
    data to fill the channels won't work.
    */
    s0_buf_info_t info2 = {
        .n_frames = 5,
        .n_channels = 2,
        .dtype = s0_FLOAT32
    };
    return tester (x, y, info1, info2,"test6");
}

int test_7(void)
{

    s0_f32 x[] = {11,12,13,14,15,-11,-12,-13,-14,-15},
           y[] = {1,3,2,4,5,7,6,8,7,9};
    s0_buf_info_t info1 = {
        .n_frames = 5,
        .n_channels = 2,
        .dtype = s0_FLOAT32
    };
    /*
    TODO If channels cease to be packed contiguously, a memcpy of the raw
    data to fill the channels won't work.
    */
    s0_buf_info_t info2 = {
        .n_frames = 5,
        .n_channels = 2,
        .dtype = s0_FLOAT32
    };
    return tester (x, y, info1, info2,"test7");
}

int test_8(void)
{

    s0_f32 x[] = {11,12,13,14,15},
           y[] = {1,3,2,4,5,7,6,8,7,9};
    s0_buf_info_t info1 = {
        .n_frames = 5,
        .n_channels = 2,
        .dtype = s0_FLOAT32
    };
    /*
    TODO If channels cease to be packed contiguously, a memcpy of the raw
    data to fill the channels won't work.
    */
    s0_buf_info_t info2 = {
        .n_frames = 5,
        .n_channels = 1,
        .dtype = s0_FLOAT32
    };
    return tester (y, x, info1, info2,"test8");
}

int test_9(void)
{

    s0_f32 x[] = {11,12,13,14,15},
           y[] = {1,3,2,4,5,7,6,8,7,9};
    s0_buf_info_t info1 = {
        .n_frames = 1,
        .n_channels = 5,
        .dtype = s0_FLOAT32
    };
    /*
    TODO If channels cease to be packed contiguously, a memcpy of the raw
    data to fill the channels won't work.
    */
    s0_buf_info_t info2 = {
        .n_frames = 2,
        .n_channels = 5,
        .dtype = s0_FLOAT32
    };
    return tester (y, x, info2, info1,"test9");
}

int test_10(void)
{

    s0_f32 x[] = {11.},
           y[] = {1,3,2,4,5,7,6,8,7,9};
    s0_buf_info_t info1 = {
        .n_frames = 1,
        .n_channels = 1,
        .dtype = s0_FLOAT32
    };
    /*
    TODO If channels cease to be packed contiguously, a memcpy of the raw
    data to fill the channels won't work.
    */
    s0_buf_info_t info2 = {
        .n_frames = 2,
        .n_channels = 5,
        .dtype = s0_FLOAT32
    };
    return tester (y, x, info2, info1,"test10");
}

int test_11(void)
{

    s0_s32 x[] = {11,12,13,14,15},
           y[] = {1,3,2,4,5,7,6,8,7,9};
    s0_buf_info_t info1 = {
        .n_frames = 1,
        .n_channels = 5,
        .dtype = s0_INT32
    };
    /*
    TODO If channels cease to be packed contiguously, a memcpy of the raw
    data to fill the channels won't work.
    */
    s0_buf_info_t info2 = {
        .n_frames = 10,
        .n_channels = 1,
        .dtype = s0_INT32
    };
    return tester (x, y, info1, info2,"test11");
}


int main (void)
{
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    test_8();
    test_9();
    test_10();
    test_11();
    return 0;
}
