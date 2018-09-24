// pyramid.c
//
// see pyramidrc for build hints

#include "babel.h"
#include "pearson.h"
#include "mem.h"

////main
////
//int main(int argc, char **argv, char **envp){ // main#
//
//    uint64_t init[2] = {0,0};
//    uint64_t result[2];
// 
//    pearson128(result, init, "foo", 3); // hash "foo"
//    printf("h(0,'foo')         : %016lx%016lx\n", result[1], result[0]);
//
//    return 0;
//
//}


//
//
int pyramid_init(babel_env *be){ // pyramid_init#

    pc->zero_hash = mem_sys_alloc(UNITS_MTO8(HASH_ALLOC_SIZE));
    ldv(pc->zero_hash,0) = UNITS_MTO8(HASH_SIZE);
    pc->zero_hash++;
    memset((char*)pc->zero_hash, 0, UNITS_MTO8(HASH_SIZE));

    pc->nil = mem_sys_alloc(UNITS_MTO8(TPTR_SIZE));
    pc->nil++;

    sfield(pc->nil) = 0;
    sfield(pc->nil+HASH_SIZE+1) = (-1*MWORD_SIZE);

    pearson128(pc->nil, pc->zero_hash, "/babel/tag/nil", STRLEN("/babel/tag/nil") );

    tptr_set_ptr(pc->nil, pc->nil);

    return 1;

}

// Clayton Bauman 2018

