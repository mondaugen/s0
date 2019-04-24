#include "aproc.h"

void
s0_aproc_inst_init(s0_aproc_inst_t *inst, s0_aproc_inst_init_t *init)
{
    inst->_methods = init->methods;
}
