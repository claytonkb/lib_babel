// array.h
// 

#ifndef ARRAY_H
#define ARRAY_H

#define ARRAY_SORT_TYPE_LEAF     0
#define ARRAY_SORT_TYPE_NON_LEAF 1

#define ARRAY_SEARCH_NOT_FOUND NEG_ONE      // ARRAY_SEARCH_NOT_FOUND#
#define ARRAY_LINEAR_THRESH 2               // ARRAY_LINEAR_THRESH#
// XXX PERF: ^^^LINEAR_THRESH should be higher than 2... 2 is for testing
#define ARRAY_PERF_LINEAR_HOLDOFF 3 // ARRAY_PERF_LINEAR_HOLDOFF#

#define array8_aligned(x) ((x) % BITS_PER_BYTE == 0)

arr array_read(babel_env *be, mword *bs, mword offset, access_size asize);
arr array_write(babel_env *be, mword *bs, mword offset, mword *payload, access_size asize);

arr array_th(babel_env *be, mword *bs, mword entry);

mword array8_read(mword *array, mword offset);
void array8_write(mword *array, mword offset, mword value);
mword *array8_th(babel_env *be, mword *val_array, mword entry8);
mword array8_size(const mword *string);
mword array8_dec_align(mword alignment_word);
mword array8_enc_align(mword size8);
mword array8_mword_size(mword size8);

mword *array8_slice(babel_env *be, mword *array, mword start, mword end);

mword *array_slice(babel_env *be, mword *array, mword start, mword end);
mword *array_to_string(babel_env *be, mword *array);


/*****************************************************************************
 *                                                                           *
 *                          ARRAY-1 PRIMITIVES                               *
 *                                                                           *
 ****************************************************************************/

mword array1_read(mword *array, mword offset);
void array1_write(mword *array, mword offset, mword value);
mword *array1_th(babel_env *be, mword *val_array, mword entry1);
mword array1_size(babel_env *be, mword *string);
mword array1_dec_align(babel_env *be, mword alignment_word);
mword array1_enc_align(babel_env *be, mword size1);
mword array1_mword_size(babel_env *be, mword size1);


/*****************************************************************************
 *                                                                           *
 *                            ARRAY COMPARISON                               *
 *                                                                           *
 ****************************************************************************/
int array_cmp_lex(const mword *left, const mword *right, access_size asize);
int array_ncmp(babel_env *be, mword *left, mword left_offset, mword *right, mword length, access_size asize);
int array_cmp_alpha(mword *left, mword *right, access_size asize);
int array_cmp_num(mword *left, mword *right);
int array_cmp_num_range(mword *left, mword *left_end, mword *right, mword *right_end);
int array_cmp_num_signed(mword *left, mword *right);

#define array_eq_num(l, r)  (array_cmp_num(l, r) == 0) // array_eq_num#
#define array_lt_num(l, r)  (array_cmp_num(l, r) <  0) // array_lt_num#
#define array_gt_num(l, r)  (array_cmp_num(l, r) >  0) // array_gt_num#

#define array_eq(pyr, l, r)  (array_cmp_lex(l, r, MWORD_ASIZE) == 0) // array_eq#
#define array8_eq(pyr, l, r) (array_cmp_lex(l, r, U8_ASIZE)  == 0) // array8_eq#

#define array_lt(pyr, l, r)  (array_cmp_lex(l, r, MWORD_ASIZE) <  0) // array_lt#
#define array8_lt(pyr, l, r) (array_cmp_lex(l, r, U8_ASIZE)  <  0) // array8_lt#

#define array_gt(pyr, l, r)  (array_cmp_lex(l, r, MWORD_ASIZE) >  0) // array_gt#
#define array8_gt(pyr, l, r) (array_cmp_lex(l, r, U8_ASIZE)  >  0) // array8_gt#


mword *array1_slice(babel_env *be, mword *array, mword start, mword end);
void array1_move(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_unsafe(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
mword array1_calc_splits(mword begin, mword size_arg);
void array1_move_split_0_0(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_0_1(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_1_0(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_1_1(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_2_1(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_1_2(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);
void array1_move_split_n(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg);

mword *array_shrink(babel_env *be, mword *array, mword new_begin, mword new_end, access_size asize);


/*****************************************************************************
 *                                                                           *
 *                             ARRAY SEARCH                                  *
 *                                                                           *
 ****************************************************************************/
mword array_search(babel_env *be, mword *array, mword *target, sort_type st);
mword array_search_binary(babel_env *be, mword *begin, mword *end, mword *target, sort_type st);
mword array_search_linear(babel_env *be, mword *array, int start, int end, mword *target, sort_type st);

mword *array_to_list(babel_env *be, mword *arr);

void array_max_heapify(mword *array, mword i, mword array_size);

#endif //ARRAY_H

// Clayton Bauman 2018

