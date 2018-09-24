// introspect.h
// 

#ifndef INTROSPECT_H
#define INTROSPECT_H

str introspect_str(babel_env *be, mword *bs);

mword introspect_calc_buf_length( babel_env *be, mword *bs, mword const_padding, mword nlf_scaling, mword nva_scaling, mword nin_scaling, mword nptr_scaling, mword ntag_scaling);

str introspect_gv(babel_env *be, mword *bs);
void introspect_gv_r(babel_env *be, mword *bs, mword *result, mword *offset, int initial);

str introspect_svg(babel_env *be, val8 arr, mword width, mword height);

#endif //INTROSPECT_H

// Clayton Bauman 2018

