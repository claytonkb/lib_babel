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

        comparison = cmp_fn(&(rdp(sap,guess_index)), &key);

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

    mword *search_key = pcar(key);

    double partition_num = (double)(search_key[1]) / (double)ULONG_MAX;

//printf("partition_num %lf\n", partition_num);

    mword guess_index = sap_size * partition_num;

    mword probe_range = (sap_size/SAP_PROBE_REGION_DENOM);

    int lower_bound = guess_index-probe_range;
    int upper_bound = guess_index+probe_range;

//    lower_bound = (lower_bound < 1) ? 0 : lower_bound;
//    upper_bound = (upper_bound > sap_size) ? sap_size : upper_bound;

//_dd(lower_bound);
//_dd(guess_index);
//_dd(upper_bound);

    mword result = sap_find_index_linear(be,
                            sap,
                            lower_bound,
                            upper_bound,
                            key,
                            UNSIGNED_ST);

//_dd(result);
//
//mword lower_comparison = cmp_fn(&key, &(rdp(sap,lower_bound)));
//mword upper_comparison = cmp_fn(&key, &(rdp(sap,upper_bound)));
//
//_dd(lower_comparison);
//_dd(upper_comparison);
//
//_mem(pcar(key));
//_mem(pcar(rdp(sap,lower_bound)));
//_mem(pcar(rdp(sap,upper_bound)));
//
//_die;

//    mword done=0;

    if(result == NEG_ONE){

        while(1){

            lower_bound = (lower_bound < 1) ? 0 : lower_bound;
            upper_bound = (upper_bound > sap_size) ? sap_size : upper_bound;

            int lower_comparison = cmp_fn(&key, &(rdp(sap,lower_bound)));
            int upper_comparison = cmp_fn(&key, &(rdp(sap,upper_bound)));

//_dd(lower_comparison);
//_dd(upper_comparison);

//_mem(pcar(key));
//_mem(pcar(rdp(sap,lower_bound)));
//_mem(pcar(rdp(sap,upper_bound)));
//
//_die;

            if((lower_comparison >= 0) && (upper_comparison <= 0)){
                return result;
            }

            probe_range *= 2;
            lower_bound = guess_index-probe_range;
            upper_bound = guess_index+probe_range;

//_dd(lower_bound);
//_dd(upper_bound);

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

