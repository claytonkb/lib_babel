// sap.c
//

#include "babel.h"
#include "sap.h"
#include "aop.h"
#include "mem.h"
#include "bstruct.h"
#include "list.h"
#include "introspect.h"


// sorted array-of-pairs notes:
//
//  sap is optimized for read/update on a fixed array.
//
//  You can remove an element from the sap by setting its entry to be->nil or
//  use sap_remove(). Note that sap_remove() only works with PROBE_S on std tags.
//
//  Empty slots can be filled by inserting a pair in that slot. Note that 
//  sap_insert only works with PROBE_S on std tags. Insert will test the target
//  slot first, then it will search the probe_range for an empty slot if one
//  is not found.
//
//  Existence test for x is done by:
//
//      sap_lookup(x) != be->non_exist
//
//  If you're operating on a data-set that is dynamic in size, you're probably
//  better off using a hash-trie instead.
//
//  If you're operating on a fixed data-set, first construct a trie then
//  convert to an sap. This allows 100% utilization of the array.
//
//  If you can have some non-zero failure-rate (wrong data returned during
//  lookup, or fail to store data), then consider using pds_la_* (lossy arrays)
//  instead. sap is fast but failed lookups may have to potentially search the
//  entire array to ensure that returning NEG_ONE is correct. (Ensuring the
//  sap stays in proper sorted order can protect against this. sap_insert()
//  and sap_remove() do NOT sort the array after operating on it.)
//
//  You can implement multiple hashing to get better utilization of an sap array
//  when using insert/remove. Just apply first hash function, search, then second
//  hash function, search, and so on. sap_insert() and sap_remove() each return a
//  boolean indication of success/failure for this use. Each operation is O(1) so
//  the overall search is still O(1). 
//
//  An sap can have any search-type but an sap used with probing is restricted 
//  to tags sorted by UNSIGNED_ST sort-type.


//
//
mword *sap_lookup(babel_env *be, mword *sap, mword *key, search_type s, sort_type st){

    mword sap_index = sap_find_index(be, sap, key, s, st);

    if(sap_index == NEG_ONE)
        return be->nil; // FIXME: be->non_exist

    return pcdr(rdp(sap,sap_index));

}


//
//
mword sap_update(babel_env *be, mword *sap, mword *key, mword *payload, search_type s, sort_type st){

    mword sap_index = sap_find_index(be, sap, key, s, st);

    if(sap_index != NEG_ONE)
        ldp(ldp(sap,sap_index),1) = payload;

    return sap_index;

}


//
//
mword sap_remove(babel_env *be, mword *sap, mword *key){

    mword sap_index = sap_find_index(be, sap, key, PROBE_S, UNSIGNED_ST);

    if(sap_index != NEG_ONE)
        ldp(sap,sap_index) = be->nil;

    return sap_index;

}


//
//
mword sap_insert(babel_env *be, mword *sap, mword *key, mword *payload){

    mword sap_index = sap_update(be, sap, key, payload, PROBE_S, UNSIGNED_ST);

    if(sap_index == NEG_ONE){

        sap_index = sap_find_empty_index(be, sap, key);

        if(sap_index != NEG_ONE)
            ldp(sap,sap_index) = list_cons(be, key, payload);

    }

    return sap_index;

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

    mword **target;

    for(; i<end; i++){
        target = &(rdp(sap,i));
        if(is_nil(*target))
            continue;
        if(cmp_fn(target, &search_key) == 0){
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

    mword **lower_target;
    mword **upper_target;

    if(result == NEG_ONE){

        mword *search_key_cons[2];
        mword *search_key = (mword*)&search_key_cons;

        search_key_cons[0] = key;
        search_key_cons[1] = be->nil;

        while(1){

            lower_bound = (lower_bound < 1) ? 0 : lower_bound;
            upper_bound = (upper_bound > sap_size) ? sap_size : upper_bound;

            lower_target = &(rdp(sap,lower_bound));
            upper_target = &(rdp(sap,upper_bound));

            while(is_nil(*lower_target)){ // XXX PERF: This loop can seriously degrade performance
                lower_bound--;
                lower_target = &(rdp(sap,lower_bound));
                if(lower_bound == 0)
                    break;
            }

            while(is_nil(*upper_target)){
                upper_bound++;
                upper_target = &(rdp(sap,upper_bound));
                if(upper_bound == sap_size)
                    break;
            }

            int lower_comparison = cmp_fn(&search_key, lower_target);
            int upper_comparison = cmp_fn(&search_key, upper_target);

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


// only works with PROBE_S on std hashed keys (UNSIGNED_ST)
//
mword sap_find_empty_index(babel_env *be, mword *sap, mword *key){

    mword sap_size = size(sap);

    // FIXME: 64-bit specific:
    double partition_num = (double)(key[1]) / (double)ULONG_MAX;

    mword guess_index = sap_size * partition_num;

    if(is_nil(rdp(sap,guess_index)))
        return guess_index;

    mword probe_range = (sap_size/SAP_PROBE_REGION_DENOM);

    int lower_bound = guess_index-probe_range;
    int upper_bound = guess_index+probe_range;

    int i;
    for(i=lower_bound; i<=upper_bound; i++){
        if(is_nil(rdp(sap,i)))
            return i;
    }

    return NEG_ONE;

}


// Clayton Bauman 2018

