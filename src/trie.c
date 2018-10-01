// trie.c
//

#include "babel.h"
#include "trie.h"
#include "bstruct.h"
#include "tptr.h"
#include "pearson.h"
#include "string.h"
#include "list.h"
#include "array.h"
#include "introspect.h"
#include "mem.h"


//
//
mword *trie_new(babel_env *be){

    return tptr_new(be, 
                HASHC(be, "/babel/tag/trie"), 
                _mkptr(be, 2, be->nil, be->nil));

}


//
//
mword *trie_new_cell(babel_env *be, mword *key, mword *secondary_key, mword *payload){

    return tptr_new(be, HASHC(be, "/babel/tag/trie_entry"),
                    trie_new_entry(be, payload, secondary_key, key));

}


//
//
void trie_insert(babel_env *be, mword *trie, mword *key, mword *secondary_key, mword *payload){

    if(is_tptr(trie)){ 
        trie = tcar(trie);  // XXX We don't check the tag
    }

    if( (is_nil(key) && is_nil(secondary_key) )
            || !is_ptr(trie))
        return; // do nothing

    if(is_nil(key)){

        if(is_nil(secondary_key)){
            return; // do nothing
        }
        else{
            key = pearson_hash8(be, be->zero_hash, (char*)secondary_key, array8_size(secondary_key));
        }

    }
    else{

        if(is_tptr(key)){ // FIXME: kludge due to single-tptr-copy bug in bstruct_unload()
            key = tptr_extract_hash(be, key);
        }
        else{
            key = bstruct_cp(be, key);
        }

    }

    if(!is_nil(secondary_key))
        secondary_key = bstruct_cp(be, secondary_key);

    rtrie_insert(be, trie, key, secondary_key, payload, 0);

}


//
//
void rtrie_insert(babel_env *be, mword *trie, mword *key, mword *secondary_key, mword *payload, mword level){

    mword cons_side   = array1_read(key, level);
    mword *next_level = rdp(trie, cons_side);

    // 1. cons_side = nil
    //      insert
    // 2. cons_side is inte AND size = 2
    //      recurse
    // 3. cons_side is inte AND size = HASH_ENTRY_SIZE
    //      split and insert

    if(is_ptr(next_level)){
        rtrie_insert(be, next_level, key, secondary_key, payload, level+1);
    }
    else if(is_nil(next_level)){

        ldp(trie,cons_side) =
            tptr_new(be, HASHC(be, "/babel/tag/trie_entry"), trie_new_entry(be, payload, secondary_key, key));

//        ldp(trie,cons_side) =
//            tptr_new(be, be->zero_hash, trie_new_entry(be, payload, secondary_key, key));

    }
    else if(is_tptr(next_level)){ //XXX: We are ASSUMING it's a trie_entry...

        mword *entry     = tptr_detag(be, next_level);

        mword *entry_key = trie_entry_get_key(be, entry);

        if( tageq(entry_key, key, TAG_SIZE) ){ //already exists...  
            trie_entry_set_payload2(be, entry, payload); 
        }
        else{

            //mword *existing_key = trie_entry_get_key(be, entry);
            if( array1_read(entry_key, level+1) ){
                // note that ldp(trie, cons_side) is equivalent to next_level
                ldp(trie, cons_side) = list_cons(be,  be->nil,         next_level  );
            }
            else{
                ldp(trie, cons_side) = list_cons(be,  next_level,  be->nil         );
            }

            rtrie_insert(be, ldp(trie, cons_side), key, secondary_key, payload, level+1);

        }
    }
    else{ // is_val(next_level)
        return; // FIXME: Should throw an exception here due to mal-formed trie...
    }

}


//
//
mword *trie_lookup_hash(babel_env *be, mword *trie, mword *hash_key, mword *string_key){

    if(is_tptr(trie)){ 
        trie = tcar(trie);  // XXX We don't check the tag
    }

    if(!is_ptr(trie))
        return be->nil; // return nothing

    if(is_nil(hash_key)){

        if(is_nil(string_key)){
            return be->nil;
        }
        else{
            hash_key = pearson_hash8(be, be->zero_hash, (char*)string_key, array8_size(string_key));
        }

    }

    return rtrie_lookup(be, trie, hash_key, 0);

}


//
//
mword *rtrie_lookup(babel_env *be, mword *trie, mword *key, mword level){

    mword cons_side   = array1_read(key, level);
    mword *next_level = rdp(trie, cons_side);

    if(is_ptr(next_level)){
        return rtrie_lookup(be, next_level, key, level+1);
    }
    if(is_nil(next_level)){ //dead-end
        return be->nil;
    }
    else if(is_tptr(next_level)){

        mword *trie_entry = tptr_detag(be, next_level); // XXX assumes well-formedness
        mword *entry_key = trie_entry_get_key(be, trie_entry);

        if(tageq(entry_key, key, TAG_SIZE)){ 
            return trie_entry; // found it!
        }
        else{
            return be->nil; //dead-end
        }

    }
    // else is_val(next_level)... FIXME should raise an exception

    return be->nil;

}


//
//
mword trie_remove(babel_env *be, mword *trie, mword *key, mword *secondary_key){

    if(is_tptr(trie)){ 
        trie = tcar(trie);  // XXX We don't check the tag
    }

    if(!is_ptr(trie))
        return 0; // nothing removed

    if(is_nil(key)){

        if(is_nil(secondary_key)){
            return 0;
        }
        else{
            key = pearson_hash8(be, be->zero_hash, (char*)secondary_key, array8_size(secondary_key));
        }

    }

    return rtrie_remove(be, trie, key, 0);

}


//
//
mword rtrie_remove(babel_env *be, mword *trie, mword *key, mword level){

    mword cons_side   = array1_read(key, level);
    mword *next_level = rdp(trie, cons_side);

    if(is_ptr(next_level)){
        if(rtrie_remove(be, rdp(trie,cons_side), key, level+1)){

            if(    is_nil( pcar(next_level) ) 
                && is_nil( pcdr(next_level) )){
                ldp(trie,cons_side) = be->nil; 
            }

            return 1;
        }
        return 0;
    }
    else if(is_nil(next_level)){ //dead-end
        return 0;
    }
    else if(is_tptr(next_level)){ // XXX assumes well-formed key-entry

        if(tageq(pcar(tcar(next_level)),key,TAG_SIZE)){ //match
            ldp(trie,cons_side) = be->nil;
            return 1;
        }
        else{
            return 0;
        }

    }

    return 0; // silence compiler warning

}


//
//
mword *trie_entries(babel_env *be, mword *trie){

    if(is_tptr(trie)){ 
        trie = tcar(trie);
    }

    if(is_nil(trie)){
        return be->nil;
    }
    else{
        return rtrie_entries(be, trie, 0);
    }

}


//
//
mword *rtrie_entries(babel_env *be, mword *trie, mword level){

    //FIXME: Check for level > HASH_BIT_SIZE

    if(is_nil(trie)){ //dead-end
        return be->nil;
    }
    else if(is_conslike(trie)){

        mword *list0 = rtrie_entries(be, pcar(trie), level+1);
        mword *list1 = rtrie_entries(be, pcdr(trie), level+1);

        if(is_nil(list0)){
            if(is_nil(list1)){
                return be->nil;
            }
            else{
                return list1;
            }
        }
        else{
            if(is_nil(list1)){
                return list0;
            }
            else{
                return list_append_direct(be, list0, list1);
            }
        }

    }
    else if(is_tptr(trie)){
        return list_cons(be, tptr_hard_detag(be, trie), be->nil);
    }
    else{
        _fatal("unexpected element in hash-table"); //FIXME: except, not fatal
    }

}


// Clayton Bauman 2018

