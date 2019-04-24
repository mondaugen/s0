#include "buf_bin_op.h"

/* Addition binary operation on buffers */
static s0_buf_bin_op_interface_t interface {
    .methods = s0_buf_bin_op_methods,
    .fun_table = {
        [s0_FLOAT32] = (union s0_buf_bin_op_fun_table) {
            .m0_1lt_m1_n0_1lt_n1 = add_f32_m0_1lt_m1_n0_1lt_n1,
            .m0_1lt_m1_n0_eq_n1 = add_f32_m0_1lt_m1_n0_eq_n1,
            .m0_1lt_m1_n0_gt1_n1 = add_f32_m0_1lt_m1_n0_gt1_n1,
            .m0_eq_m1_n0_1lt_n1 = add_f32_m0_eq_m1_n0_1lt_n1,
            .m0_eq_m1_n0_eq_n1 = add_f32_m0_eq_m1_n0_eq_n1;,
            .m0_eq_m1_n0_gt1_n1 = add_f32_m0_eq_m1_n0_gt1_n1,
            .m0_gt1_m1_n0_1lt_n1 = add_f32_m0_gt1_m1_n0_1lt_n1,
            .m0_gt1_m1_n0_eq_n1 = add_f32_m0_gt1_m1_n0_eq_n1,
            .m0_gt1_m1_n0_gt1_n1 = add_f32_m0_gt1_m1_n0_gt1_n1,
        },
        [s0_INT32] = (union s0_buf_bin_op_fun_table) {
            .m0_1lt_m1_n0_1lt_n1 = add_s32_m0_1lt_m1_n0_1lt_n1,
            .m0_1lt_m1_n0_eq_n1 = add_s32_m0_1lt_m1_n0_eq_n1,
            .m0_1lt_m1_n0_gt1_n1 = add_s32_m0_1lt_m1_n0_gt1_n1,
            .m0_eq_m1_n0_1lt_n1 = add_s32_m0_eq_m1_n0_1lt_n1,
            .m0_eq_m1_n0_eq_n1 = add_s32_m0_eq_m1_n0_eq_n1;,
            .m0_eq_m1_n0_gt1_n1 = add_s32_m0_eq_m1_n0_gt1_n1,
            .m0_gt1_m1_n0_1lt_n1 = add_s32_m0_gt1_m1_n0_1lt_n1,
            .m0_gt1_m1_n0_eq_n1 = add_s32_m0_gt1_m1_n0_eq_n1,
            .m0_gt1_m1_n0_gt1_n1 = add_s32_m0_gt1_m1_n0_gt1_n1,
        },
    }
};

