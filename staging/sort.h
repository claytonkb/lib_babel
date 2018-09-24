// sort.h
// 

#ifndef SORT_H
#define SORT_H

#define SORT_ARRAY_TYPE_VAL 0
#define SORT_ARRAY_TYPE_PTR 1

void sort(babel_env *be, mword *array, sort_type st);
void sort_r(babel_env *be, mword left, mword right, mword *array, sort_type st);
void sort_aop(babel_env *be, mword *array, mword key_index, sort_type st);
void sort_aop_unsigned(babel_env *be, mword left, mword right, mword *array, mword key_index);
void sort_aop_signed(babel_env *be, mword left, mword right, mword *array, mword key_index);
void sort_aop_alpha_mword(babel_env *be, mword left, mword right, mword *array, mword key_index);
void sort_aop_alpha_byte(babel_env *be, mword left, mword right, mword *array, mword key_index);
void sort_aop_lex_mword(babel_env *be, mword left, mword right, mword *array, mword key_index);
void sort_aop_lex_byte(babel_env *be, mword left, mword right, mword *array, mword key_index);
void sort_insertion(babel_env *be, mword *begin, mword *end, mword array_type, sort_type st, mword key_index);

#endif //SORT_H

// Clayton Bauman 2017

