// aop.c
//

#include "babel.h"
#include "aop.h"
#include "list.h"
#include "trie.h"

// array-of-pairs
// Conversion operators
//
//    trie>aop
//    aop>trie
//            
//    arr,arr>aop
//    aop>arr,arr
//
//    ls>aop
//    aop>ls
//
//    sap>aop (no effect)
//    aop>sap


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


// Clayton Bauman 2018

