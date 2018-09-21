// bstruct.h
// 

#ifndef BSTRUCT_H
#define BSTRUCT_H

// FIXME Move this into babel.h:
typedef mword (*bstruct_op_fn_ptr)(babel_env *be, mword *bs, void *v);

void _rclean(babel_env *be, mword *bs);
void _recurse(babel_env *be, mword *bs, bstruct_op_fn_ptr bfn, void *v);
mword _fn_recurse(babel_env *be, mword *bs, bstruct_op_fn_ptr bfn, void *v);

mword _mu(babel_env *be, mword *bs);
mword _rmu(babel_env *be, mword *bs, void *v);
mword _nar(babel_env *be, mword *bs);
mword _rnar(babel_env *be, mword *bs, void *v);
mword _nlf(babel_env *be, mword *bs);
mword _rnlf(babel_env *be, mword *bs, void *v);
mword _nin(babel_env *be, mword *bs);
mword _rnin(babel_env *be, mword *bs, void *v);
mword _ntag(babel_env *be, mword *bs);
mword _rntag(babel_env *be, mword *bs, void *v);
mword _nva(babel_env *be, mword *bs);
mword _rnva(babel_env *be, mword *bs, void *v);
mword _nptr(babel_env *be, mword *bs);
mword _rnptr(babel_env *be, mword *bs, void *v);

//mword get_offset_from_ptr(babel_env *be, mword *span_array, mword *ptr);
void set_offset_for_ptr(babel_env *be, mword *span_array, mword *ptr, mword *offset_array, mword this_offset);

mword *bstruct_to_array(babel_env *be, mword *bs);
void bstruct_to_array_r(babel_env *be, mword *bs, mword *arr_list, mword *offset);

mword *bstruct_cp(babel_env *be, mword *bs);

mword *bstruct_load(babel_env *be, mword *bs, mword size);
void bstruct_rload(babel_env *be, mword *tree, mword offset);


mword *bstruct_unload(babel_env *be, mword *bs);
mword bstruct_unload_r(
        babel_env   *be,
        mword       *bs, 
        mword       *dest, 
        mword       *dest_offset, 
        mword       *span_array, 
        mword       *offset_array);

mword *bstruct_find(babel_env *be, mword *target_bs, mword *find_bs);


#endif //BSTRUCT_H

// Clayton Bauman 2017

