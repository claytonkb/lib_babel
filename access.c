// access.c
//

#include "babel.h"
#include "access.h"


//
//
void access_api_rd(babel_env *be, bstruct b, mword offset, cptr result, access_size asize){

    // XXX get_tptr(b); --> replace get_tptr_tptr
    // XXX get_tag(b);  --> replace get_tptr_tag

    if(is_tptr(b)){
        if(is_builtin(b)){
            // dispatch
            // setp(result,0,dispatched_result);
        }
        else{
            // NOTE: If you want to read the tag out of a tptr, use get_tag()
            //       not an access_* function
            setp(result,0,get_tptr(b));
        }
    }
    else if(is_large_arr(b)){
        // call paged-array accessor
    }
    else{
        if(is_ptr(b)){
            setp(result,0,getp(b,offset));
        }
        else{ // is_val(b)
            setv(result,0,access_api_rd_val(be,b,offset,asize));
        }
    }

}


//
//
void access_api_wr(babel_env *be, bstruct b, mword offset, bstruct payload, access_size asize){

    if(is_tptr(b)){
        // NOTE: If you want to write the tag of a tptr, use tptr_set_tag()
        //       not an access_* function
        tptr_set_ptr(b, payload);
    }
    else if(is_large_arr(b)){
        // call paged-array accessor
    }
    else{
        if(is_ptr(b)){
            setp(b,offset,getp(payload,0));
        }
        else{ // is_val(b)
            //void access_api_wr_val(babel_env *be, val bs, mword offset, cptr payload, access_size asize){ 
        }
    }

}


// NOTE: assumes is_val(bs)
//
mword access_api_rd_val(babel_env *be, val v, mword offset, access_size asize){

    switch(asize){
       case U1_ASIZE:
            //mword array1_read(mword *array, mword offset){ // array1_read#
            break;
        case U8_ASIZE:
            //mword array8_read(mword *array, mword offset){ // array8_read#
            break;
        case U16_ASIZE:
            //mword array8_read(mword *array, mword offset){ // array8_read#
            //mword array8_read(mword *array, mword offset+1){ // array8_read#
            break;
        case U32_ASIZE:
            // assert(sizeof(mword) == sizeof(uint64_t)
            // return getv(v, offset) & 0x00000000ffffffff;
            break;
        case U64_ASIZE:
        case MWORD_ASIZE:
            return getv(v, offset);
         default:
            _pigs_fly;
    }

}


// NOTE: assumes is_val(bs)
//
void access_api_wr_val(babel_env *be, val v, mword offset, cptr payload, access_size asize){ 

    switch(asize){
       case U1_ASIZE:
            //void array1_write(mword *array, mword offset, mword value){ // array1_write#
            break;
        case U8_ASIZE:
            //void array8_write(mword *array, mword offset, mword value){ // array8_write#
            break;
        case U16_ASIZE:
            //void array8_write(mword *array, mword offset, mword value){ // array8_write#
            //void array8_write(mword *array, mword offset+1, mword value){ // array8_write#
            break;
        case U32_ASIZE:
            //void array8_write(mword *array, mword offset, mword value){ // array8_write#
            break;
        case U64_ASIZE:
        case MWORD_ASIZE:
            ldv(v, offset) = *payload;
        default:
            _pigs_fly;
    }

}


// NOTE: assumes is_ptr(bs)
//
mword *access_api_rd_ptr(ptr p, mword offset){

    return getp(p,offset);

}


// NOTE: assumes is_ptr(bs)
//
void access_api_wr_ptr(ptr p, mword offset, cptr payload){

    setp(p,offset,payload);

}


// Clayton Bauman 2018

