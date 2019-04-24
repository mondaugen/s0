#include "dtypes.h"

unsigned int
s0_dtype_size(s0_dtype_t t)
{
    switch (t) {
        case s0_FLOAT32:
            return sizeof(s0_f32);
    }
    return 0;
}
