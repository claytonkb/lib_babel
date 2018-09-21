// array.h
// 

#ifndef ARRAY_H
#define ARRAY_H

#define ARRAY_SORT_TYPE_LEAF     0
#define ARRAY_SORT_TYPE_NON_LEAF 1

#define ARRAY_SEARCH_NOT_FOUND NEG_ONE      // ARRAY_SEARCH_NOT_FOUND#
#define ARRAY_LINEAR_THRESH 2               // ARRAY_LINEAR_THRESH#

#define array8_aligned(x) ((x) % BITS_PER_BYTE == 0)

mword *array_slice(babel_env *be, mword *array, mword start, mword end);
mword *array_th(babel_env *be, mword *bs, mword entry);

mword array8_read(mword *array, mword offset);
void array8_write(mword *array, mword offset, mword value);
mword *array8_th(babel_env *be, mword *val_array, mword entry8);

mword array8_size(babel_env *be, mword *string);
mword array8_dec_align(babel_env *be, mword alignment_word);
mword array8_enc_align(babel_env *be, mword size8);
mword array8_mword_size(babel_env *be, mword size8);

mword array1_read(mword *array, mword offset);
void array1_write(mword *array, mword offset, mword value);
mword *array1_th(babel_env *be, mword *val_array, mword entry1);

mword array1_size(babel_env *be, mword *string);
mword array1_dec_align(babel_env *be, mword alignment_word);
mword array1_enc_align(babel_env *be, mword size1);
mword array1_mword_size(babel_env *be, mword size1);

mword *array_mwords_to_bytes(babel_env *be, mword *array);
mword *array_bytes_to_mwords(babel_env *be, mword *array8);
mword *array_bits_to_mwords(babel_env *be, mword *array1);
mword *array_mwords_to_bits(babel_env *be, mword *array);

mword *array8_cat(babel_env *be, mword *left, mword *right);
mword *array_cat(babel_env *be, mword *left, mword *right);
mword *array1_cat(babel_env *be, mword *left, mword *right);

int array_cmp_lex(babel_env *be, mword *left, mword *right, access_size asize);
int array_ncmp(babel_env *be, mword *left, mword left_offset, mword *right, mword length, access_size asize);

//int array_cmp_lex(mword *left, mword *right);
//int array8_cmp_lex(babel_env *be, mword *left, mword *right);

#define array_eq_num(l, r)  (array_cmp_num(l, r) == 0) // array_eq_num#
#define array_lt_num(l, r)  (array_cmp_num(l, r) <  0) // array_lt_num#
#define array_gt_num(l, r)  (array_cmp_num(l, r) >  0) // array_gt_num#

#define array_eq(pyr, l, r)  (array_cmp_lex(pyr, l, r, MWORD_ASIZE) == 0) // array_eq#
#define array8_eq(pyr, l, r) (array_cmp_lex(pyr, l, r, BYTE_ASIZE)  == 0) // array8_eq#

#define array_lt(pyr, l, r)  (array_cmp_lex(pyr, l, r, MWORD_ASIZE) <  0) // array_lt#
#define array8_lt(pyr, l, r) (array_cmp_lex(pyr, l, r, BYTE_ASIZE)  <  0) // array8_lt#

#define array_gt(pyr, l, r)  (array_cmp_lex(pyr, l, r, MWORD_ASIZE) >  0) // array_gt#
#define array8_gt(pyr, l, r) (array_cmp_lex(pyr, l, r, BYTE_ASIZE)  >  0) // array8_gt#

int array_cmp_alpha(babel_env *be, mword *left, mword *right, access_size asize);
//int array_cmp(mword *left, mword *right);
int array_cmp_num(mword *left, mword *right);
int array_cmp_num_signed(mword *left, mword *right);
int array_cmp_num_range(mword *left, mword *left_end, mword *right, mword *right_end);

//#define array_cmp(x, y) array_cmp_lex(x, y) // array_cmp#

void array_move(babel_env *be, mword *dest, mword dest_index, mword *src, mword src_index, mword size_arg, access_size asize);

//void array1_move(babel_env *be, mword *dest, mword dest_begin, mword *src, mword size_arg);
//void array1_move_single(babel_env *be, mword *dest, mword dest_mod, mword *src, mword size_arg);
//void array1_move_double(babel_env *be, mword *dest, mword dest_mod, mword *src, mword size_arg);
//void array1_move_n(babel_env *be, mword *dest, mword dest_mod, mword *src, mword size_arg);

mword *array1_slice(babel_env *be, mword *array, mword start, mword end);
//void array1_slice_single(babel_env *be, mword *dest, mword *src, mword src_mod, mword size_arg);

void array_trunc(babel_env *be, mword *operand, mword new_size); // XXX DEPRECATED XXX
mword *array_shrink(babel_env *be, mword *array, mword new_begin, mword new_end, access_size asize);

void array1_move(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_unsafe(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
mword array1_calc_splits(mword begin, mword size_arg);
void array1_move_double_full(babel_env *be, mword *dest, mword dest_begin, mword *src, mword src_begin, mword size_arg);
void array1_move_split_0_0(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_0_1(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_1_0(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_1_1(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_2_1(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_1_2(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_2_2(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_n(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);

mword *array_to_string(babel_env *be, mword *array);

mword *array_find_val(babel_env *be, mword *bs, mword *val);
mword *array_find_ptr(babel_env *be, mword *bs, mword *ptr);
mword *array_to_list(babel_env *be, mword *arr);

void array_build_max_heap(mword *array);
void array_max_heapify(mword *array, mword i, mword array_size);

mword array_search(babel_env *be, mword *array, mword *target, sort_type st);
mword array_search_binary(babel_env *be, mword *begin, mword *end, mword *target, sort_type st);
mword array_search_linear(babel_env *be, mword *array, int start, int end, mword *target, sort_type st);

blob array_cat_pyr_op(babel_env *be, blob arrays);

    //#define _arcmp8(x,y,z) array8_cmp_lex(x, y, z)
//#define  _arcmp(x,y)  array_cmp_lex(x, y)
//
//#define _areq8(x,y,z) (array8_cmp_lex(x, y, z) == 0)
//#define  _areq(x,y)  (array_cmp_lex(x, y) == 0)

//mword  _dec_alignment_word1(babel_env *be, mword alignment_word);
//mword  _alignment_word1(babel_env *be, mword size1);
//void   _wrcxr1(babel_env *be, mword *arr, mword off, mword val);
//mword  _array1_size(babel_env *be, mword size1);
//void   _msort(babel_env *be, mword *array);
//void   _rmsort(babel_env *be, mword left, mword right, mword *array);
//void   _merge(babel_env *be, mword *array, mword left_start, mword left_end, mword right_start, mword right_end);
////void    move(mword *dest, mword dest_index, mword *src, mword src_index, mword size);
//void    move(babel_env *be, mword *dest, mword dest_index, mword *src, mword src_index, mword size, mword asize);
//int    _arcmp(mword *left, mword *right);
//int _arcmp8(babel_env *be, mword *left, mword *right);
//mword *_ar2bytes(babel_env *be, mword *array);
//mword *_th8(babel_env *be, mword *bs, mword entry);
//mword *array_find_val(babel_env *be, mword *bs, mword *val);
//mword *array_find_ptr(babel_env *be, mword *bs, mword *ptr);
//mword *_slice8(babel_env *be, mword *array, mword start, mword end);
//
//void array_ptr_sort(babel_env *be, mword *array, mword *comparator);
//void array_rptr_sort(babel_env *be, mword left, mword right, mword *array, mword *comparator, mword sort_type);
//void array_ptr_sort_merge(babel_env *be, mword *array, mword left_start, mword left_end, mword right_start, mword right_end, mword *comparator, mword sort_type);
//
//int array_cmp_alpha(babel_env *be, mword *left, mword *right, mword lex_alpha_select, mword asize);
//
//pyr_cache *th_r8d(babel_env *be);
//pyr_cache *th_rmd(babel_env *be);
//pyr_cache *th_wmp(babel_env *be);
//pyr_cache *move_md(babel_env *be);
//pyr_cache *move_8d(babel_env *be);
//pyr_cache *arlen_md(babel_env *be);
//pyr_cache *cat_md(babel_env *be);
//pyr_cache *slice_md(babel_env *be);
//pyr_cache *slice_8d(babel_env *be);
//pyr_cache *arcmp_md(babel_env *be);
//pyr_cache *arcmp_8d(babel_env *be);
//
//pyr_cache *sortval_d(babel_env *be);
//pyr_cache *merge_sort_d(babel_env *be);
//
//pyr_cache *cat_8d(babel_env *be);
//pyr_cache *arlen_8d(babel_env *be);
//
//pyr_cache *strcmp_8d(babel_env *be);
//pyr_cache *strcmp_md(babel_env *be);

#endif //ARRAY_H

// Clayton Bauman 2017

