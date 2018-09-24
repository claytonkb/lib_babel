// cmp.h
// 

#ifndef CMP_H
#define CMP_H

int cmp_cint(const void *p1, const void *p2);
int cmp_cuint(const void *p1, const void *p2);
int cmp_float(const void *p1, const void *p2);
int cmp_aop_unsigned(const void *p1, const void *p2);
int cmp_aop_signed(const void *p1, const void *p2);
int cmp_aop_alpha_mword(const void *p1, const void *p2);
int cmp_aop_alpha_byte(const void *p1, const void *p2);
int cmp_aop_lex_mword(const void *p1, const void *p2);
int cmp_aop_lex_byte(const void *p1, const void *p2);
int cmp_aop_custom(const void *p1, const void *p2);


#endif //CMP_H

// Clayton Bauman 2018

