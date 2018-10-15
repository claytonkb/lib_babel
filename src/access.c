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
            // bsetp(result,0,dispatched_result);
        }
        else{
            // NOTE: If you want to read the tag out of a tptr, use get_tag()
            //       not an access_* function
//            bsetp(result, 0, get_tptr(b));
        }
    }
    else if(is_large_arr(b)){
        // call paged-array accessor
        access_pa_rd(be, b, offset, result, asize); // XXX this can livelock!
    }
    else{
        if(is_ptr(b)){
            bsetp(result, 0, bgetp(b, offset));
        }
        else{ // is_val(b)
            bsetv(result, 0, access_api_rd_val(be, b, offset, asize));
        }
    }

}


//
//
void access_api_wr(babel_env *be, bstruct b, mword offset, bstruct payload, access_size asize){

    if(is_tptr(b)){
        if(is_builtin(b)){
            // dispatch
        }
        else{
            // NOTE: If you want to write the tag of a tptr, use tptr_set_tag()
            //       not an access_* function
            tptr_set_ptr(b, payload);
        }
    }
    else if(is_large_arr(b)){
        // call paged-array accessor
        access_pa_wr(be, b, offset, payload, asize); // XXX this can livelock!
    }
    else{
        if(is_ptr(b)){
            bsetp(b, offset, bgetp(payload, 0));
        }
        else{ // is_val(b)
            access_api_wr_val(be, b, offset, payload, asize);
        }
    }

}


// NOTE: assumes is_val(bs)
//
mword access_api_rd_val(babel_env *be, val v, mword offset, access_size asize){

    if(is_large_arr(v)){
        // call paged-array accessor
        return access_pa_rd_val(be, v, offset, asize);
    }

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
            // return bgetv(v, offset) & 0x00000000ffffffff;
            break;
        case U64_ASIZE:
        case MWORD_ASIZE:
            return bgetv(v, offset);
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

//    return bgetp(p, offset);
    _trace;
    return (mword*)0;

}


// NOTE: assumes is_ptr(bs)
//
void access_api_wr_ptr(ptr p, mword offset, cptr payload){

    bsetp(p, offset, payload);

}


//
//
void access_pa_rd(babel_env *be, bstruct b, mword offset, cptr result, access_size asize){

    mword level2_offset = BIT_SELECT(offset, PA_LEVEL2_MSB, PA_LEVEL2_LSB);
    mword level1_offset = BIT_SELECT(offset, PA_LEVEL1_MSB, PA_LEVEL1_LSB);
    mword level0_offset = BIT_SELECT(offset, PA_LEVEL0_MSB, PA_LEVEL0_LSB);

//    mword *result2 = rdp(be->mem->paging_base, level2_offset);
//    mword *result1 = rdp(result2, level1_offset);
//
//    access_api_rd(be, result1, level0_offset, result, asize); // XXX this can live-lock!

}


//
//
void access_pa_wr(babel_env *be, bstruct b, mword offset, bstruct payload, access_size asize){

    mword level2_offset = BIT_SELECT(offset, PA_LEVEL2_MSB, PA_LEVEL2_LSB);
    mword level1_offset = BIT_SELECT(offset, PA_LEVEL1_MSB, PA_LEVEL1_LSB);
    mword level0_offset = BIT_SELECT(offset, PA_LEVEL0_MSB, PA_LEVEL0_LSB);

//    mword *result2 = rdp(be->mem->paging_base, level2_offset);
//    mword *result1 = rdp(result2, level1_offset);
//
//    access_api_wr(be, result1, level0_offset, payload, asize); // XXX this can live-lock!

}


//
//
mword access_pa_rd_val(babel_env *be, bstruct b, mword offset, access_size asize){

    mword level2_offset = BIT_SELECT(offset, PA_LEVEL2_MSB, PA_LEVEL2_LSB);
    mword level1_offset = BIT_SELECT(offset, PA_LEVEL1_MSB, PA_LEVEL1_LSB);
    mword level0_offset = BIT_SELECT(offset, PA_LEVEL0_MSB, PA_LEVEL0_LSB);

//    mword *result2 = rdp(be->mem->paging_base, level2_offset);
//    mword *result1 = rdp(result2, level1_offset);
//
//    return access_api_rd_val(be, result1, level0_offset, asize); // XXX this can live-lock!

}


//
//
void access_pa_wr_val(babel_env *be, bstruct b, mword offset, bstruct payload, access_size asize){

    mword level2_offset = BIT_SELECT(offset, PA_LEVEL2_MSB, PA_LEVEL2_LSB);
    mword level1_offset = BIT_SELECT(offset, PA_LEVEL1_MSB, PA_LEVEL1_LSB);
    mword level0_offset = BIT_SELECT(offset, PA_LEVEL0_MSB, PA_LEVEL0_LSB);

//    mword *result2 = rdp(be->mem->paging_base, level2_offset);
//    mword *result1 = rdp(result2, level1_offset);
//
//    access_api_wr_val(be, result1, level0_offset, payload, asize); // XXX this can live-lock!

}


//
//
mword *access_pa_rd_ptr(babel_env *be, bstruct b, mword offset){

    mword level2_offset = BIT_SELECT(offset, PA_LEVEL2_MSB, PA_LEVEL2_LSB);
    mword level1_offset = BIT_SELECT(offset, PA_LEVEL1_MSB, PA_LEVEL1_LSB);
    mword level0_offset = BIT_SELECT(offset, PA_LEVEL0_MSB, PA_LEVEL0_LSB);

//    mword *result2 = rdp(be->mem->paging_base, level2_offset);
//    mword *result1 = rdp(result2, level1_offset);
//
//    return access_api_rd_ptr(result1, level0_offset); // XXX this can live-lock!

}


//
//
void access_pa_wr_ptr(babel_env *be, bstruct b, mword offset, bstruct payload, access_size asize){

    mword level2_offset = BIT_SELECT(offset, PA_LEVEL2_MSB, PA_LEVEL2_LSB);
    mword level1_offset = BIT_SELECT(offset, PA_LEVEL1_MSB, PA_LEVEL1_LSB);
    mword level0_offset = BIT_SELECT(offset, PA_LEVEL0_MSB, PA_LEVEL0_LSB);

//    mword *result2 = rdp(be->mem->paging_base, level2_offset);
//    mword *result1 = rdp(result2, level1_offset);
//
//    access_api_wr_ptr(result1, level0_offset, payload); // XXX this can live-lock!

}


// Clayton Bauman 2018

