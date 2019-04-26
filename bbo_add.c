#include "buf_bin_op.h"
#include "vmath.h"

s0_buf_bin_op_gen_funs(add,s0_f32);
s0_buf_bin_op_gen_funs(add,s0_s32);

/* Addition binary operation on buffers */
static s0_buf_bin_op_interface_t interface = {
    /* .methods intialized by s0_buf_bin_op_new */
    .fun_table = {
        [s0_FLOAT32] = s0_buf_bin_op_gen_fun_table(add,s0_f32),
        [s0_INT32] = s0_buf_bin_op_gen_fun_table(add,s0_s32),
    }
};

s0_aproc_t *
s0_bbo_add_new(s0_aproc_new_args_t *args)
{
    return (s0_aproc_t*)s0_buf_bin_op_new(&interface);
}
    
