// mem.c
//

#include "babel.h"
#include "mem.h"
#include "array.h"


// init_mem_size is in units of bytes
//
mem_context *mem_context_new(babel_env *be, mword init_mem_size){

#ifdef GC_TRACE
_trace;
#endif

    int i;

    mem_context *mc = malloc(sizeof(mem_context)); // XXX WAIVER(malloc) XXX

    if(!init_mem_size) // Returns an uninitialized mem_context
        return mc;

    ptr level2_dir;
    ptr level1_dir;
    val level0_pg;

    mword mword_mem_size = UNITS_8TOM(init_mem_size);

    if(mword_mem_size < LARGE_PAGE_SIZE){

        level2_dir = mem_sys_new_bstruct(VAL_TO_PTR(UNITS_MTO8(PA_DIR_SIZE)));
        level1_dir = mem_sys_new_bstruct(VAL_TO_PTR(UNITS_MTO8(PA_DIR_SIZE)));
        level0_pg  = mem_sys_new_bstruct(UNITS_MTO8(mword_mem_size));

        for(i=0;i<PA_DIR_SIZE;i++){ // manual initializion of ptr-arrays
            ldp(level1_dir,i) = be->nil;
            ldp(level2_dir,i) = be->nil;
        }

        memset((char*)level0_pg, 0, init_mem_size); // zero out memory

        ldp(level1_dir,0) = level0_pg;
        ldp(level2_dir,0) = level1_dir;
        mc->paging_base   = level2_dir;

    }
    else{
        _enhance("init_mem_size >= LARGE_PAGE_SIZE");
    }

    mc->sys_alloc_count=0;
    mc->sys_free_count=0;

    // TODO: init GC flags
    // TODO: init nested context list

    return mc;

}


//
//
void mem_context_destroy(mem_context *mc){

    bstruct init_level2_dir = mc->paging_base;
    bstruct init_level1_dir = rdp(init_level2_dir,0);
    bstruct init_level0_pg  = rdp(init_level1_dir,0);

    mem_sys_destroy_bstruct(init_level2_dir);
    mem_sys_destroy_bstruct(init_level1_dir);
    mem_sys_destroy_bstruct(init_level0_pg);

}


// NOTE: Does NOT initialize ptr/tptr areas
//
bstruct mem_sys_new_bstruct(mword alloc_sfield){

    bstruct result = mem_sys_alloc(UNITS_MTO8(mem_alloc_size(alloc_sfield)+1));
    result++;
    sfield(result) = alloc_sfield;

    return result;

}


//
//
void mem_sys_destroy_bstruct(bstruct b){

    b--;
    mem_sys_free(b);

}




// size is in units of bytes
//
void *mem_sys_alloc(int size){ // mem_sys_alloc#

    void *alloc_attempt = malloc(size); // XXX WAIVER(malloc) XXX

    if(alloc_attempt == NULL){ // malloc failed
        _fatal("malloc failed");
    }

    return alloc_attempt;

}


//
//
void mem_sys_free(void *p){ // mem_sys_free#

    free(p); // XXX WAIVER(free) XXX

}


// FIXME: Temporary sol'n until GC is in place
//
bstruct mem_alloc(babel_env *be, mword alloc_sfield){ // mem_alloc#

    mword alloc_request_size = mem_alloc_size(alloc_sfield)+1; // +1 is for s-field

    bstruct result = mem_sys_alloc(UNITS_MTO8(alloc_request_size));
    result++;
    sfield(result) = alloc_sfield;

    return result;

}


//
//
void *mem_new_val(babel_env *be, mword size, mword init){ // mem_new_val#

    mword local_size = UNITS_MTO8(size);

    void *ptr = (void*)mem_alloc(be, local_size);

    memset((char*)ptr,init,local_size);

    return ptr;

}


//
//
void *mem_new_ptr(babel_env *be, mword size){ // mem_new_ptr#

    void *ptr;

    if(!size){

        return be->nil;

    }
    else{

        ptr = (void*)mem_alloc(be, -1*UNITS_MTO8(size));

        int i;
        for(i = 0; i<size; i++){
            ldp(ptr,i) = (void*)be->nil;
        }

    }

    return ptr;

}


//
//
void *mem_new_tptr(babel_env *be, const mword *hash, mword *bs){

    mword *ptr = mem_alloc(be, TPTR_SFIELD);

    tagcpy(ptr, hash);

    // ptr s-field
    ldv(ptr,HASH_SIZE) = (NEG_ONE * MWORD_SIZE);
    tptr_set_ptr(ptr, bs);

    return ptr;

}


// Accepts a data value and returns a val-array of size 1 containing that 
// data value
//
void *_val(babel_env *be, mword value){ // _val#

    void *ptr = mem_new_val(be, 1, 0);
    ldv(ptr,0) = value;

    return ptr;

}


// Accepts a single unsafe pointer and returns a safe ptr-array of size
// 1 containing the unsafe pointer
//
void *_ptr(babel_env *be, mword *unsafe_ptr){ // _ptr#

    void *ptr = mem_new_ptr(be, 1);
    ldp(ptr,0) = unsafe_ptr;

    return ptr;

}


// Intended for internal-use... 
//
mword *mem_new_str(babel_env *be, mword size8, char set_char){ // mem_new_str#

    mword arlength = array8_mword_size(size8);

    mword *result = mem_new_val(be, arlength, set_char);

    mword alignment_word = array8_enc_align(size8);

    ldv(result, arlength-1) = alignment_word;
    ldv(result, arlength-2) = (rdv(result, arlength-2)) & ~alignment_word;

    return result;

}


//// Intended for internal-use... 
////
//mword *_newbits(babel_env *be, mword size1){ // _newbits#
//
//    mword arlength = array1_mword_size(be, size1);
//
//    mword *result = mem_new_val(be, arlength, 0);
//
//    mword alignment_word = array1_enc_align(be, size1);
//
//    ldv(result, arlength-1) = alignment_word;
//    ldv(result, arlength-2) = (rdv(result, arlength-2)) & ~alignment_word;
//
//    return result;
//
//}


//
//
mword *list_cons(babel_env *be, mword *car, mword *cdr){ // list_cons#

    mword **cons_cell = (mword**)mem_new_cons(be);
    ldp(cons_cell,0) = car;
    ldp(cons_cell,1) = cdr;

    return (mword*)cons_cell;

}


// Make a double-linked list cons cell (three entries instead of two)
//
mword *list_dcons(babel_env *be, mword *car, mword *cdr, mword *cpr){ // list_dcons#

    mword **dcons_cell = (mword**)mem_new_cons(be);
    ldp(dcons_cell,0) = car;
    ldp(dcons_cell,1) = cdr;
    ldp(dcons_cell,2) = cpr;

    return (mword*)dcons_cell;

}


//
//
void *_mkval(babel_env *be, mword array_size, ...){ // _mkval#

    void *val = (void*)mem_new_val(be, array_size, 0);

    va_list vl;
    va_start(vl,array_size);

    int i;

    for(i=0;i<array_size;i++){
        setv(val,i,va_arg(vl,mword));
//        ldv(val,i) = va_arg(vl,mword);
    }

    va_end(vl);

    return val;

}


//
//
void *_mkptr(babel_env *be, mword array_size, ...){ // _mkptr#

    void *ptr = (void*)mem_new_ptr(be, array_size);

    va_list vl;
    va_start(vl,array_size);

    int i;

    for(i=0;i<array_size;i++){
        ldp(ptr,i) = va_arg(vl,mword*);
    }

    va_end(vl);

    return ptr;

}


//// make aop ==> make "array-of-pairs"
//// array_size = number-of-arguments / 2
////
//void *_mk_aop(babel_env *be, mword array_size, ...){ // _mk_aop#
//
//    void *ptr = (void*)mem_new_ptr(be, array_size);
//
//    va_list vl;
//    va_start(vl,array_size);
//
//    int i;
//    mword *car;
//    mword *cdr;
//
//    for(i=0;i<array_size;i++){
//        car = va_arg(vl,mword*);
//        cdr = va_arg(vl,mword*);
//        ldp(ptr,i) = list_cons(be, car, cdr);
//    }
//
//    va_end(vl);
//
//    return ptr;
//
//}
//



// Clayton Bauman 2018

