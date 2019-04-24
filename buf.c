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
    
s0_err_t 
s0_buf_chk_match(s0_buf_t *a, s0_buf_t *b)
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
    
