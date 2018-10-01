// tptr.c
//

#include "babel.h"
#include "tptr.h"
#include "mem.h"
#include "bstruct.h"

#define MAX_DETAG_DEPTH 1024


//
//
tptr tptr_new(babel_env *be, const mword *hash, mword *bs){ // tptr_new#

    mword *ptr = mem_alloc( be, TPTR_SFIELD );

    memcpy((char*)ptr, hash, HASH_BYTE_SIZE);

    ldv(ptr,HASH_SIZE) = NEG_ONE*MWORD_SIZE;

    tptr_set_ptr(ptr,bs);

    return ptr;

}


// Safely, recursively de-references a tag
//
mword *tptr_detag(babel_env *be, tptr t){ // tptr_detag#

    static int livelock_detect=0;

    if(is_nil(t)){
        return be->nil;
    }

    if(is_tptr(t)){
        if(livelock_detect++ > MAX_DETAG_DEPTH){
            //cat_except(be);
            _fatal("FIXME: this should have been a cat_except...");
        }
        return tptr_detag(be, tcar(t));
    }
    else{
        livelock_detect=0;
        return t;
    }

}


//
//
val tptr_extract_hash(babel_env *be, mword *tptr){ // tptr_extract_hash#

    mword *ptr = mem_new_valz(be, HASH_SIZE);
    int i;

    for(i=0; i<HASH_SIZE; i++){// FIXME: PERF... use memcpy
        ptr[i] = rdv(tptr,i);
    }

    return ptr;

}


//
//
void tptr_update_hash(babel_env *be, mword *tptr, mword *hash){ // tptr_update_hash#

    int i;

    for(i=0; i<HASH_SIZE; i++){// FIXME: PERF... use memcpy
        tptr[i] = rdv(hash,i);
    }

}


//
//
mword *tptr_extract_ptr(babel_env *be, mword *tptr){ // tptr_extract_ptr#

    mword* temp = (mword*)(tptr+TPTR_PTR_OFFSET);

    if(is_tptr(temp)){
        return tptr_extract_ptr(be, temp);
    }
    else{
        return temp;
    }

}


// Hard de-references a tag (recursive)
//
mword *tptr_hard_detag(babel_env *be, mword *tptr){ // tptr_hard_detag#

    mword *temp = tcar(tptr);

    if(is_tptr(temp)){
        return tptr_hard_detag(be, temp);
    }
    else{
        return temp;
    }

}

#if 0
// Creates a pseudo-random tag
//
tptr tptr_uniq(babel_env *be, mword *bs){ // tptr_uniq#

    mword *ptr = mem_alloc( be, TPTR_SFIELD );

// FIXME: genrand_int32() not implemented...
//    int i;
//    for(i=0; i<HASH_SIZE; i++){ // FIXME: PERF... use memcpy
//        ptr[i] = (mword)genrand_int32();
//    }
//
//    ldv(ptr,HASH_SIZE) = NEG_ONE*MWORD_SIZE;
//
//    tptr_set_ptr(ptr,bs);

    return ptr;

}
#endif


// Clayton Bauman 2018

