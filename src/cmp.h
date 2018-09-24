// cmp.h
// 

#ifndef CMP_H
#define CMP_H

int cmp_cint(const void *a, const void *b);
int cmp_cuint(const void *a, const void *b);
int cmp_float(const void *a, const void *b);
int cmp_aop_unsigned(const void *a, const void *b);
int cmp_aop_signed(const void *a, const void *b);
int cmp_aop_alpha_mword(const void *a, const void *b);
int cmp_aop_alpha_byte(const void *a, const void *b);
int cmp_aop_lex_mword(const void *a, const void *b);
int cmp_aop_lex_byte(const void *a, const void *b);
int cmp_aop_custom(const void *a, const void *b);


#endif //CMP_H

// Clayton Bauman 2018

