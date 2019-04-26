#include "dtypes.h"

unsigned int
s0_dtype_size(s0_dtype_t t)
{
    switch (t) {
        case s0_FLOAT32:
            return sizeof(s0_f32);
        case s0_INT32:
            return sizeof(s0_s32);
    }
    return 0;
}

void
s0_dtype_fprint(FILE *stream, s0_dtype_t dtype, void *dat)
{
    switch (dtype) {
        case s0_FLOAT32:
            fprintf(stream,"%.2e",*(s0_f32*)dat);
            return;
        case s0_INT32:
            fprintf(stream,"%d",*(s0_s32*)dat);
            return;
    }
}
