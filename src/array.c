// array.c
//

#include "babel.h"
#include "array.h"
#include "mem.h"
#include "utf8.h"
#include "bstruct.h"
#include "list.h"


//
//
mword array8_read(mword *array, mword offset){ // array8_read#

    mword mword_select = (offset / MWORD_SIZE);
    mword byte_offset  = (offset % MWORD_SIZE);

    if(mword_select > size(array)-1){
        _fatal("error");
    }

    mword read_mask = ((mword)0xff<<UNITS_8TO1(byte_offset));

    return ((rdv(array,mword_select) & read_mask) >> UNITS_8TO1(offset));

}


//
//
void array8_write(mword *array, mword offset, mword value){ // array8_write#

    mword mword_select = (offset / MWORD_SIZE);
    mword byte_offset  = (offset % MWORD_SIZE);

    if(mword_select > size(array)-1){
        _fatal("error");
    }

    mword write_mask = ((mword)0xff<<UNITS_8TO1(byte_offset));

    ldv(array,mword_select) = 
          (rdv(array,mword_select) & ~write_mask) 
        | ((value & MASK_1_BYTE) << UNITS_8TO1(byte_offset));

}


////
////
//void array8_write(mword *array, mword offset, mword value){ // array8_write#
//
//    array8_mask_generate(offset, array, write_mask, mword_select);
//    ldv(array,mword_select) = 
//          (rdv(array,mword_select) & ~write_mask) 
//        | ((value & MASK_1_BYTE) << UNITS_8TO1(byte_offset));
//
//}


// calculates array8 size from sfield and alignment word
//
mword array8_size(const mword *string){ // array8_size#

    mword strsize = size(string) - 1;
    mword last_mword = rdv(string, strsize);

    if(last_mword){
        return (strsize * MWORD_SIZE) - (MWORD_SIZE - array8_dec_align(last_mword));
    }
    else{
        return (strsize * MWORD_SIZE);
    }

}


// decodes the alignment word
//
mword array8_dec_align(mword alignment_word){ // array8_dec_align#

    if(alignment_word == 0){
        return 0;
    }

    alignment_word = ~alignment_word;
    mword alignment = 0;

    while(alignment_word != 0){ //FIXME: PERF ... really inefficient
        alignment_word = alignment_word >> BITS_PER_BYTE;
        alignment++;
    }

    return alignment;

}



//Returns an alignment word based on size8
//
mword array8_enc_align(mword size8){ // array8_enc_align#

    if(size8 % MWORD_SIZE == 0)
        return 0;

    mword alignment = (mword)-1;

    return alignment << ((size8 % MWORD_SIZE) * BITS_PER_BYTE);

}



// returns the number of MWORDS required to store a val8 array of length size8
//
mword array8_mword_size(mword size8){ // array8_mword_size#

    mword size = (size8 / MWORD_SIZE);

    if(size8 % MWORD_SIZE != 0){ //XXX Assumes that int div rounds to floor
        size++;
    }

    return size+1; //for the alignment_word

}


/*****************************************************************************
 *                                                                           *
 *                          ARRAY-1 PRIMITIVES                               *
 *                                                                           *
 ****************************************************************************/

//#define array1_mask_generate(off,arr,mask,sel)      \
//    mword sel          = (off / MWORD_BIT_SIZE);    \
//    mword bit_offset   = (off % MWORD_BIT_SIZE);    \
//                                                    \
//    if(sel > size(arr)-1){                 \
//        _fatal("error");                            \
//    }                                               \
//                                                    \
//    mword mask = ((mword)1<<bit_offset);
    

// XXX TESTED XXX
//
mword array1_read(mword *array, mword offset){ // array1_read#

    mword mword_select = (offset / MWORD_BIT_SIZE);
    mword bit_offset   = (offset % MWORD_BIT_SIZE);

    if(mword_select > size(array)-1)
        _fatal("error");

    mword read_mask = ((mword)1<<bit_offset);
 
//    array1_mask_generate(offset, array, read_mask, mword_select);
    return ((rdv(array,mword_select) & read_mask) >> offset);

}


// XXX TESTED XXX
//
void array1_write(mword *array, mword offset, mword value){ // array1_write#

    mword mword_select = (offset / MWORD_BIT_SIZE);
    mword bit_offset   = (offset % MWORD_BIT_SIZE);

    if(mword_select > size(array)-1)
        _fatal("error");

    mword write_mask = ((mword)1<<bit_offset);
    
    ldv(array,mword_select) = 
          (rdv(array,mword_select) & ~write_mask) 
        | ((value<<bit_offset) & write_mask);

}


//// XXX TESTED XXX
////
//mword array1_read(mword *array, mword offset){ // array1_read#
//
//    array1_mask_generate(offset, array, read_mask, mword_select);
//    return ((rdv(array,mword_select) & read_mask) >> offset);
//
//}
//
//
//// XXX TESTED XXX
////
//void array1_write(mword *array, mword offset, mword value){ // array1_write#
//
//    array1_mask_generate(offset, array, write_mask, mword_select);
//    ldv(array,mword_select) = (rdv(array,mword_select) & ~write_mask) | ((value<<bit_offset) & write_mask);
//
//}


// Returns a val containing the bit at val_array[entry1] (bitwise addressing)
// XXX TESTED XXX
//
mword *array1_th(babel_env *be, mword *val_array, mword entry1){ // array1_th#

//    mword *bit = _newbits(be, 1);
    mword *bit = mem_new_bits(be, 1);
    ldv(bit,0) = array1_read(val_array, entry1);

    return bit;

}


//
//
mword array1_size(mword *string){ // array1_size#

    mword strsize = size(string) - 1;
    mword last_mword = rdv(string, strsize);
    mword alignment = array1_dec_align(last_mword);

    if(last_mword){
        return  (strsize * MWORD_BIT_SIZE) - (MWORD_BIT_SIZE - alignment);
    }
    else{
        return (strsize * MWORD_BIT_SIZE);
    }

}


// decodes the alignment word
//
mword array1_dec_align(mword alignment_word){ // array1_dec_align#

    if(alignment_word == 0){
        return 0;
    }

    alignment_word = ~alignment_word;
    mword alignment = 0;

    while(alignment_word != 0){ //FIXME: PERF ... really inefficient
        alignment_word = alignment_word >> 1;
        alignment++;
    }

    return alignment;

}


//Returns an alignment word based on size1
//
mword array1_enc_align(babel_env *be, mword size1){ // array1_enc_align#

    if((size1 % MWORD_BIT_SIZE) == 0)
        return 0;

    mword alignment = FMAX; //(mword)-1;

    return alignment << (size1 % MWORD_BIT_SIZE);

}


//
//
mword array1_mword_size(babel_env *be, mword size1){ // array1_mword_size#

    mword size = (size1 / MWORD_BIT_SIZE);

    if((size1 % MWORD_BIT_SIZE) != 0){ //XXX Assumes that int div rounds to floor
        size++;
    }

    return size+1; //for the alignment_word

}





/*****************************************************************************
 *                                                                           *
 *                            ARRAY CONVERSION                               *
 *                                                                           *
 ****************************************************************************/


// XXX TESTED XXX
//
mword *array_mwords_to_bytes(babel_env *be, mword *array){ // array_mwords_to_bytes#

    mword arr_size  = size(array);
    char *result = (char*)mem_new_str(be, arr_size, ' ');

    int i;

    for(i=0; i<arr_size; i++){
        result[i] = (char)(array[i] & 0xff);
    }

    return (mword*)result;

}


// XXX TESTED XXX
//
mword *array_bytes_to_mwords(babel_env *be, mword *array8){ // array_bytes_to_mwords#

    unsigned char *cast_array = (unsigned char *)array8;
    mword arr8_size = array8_size(array8);
    mword *result = mem_new_val(be, arr8_size, 0);

    int i;

    for(i=0; i<arr8_size; i++){
        result[i] = (mword)cast_array[i];
    }

    return result;

}


// XXX TESTED XXX
//
mword *array_mwords_to_bits(babel_env *be, mword *array){ // array_mwords_to_bits#

    mword arr_size  = size(array);
    mword *result   = mem_new_bits(be, arr_size);

    int i;

    for(i=0; i<arr_size; i++){
        array1_write(result, i, (array[i] != 0));
    }

    return result;

}


// XXX TESTED XXX
//
mword *array_bits_to_mwords(babel_env *be, mword *array1){ // array_bits_to_mwords#

    mword arr1_size  = array1_size(array1);
    mword *result = mem_new_val(be, arr1_size, 0);

    int i;

    for(i=0; i<arr1_size; i++){

        result[i] = array1_read(array1, i);

    }

    return result;

}


/*****************************************************************************
 *                                                                           *
 *                          ARRAY CONCATENATION                              *
 *                                                                           *
 ****************************************************************************/


// XXX TESTED XXX
//
mword *array_cat(babel_env *be, mword *left, mword *right){ // array_cat#

    mword s_left;
    mword s_right;

    char *result;

    if(is_val(right) && is_val(left)){
        s_left  = sfield(left);
        s_right = sfield(right);
        result = (char*)mem_new_valz(be, UNITS_8TOM(s_left+s_right));
    }
    else if(is_ptr(right) && is_ptr(left)){
        s_left  = sfield(left);
        s_right = sfield(right);
        result = (char*)mem_new_ptr(be, UNITS_8TOM(s_left+s_right));
    }
    else{ // FIXME(throw_exception)
        _fatal("cannot concatenate val-array and ptr-array");
    }

    memcpy(result,        left,  s_left);
    memcpy(result+s_left, right, s_right);

    return (mword*)result;

}


// XXX TESTED XXX
//
mword *array8_cat(babel_env *be, mword *left, mword *right){ // array8_cat#

    mword size_left;
    mword size_right;

    char *result;

    if(!is_val(right) || !is_val(left)){
        _fatal("cannot byte-concatenate ptr-array");
    }

    size_left  = array8_size(left);
    size_right = array8_size(right);

    result = (char*)mem_new_str(be, size_left+size_right, '\0');

    memcpy(result,           left,  size_left);
    memcpy(result+size_left, right, size_right);

    return (mword*)result;

}


//
//
mword *array1_cat(babel_env *be, mword *left, mword *right){ // array1_cat#

    mword size_left;
    mword size_right;

    char *result = (char*)be->nil;

    _enhance("unimplemented");
    return (mword*)result;


    if(!is_val(right) || !is_val(left)){
        _fatal("cannot bit-concatenate ptr-array");
    }

    size_left  = array1_size(left);
    size_right = array1_size(right);

    if(   (size_left  % BITS_PER_BYTE == 0)
       && (size_right % BITS_PER_BYTE == 0)) // array8_cat is *much* faster
        return array8_cat(be, left, right);

    result = (char*)mem_new_bits(be, size_left+size_right);

    // The hard part...

//    memcpy(result,           left,  size_left);
//    memcpy(result+size_left, right, size_right);

    return (mword*)result;


}



/*****************************************************************************
 *                                                                           *
 *                            ARRAY COMPARISON                               *
 *                                                                           *
 ****************************************************************************/


//
//
int array_cmp_lex(const mword *left, const mword *right, access_size asize){ // array_cmp_lex#

    mword left_size;
    mword right_size;

    if(asize == MWORD_ASIZE){
        left_size  = UNITS_MTO8(size_special(left));
        right_size = UNITS_MTO8(size_special(right));
    }
    else{ // asize == U8_SIZE
//        _fatal("BUG when asize==U8_ASIZE"); // FIXME
        left_size  = array8_size(left);
        right_size = array8_size(right);
    }

    if(left_size > right_size){
        return 1;
    }
    else if(left_size < right_size){
        return -1;
    }
    //else:
//_trace;
    return memcmp(left, right, left_size);

}



// LLLLLLLLLLL
//     RRRRRRRR
//     \---/
//       n
//
int array_ncmp(babel_env *be, mword *left, mword left_offset, mword *right, mword length, access_size asize){

    mword left_size;
    mword right_size;

    if(asize == MWORD_ASIZE){
        left_size  = UNITS_MTO8(size(left) - left_offset);
        right_size = UNITS_MTO8(size(right));
        length     = UNITS_MTO8(length);
    }
    else{ // access_size == U8_SIZE
        left_size  = array8_size(left) - left_offset;
        right_size = array8_size(right);
    }

    if( (length > left_size)
            ||
        (length > right_size)){
        return 0;
    }

    return (memcmp(((char*)left + left_offset), right, length) == 0);

}


//
//
int array_cmp_alpha(mword *left, mword *right, access_size asize){ // array_cmp_alpha#

    mword left_size;
    mword right_size;
    mword result;

    if(asize == MWORD_ASIZE){
        left_size  = size(left);
        right_size = size(right);
    }
    else{ // access_size == U8_SIZE
        left_size  = array8_size(left);
        right_size = array8_size(right);
    }

    if(left_size == right_size){
        result = memcmp(left, right, left_size);
    }
    else{
        result = memcmp(left, right, MIN(left_size,right_size));
        if(result == 0){
            if(left_size > right_size){
                result = 1;
            }
            else{
                result = -1;
            }
        }
    }

    return result;

}


// Multi-word unsigned numerical comparison
//
int array_cmp_num(mword *left, mword *right){ // array_cmp_num#

    return array_cmp_num_range(left, (left+size(left)-1), right, (right+size(right)-1));

}


// Multi-word (range-based) unsigned numerical comparison
//
int array_cmp_num_range(mword *left, mword *left_end, mword *right, mword *right_end){ // array_cmp_num_range#

    mword left_size  = (left_end-left);
    mword right_size = (right_end-right);

    mword left_non_zero=0;
    mword right_non_zero=0;

    int i;

    for(i=right_size;i>=0;i--){
        if(right[i] != 0){
            right_non_zero=i;
            break;
        }
    }

    for(i=left_size;i>=0;i--){
        if(left[i] != 0){
            left_non_zero=i;
            break;
        }
    }

    if(left_non_zero > right_non_zero){
        return 1;
    }
    else if(left_non_zero < right_non_zero){
        return -1;
    }
    else{
        for(;i>=0;i--){ // i is in correct position from previous for-loop
            if(rdp(left,i) > rdp(right,i)){ 
                return 1;
            }
            if(rdp(left,i) < rdp(right,i)){ 
                return -1;
            }
        }
    }

    return 0; // left and right are numerically equal

}



// Multi-word signed numerical comparison
//
int array_cmp_num_signed(mword *left, mword *right){ // array_cmp_num_signed#

    mword left_size  = size(left);
    mword right_size = size(right);

    mword left_sign  = rdv(left,left_size-1)   & MSB_MASK;
    mword right_sign = rdv(right,right_size-1) & MSB_MASK;

    if(left_sign && !right_sign){ // left is negative, right is non-negative
        return -1;
    }
    else if(!left_sign && right_sign){ // left is non-negative, right is negative
        return 1;
    }
    else{ // both left and right have the same sign
        return array_cmp_num(left, right);
    }

}



/*****************************************************************************
 *                                                                           *
 *                           ARRAY MOVE/SLICE                                *
 *                                                                           *
 ****************************************************************************/


// Babelized wrapper around memmove()
//
void array_move(babel_env *be, mword *dest, mword dest_index, mword *src, mword src_index, mword size_arg, access_size asize){ // array_move#

    mword src_size;
    mword dest_size;
    mword src_headroom;
    mword dest_headroom;
    mword size_limit;
    mword final_size;

    if(asize == MWORD_ASIZE){

        src_size  = size(src );
        dest_size = size(dest);

    }
    else{ // asize = U8_SIZE

        src_size  = array8_size(src );
        dest_size = array8_size(dest);

    }

    src_index  =  src_index %  src_size;
    dest_index = dest_index % dest_size;

    src_headroom  = ( src_size -  src_index);
    dest_headroom = (dest_size - dest_index);

    size_limit = (src_headroom < dest_headroom) ? src_headroom : dest_headroom;

    final_size = (size_arg < size_limit) ? size_arg : size_limit;

//    final_size = size_arg;

    if(asize == MWORD_ASIZE){

        memmove( dest+dest_index, src+src_index, (size_t)UNITS_MTO8(final_size) );

    }
    else{ // asize = U8_SIZE

        memmove( ((char*)dest+dest_index), ((char*)src+src_index), (size_t)final_size );

    }

}


// NB: Do not use on tptr's
//
mword *array_slice(babel_env *be, mword *array, mword start, mword end){ // array_slice#

    mword *result=be->nil;

    mword arr_size = size(array);
    end = MIN(end, arr_size);
    arr_size = end-start;

    if(end>start){

        if(is_val(array)){
            result = mem_new_val(be, arr_size, 0);
        }
        else{
            result = mem_new_ptr(be, arr_size);
        }

        array_move(be, result, 0, array, start, arr_size, MWORD_ASIZE);

    }

    return result;

}


//
//
mword *array8_slice(babel_env *be, mword *array, mword start, mword end){ // array8_slice#

    mword *result = _val(be, 0); // Empty-string

    mword size8 = array8_size(array);
    end = MIN(end, size8);

    if(end>start){

        result = mem_new_str(be, end-start, '\0');
        array_move(be, result, 0, array, start, end-start, U8_ASIZE);

    }

    return result;

}


//
//
mword *array1_slice(babel_env *be, mword *array, mword start, mword end){ // array1_slice#

    mword *result=be->nil;
    return result;

}


// XXX SMOKE-TESTED XXX
//
void array1_move(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg){ // array1_move#

    mword dest_begin_mword = UNITS_1TOM(dest_begin);
    dest_begin -= UNITS_MTO1(dest_begin_mword);

    mword src_begin_mword = UNITS_1TOM(src_begin);
    src_begin -= UNITS_MTO1(src_begin_mword);

    array1_move_unsafe(be, (dest+dest_begin_mword), dest_begin, (src+src_begin_mword), src_begin, size_arg);

}

// comp --> "complementary modulus", i.e. comp(modulus) = (MWORD_BIT_SIZE - modulus)
// dest_begin and src_begin MUST BE less than MWORD_BIT_SIZE
// This function performs a full bitwise move from src to dest...
// XXX src and dest must not overlap... this is NOT checked in this function XXX
//
void array1_move_unsafe(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg){ // array1_move_unsafe#

    // if src, dest & size_arg are aligned, memmove
    // if src is aligned, but dest is not aligned, this is a move
    // if src is not aligned, and dest is aligned, this is a slice

    if(!size_arg)  // nothing to move
        return;

    if(    (  dest_begin % BITS_PER_BYTE == 0)
        && (   src_begin % BITS_PER_BYTE == 0) 
        && (    size_arg % BITS_PER_BYTE == 0) ){
//        _notify("This was supposed to memmove...");
//        memmove( ((char*)dest+UNITS_1TO8(dest_begin)), (char*)src, (size_t)UNITS_1TO8(size_arg) );
//        return;
    }

    mword num_src_splits  = array1_calc_splits(src_begin,  size_arg);
    mword num_dest_splits = array1_calc_splits(dest_begin, size_arg);

    if((num_dest_splits==0) && (num_src_splits==0)){
//_say("splits - dest:0 src:0");
        array1_move_split_0_0(be, dest, dest_begin, src, src_begin, size_arg);
    }
    else if((num_dest_splits==1) && (num_src_splits==0)){
//_say("splits - dest:1 src:0");
        array1_move_split_1_0(be, dest, dest_begin, src, src_begin, size_arg);
    }
    else if((num_dest_splits==0) && (num_src_splits==1)){
//_say("splits - dest:0 src:1");
        array1_move_split_0_1(be, dest, dest_begin, src, src_begin, size_arg);
    }
    else if((num_dest_splits==1) && (num_src_splits==1)){
//_say("splits - dest:1 src:1");
        array1_move_split_1_1(be, dest, dest_begin, src, src_begin, size_arg);
    }
    else if((num_dest_splits==2) && (num_src_splits==1)){
//_say("splits - dest:2 src:1");
        array1_move_split_2_1(be, dest, dest_begin, src, src_begin, size_arg);
    }
    else if((num_dest_splits==1) && (num_src_splits==2)){
//_say("splits - dest:1 src:2");
        array1_move_split_1_2(be, dest, dest_begin, src, src_begin, size_arg);
    }
    else if((num_dest_splits==2) && (num_src_splits==2)){
//_say("splits - dest:n src:n");
        array1_move_split_n(be, dest, dest_begin, src, src_begin, size_arg);
    }
    else{
        _pigs_fly;
    }

}


//
//
mword array1_calc_splits(mword begin, mword size_arg){ // array1_calc_splits#

    mword begin_comp = MWORD_BIT_SIZE-begin;
    mword end_mod = (begin+size_arg) % MWORD_BIT_SIZE;

    mword num_splits = 0;

    if(size_arg <= MWORD_BIT_SIZE){ // 0 or 1 boundaries crossed
        num_splits = (begin_comp < size_arg);
    }
    else if(size_arg < (2*MWORD_BIT_SIZE)){ // 1 or 2 boundaries crossed
        if((size_arg+begin) <= (2*MWORD_BIT_SIZE)){
            num_splits = 1;
        }
        else{
            num_splits = 2;
        }
    }
    else if(size_arg == (2*MWORD_BIT_SIZE)){ // 1 or 2 boundaries crossed
        if( !begin && !end_mod ){
            num_splits = 1;
        }
        else{
            num_splits = 2;
        }
    }
    else{ // 2 or more boundaries crossed
        num_splits = 2;
    }

    return num_splits;

}


//
//
void array1_move_split_0_0(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg){ // array1_move_split_0_0#

    // src    | .xxx.... |
    //          deadbeef
    // 
    // dest   | ...xxx.. |
    //          000ead00
    ldv(dest,0) = MWORD_MUX(
        (MWORD_SHIFT((rdv(src,0)),(dest_begin-src_begin))),
        (rdv(dest,0)),
        (BIT_RANGE((dest_begin+size_arg-1),dest_begin)));

}



// dest:0 src:1
//
void array1_move_split_0_1(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg){ // array1_move_split_0_1#

    // src    |. . . . . . x x|x . . . . . . .|
    //         f a c e f e e d d e a d b e e f
    // 
    // dest   |. . . . x x x .|
    //         c c c c e d d c 

    mword src_mwordA = BIT_SELECT(rdv(src,0), MWORD_MSB, src_begin);
    mword src_mwordB = BIT_SELECT(rdv(src,1), (size_arg-(MWORD_MSB-src_begin))-1, 0);

    mword src_mword = (src_mwordA << dest_begin)
                        |
                      (src_mwordB << (dest_begin+(MWORD_MSB-src_begin)+1));

    ldv(dest,0) = MWORD_MUX(
                    src_mword,
                    rdv(dest,0),
                    BIT_RANGE((dest_begin+size_arg-1),dest_begin));

}



// dest:1 src:0
//
void array1_move_split_1_0(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg){ // array1_move_split_1_0#

    // src    |. x x x . . . .|
    //         d e a d b e e f
    // 
    // dest   |. . . . . . x x|x . . . . . . .|
    //         c c c c c c e a d 0 0 0 0 0 0 0

    int shiftA = dest_begin-src_begin;
    int shiftB = -1*COMPLEMENT_MTO1(shiftA);

    mword src_mwordA = MWORD_SHIFT(rdv(src,0),shiftA);
    mword src_mwordB = MWORD_SHIFT(rdv(src,0),shiftB);

    ldv(dest,0) = MWORD_MUX(
                    src_mwordA,
                    rdv(dest,0),
                    BIT_RANGE(MWORD_MSB,dest_begin));

    ldv(dest,1) = MWORD_MUX(
                    src_mwordB,
                    rdv(dest,1),
                    BIT_RANGE(MODULO_MTO1(dest_begin+size_arg-1),0));

}


//
//
void array1_move_split_1_1(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg){ // array1_move_split_1_1#

    mword src_comp, dest_comp;
    mword size_hi,  size_mid, size_lo;
    mword mask_hi,  mask_mid, mask_lo;
    mword src_val0, src_val1;
    mword buff0,    buff1;

    // size_arg <= MWORD_SIZE
    //               1                0
    //                  hi       lo
    // src:  |-----v----------|------v---------|
    //             |-----------------|
    //                   |-----------------|
    // dest: |...........^....|............^...|
    //                     hi       lo
    if(size_arg <= MWORD_BIT_SIZE){

        src_comp  = MWORD_BIT_SIZE-src_begin;
        dest_comp = MWORD_BIT_SIZE-dest_begin;

        size_hi = src_begin;

        mask_hi = NBIT_LO_MASK(size_arg-src_comp);

        src_val0 = rdv(src,0);
        src_val1 = rdv(src,1);

        buff0 = BIT_SELECT(src_val0, MWORD_MSB, src_begin)
                        |
                     ((src_val1 & mask_hi) << src_comp);

        size_lo = dest_comp;
        size_hi = size_arg-size_lo;

        mask_hi = NBIT_LO_MASK(size_hi);
        mask_lo = NBIT_HI_MASK(size_lo);

        ldv(dest,0) = MWORD_MUX(buff0 << dest_begin, rdv(dest,0), mask_lo);
        ldv(dest,1) = MWORD_MUX(buff0 >> dest_comp,  rdv(dest,1), mask_hi);

    }
    // size_arg > MWORD_SIZE
    //               1                0
    //           hi      mid     lo
    // src:  |--v--v----------|------v---------|
    //          |--|-----------------|
    //                |--|-----------------|
    // dest: |........^..^....|............^...|
    //                 hi  mid     lo
    else{

        src_comp  = MWORD_BIT_SIZE-src_begin;
        dest_comp = MWORD_BIT_SIZE-dest_begin;

        size_hi = size_arg-MWORD_BIT_SIZE;
        size_lo = src_comp;
        size_mid = size_arg-(size_hi+size_lo);

        src_val0 = rdv(src,0);
        src_val1 = rdv(src,1);

        if(size_mid){

            buff0 =
                //lo
                BIT_SELECT(src_val0, MWORD_MSB, src_begin)
                    |
                //mid
                (BIT_SELECT(src_val1, size_mid-1, 0) << size_lo);

        }
        else{

            //lo (no mid)
            buff0 = BIT_SELECT(src_val0, MWORD_MSB, src_begin);

        }

        //hi
        buff1 = BIT_SELECT(src_val1, size_hi+size_mid-1, size_mid);

//_d(buff0);
//_d(buff1);

        size_lo  = dest_comp;
        size_mid = size_arg-(size_hi+size_lo);

        mask_mid = NBIT_LO_MASK(size_mid);
        mask_lo  = NBIT_HI_MASK(size_lo);
        mask_hi  = BIT_RANGE((size_hi+size_mid-1),size_mid);

        ldv(dest,0) = MWORD_MUX(buff0 << dest_begin, rdv(dest,0), mask_lo);
        ldv(dest,1) = MWORD_MUX(buff0 >> dest_comp,  rdv(dest,1), mask_mid);
        ldv(dest,1) = MWORD_MUX(buff1 << size_mid,   rdv(dest,1), mask_hi);

    }

}


// dest:2 src:1
//
void array1_move_split_2_1(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg){ // array1_move_split_2_1#

    //               1                0
    //             A      B       C
    // src:  |--v-----v-------|--------v-------|
    //          |-----|----------------|
    //                     |-----|----------------|
    // dest: |.............^..|..^.............|..^.............|
    //                      A  B        C       D

    mword src_comp, dest_comp;
    mword size_A, size_B, size_C;
    mword src_val0, src_val1;
    mword buff0, buff1;

    src_comp  = MWORD_BIT_SIZE-src_begin;
    dest_comp = MWORD_BIT_SIZE-dest_begin;

    size_A = size_arg-MWORD_BIT_SIZE;
    size_C = src_comp;
    size_B = size_arg-(size_A+size_C);

    src_val0 = rdv(src,0);
    src_val1 = rdv(src,1);

    if(size_B){

        buff0 =
            //lo
            BIT_SELECT(src_val0, MWORD_MSB, src_begin)
                |
            //mid
            (BIT_SELECT(src_val1, size_B-1, 0) << size_C);

    }
    else{

        //lo (no mid)
        buff0 = BIT_SELECT(src_val0, MWORD_MSB, src_begin);

    }

    //hi
    buff1 = BIT_SELECT(src_val1, size_A+size_B-1, size_B);

//_d(buff0);
//_d(buff1);

    ldv(dest,0) = MWORD_MUX(
                    (buff0<<dest_begin),
                    rdv(dest,0),
                    BIT_RANGE(MWORD_MSB,dest_begin));

    ldv(dest,1) = MWORD_MUX(
                    (buff0>>dest_comp),
                    rdv(dest,1),
                    BIT_RANGE(dest_begin-1,0));

    ldv(dest,1) = MWORD_MUX(
                    (buff1<<dest_begin),
                    rdv(dest,1),
                    BIT_RANGE(MWORD_MSB,dest_begin));

    ldv(dest,2) = MWORD_MUX(
                    (buff1>>dest_comp),
                    rdv(dest,2),
                    BIT_RANGE((size_arg-(MWORD_BIT_SIZE+dest_comp)-1),0));

}


// dest:1 src:2
//
void array1_move_split_1_2(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg){ // array1_move_split_1_2#

    //               1                0
    //                      A  B        C       D
    // src:  |.............v..|..v.............|..v.............|
    //                     |-----|----------------|
    //          |-----|----------------|
    // dest: |--^-----^-------|--------^-------|
    //             A      B       C

    mword src_comp, dest_comp;
    mword size_A, size_B, size_C, size_D;
    mword src_val0, src_val1, src_val2;
    mword buff0, buff1;

    src_comp  = MWORD_BIT_SIZE-src_begin;
    dest_comp = MWORD_BIT_SIZE-dest_begin;

    size_D = src_comp;
    size_B = size_D;
    size_C = src_begin;
    size_A = size_arg-(size_B+size_C+size_D);

    src_val0 = rdv(src,0);
    src_val1 = rdv(src,1);
    src_val2 = rdv(src,2);

    buff0 =
        BIT_SELECT(src_val0, MWORD_MSB, src_begin)
            |
        (BIT_SELECT(src_val1, size_C-1, 0) << size_D);

    buff1 =
        BIT_SELECT(src_val1, MWORD_MSB, size_C)
            |
        (BIT_SELECT(src_val2, size_A-1, 0) << size_B);

//_d(buff0);
//_d(buff1);

    ldv(dest,0) = MWORD_MUX(
                    (buff0<<dest_begin),
                    rdv(dest,0),
                    BIT_RANGE(MWORD_MSB,dest_begin));

    ldv(dest,1) = MWORD_MUX(
                    (buff0>>dest_comp),
                    rdv(dest,1),
                    BIT_RANGE(dest_begin-1,0));

    ldv(dest,1) = MWORD_MUX(
                    (buff1<<dest_begin),
                    rdv(dest,1),
                    BIT_RANGE((size_arg-MWORD_BIT_SIZE+dest_begin-1),dest_begin));

}


// dest>=2, src>=2
//
void array1_move_split_n(babel_env *be, mword *dest, int dest_begin, mword *src, int src_begin, mword size_arg){ // array1_move_split_n#

    // Main loop (0 to n-1):
    //               1                0
    //                  hi       lo
    // src:  |-----v----------|------v---------|
    //             |-----------------|
    //                   |-----------------|
    // dest: |...........^....|............^...|
    //                     hi       lo
    //
    // Final loop: Use array1_move_split_1_1() to move last bits
    mword src_comp,     dest_comp;
    mword dest_size_hi, dest_size_lo;
    mword dest_mask_hi, dest_mask_lo;
    mword src_mask_hi;
    mword buff;

    int i, ctr;

    src_comp  = MWORD_BIT_SIZE-src_begin;
    dest_comp = MWORD_BIT_SIZE-dest_begin;

    src_mask_hi = NBIT_LO_MASK(src_begin);

    dest_size_lo = dest_comp;
    dest_size_hi = dest_begin;

    dest_mask_hi = NBIT_LO_MASK(dest_size_hi);
    dest_mask_lo = NBIT_HI_MASK(dest_size_lo);

    for(    i=size_arg,        ctr=0;
            i>MWORD_BIT_SIZE;
            i-=MWORD_BIT_SIZE, ctr++){

        buff = BIT_SELECT(rdv(src,ctr+0), MWORD_MSB, src_begin)
                        |
                     ((rdv(src,ctr+1) & src_mask_hi) << src_comp);

//_d(buff);

        ldv(dest,ctr+0) = MWORD_MUX(buff << dest_begin, rdv(dest,ctr+0), dest_mask_lo);
        ldv(dest,ctr+1) = MWORD_MUX(buff >> dest_comp,  rdv(dest,ctr+1), dest_mask_hi);

    }

//_dd(i);
//_dd(ctr);

    //clean up remaining bits
    array1_move_unsafe(be, (dest+ctr), dest_begin, (src+ctr), src_begin, i);

}



/*****************************************************************************
 *                                                                           *
 *                               ARRAY MISC                                  *
 *                                                                           *
 ****************************************************************************/

// shrinks an array in-place, where applicable
// new_begin and new_end are not sanity-checked here; MUST BE checked by the caller
//
mword *array_shrink(babel_env *be, mword *array, mword new_begin, mword new_end, access_size asize){ // array_shrink#

    mword new_size;
    mword *result = array;
    mword new_sfield, new_align;

    if(asize == MWORD_ASIZE){

        new_size = new_end-new_begin+1;

        result+=new_begin;
        if(is_val(result)){
            sfield(result) = UNITS_MTO8(new_size);
        }
        else if(is_ptr(result)){ //is_ptr
            sfield(result) = (int)-1*(UNITS_MTO8(new_size));
        }
        //else: do nothing, tptrs can't be trunc'd

    }
    else if(asize == U8_ASIZE){ // NB: new_begin, new_end are byte-offsets

        new_size = new_end-new_begin;

        if(!is_val(result)) return result;

        if(new_begin){
            if(MODULO_MTO8(new_begin)){ // not MWORD-aligned
                memmove( ((char*)result), ((char*)result+new_begin), (size_t)new_size );
            }
            else{ // MWORD-aligned
                result+=UNITS_8TOM(new_begin);
            }
        }

        new_sfield = UNITS_MTO8(array8_mword_size(new_size));
        new_align = array8_enc_align(new_size);
        sfield(result) = new_sfield;
        ldv(result,UNITS_8TOM(new_sfield)-1)  =   new_align;
        ldv(result,UNITS_8TOM(new_sfield)-2) &= ~(new_align); // padding bytes MUST BE ZERO

    }
    else{ // if(asize == U1_ASIZE){
        _enhance("asize == U1_ASIZE");
    }

    return result;

}


//
//
mword *array_to_string(babel_env *be, mword *array){ // array_to_string#

    if(is_nil(array)){
        return mem_new_val(be, 1, 0);
    }

    mword *result;
    #define MAX_UTF8_CHAR_SIZE 4

    mword arsize = size(array);
    int temp_buffer_size = MAX_UTF8_CHAR_SIZE * (arsize);

    // free'd below
    char *temp_buffer = mem_sys_alloc( temp_buffer_size ); // XXX WAIVER(mem_sys_alloc) XXX //
    
    mword utf8_length = (mword)u8_toutf8(temp_buffer, temp_buffer_size, (uint32_t *)array, arsize) - 1;

    mword arlength = (utf8_length / 4) + 1;

    if(utf8_length % 4){
        arlength++;
    }

    result = mem_new_val(be, arlength,0);
    memcpy(result, temp_buffer, utf8_length);
    mem_sys_free(temp_buffer);

    ldv(result,arlength-1) = array8_enc_align(utf8_length);

    return result;

}



// XXX The return-value from this function contains unsafe pointers!!! XXX
// XXX internal interp use ONLY                                        XXX
// XXX If you pass tag=nil, returns ALL tags in bs                     XXX
// XXX PERF: A _tags2ar (like bstruct_to_array) would be more efficient          XXX
//
mword *array_find_val(babel_env *be, mword *bs, mword *val){ // array_find_val#

    mword *span_array     = bstruct_to_array(be, bs);
    mword size_span_array = size(span_array);
    mword size_ptr;
    mword *val_list       = be->nil;
    mword *curr_span_elem;
    mword *curr_ptr_elem;

    int i,j;
    for(i=0; i<size_span_array; i++){

        curr_span_elem = rdp(span_array,i);

        if(is_ptr(curr_span_elem)){ // check each element

            size_ptr = size(curr_span_elem);

            for(j=0;j<size_ptr;j++){

                curr_ptr_elem = rdp(curr_span_elem,j);

                if(is_nil(curr_ptr_elem)){
                    continue;
                }

                if(is_val(curr_ptr_elem)){

                    if( !array_cmp_lex(curr_ptr_elem, val, MWORD_ASIZE) ){

                        // push onto val_list
                        if(is_nil(val_list)){
                            val_list = list_cons(be, (curr_span_elem+j), be->nil);
                        }
                        else{
                            list_unshift(be, val_list, (curr_span_elem+j));
                        }

                    }

                }

            }

        }

    }

    return val_list;

}


// XXX The return-value from this function contains unsafe pointers!!! XXX
// XXX internal interp use ONLY                                        XXX
// XXX If you pass tag=nil, returns ALL tags in bs                     XXX
// XXX PERF: A _tags2ar (like bstruct_to_array) would be more efficient          XXX
//
mword *array_find_ptr(babel_env *be, mword *bs, mword *ptr){ // array_find_ptr#

    mword *span_array     = bstruct_to_array(be, bs);
    mword size_span_array = size(span_array);
    mword size_ptr;
    mword *ptr_list       = be->nil;
    mword *curr_span_elem;
    mword *curr_ptr_elem;

    int i,j;
    for(i=0; i<size_span_array; i++){

        curr_span_elem = rdp(span_array,i);

        if(is_ptr(curr_span_elem)){ // check each element

            size_ptr = size(curr_span_elem);

            for(j=0;j<size_ptr;j++){

                curr_ptr_elem = rdp(curr_span_elem,j);

                if(is_nil(curr_ptr_elem)){
                    continue;
                }

                if(is_ptr(curr_ptr_elem)){

                    if( !array_cmp_lex(curr_ptr_elem, ptr, MWORD_ASIZE) ){

                        // push onto ptr_list
                        if(is_nil(ptr_list)){
                            ptr_list = list_cons(be, (curr_span_elem+j), be->nil);
                        }
                        else{
                            list_unshift(be, ptr_list, (curr_span_elem+j));
                        }

                    }

                }

            }

        }

    }

    return ptr_list;

}


//
//
mword *array_to_list(babel_env *be, mword *arr){ // array_to_list#

    mword *last_cons = be->nil;
    int i;
    mword *entry;

    if(is_ptr(arr)){
        for(i=size(arr)-1;i>=0;i--){
            last_cons = list_cons(be, rdp(arr,i),last_cons);
        }
    }
    else{
        for(i=size(arr)-1;i>=0;i--){
            entry = mem_new_val(be, 1, 0);
            *entry = rdv(arr,i);
            last_cons = list_cons(be, entry,last_cons);
        }
    }

    return last_cons;

}


//
//
void array_build_max_heap(mword *array){

    mword array_size = size(array);
    int i = array_size;
    i = (i%2) ? (i/2) : ((i/2)-1);

    for(;i>=0;i--){
        array_max_heapify(array, i, array_size);
    }

}


//
//
void array_max_heapify(mword *array, mword i, mword array_size){

    mword left  = 2*i+1;
    mword right = 2*i+2;
    mword largest = i;

    mword *temp;

    if( (left < array_size)
            && 
        val_gt(key_aop(array,left), key_aop(array,largest)) ){
        largest = left;
    }

    if( (right < array_size) 
            && 
        val_gt(key_aop(array,right), key_aop(array,largest)) ){
        largest = right;
    }

    if(largest != i){
        temp = rdp(array,i);
        ldp(array,i) = rdp(array,largest);
        ldp(array,largest) = temp;
        array_max_heapify(array, largest, array_size);
    }

}


/*****************************************************************************
 *                                                                           *
 *                             ARRAY SEARCH                                  *
 *                                                                           *
 ****************************************************************************/


// array must be in sorted order (non-decreasing)
// NOTE: It might be nice to know where an element *would* fit in the array
//       if it were inserted. Need two return values - whether the element was
//       found, and where the search found the closest match (return a 
//       boundary index)
//
mword array_search(babel_env *be, mword *array, mword *target, sort_type st){ // array_search#

    return array_search_binary(be, array, (mword*)(array+size(array)), target, st);

}


// XXX(INTERNAL_USE_ONLY)
//
mword array_search_binary(babel_env *be, mword *begin, mword *end, mword *target, sort_type st){ // array_search_binary#

    mword *array = begin;
    int array_size = (begin-end);

    if(array_size <= ARRAY_LINEAR_THRESH)
        return array_search_linear(be,
                    array,
                    0,
                    array_size,
                    target,
                    st);

    mword target_val;

    int shift       = array_size >> 1;
    int guess_index = shift;
        shift       >>= 1;

    int lower_bound = 0;
    int upper_bound = array_size;

    int local_array_size = array_size>>ARRAY_PERF_LINEAR_HOLDOFF;

    //////////////////////////// val-array //////////////////////////////////
    if(is_val(array) || (st == VAL_ST)){

        mword guess;
        target_val = vcar(target);

        while(local_array_size){

            guess = rdv(array,guess_index);

            if(guess < target_val){
                lower_bound = guess_index;
                guess_index += shift;
            }
            else if(guess > target_val){
                upper_bound = guess_index;
                guess_index -= shift;
            }
            else if(guess == target_val){
                return guess_index;
            }
            else{
                _pigs_fly;
            }

            shift >>= 1;
            shift = (shift == 0) ? 1 : shift;

            local_array_size >>= 1;

        }

        return array_search_linear(be,
                    array,
                    lower_bound,
                    upper_bound+1,
                    target,
                    st);

    }
    else{

        mword *guess;

        ////////////////////////// numeric //////////////////////////////////
        if(st == SIGNED_ST || st == UNSIGNED_ST){

            while(local_array_size){

                guess = key_aop(array,guess_index);

                if(array_lt_num(guess, target)){
                    lower_bound = guess_index;
                    guess_index += shift;
                }
                else if(array_gt_num(guess, target)){
                    upper_bound = guess_index;
                    guess_index -= shift;
                }
                else if(array_eq_num(guess, target)){
                    return guess_index;
                }
                else{
                    _pigs_fly;
                }

                shift >>= 1;
                shift = (shift == 0) ? 1 : shift;

                local_array_size >>= 1;

            }

            return array_search_linear(be,
                        array,
                        lower_bound,
                        upper_bound+1,
                        target,
                        st);

        }
        ////////////////////////// aop-array ////////////////////////////////
        else if(st == ALPHA_MWORD_ST || st == LEX_MWORD_ST){

            while(local_array_size){

                guess = key_aop(array,guess_index);

                if(array_lt(be, guess, target)){
                    lower_bound = guess_index;
                    guess_index += shift;
                }
                else if(array_gt(be, guess, target)){
                    upper_bound = guess_index;
                    guess_index -= shift;
                }
                else if(array_eq(be, guess, target)){
                    return guess_index;
                }
                else{
                    _pigs_fly;
                }

                shift >>= 1;
                shift = (shift == 0) ? 1 : shift;

                local_array_size >>= 1;

            }

            return array_search_linear(be,
                        array,
                        lower_bound,
                        upper_bound+1,
                        target,
                        st);

        }
        ////////////////////////// aop-array8 ///////////////////////////////
        else if(st == ALPHA_BYTE_ST || st == LEX_BYTE_ST){

            while(local_array_size){

                guess = key_aop(array,guess_index);

                if(array8_lt(be, guess, target)){
                    lower_bound = guess_index;
                    guess_index += shift;
                }
                else if(array8_gt(be, guess, target)){
                    upper_bound = guess_index;
                    guess_index -= shift;
                }
                else if(array8_eq(be, guess, target)){
                    return guess_index;
                }
                else{
                    _pigs_fly;
                }


                shift >>= 1;
                shift = (shift == 0) ? 1 : shift;

                local_array_size >>= 1;

            }

            return array_search_linear(be,
                        array,
                        lower_bound,
                        upper_bound+1,
                        target,
                        st);

        }
        else{
            _enhance("st == CUSTOM_ST");
        }

        return ARRAY_SEARCH_NOT_FOUND;
    }

    _pigs_fly;

    return ARRAY_SEARCH_NOT_FOUND; // silence compiler warning...

}



//
//
mword array_search_linear(babel_env *be, mword *array, int start, int end, mword *target, sort_type st){ // array_search_linear#

    start = (start < 0) ? 0 : start;
    end   = (end   > size(array)) ? size(array) : end;

    int i=start;
    mword target_val;

    if(is_val(array) || st == VAL_ST){
        target_val = vcar(target);
        for(; i<end; i++){
            if(rdv(array,i) == target_val){
                return i;
            }
        }
        return -1; // we didn't find what you were looking for...
    }
    else if(st == SIGNED_ST || st == UNSIGNED_ST){ // numeric
        for(; i<end; i++){
            if(array_eq_num(key_aop(array,i), target)){
                return i;
            }
        }
    }
    else if(st == ALPHA_MWORD_ST || st == LEX_MWORD_ST){
        for(; i<end; i++){
            if(array_eq(be, key_aop(array,i), target)){
                return i;
            }
        }
    }
    else if(st == ALPHA_BYTE_ST || st == LEX_BYTE_ST){
        for(; i<end; i++){
            if(array8_eq(be, key_aop(array,i), target)){
                return i;
            }
        }
    }
    else{
        _enhance("st == CUSTOM_ST");
    }

    return -1; // we didn't find what you were looking for...

}


// Clayton Bauman 2018

