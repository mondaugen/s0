#include "buf.h"
#include "dtypes.h"
#include "err.h"

#include <stdlib.h>

/* Check if the buffer info values are plausible */
static s0_err_t
chk_buf_info(s0_buf_info_t *info)
{
    /*
    TODO Do we want to fail on some extremely large number of frames or
    channels?
    */
    if (info->n_frames == 0) { return s0_err_BADFRAMES; }
    if (info->n_channels == 0) { return s0_err_BADCHAN; }
    return s0_SUCCESS;
}

/* Assuming the info field of buf has been initialized, allocate the memory for it */
s0_err_t
s0_buf_init(s0_buf_t *buf)
{
    s0_buf_info_t info = buf->info;
    s0_err_t err;
    if ((err = chk_buf_info(&info)) != s0_SUCCESS) { return err; }
    buf->data = calloc(s0_dtype_size(info.dtype),info.n_frames*info.n_channels);
    if (!buf->data) { return s0_err_BADALLOC; }
    return s0_SUCCESS;
}

/* Frees the memory where buf stores the data, buf not buf itself */
void
s0_buf_destroy(s0_buf_t *buf)
{
    if (buf->data) { free(buf->data); }
}

/* Get a pointer to the start of the values in the nth channel. Doesn't check if data is NULL. */
void *
s0_buf_get_channel(const s0_buf_t *b, unsigned int n)
{
    return b->data + s0_dtype_size(b->info.dtype) * n * b->info.n_frames;
}

/*
Get the "frameth" frame in channel "chan". This should only be used for infrequent
querying of buffer values, more frequent manipulations should use a vectorized
approach.
*/
void *
s0_buf_get_value(const s0_buf_t *b, unsigned int chan, unsigned int frame)
{
    return s0_buf_get_channel(b,chan) + s0_dtype_size(b->info.dtype) * frame;
}
    
s0_err_t 
s0_buf_chk_match(const s0_buf_t *a, s0_buf_t *b)
{
    if (a->info.dtype != b->info.dtype) {
        return s0_err_BADTYPE;
    }
    if (a->info.n_channels != b->info.n_channels) {
        return s0_err_BADCHAN;
    }
    if (a->info.n_frames != b->info.n_frames) {
        return s0_err_BADFRAMES;
    }
    return s0_SUCCESS;
}

void
s0_buf_fprint(FILE *stream,const s0_buf_t *b)
{
    unsigned int chan, frame;
    for (chan = 0; chan < b->info.n_channels; chan++) {
        for (frame = 0; frame < b->info.n_frames; frame++) {
            s0_dtype_fprint(stream, b->info.dtype, s0_buf_get_value(b,chan,frame));
            fprintf(stream," ");
        }
        fprintf(stream,"\n");
    }
}
