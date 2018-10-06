// cmp.c
//

#include "babel.h"
#include "cmp.h"
#include "array.h"


//
//
int cmp_cint(const void *a, const void *b){
    return ( *(int*)a - *(int*)b );
}


//
//
int cmp_cuint(const void *a, const void *b){
    return ( *(mword*)a - *(mword*)b );
}


//
//
int cmp_float(const void *a, const void *b){
    return ( *(float*)a - *(float*)b );
}


//
//
int cmp_aop_unsigned(const void *a, const void *b){
    return array_cmp_num(pcar(*(void**)a),pcar(*(void**)b));
}


//
//
int cmp_aop_signed(const void *a, const void *b){
    return array_cmp_num_signed(pcar(*(void**)a),pcar(*(void**)b));
}


//
//
int cmp_aop_alpha_mword(const void *a, const void *b){
    return array_cmp_alpha(pcar(*(void**)a),pcar(*(void**)b),MWORD_ASIZE);
}


//
//
int cmp_aop_alpha_byte(const void *a, const void *b){
    return array_cmp_alpha(pcar(*(void**)a),pcar(*(void**)b),U8_ASIZE);
}


//
//
int cmp_aop_lex_mword(const void *a, const void *b){
    return array_cmp_lex(pcar(*(void**)a),pcar(*(void**)b),MWORD_ASIZE);
}


//
//
int cmp_aop_lex_byte(const void *a, const void *b){
    return array_cmp_lex(pcar(*(void**)a),pcar(*(void**)b),U8_ASIZE);
}


//
//
int cmp_aop_custom(const void *a, const void *b){
    return 0;
}


// Clayton Bauman 2018

