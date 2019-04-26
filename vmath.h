#ifndef VMATH_H
#define VMATH_H 

#include "dtypes.h"

/* vectorized math routines */

/* Add elements */
void s0_vmath_add_s0_f32_vv(s0_f32 *in0, s0_f32 *in1, s0_f32 *out, unsigned int len);
void s0_vmath_add_s0_f32_vc(s0_f32 *in0, s0_f32 in1, s0_f32 *out, unsigned int len);
void s0_vmath_add_s0_f32_cv(s0_f32 in0, s0_f32 *in1, s0_f32 *out, unsigned int len);
void s0_vmath_add_s0_s32_vv(s0_s32 *in0, s0_s32 *in1, s0_s32 *out, unsigned int len);
void s0_vmath_add_s0_s32_vc(s0_s32 *in0, s0_s32 in1, s0_s32 *out, unsigned int len);
void s0_vmath_add_s0_s32_cv(s0_s32 in0, s0_s32 *in1, s0_s32 *out, unsigned int len);

#endif /* VMATH_H */
