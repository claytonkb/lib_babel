// aop.c
//

#include "babel.h"
#include "aop.h"
#include "list.h"
#include "trie.h"
#include "mem.h"
#include "array.h"
#include "cmp.h"


//
//
aop aop_from_trie(babel_env *be, trie t){

    aop result = list_to_ptr_array(be, trie_entries(be, t));

    mword result_size = size(result);

    mword *payload;
    int i;

    for(i=0;i<result_size;i++){
        payload = rdp(rdp(result,i),1);
        ldp(ldp(result,i),1) = pcar(pcdr(payload));
    }

    return result;

}


//
//
trie aop_to_trie(babel_env *be, aop a){

    trie result = trie_new(be);
    mword aop_size = size(a);

    int i;
    for(i=0; i<aop_size; i++){
        trie_insert(be, result, rdp(rdp(a,i),0), be->nil, rdp(rdp(a,i),1));
    }

    return result;

}


// same as aop_from_trie, but saves secondary_key (string) instead of key (hash)
// results are undefined if secondary_key field is nil for any trie-entry
//
aop aop_from_trie_keys(babel_env *be, trie t){

    aop result = list_to_ptr_array(be, trie_entries(be, t));

    mword result_size = size(result);

    mword *payload;
    int i;

    for(i=0;i<result_size;i++){
        payload = rdp(rdp(result,i),1);
        ldp(ldp(result,i),0) = pcar(payload);
        ldp(ldp(result,i),1) = pcar(pcdr(payload));
    }

    return result;

}


// same as aop_from_trie, but treats first element of pair as a secondary_key
//     (string) instead of key (hash)
// results are undefined if first element of pair is anything other than a 
//     val8-array
//
trie aop_to_trie_keys(babel_env *be, aop a){

    trie result = trie_new(be);
    mword aop_size = size(a);

    int i;
    for(i=0; i<aop_size; i++){
        trie_insert(be, result, be->nil, rdp(rdp(a,i),0), rdp(rdp(a,i),1));
    }

    return result;

}


//
//
aop aop_from_array(babel_env *be, arr a, arr b){

    mword arr_size = size(a);

    aop result = mem_new_ptr(be, arr_size);
    ls pair;

    int i;
    for(i=0; i<arr_size; i++){
        pair = list_cons(be, rdp(a,i), rdp(b,i));
        ldp(result,i) = pair;
    }

    return result;

}


// returns cons'd pair of arrays
//
bstruct aop_to_array(babel_env *be, aop a){

    mword aop_size = size(a);

    arr car_array = mem_new_ptr(be, aop_size);
    arr cdr_array = mem_new_ptr(be, aop_size);

    bstruct result = list_cons(be, car_array, cdr_array);

    int i;
    for(i=0; i<aop_size; i++){
        ldp(car_array,i) = pcar(rdp(a,i));
        ldp(cdr_array,i) = pcdr(rdp(a,i));
    }

    return result;

}


//
//
aop aop_from_list(babel_env *be, ls a, ls b){

    return aop_from_array(be, list_to_ptr_array(be, a), list_to_ptr_array(be, b));

}


//
//
bstruct aop_to_list(babel_env *be, aop a){

    bstruct b = aop_to_array(be, a);

    bstruct car_list = array_to_list(be, pcar(b));
    bstruct cdr_list = array_to_list(be, pcdr(b));

    return list_cons(be, car_list, cdr_list);

}


//
//
aop aop_from_sap(babel_env *be, sap a){

    return a;

}


// note: each sort_type technically creates a different sorted-array-of-pairs
//
sap aop_to_sap(babel_env *be, aop a, sort_type st){

//    void *sort_fn = aop_select_cmp_fn(st);
//    qsort(a, size(a), sizeof(mword), sort_fn);

    qsort(a, size(a), sizeof(mword), aop_select_cmp_fn(st));

    return a;

}


//
//
void *aop_select_cmp_fn(sort_type st){

    switch(st){
        case UNSIGNED_ST:
            return cmp_aop_unsigned;
        case SIGNED_ST:
            return cmp_aop_signed;
        case LEX_MWORD_ST:
            return cmp_aop_lex_mword;
        case LEX_BYTE_ST:
            return cmp_aop_lex_byte;
        case ALPHA_MWORD_ST:
            return cmp_aop_alpha_mword;
        case ALPHA_BYTE_ST:
            return cmp_aop_alpha_byte;
        case VAL_ST:
            return cmp_cuint;
        case CUSTOM_ST: // use array_sort() for this
            return cmp_aop_custom;
        default:
            _pigs_fly;
    }
 
}


// Clayton Bauman 2018

