// mem.c
//

#include "babel.h"
#include "mem.h"
#include "array.h"


// init_mem_size is in units of bytes
//
mem_context *mem_context_new(babel_env *be){

    int i;

    mem_context *mc = mem_sys_alloc(sizeof(mem_context)); // XXX WAIVER(malloc) XXX

     ptr level2_page = mem_sys_new_bstruct(VAL_TO_PTR(UNITS_MTO8(PA_DIR_SIZE)));
     ptr level1_page = mem_sys_new_bstruct(VAL_TO_PTR(UNITS_MTO8(PA_DIR_SIZE)));
    cptr level0_page = mem_sys_alloc(UNITS_MTO8(LARGE_PAGE_SIZE));

    for(i=0;i<PA_DIR_SIZE;i++){ // manual initializion of ptr-arrays
        ldp(level1_page,i) = be->nil;
        ldp(level2_page,i) = be->nil;
    }

    ldp(level1_page,0) = level0_page;
    ldp(level2_page,0) = level1_page;

    mc->paging_base    = level2_page;

    mc->alloc_ptr.level2_index = 1;
    mc->alloc_ptr.level1_index = 1;
    mc->alloc_ptr.level0_index = 0;

    mc->sys_alloc_count=0;
    mc->sys_free_count=0;

    // TODO: init frame_list
    mc->frame_list = mem_sys_alloc(sizeof(mem_ll));

    mc->frame_list->a.level2_index = 1;
    mc->frame_list->a.level1_index = 1;
    mc->frame_list->a.level0_index = 0;

    mc->frame_list->next = (mem_ll*)be->nil;

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
    mem_sys_free(init_level0_pg);

}


//
//
cptr mem_context_expand(mem_context *mc){

    bstruct new_page;

    bstruct paging_base = mc->paging_base;

    mword *level1_page = rdp(paging_base, mc->alloc_ptr.level2_index);
    mword *level0_page = rdp(level1_page, mc->alloc_ptr.level1_index);
    mword *alloc_ptr   = level0_page + mc->alloc_ptr.level0_index;

    // if level1 is maxed out
    //      if level2 is maxed out
    //          fatal
    //      level1 <-- alloc a new level1 page (register in level2)
    //
    // alloc new level0 page (register in level1)
    // return base address of level0 page
    if(mc->alloc_ptr.level1_index == PA_DIR_SIZE){ // level1 page is maxed out

        if(mc->alloc_ptr.level2_index == PA_DIR_SIZE){ // level2 page is maxed out
            _fatal("paging directories full"); // FIXME: fail gracefully!
        }

        ptr new_level1_page = mem_sys_new_bstruct(VAL_TO_PTR(UNITS_MTO8(PA_DIR_SIZE)));
        mc->alloc_ptr.level2_index++;
        ldp(paging_base, mc->alloc_ptr.level2_index) = level1_page;

        mc->alloc_ptr.level1_index=0;

    }

    cptr new_level0_page = mem_sys_alloc(UNITS_MTO8(LARGE_PAGE_SIZE));
    ldp(level1_page, mc->alloc_ptr.level1_index) = new_level0_page;
    mc->alloc_ptr.level1_index++;

    mc->alloc_ptr.level0_index=0;

    new_page = new_level0_page;

    return new_page;

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
void *mem_sys_alloc(int size){

    void *alloc_attempt = malloc(size);

    if(alloc_attempt == NULL){ // malloc failed
        _fatal("malloc failed");
    }

    return alloc_attempt;

}


//
//
void mem_sys_free(void *p){

    free(p); 

}

//#if 0
// FIXME: Temporary sol'n until GC is in place
//
bstruct mem_alloc(babel_env *be, mword alloc_sfield){

    mword alloc_request_size = mem_alloc_size(alloc_sfield)+1; // +1 is for s-field

    bstruct result = mem_sys_alloc(UNITS_MTO8(alloc_request_size));
    result++;
    sfield(result) = alloc_sfield;

    return result;

}
//#endif

#define LEVEL0_PAGE_SIZE LARGE_PAGE_SIZE

#if 0
//
//
bstruct mem_alloc(babel_env *be, mword alloc_sfield){

    mword alloc_request_size = mem_alloc_size(alloc_sfield)+1; // +1 is for s-field

    bstruct result;

    if(alloc_request_size >= LARGE_PAGE_SIZE){
        result = mem_sys_alloc(UNITS_MTO8(alloc_request_size));
        result++;
        sfield(result) = alloc_sfield;
        _warn("large page allocation");
        return result;
    }

    thread_context *tc = be->threads[be->thread_id];

    bstruct paging_base = tc->mem->paging_base;

    mword *level1_page = rdp(paging_base, tc->mem->alloc_ptr.level2_index);
    mword *level0_page = rdp(level1_page, tc->mem->alloc_ptr.level1_index);
    mword *alloc_ptr   = level0_page + tc->mem->alloc_ptr.level0_index;

    // calculate headroom in alloc_pg from alloc_ptr
    mword level0_remaining = LEVEL0_PAGE_SIZE - tc->mem->alloc_ptr.level0_index;

    // XXX Note: previous iterations of Babel used high-to-low allocation;
    //      lib_babel uses low-to-high allocation

    if(alloc_request_size > level0_remaining){
        _enhance("alloc_request_size > level0_remaining");
        // mem_context_expand() (returns new alloc_ptr)
        // alloc_ptr = mem_context_expand(tc);
    }

    result = alloc_ptr;
    tc->mem->alloc_ptr.level0_index += alloc_request_size;

    return result;

}
#endif


//
// 
void mem_frame_open(mem_context *mc){

    // save the current alloc_ptr for later unwinding of the heap
    mem_ll *new_frame = mem_sys_alloc(sizeof(mem_ll));

    new_frame->a.level2_index = mc->alloc_ptr.level2_index;
    new_frame->a.level1_index = mc->alloc_ptr.level1_index;
    new_frame->a.level0_index = mc->alloc_ptr.level0_index;

    new_frame->next = mc->frame_list;
    mc->frame_list = new_frame;

}


//
//
void mem_frame_close(babel_env *be){

//    mem_context *mc = be->mem; // XXX wrong, use threads
//
//    // if is_nil(ptr), ptr = last entry in mem_context list
//    if(is_nil(mc->frame_list->next)){
//        return; // there are no more frames to close
//    }
//
//    mem_ll *curr_frame = mc->frame_list;
//    mc->frame_list = curr_frame->next;
//
////    mword *level2_page = mc->paging_base;
////    mword *level1_page = rdp(level2_page, mc->alloc_ptr.level2_index);
////    mword *level0_page = rdp(level1_page, mc->alloc_ptr.level1_index);
//
//    bstruct paging_base = tc->mem->paging_base;
//
//    mword *level1_page = rdp(paging_base, tc->mem->alloc_ptr.level2_index);
//    mword *level0_page = rdp(level1_page, tc->mem->alloc_ptr.level1_index);
//    mword *alloc_ptr   = level0_page + tc->mem->alloc_ptr.level0_index;
//

}


//
//
void *mem_new_val(babel_env *be, mword size, mword init){

    mword local_size = UNITS_MTO8(size);

    void *ptr = (void*)mem_alloc(be, local_size);

    memset((char*)ptr,init,local_size);

    return ptr;

}


//
//
void *mem_new_ptr(babel_env *be, mword size){

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
void *_val(babel_env *be, mword value){

    void *ptr = mem_new_val(be, 1, 0);
    ldv(ptr,0) = value;

    return ptr;

}


// Accepts a single unsafe pointer and returns a safe ptr-array of size
// 1 containing the unsafe pointer
//
void *_ptr(babel_env *be, mword *unsafe_ptr){

    void *ptr = mem_new_ptr(be, 1);
    ldp(ptr,0) = unsafe_ptr;

    return ptr;

}


// Intended for internal-use... 
//
mword *mem_new_str(babel_env *be, mword size8, char set_char){

    mword arlength = array8_mword_size(size8);

    mword *result = mem_new_val(be, arlength, set_char);

    mword alignment_word = array8_enc_align(size8);

    ldv(result, arlength-1) = alignment_word;
    ldv(result, arlength-2) = (rdv(result, arlength-2)) & ~alignment_word;

    return result;

}


//
//
mword *mem_new_bits(babel_env *be, mword size1){

    mword arlength = array1_mword_size(be, size1);

    mword *result = mem_new_val(be, arlength, 0);

    mword alignment_word = array1_enc_align(be, size1);

    ldv(result, arlength-1) = alignment_word;
    ldv(result, arlength-2) = (rdv(result, arlength-2)) & ~alignment_word;

    return result;

}


//
//
mword *list_cons(babel_env *be, mword *car, mword *cdr){

    mword **cons_cell = (mword**)mem_new_cons(be);
    ldp(cons_cell,0) = car;
    ldp(cons_cell,1) = cdr;

    return (mword*)cons_cell;

}


// Make a double-linked list cons cell (three entries instead of two)
//
mword *list_dcons(babel_env *be, mword *car, mword *cdr, mword *cpr){

    mword **dcons_cell = (mword**)mem_new_cons(be);
    ldp(dcons_cell,0) = car;
    ldp(dcons_cell,1) = cdr;
    ldp(dcons_cell,2) = cpr;

    return (mword*)dcons_cell;

}


//
//
void *_mkval(babel_env *be, mword array_size, ...){

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
void *_mkptr(babel_env *be, mword array_size, ...){

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


// creates a new list of given size
// note that the list is created in REVERSE order:
// _newls(this_pyr, 3, _val(this_pyr,3), _val(this_pyr,2), _val(this_pyr,1)) --> (1 2 3)
//
// [ptr [val 0x1 ] [ptr [val 0x2 ] [ptr [val 0x3 ]  nil ] ] ]
//
mword *_mkls(babel_env *be, mword list_size, ...){

    va_list vl;
    va_start(vl,list_size);

    int i;

    mword *last_cons = be->nil;
//    mword *new_cons = mem_new_ptr(be, 2);
    mword *new_cons = list_cons(be, be->nil, be->nil);
    mword *head = new_cons;

    ldp(new_cons,0) = va_arg(vl,mword*);

    for(i=1;i<list_size;i++){

        last_cons = new_cons;
        new_cons = mem_new_ptr(be, 2);
        ldp(new_cons,0) = va_arg(vl,mword*);
        ldp(last_cons,1) = new_cons;

    }

    va_end(vl);

    return head;

}



// make aop ==> make "array-of-pairs"
// array_size = number-of-arguments / 2
//
void *_mkaop(babel_env *be, mword array_size, ...){

    void *ptr = (void*)mem_new_ptr(be, array_size);

    va_list vl;
    va_start(vl,array_size);

    int i;
    mword *car;
    mword *cdr;

    for(i=0;i<array_size;i++){
        car = va_arg(vl,mword*);
        cdr = va_arg(vl,mword*);
        ldp(ptr,i) = list_cons(be, car, cdr);
    }

    va_end(vl);

    return ptr;

}



// Clayton Bauman 2018

