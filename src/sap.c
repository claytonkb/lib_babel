// sap.c
//

#include "babel.h"
#include "sap.h"
#include "aop.h"
#include "mem.h"
#include "bstruct.h"
#include "introspect.h"

// NOTE: The csort_fn expects keys to be in pair form. When calling an sap_*
// function, be sure to "wrap" a bare hash key in a pair, as follows:
//
//      sap_key = list_cons(be, hash_key, be->nil);


//
//
void sap_overwrite(babel_env *be, mword *sap, mword *key, mword *payload, search_type s, sort_type st){



}


//
//
mword *sap_lookup(babel_env *be, mword *sap, mword *key, search_type s, sort_type st){
}


//
//
mword  sap_remove(babel_env *be, mword *sap, mword *key, search_type s, sort_type st){
}


//
//
mword *sap_entries(babel_env *be, mword *sap){
}


// returns NEG_ONE if key is not found
//
mword sap_find_index(babel_env *be, mword *sap, mword *key, search_type s, sort_type st){

    switch(s){
        case LINEAR_S:
            return sap_find_index_linear(be, sap, 0, size(sap), key, st);
        case BINARY_S:
            return sap_find_index_binary(be, sap, key, st);
        case PROBE_S:
            return sap_find_index_probe(be, sap, key);
        default:
            _pigs_fly;
    }

}


// returns NEG_ONE if no match found
//
mword sap_find_index_linear(babel_env *be, mword *sap, mword start, mword end, mword *key, sort_type st){

    mword  sap_size = size(sap);

    start = (start < 0) ? 0 : start;
    end   = (end   > sap_size) ? sap_size : end;

    int i=start;

    csort_fn cmp_fn = aop_select_cmp_fn(st);

    for(; i<end; i++){
        if(cmp_fn(&key, &(rdp(sap,i))) == 0){
            return i;
        }
    }

    return NEG_ONE;

}


//
//
mword sap_find_index_binary(babel_env *be, mword *sap, mword *key, sort_type st){

    mword sap_size = size(sap);

    if(sap_size <= SAP_LINEAR_THRESH)
        return sap_find_index_linear(be, sap, 0, sap_size, key, st);

    int shift       = sap_size >> 1;
    int guess_index = shift;
        shift     >>= 1;

    int lower_bound = 0;
    int upper_bound = sap_size;

    int local_sap_size = sap_size >> SAP_LINEAR_HOLDOFF;

    csort_fn cmp_fn = aop_select_cmp_fn(st);
    int comparison;

    while(local_sap_size){

        comparison = cmp_fn(&key, &(rdp(sap,guess_index)));

        if(comparison < 0){
            lower_bound = guess_index;
            guess_index += shift;
        }
        else if(comparison > 0){
            upper_bound = guess_index;
            guess_index -= shift;
        }
        else{ // comparison == 0
            return guess_index;
        }

        shift >>= 1;
        shift = (shift == 0) ? 1 : shift;

        local_sap_size >>= 1;

    }

    return sap_find_index_linear(be, sap, lower_bound, upper_bound+1, key, st);

}


// probing only works with std hashed keys (LEX_MWORD_ST)
//
mword sap_find_index_probe(babel_env *be, mword *sap, mword *key){

    mword sap_size = size(sap);

    if(sap_size <= SAP_LINEAR_THRESH)
        return sap_find_index_linear(be, sap, 0, sap_size, key, LEX_MWORD_ST);

    csort_fn cmp_fn = aop_select_cmp_fn(LEX_MWORD_ST);
//    int comparison;

//    mword guess_index = ();
//
//
//    while(local_sap_size){
//
//        comparison = cmp_fn(key, rdp(sap,guess_index));
//
//        if(comparison < 0){
//            lower_bound = guess_index;
//            guess_index += shift;
//        }
//        else if(comparison > 0){
//            upper_bound = guess_index;
//            guess_index -= shift;
//        }
//        else{ // comparison == 0
//            return guess_index;
//        }
//
//        shift >>= 1;
//        shift = (shift == 0) ? 1 : shift;
//
//        local_sap_size >>= 1;
//
//    }
//
//    return sap_find_index_linear(be, sap, lower_bound, upper_bound+1, key, st);

}


// Clayton Bauman 2018

