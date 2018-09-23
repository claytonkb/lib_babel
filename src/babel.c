// babel.c
//

#include "babel.h"
#include "mem.h"
#include "pearson.h"


//
//
babel_env *babel_new_env(int init_mem_size, int max_mem_size){

    babel_env *be = malloc(sizeof(babel_env));

    be->zero_hash = malloc(UNITS_MTO8(HASH_ALLOC_SIZE));
    ldv(be->zero_hash,0) = UNITS_MTO8(HASH_SIZE);
    be->zero_hash++;
    memset((char*)be->zero_hash, 0, UNITS_MTO8(HASH_SIZE));

    be->nil = malloc(UNITS_MTO8(TPTR_ALLOC_SIZE));
    be->nil++;

    sfield(be->nil) = 0;
    sfield(be->nil+TPTR_PTR_OFFSET) = VAL_TO_PTR(MWORD_SIZE);

    pearson128(be->nil, be->zero_hash, "/babel/tag/nil", STRLEN("/babel/tag/nil") );

    tptr_set_ptr(be->nil, be->nil);

    mem_context *mc = mem_context_new(be,init_mem_size);

    be->mem = mc;

    return be;

}


// Clayton Bauman 2018

