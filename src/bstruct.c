// bstruct.c
//

#include "babel.h"
#include "bstruct.h"
#include "mem.h"
#include "cmp.h"
#include "array.h"


// recursively cleans a bstruct after traversal
//
void bstruct_clean(babel_env *be, mword *bs){ // bstruct_clean#

    int i;

    if( !(sfield(bs) & CTL_MASK) ){ //Already cleaned
        return;
    }

    sfield(bs) = sfield(bs) & ~CTL_MASK; //Mark clean

    if( is_ptr(bs) ){
        int num_elem = size(bs);
        for(i=0; i<num_elem; i++){
            bstruct_clean(be, (mword *)*(bs+i));
        }
    }
    else if(is_tptr(bs)){
        bstruct_clean(be, bs+HASH_SIZE+1);
    }

}


//
//
void bstruct_recurse(babel_env *be, mword *bs, bstruct_op_fn_ptr bfn, void *v){ // bstruct_recurse#

    _fn_recurse(be, bs, bfn, v);
    bstruct_clean(be, bs);

}



//
//
mword _fn_recurse(babel_env *be, mword *bs, bstruct_op_fn_ptr bfn, void *v){ // _fn_recurse#

    int i;

    if( is_traversed_U(bs) ){
        return 1;
    }

    if( !bfn(be, bs,v) ){
        return 0;
    }

    if( is_ptr(bs) ){
        int num_elem = size(bs);
        mark_traversed_U(bs);
        for(i=0; i<num_elem; i++){
            if(!_fn_recurse(be, (mword *)*(bs+i),bfn,v)){
                return 0;
            }
        }
        return 1;
    }
    else if( is_tptr(bs) ){
        mword *tptr_ptr = (bs+TPTR_PTR_OFFSET);

        mark_traversed_U(bs);

        mark_traversed_U(tptr_ptr);

        _fn_recurse(be, (mword*)*tptr_ptr, bfn, v);
    }
    else{
        mark_traversed_U(bs);
    }
    return 1;

}



// bstruct_mu -> memory usage
// bstruct_mu(be, x) = 
//      + bstruct_nlf(be, x) 
//      + bstruct_nva(be, x)
//      + bstruct_nin(be, x) 
//      + bstruct_nptr(be, x) 
//      + bstruct_ntag(be, x)*(HASH_SIZE+1)
//      
//  Don't forget that nil will add a "silent" HASH_SIZE+1 to 
//  your bstruct if anything in your bstruct points to it...
mword bstruct_mu(babel_env *be, mword *bs){ // _mu#

    mword counter=0;
    bstruct_recurse(be, bs, bstruct_rmu, &counter);
    return counter;

}


//
//
mword bstruct_rmu(babel_env *be, mword *bs, void *v){ // _rmu#

    if( is_tptr(bs) ){
        *(mword*)v += HASH_SIZE+3;
    }
    else if( is_val(bs) ){
        *(mword*)v += size(bs)+1;
    }
    else{
        *(mword*)v += size(bs)+1;
    }

    return 1;

}


// bstruct_nar -> number of arrays (ptr- or val-array)
//
mword bstruct_nar(babel_env *be, mword *bs){ // _nar#

    mword counter=0;
    bstruct_recurse(be, bs, bstruct_rnar, &counter);
    return counter;

}


//
//
mword bstruct_rnar(babel_env *be, mword *bs, void *v){ // _rnar#

    *(mword*)v += 1;

    return 1;

}


// bstruct_nlf -> number of leaf arrays (val-arrays)
//
mword bstruct_nlf(babel_env *be, mword *bs){ // _nlf#

    mword counter=0;
    bstruct_recurse(be, bs, bstruct_rnlf, &counter);
    return counter;

}


//
//
mword bstruct_rnlf(babel_env *be, mword *bs, void *v){ // _rnlf#

    if( is_val(bs) ){
        *(mword*)v += 1;
    }

    return 1;

}


// bstruct_nin -> number of interior arrays (ptr-arrays)
//
mword bstruct_nin(babel_env *be, mword *bs){ // _nin#

    mword counter=0;
    bstruct_recurse(be, bs, bstruct_rnin, &counter);
    return counter;

}


//
//
mword bstruct_rnin(babel_env *be, mword *bs, void *v){ // _rnin#

    if( is_ptr(bs) ){
        *(mword*)v += 1;
    }

    return 1;

}


// bstruct_ntag -> number of tagged-pointers
//
mword bstruct_ntag(babel_env *be, mword *bs){ // _ntag#

    mword counter=0;
    bstruct_recurse(be, bs, bstruct_rntag, &counter);
    return counter;

}


//
//
mword bstruct_rntag(babel_env *be, mword *bs, void *v){ // _rntag#

    if( is_tptr(bs) ){
        *(mword*)v += 1;
    }

    return 1;

}


// bstruct_nva -> number of values
//
mword bstruct_nva(babel_env *be, mword *bs){ // _nva#

    mword counter=0;
    bstruct_recurse(be, bs, bstruct_rnva, &counter);
    return counter;

}


//
//
mword bstruct_rnva(babel_env *be, mword *bs, void *v){ // _rnva#

    if( is_val(bs) ){
        *(mword*)v += size(bs);
    }

    return 1;

}


// _nptr -> number of pointers
//
mword bstruct_nptr(babel_env *be, mword *bs){ // _nptr#

    mword counter=0;
    bstruct_recurse(be, bs, bstruct_rnptr, &counter);
    return counter;

}


//
//
mword bstruct_rnptr(babel_env *be, mword *bs, void *v){ // _rnptr#

    if( is_ptr(bs) ){
        *(mword*)v += size(bs);
    }

    return 1;

}

#if 0
//
//
mword *bstruct_cp(babel_env *be, mword *bs){ // bstruct_cp#

    mword *temp = bstruct_unload(be, bs);

    bs = bstruct_load(be, temp, size(temp));

    return bs;

}
#endif


//
//
mword *bstruct_load(babel_env *be, mword *bs, mword size){ // bstruct_load#

    mword *curr_ptr = bs+1;
    mword *base_ptr = curr_ptr;

    mword curr_size;
    int i;

    mword *bs_last_mword = bs+size-1;

    while(curr_ptr < bs_last_mword){

        if(is_ptr(curr_ptr)){

            curr_size = size(curr_ptr);

            for(i=0;i<curr_size;i++){
                ldv(curr_ptr,i) = rdv(curr_ptr,i) + (mword)bs;
            }

        }
        else if(is_tptr(curr_ptr)){

            ldv(curr_ptr,TPTR_PTR_OFFSET) = rdv(curr_ptr,TPTR_PTR_OFFSET) + (mword)bs;

        }
        // else is_val, do nothing

        curr_ptr += alloc_size(curr_ptr);

    }

    return base_ptr;

}



//
//
mword *bstruct_unload(babel_env *be, mword *bs){

    mword *dest;

    if(is_val(bs)){
        dest = mem_new_val(be, alloc_size(bs), 0);
        memcpy(dest,(bs-1),(size_t)UNITS_MTO8(size(dest)));
        return dest;
    }

    dest = mem_new_val(be, bstruct_mu(be, bs), 0);

    mword *span_array = bstruct_to_array(be, bs);

    qsort(span_array, size(span_array), sizeof(mword), cmp_cuint);

//_mem(span_array);
//return dest;

    mword *offset_array = mem_new_val(be, size(span_array), 0xff);

    mword dest_offset = 0;

    bstruct_unload_r(be, bs, dest, &dest_offset, span_array, offset_array);
    bstruct_clean(be, bs);

    return dest;

}



//
//
mword bstruct_unload_r(
        babel_env *be, 
        mword       *bs, 
        mword       *dest, 
        mword       *dest_offset, 
        mword       *span_array, 
        mword       *offset_array){ // bstruct_unload_r#

    int i;

    if( is_traversed_U(bs) ){
        return offset_array[array_search(be, span_array, (mword*)(&bs), VAL_ST)];
    }

    int num_elem = size(bs);

    *(dest+(*dest_offset))   = sfield(bs);
    *dest_offset             = *dest_offset+1;

    mword local_offset = *dest_offset;
    mword this_offset  = (*dest_offset)*MWORD_SIZE;

    set_offset_for_ptr(be, span_array, bs, offset_array, this_offset);

    if(is_tptr(bs)){ // is_tptr

        mark_traversed_U(bs);

        for(i=0; i<=HASH_SIZE; i++){
            ldv(dest,(*dest_offset)) = rdv(bs,i);
            *dest_offset = *dest_offset+1;
        }

        mword *tptr_ptr = (bs+TPTR_PTR_OFFSET);

        mark_traversed_U(tptr_ptr); //FIXME: Does this actually need to be here??

        mword tptr_offset = *dest_offset;

        *dest_offset = *dest_offset + 1;

        ldv(dest,tptr_offset)
            = bstruct_unload_r(be, (mword*)*tptr_ptr, dest, dest_offset, span_array, offset_array);

    }
    else if(is_ptr(bs)){

        mark_traversed_U(bs);

        *dest_offset = *dest_offset + num_elem;

        for(i=0; i<num_elem; i++){
            ldv(dest,local_offset+i) 
                = bstruct_unload_r(be, rdp(bs,i), dest, dest_offset, span_array, offset_array);
        }

    }
    else{// if(is_val(bs))

        mark_traversed_U(bs);

//_d(sfield(bs));
//_bounds_check(be, bs);

        for(i=0; i<num_elem; i++){
            ldv(dest,(*dest_offset)) = rdv(bs,i);
            *dest_offset = *dest_offset+1;
        }

    }

    return this_offset;

}


//
//
void set_offset_for_ptr(
        babel_env *be, 
        mword     *span_array,      mword *ptr, 
        mword     *offset_array,    mword this_offset){ // set_offset_for_ptr#

    mword span_offset = array_search(be, span_array, (mword*)(&ptr), VAL_ST);

    offset_array[span_offset] = this_offset;

}


//Creates an interior array with one pointer to each array in a bstruct
//
mword *bstruct_to_array(babel_env *be, mword *bs){  // bstruct_to_array#

    mword num_arrays = bstruct_nar(be, bs);

    mword *arr_list = mem_new_ptr(be, num_arrays);
    mword offset = 0;

    bstruct_to_array_r(be, bs, arr_list, &offset);
    bstruct_clean(be, bs);

    return arr_list;

}


//
//
void bstruct_to_array_r(babel_env *be, mword *bs, mword *arr_list, mword *offset){ // bstruct_to_array_r#

    int i;

    if( is_traversed_U(bs) ){ //Already dumped
        return;
    }

    int num_elem = size(bs);

    ldp(arr_list,*offset) = bs;
    *offset = *offset+1;

    if(is_tptr(bs)){

        mark_traversed_U(bs);

        mword *tptr_ptr = (bs+TPTR_PTR_OFFSET);

        mark_traversed_U(tptr_ptr);

        bstruct_to_array_r(be, tcar(bs), arr_list, offset);

    }
    else if(is_ptr(bs)){

        mark_traversed_U(bs);

        for(i=0; i<num_elem; i++){
            bstruct_to_array_r(be, (mword*)rdp(bs,i), arr_list, offset);
        }

    }
    //else(is_val(bs))

    mark_traversed_U(bs);

}

#if 0

//---------------------------------------------------------------------------
//
//    bstruct_load_str():
//
//        1. Read in all text lines of the bs array
//            - Split into label-array pairs as you go
//            - Hash each label as you go
//            - Save the hash of the very first label (this is defined as the starting
//                    point of the data-structure)
//            - For each array, send to sexpr and get an sexpr back
//            - Track the total length of each array and total number of tptrs
//                - Need this for allocating the unloaded bstruct
//        2. Sort the array by label-hash
//        3. Allocate a val-array to hold the unloaded bstruct
//            - Allocate enough extra space to hold an unresolved-symbol tptr and nil
//        3. Starting with the first label:
//            - If it is a val-array
//                calculate and write the sfield
//                convert and store accordingly
//            - If it is a ptr-array:
//                calculate and write the sfield
//                For each entry (label):
//                    lookup the label in the label array
//                    if the label is fully resolved:
//                        Substitute its address in this entry of the ptr-array
//                    else if the label does not exist:
//                        Substitute the address of the unresolved-symbol tptr (place
//                            it at the end of the array, if not already done)
//                    else:
//                        Recurse on the unresolved label
//            - If it is a tptr or tag
//                sfield=0
//                tptr: Store the tptr tag bytes
//                tag: Hash the tag label and store the tptr tag bytes
//                if the tptr-label is nil or is fully resolved:
//                    Substitute its address in this entry of the ptr-array
//                else if the label does not exist:
//                    Substitute the address of the unresolved-symbol tptr (place
//                        it at the end of the array, if not already done)
//                else:
//                    Recurse on the unresolved label
//        ------------------------------------------------------------------
//
//        [bs
//          s001 [ptr foo bar baz ]
//          foo [val 0x1 0x2 0x3 ]
//          bar [val 0x4 0x5 0x6 ]
//          baz [val 0x7 0x8 0x9 ] ]
//
//        Note: this is a stripped-down version of [bs ] with no fancy bells &
//            whistles (i.e. we don't pass the arrays to sexpr() to be
//            individually parsed). A later version may add more robust
//            functionality.


//
//
mword *bstruct_load_str(babel_env *be, mword *bs){ // bstruct_load_str#

    mword *dest;

    return dest;

}


////
////
//mword *bstruct_find(babel_env *be, mword *target_bs, mword *find_bs){ // bstruct_find#
//
//    mword *result;
//
//    if(is_tptr(find_bs)){
//        result = tptr_find_tag_unsafe(be, target_bs, find_bs);
//    }
//    else if(is_val(find_bs)){
//        result = array_find_val(be, target_bs, find_bs);
//    }
//    else{ // is_ptr(find_bs)
//        result = array_find_ptr(be, target_bs, find_bs);
//    }
//
//    return result;
//
//}

#endif


// Clayton Bauman 2018

