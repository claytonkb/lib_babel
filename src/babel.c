// babel.c
//

#include "babel.h"
#include "mem.h"
#include "pearson.h"


//
//
babel_env *babel_env_new(int num_threads){

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

    mem_context *mc = mem_context_new(be);
    be->mem = mc;

    be->num_threads = num_threads;
    be->threads = malloc(num_threads*sizeof(thread_context*));

    for(int i=0; i<num_threads; i++){
        be->threads[i] = malloc(sizeof(thread_context));
        be->threads[i]->thread_id = i;
        be->threads[i]->mem = mem_context_new(be);
    }

    return be;

}


// Clayton Bauman 2018

