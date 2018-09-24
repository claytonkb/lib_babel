// list.c
//

#include "babel.h"
#include "list.h"
#include "bstruct.h"
#include "tptr.h"
#include "array.h"
//#include "string.h"
#include "mem.h"
//#include "introspect.h"
//#include "io.h"


//
//
mword *list_insert(babel_env *be, mword *src_list, mword *dest_list){ // list_insert#

    if(is_nil(dest_list)){
        return src_list;
    }

    mword *next_dest_list = pcdr(dest_list);
    mword *end_src_list   = list_find_end(pc, src_list);

    ldp(dest_list,1)      = src_list;
    ldp(end_src_list,1)   = next_dest_list;

    return dest_list;

}


//
//
mword *list_unshift(babel_env *be, mword *list, mword *bs){ // list_unshift#

    mword *endls = list_find_end(pc, list);
    ldp(endls,1) = list_cons(pc,  bs, pc->nil );

    return list;

}


//
//
mword *list_shift(babel_env *be, mword *list){ // list_shift#

    if(is_nil(list)) return pc->nil;

    if(list_len(pc,list) < 2) return list;

    mword *endls = list_find_next_to_end(pc, list);

    if(is_nil(endls)) return pc->nil;

    mword *temp = rdp(endls,1);
    ldp(endls,1) = pc->nil;

    return temp;

}


// NOTE: _push does not update list to point to the new head, it
// is the caller's responsibility to maintain this
//
void list_push(babel_env *be, mword *list, mword *bs){ // list_push#

    ldp(bs,1) = list;

}


//
//
mword *list_pop(babel_env *be, mword *list){ // list_pop#

    if(is_nil(list)) return pc->nil;

    mword *temp = pcdr(list);
    ldp(list,1) = pc->nil;

    return temp;

}


//
//
mword *list_find_end(babel_env *be, mword *list){ // list_find_end#

    while(!is_nil(pcdr(list))){
        list = pcdr(list);
    }
    return list;
    
}


//
//
mword *list_find_next_to_end(babel_env *be, mword *list){ // *list_find_next_to_end#

    while(!is_nil(pcdr(pcdr(list)))){
        list = pcdr(list);
    }
    return list;
    
}


//
//
mword list_len(babel_env *be, mword *list){ // list_len#

    mword length = 0;

    while(!is_nil(list)){
        length++;
        list = pcdr(list);
    }

    return length;

}


//
//
mword *list_to_ptr_array(babel_env *be, mword *list){ // list_to_ptr_array#

    mword *head = list;
    mword *arr = mem_new_ptr(pc, list_len(pc, list));
    mword *curr_node;

    int i=0;

    while(!is_nil(list)){
        curr_node = pcar(list);
        if(curr_node == head){
            ldp(arr,i) = arr;
        }
        else{
            ldp(arr,i) = curr_node;
        }        
        i++;
        list = pcdr(list);
    }

    return arr;

}


//
//
mword *list_to_val_array(babel_env *be, mword *list){ // list_to_val_array#

    mword *head = list;
    int total_size=0;
    int list_length=0; // We don't use _len because we're already traversing the list

    while(!is_nil(list)){
        if(!is_val(pcar(list))){
            _fatal("can't cat leaf and inte arrays"); //FIXME: Exception
        }
        total_size += size(pcar(list));
        list_length++;
        list = pcdr(list);        
    }

    if(!list_length){
        return mem_new_val(pc, 1, 0);
    }

    mword *arr = mem_new_val(pc, total_size, 0);

    list = head;

    int i=0;
    int element_size;
    while(!is_nil(list)){

        element_size = size(pcar(list));
        memmove(arr+i, pcar(list), (size_t)UNITS_MTO8(element_size));
        i += element_size;
        list = pcdr(list);

    }

    return arr;

}


// equiv to Lisp's cdrn
//
mword *list_cdri(babel_env *be, mword *list, mword i){ // list_cdri#

    while(i > 0){
        i--;
        list = pcdr(list);
    }

    return list;

}


//
//
mword *list_ith(babel_env *be, mword *list, mword i){ // list_ith#

    list = list_cdri(pc, list, i);

    return pcar(list);

}



//
//
mword *list_reverse(babel_env *be, mword *list, mword *new_cdr){ // list_reverse#

    mword *temp = pcdr(list);

    ldp(list,1) = new_cdr;

    if(is_nil(temp))
        return list;

    return list_reverse(pc, temp, list);

}


//
//
mword *list_split(babel_env *be, mword *list, mword *indices){ // list_split#

    return list_split_r(pc, list,indices,0);

}


//
//
mword *list_split_r(babel_env *be, mword *list, mword *indices, mword count){ // list_split_r#

    mword *orig_list = list;

    if (is_nil(indices)) return list_cons(pc,  orig_list, pc->nil );

    if (is_nil(list)) return pc->nil;// 

    mword curr_index = vcar(pcar(indices));

    if (curr_index < count) return list_cons(pc,  orig_list, pc->nil );

    if (curr_index == 0) return list_cons(pc,  pc->nil, list_cons(pc,  orig_list, pc->nil ) );

    indices = pcdr(indices);

    //if (is_nil(list)) return list;
    
    mword *prev_list = list;

    while(!is_nil(list)){

//        if(count++ > 10){
//            _dump(list);
//            die;
//        }

        if(curr_index == count){
            break;
        }

        count++;

        prev_list = list;
        list = pcdr(list);

    }

    if(!is_nil(pcdr(prev_list))){
        ldp(prev_list,1) = pc->nil;
    }

    return list_cons(pc,  orig_list, list_split_r(pc, list, indices, count) );

}


// XXX Isn't this just a special case of list_split? XXX
//
mword *list_cut(babel_env *be, mword *list, mword index){ // list_cut#

    mword *temp;

    if(is_nil(list)) return pc->nil;

    if(index == 0) return list;

    if(index == 1){

        temp = pcdr(list);
        ldp(list,1) = pc->nil;

        return temp;

    }

    return list_cut(pc, pcdr(list), index-1 );

}


//
//
mword *list_append(babel_env *be, mword *lists){ // list_append#

    if(is_nil(pcdr(lists)))
        return pcar(lists);
        //return lists;

    return list_append_direct(pc, list_ith(pc, lists,0), list_append(pc, pcdr(lists)));

}


///
//
mword *list_append_direct(babel_env *be, mword *head_list, mword *tail_list){ // list_append_direct#

    mword *endls = list_find_end(pc, head_list);

    ldp(endls,1) = tail_list;

    return head_list;

}


/*****************************************************************************
 *                                                                           *
 *                           DOUBLE-LINKED LIST                              *
 *                                                                           *
 ****************************************************************************/


//
//
mword *dlist_append_direct(babel_env *be, mword *head_list, mword *tail_list){ // dlist_append_direct#

    mword *endls = dlist_find_end(pc, head_list);

    ldp(endls,     1) = tail_list;
    ldp(tail_list, 2) = endls;

    return head_list;

}


// insert into double-list
//
mword *dlist_insert(babel_env *be, mword *src_list, mword *dest_list){ // dlist_insert#

    if(is_nil(dest_list)){
        return src_list;
    }
    
    if(is_nil(src_list)){
        return dest_list;
    }

    mword *next_dest_list = pcdr(dest_list);
    mword *end_src_list   = dlist_find_end(pc, src_list);

    ldp(dest_list,    1) = src_list;
    ldp(end_src_list, 1) = next_dest_list;
    ldp(src_list,     2) = dest_list;

    if(is_nil(next_dest_list)){
        return dest_list;
    }

    ldp(next_dest_list,2) = end_src_list;

    return dest_list;

}


// insert into double-list
//
mword *dlist_insert_prev(babel_env *be, mword *src_list, mword *dest_list){ // dlist_insert_prev#

    if(is_nil(dest_list)){
        return src_list;
    }

    if(is_nil(src_list)){
        return dest_list;
    }

    mword *prev_dest_list = pcpr(dest_list);
    mword *end_src_list   = dlist_find_end(pc, src_list);

    ldp(end_src_list,  1) = dest_list;
    ldp(dest_list,     2) = end_src_list;
    ldp(src_list,      2) = prev_dest_list;

    ldp(prev_dest_list,1) = src_list;

    return dest_list;

}


//
//
mword *dlist_find_end(babel_env *be, mword *list){ // dlist_find_end#

    mword *head = list;
    
    while(!is_nil(pcdr(list)) && head != pcdr(list)){
        list = pcdr(list);
    }

    return list;
    
}


//
//
mword dlist_len(babel_env *be, mword *list){ // dlist_len#

    mword length = 0;
    mword *head = list;

    while(!is_nil(list)){
        length++;
        if(head == pcdr(list)){
            break; }
        list = pcdr(list);
    }

    return length;

}


// This function assumes the dlist is well-formed
//
mword *dlist_reverse(babel_env *be, mword *list, mword *head, mword direction){ // dlist_reverse#

    if(is_nil(list))
        return pc->nil;

    mword *next = rdp(list,1+direction);
    mword *prev = rdp(list,2-direction);

    ldp(list,2-direction) = next;
    ldp(list,1+direction) = prev;

//    mword *next = rdp(list,1);
//    mword *prev = rdp(list,2);
//
//    ldp(list,2) = next;
//    ldp(list,1) = prev;

    if(next == head || is_nil(next))
        return list;

    return dlist_reverse(pc, next, head, direction);

}


//
//
mword *dlist_cut(babel_env *be, mword *list, mword index, mword direction){ // dlist_cut#

    mword *temp;

    if(is_nil(list)) return pc->nil;

    if(index == 0) return list;

    if(direction == CDR_DIRECTION){
        temp = rdp(list,1);
    }
    else{ //if(direction == CPR_DIRECTION)
        temp = rdp(list,2);
    }

    if(index == 1){

        ldp(list,1+direction) = pc->nil;
        ldp(temp,2-direction) = pc->nil;

        return temp;

    }

    return dlist_cut(pc, temp, index-1, direction);

}


// Clayton Bauman 2018

