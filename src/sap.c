// sap.c
//

#include "babel.h"
#include "sap.h"
#include "aop.h"
#include "mem.h"
#include "bstruct.h"
#include "introspect.h"


// sorted array-of-pairs notes:
//
//  sap is optimized for read/update. It is not suitable for insert/remove
//  operations. Use a trie instead. An sap can have any sort-type but an sap
//  used with probing is restricted to tags sorted by UNSIGNED_ST sort-type.


//
//
mword *sap_lookup(babel_env *be, mword *sap, mword *key, search_type s, sort_type st){



}


//
//
void sap_update(babel_env *be, mword *sap, mword *key, mword *payload, search_type s, sort_type st){



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

    mword *search_key_cons[2];
    mword *search_key = (mword*)&search_key_cons;

    search_key_cons[0] = key;
    search_key_cons[1] = be->nil;

    for(; i<end; i++){
        if(cmp_fn(&(rdp(sap,i)), &search_key) == 0){
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

    mword *search_key_cons[2];
    mword *search_key = (mword*)&search_key_cons;

    search_key_cons[0] = key;
    search_key_cons[1] = be->nil;

    while(local_sap_size){

        comparison = cmp_fn(&(rdp(sap,guess_index)), &search_key);

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
        shift = (shift < 1) ? 1 : shift;

        local_sap_size >>= 1;

    }

    return sap_find_index_linear(be, sap, lower_bound, upper_bound+1, key, st);

}


// probing only works with std hashed keys (UNSIGNED_ST)
//
mword sap_find_index_probe(babel_env *be, mword *sap, mword *key){

    mword sap_size = size(sap);

    if(sap_size <= SAP_LINEAR_THRESH)
        return sap_find_index_linear(be, sap, 0, sap_size, key, UNSIGNED_ST);

    csort_fn cmp_fn = aop_select_cmp_fn(UNSIGNED_ST);

    // FIXME: 64-bit specific:
    double partition_num = (double)(key[1]) / (double)ULONG_MAX;

    mword guess_index = sap_size * partition_num;

    mword probe_range = (sap_size/SAP_PROBE_REGION_DENOM);

    int lower_bound = guess_index-probe_range;
    int upper_bound = guess_index+probe_range;

    mword result = sap_find_index_linear(be,
                            sap,
                            lower_bound,
                            upper_bound,
                            key,
                            UNSIGNED_ST);

    if(result == NEG_ONE){

        while(1){

            lower_bound = (lower_bound < 1) ? 0 : lower_bound;
            upper_bound = (upper_bound > sap_size) ? sap_size : upper_bound;

            int lower_comparison = cmp_fn(&key, &(rdp(sap,lower_bound)));
            int upper_comparison = cmp_fn(&key, &(rdp(sap,upper_bound)));

            if((lower_comparison >= 0) && (upper_comparison <= 0)){
                return result;
            }

            probe_range *= 2;
            lower_bound = guess_index-probe_range;
            upper_bound = guess_index+probe_range;

            result = sap_find_index_linear(be,
                                    sap,
                                    lower_bound,
                                    upper_bound,
                                    key,
                                    UNSIGNED_ST);

            if((lower_bound < 1) && (upper_bound >= sap_size)){
                return result;
            }

        }
    }
    else{
        return result;
    }

}


// Clayton Bauman 2018

