// tptr.c
//

#include "babel.h"
#include "tptr.h"
#include "mem.h"
//#include "list.h"
#include "bstruct.h"
//#include "mt19937ar.h"

#define MAX_DETAG_DEPTH 1024


//
//
tptr tptr_new(babel_env *be, const mword *hash, mword *bs){ // tptr_new#

    mword *ptr = mem_alloc( pc, TPTR_SFIELD );

    int i;
    for(i=0; i<HASH_SIZE; i++){ // FIXME: PERF... use memcpy
        ptr[i] = hash[i];
    }

    ldv(ptr,HASH_SIZE) = NEG_ONE*MWORD_SIZE;

    tptr_set_ptr(ptr,bs);

    return ptr;

}


// Safely, recursively de-references a tag
//
mword *tptr_detag(babel_env *be, tptr t){ // tptr_detag#

    static int livelock_detect=0;

    if(is_nil(t)){
        return pc->nil;
    }

    if(is_tptr(t)){
        if(livelock_detect++ > MAX_DETAG_DEPTH){
            //cat_except(pc);
            _fatal("FIXME: this should have been a cat_except...");
        }
        return tptr_detag(pc, tcar(t));
    }
    else{
        livelock_detect=0;
        return t;
    }

}


//
//
val tptr_extract_hash(babel_env *be, mword *tptr){ // tptr_extract_hash#

    mword *ptr = mem_new_valz(pc, HASH_SIZE);
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
        return tptr_extract_ptr(pc, temp);
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
        return tptr_hard_detag(pc, temp);
    }
    else{
        return temp;
    }

}


// Creates a pseudo-random tag
//
tptr tptr_uniq(babel_env *be, mword *bs){ // tptr_uniq#

    mword *ptr = mem_alloc( pc, TPTR_SFIELD );

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


//// XXX The return-value from this function contains unsafe pointers!!! XXX
//// XXX internal interp use ONLY                                        XXX
//// XXX If you pass tag=nil, returns ALL tags in bs                     XXX
//// XXX PERF: A _tags2ar (like bstruct_to_array) would be more efficient          XXX
////
//mword *tptr_find_tag_unsafe(babel_env *be, mword *bs, mword *tag){ // tptr_find_tag_unsafe#
//
//    mword *span_array     = bstruct_to_array(pc, bs);
//    mword size_span_array = size(span_array);
//    mword size_inte;
//    mword *tag_list       = nil;
//    mword *curr_span_elem;
//    mword *curr_inte_elem;
//
//
////_dump(span_array);
//
//    int i,j;
//    for(i=0; i<size_span_array; i++){
//
//        curr_span_elem = rdp(span_array,i);
//
//        if(is_ptr(curr_span_elem)){ // check each element
//
//            size_inte = size(curr_span_elem);
//
//            for(j=0;j<size_inte;j++){
//
//                curr_inte_elem = rdp(curr_span_elem,j);
//
//                if(is_nil(curr_inte_elem)){
//                    continue;
//                }
//
//                if(is_tptr(curr_inte_elem)){
//
//                    if(is_nil(tag)){
//
//                        // push onto tag_list
//                        if(is_nil(tag_list)){
//                            tag_list = list_cons(pc, (curr_span_elem+j), nil);
//                        }
//                        else{
//                            list_unshift(pc, tag_list, (curr_span_elem+j));
//                        }
//
//                    }
//                    else{
//
//                        if( tageq(curr_inte_elem, tag, TAG_SIZE) ){
//
//                            // push onto tag_list
//                            if(is_nil(tag_list)){
//                                tag_list = list_cons(pc, (curr_span_elem+j), nil);
//                            }
//                            else{
//                                list_unshift(pc, tag_list, (curr_span_elem+j));
//                            }
//
//                        }
//
//                    }
//
//
//                }
//
//            }
//
//        }
//
//    }
//
//    return tag_list;
//
//}
//

////
////
//mword *tptr_find_tag(babel_env *be, mword *search_bs, mword *tag){  // tptr_find_tag#
//
//    if(is_nil(search_bs) || is_nil(tag)){
//        return pc->nil;
//    }
//
//    mword *search_tag;
//
//    if(is_tptr(tag)){
//        search_tag = tag;
//    }
//    else{
//        search_tag = tptr_new(pc, tag, pc->nil);
//    }
//
//    mword *sub_list = bstruct_find(pc, search_bs, search_tag);
//    mword *result = pc->nil;
//
//    while(!is_nil(sub_list)){
//        result = list_cons(pc, rdp(rdp(sub_list,0),0), result);
//        sub_list = pcdr(sub_list);
//    }
//
//    return result;
//
//}


// Clayton Bauman 2018

