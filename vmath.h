#ifndef VMATH_H
#define VMATH_H 

#include "dtypes.h"

/* math routines, generally vectorized */

/* Set all elements to a constant value */
void
s0m_set_f32(s0_f32 c, s0_f32 *dest, unsigned int len);

/* Add elements */
void
s0m_add_f32(const s0_f32 *src_a, const s0_f32 *src_b, s0_f32 *dest, unsigned int len);

#endif /* VMATH_H */
