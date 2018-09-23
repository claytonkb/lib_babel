// introspect.h
// 

#ifndef INTROSPECT_H
#define INTROSPECT_H

mword *_bs2str(babel_env *be, mword *bs);
void _rbs2str(babel_env *be, mword *bs, mword *result, mword *offset);

mword introspect_calc_buf_length( babel_env *be, mword *bs, mword const_padding, mword nlf_scaling, mword nva_scaling, mword nin_scaling, mword nptr_scaling, mword ntag_scaling);

mword *_bs2gv(babel_env *be, mword *bs);
void _rbs2gv(babel_env *be, mword *bs, mword *result, mword *offset, int initial);

#endif //INTROSPECT_H

// Clayton Bauman 2016

