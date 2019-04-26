#ifndef BUF_H
#define BUF_H 

#include <stdio.h>
#include "err.h"
#include "dtypes.h"

typedef struct {
    unsigned int n_frames;
    unsigned int n_channels;
    s0_dtype_t dtype;
} s0_buf_info_t;

typedef struct {
    s0_buf_info_t info;
    void *data;
} s0_buf_t;

s0_err_t s0_buf_init(s0_buf_t *buf);
void s0_buf_destroy(s0_buf_t *buf);
void * s0_buf_get_channel(const s0_buf_t *b, unsigned int n);
s0_err_t s0_buf_chk_match(const s0_buf_t *a, s0_buf_t *b);
void * s0_buf_get_value(const s0_buf_t *b, unsigned int chan, unsigned int frame);
void s0_buf_fprint(FILE *stream, const s0_buf_t *b);

#endif /* BUF_H */
