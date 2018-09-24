// sort.c
//

#include "babel.h"
#include "sort.h"
#include "array.h"
#include "mem.h"
//#include "utf8.h"
//#include "bstruct.h"
//#include "list.h"


// XXX(PERF): INSERTION SORT when (right - left) <= 8 or so
//      sort_insertion() works but needs to handle different comparison types
// 
// XXX: Need to parametrize the comparison function, both for built-in 
//      comparisons and for user-supplied comparisons


/*****************************************************************************
 *                                                                           *
 *                              ARRAY SORT                                   *
 *                                                                           *
 ****************************************************************************/


// In-place mergesort of an array
// st can be one of: {UNSIGNED, SIGNED, ALPHA_MWORD, ALPHA_BYTE, LEX_MWORD, LEX_BYTE, VAL, CUSTOM}
// XXX we need to build an internal version of this function XXX
// XXX that takes start-stop pointers; this will allow it to XXX
// XXX to be called by other internal functions              XXX
//
void sort(babel_env *be, mword *array, sort_type st){ // sort#

    if(is_val(array) || (st == VAL)){ // ((st == VAL) && (!is_val(array))) means "sort ptr-array as a val-array"
        sort_r(pc, 0, size(array), array, st);
    }
    else{
        sort_aop(pc, array, 0, st);
    }

}

#define merge_pre_sort                                                      \
    mword left_length  = left_end  - left_start;                            \
    mword right_length = right_end - right_start;                           \
                                                                            \
    mword *left_half  = mem_new_val(pc,left_length,0);                \
    mword *right_half = mem_new_val(pc,right_length,0);               \
                                                                            \
    mword r = 0;                                                            \
    mword l = 0;                                                            \
    mword i = 0;                                                            \
                                                                            \
    for(i = left_start; i<left_end; i++, l++){                              \
        left_half[l] = array[i];                                            \
    }                                                                       \
                                                                            \
    for(i = right_start; i<right_end; i++, r++){                            \
        right_half[r] = array[i];                                           \
    }

#define merge_post_sort                                                     \
    for(; l<left_length; i++, l++){                                         \
        array[i] = left_half[l];                                            \
    }                                                                       \
                                                                            \
    for (; r<right_length; i++, r++){                                       \
        array[i] = right_half[r];                                           \
    }


//
//
void sort_r(babel_env *be, mword left, mword right, mword *array, sort_type st){ // sort_r#

    // base case, array is already sorted
    if (right - left <= 1){
        return;
    }

    // set up bounds to slice array into
    mword left_start  = left;
    mword left_end    = (left+right)/2;
    mword right_start = left_end;
    mword right_end   = right;

    // sort left half
    sort_r(pc, left_start, left_end, array, st);

    // sort right half
    sort_r(pc, right_start, right_end, array, st);

    // merge sorted halves back together:
    merge_pre_sort;

    // merge left_half and right_half back into array
    for(i=left_start, r=0, l=0; l<left_length && r<right_length; i++){
        if(left_half[l] < right_half[r]){
            array[i] = left_half[l++];
        }
        else{
            array[i] = right_half[r++];
        }
    }

    merge_post_sort;

}


//sort_aop(pc, size(array), array, st);
void sort_aop(babel_env *be, mword *array, mword key_index, sort_type st){ // sort_aop#

    if(st == UNSIGNED){
        sort_aop_unsigned(pc, 0, size(array), array, key_index);
    }
    else if(st == SIGNED){
        sort_aop_signed(pc, 0, size(array), array, key_index);
    }
    else if(st == ALPHA_MWORD){
        sort_aop_alpha_mword(pc, 0, size(array), array, key_index);
    }
    else if(st == ALPHA_BYTE){
        sort_aop_alpha_byte(pc, 0, size(array), array, key_index);
    }
    else if(st == LEX_MWORD){
        sort_aop_lex_mword(pc, 0, size(array), array, key_index);
    }
    else if(st == LEX_BYTE){
        sort_aop_lex_byte(pc, 0, size(array), array, key_index);
    }
    // XXX DON'T FORGET ABOUT CUSTOM SORT XXX
    else{
        _pigs_fly;
    }

} 

//
//
void sort_aop_unsigned(babel_env *be, mword left, mword right, mword *array, mword key_index){ // sort_aop_unsigned#

    // base case, array is already sorted
    if (right - left <= 1){
        return;
    }

    // set up bounds to slice array into
    mword left_start  = left;
    mword left_end    = (left+right)/2;
    mword right_start = left_end;
    mword right_end   = right;

    // sort left half
    sort_aop_unsigned(pc, left_start, left_end, array, key_index);

    // sort right half
    sort_aop_unsigned(pc, right_start, right_end, array, key_index);

    // merge sorted halves back together:
    merge_pre_sort;

    // merge left_half and right_half back into array
    for(i=left_start, r=0, l=0; l<left_length && r<right_length; i++){
        if(val_lt(key_aop(left_half,l), key_aop(right_half,r))){ // numeric
            array[i] = left_half[l++];
        }
        else{
            array[i] = right_half[r++];
        }
    }

    merge_post_sort;

}


//
//
void sort_aop_signed(babel_env *be, mword left, mword right, mword *array, mword key_index){ // sort_aop_signed#

    // base case, array is already sorted
    if (right - left <= 1){
        return;
    }

    // set up bounds to slice array into
    mword left_start  = left;
    mword left_end    = (left+right)/2;
    mword right_start = left_end;
    mword right_end   = right;

    // sort left half
    sort_aop_signed(pc, left_start, left_end, array, key_index);

    // sort right half
    sort_aop_signed(pc, right_start, right_end, array, key_index);

    // merge sorted halves back together:
    merge_pre_sort;

    // merge left_half and right_half back into array
    for(i=left_start, r=0, l=0; l<left_length && r<right_length; i++){
        if((array_cmp_num_signed(key_aop(left_half,l), key_aop(right_half,r)) < 0)){
            array[i] = left_half[l++];
        }
        else{
            array[i] = right_half[r++];
        }
    }

    merge_post_sort;

}


//
//
void sort_aop_alpha_mword(babel_env *be, mword left, mword right, mword *array, mword key_index){ // sort_aop_alpha_mword#

    // base case, array is already sorted
    if (right - left <= 1){
        return;
    }

    // set up bounds to slice array into
    mword left_start  = left;
    mword left_end    = (left+right)/2;
    mword right_start = left_end;
    mword right_end   = right;

    // sort left half
    sort_aop_alpha_mword(pc, left_start, left_end, array, key_index);

    // sort right half
    sort_aop_alpha_mword(pc, right_start, right_end, array, key_index);

    // merge sorted halves back together:
    merge_pre_sort;

    for(i=left_start, r=0, l=0; l<left_length && r<right_length; i++){
        if((array_cmp_alpha(pc, key_aop(left_half,l), key_aop(right_half,r), MWORD_ASIZE) < 0)){
            array[i] = left_half[l++];
        }
        else{
            array[i] = right_half[r++];
        }
    }

    merge_post_sort;

}


//
//
void sort_aop_alpha_byte(babel_env *be, mword left, mword right, mword *array, mword key_index){ // sort_aop_alpha_byte#

    // base case, array is already sorted
    if (right - left <= 1){
        return;
    }

    // set up bounds to slice array into
    mword left_start  = left;
    mword left_end    = (left+right)/2;
    mword right_start = left_end;
    mword right_end   = right;

    // sort left half
    sort_aop_alpha_byte(pc, left_start, left_end, array, key_index);

    // sort right half
    sort_aop_alpha_byte(pc, right_start, right_end, array, key_index);

    // merge sorted halves back together:
    merge_pre_sort;

    for(i=left_start, r=0, l=0; l<left_length && r<right_length; i++){
        if((array_cmp_alpha(pc, key_aop(left_half,l), key_aop(right_half,r), BYTE_ASIZE) < 0)){
            array[i] = left_half[l++];
        }
        else{
            array[i] = right_half[r++];
        }
    }

    merge_post_sort;

}



//
//
void sort_aop_lex_mword(babel_env *be, mword left, mword right, mword *array, mword key_index){ // sort_aop_lex_mword#

    // base case, array is already sorted
    if (right - left <= 1){
        return;
    }

    // set up bounds to slice array into
    mword left_start  = left;
    mword left_end    = (left+right)/2;
    mword right_start = left_end;
    mword right_end   = right;

    // sort left half
    sort_aop_lex_mword(pc, left_start, left_end, array, key_index);

    // sort right half
    sort_aop_lex_mword(pc, right_start, right_end, array, key_index);

    // merge sorted halves back together:
    merge_pre_sort;

    for(i=left_start, r=0, l=0; l<left_length && r<right_length; i++){
        if((array_cmp_lex(pc, key_aop(left_half,l), key_aop(right_half,r), MWORD_ASIZE) < 0)){
            array[i] = left_half[l++];
        }
        else{
            array[i] = right_half[r++];
        }
    }

    merge_post_sort;

}


//
//
void sort_aop_lex_byte(babel_env *be, mword left, mword right, mword *array, mword key_index){ // sort_aop_lex_byte#

    // base case, array is already sorted
    if (right - left <= 1){
        return;
    }

    // set up bounds to slice array into
    mword left_start  = left;
    mword left_end    = (left+right)/2;
    mword right_start = left_end;
    mword right_end   = right;

    // sort left half
    sort_aop_lex_byte(pc, left_start, left_end, array, key_index);

    // sort right half
    sort_aop_lex_byte(pc, right_start, right_end, array, key_index);

    // merge sorted halves back together:
    merge_pre_sort;

    for(i=left_start, r=0, l=0; l<left_length && r<right_length; i++){
        if((array_cmp_lex(pc, key_aop(left_half,l), key_aop(right_half,r), BYTE_ASIZE) < 0)){
            array[i] = left_half[l++];
        }
        else{
            array[i] = right_half[r++];
        }
    }

    merge_post_sort;

}


// Internal-use only...
//
void sort_insertion(babel_env *be, mword *begin, mword *end, mword array_type, sort_type st, mword key_index){ // sort_insertion#

    mword *array = begin;
    mword length = (end-begin);

    if(length < 2) // array is sorted by definition
        return;

    // |        SORTED       |        UNSORTED     |
    // |------+--------------|x--------------------|
    //    <x  |  <=x          ^curr

    int curr;
    int i;
    mword x;
    mword *shift_point;

    for(curr=1;curr<length;curr++){

        x = array[curr];

        for(i=0;i<curr;i++){ // linear search through unsorted
            if(array[i] < x) continue;
            break;
        }

        if(i==curr) continue; // do nothing, this element is already sorted

        shift_point = array+i;
        memcpy((shift_point+1),shift_point,UNITS_MTO8(curr-i));

        array[i] = x;

    }

//            if(left_half[l] < right_half[r]){
//                if(val_lt(key_aop(left_half,l), key_aop(right_half,r))){ // numeric
//                if((array_cmp_num_signed(key_aop(left_half,l), key_aop(right_half,r)) < 0)){
//                if((array_cmp_alpha(pc, key_aop(left_half,l), key_aop(right_half,r), MWORD_ASIZE) < 0)){
//                if((array_cmp_alpha(pc, key_aop(left_half,l), key_aop(right_half,r), BYTE_ASIZE) < 0)){
//                if((array_cmp_lex(pc, key_aop(left_half,l), key_aop(right_half,r), MWORD_ASIZE) < 0)){
//                if((array_cmp_lex(pc, key_aop(left_half,l), key_aop(right_half,r), BYTE_ASIZE) < 0)){

}



// Clayton Bauman 2018

